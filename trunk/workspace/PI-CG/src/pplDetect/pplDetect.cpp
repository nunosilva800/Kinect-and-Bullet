/*
*	PI CG
*	detect people in a scene
*	put balls in it
*	watch interactions
*	
*/

/*
 * Instructions:
 * To use kinect simply do:
 * 		initKinect();
 * Then for each frame you want:
 * 		processKinectFrame();
 * 		getDetectedObjects();
 * The last function call will return a pointer to a std::vector containing 3D points of the silhouettes
 *
 * To use the video file or camera:
 * 		initFromFile(); / initCamera();
 * Then for each frame you want:
 * 		processFrame();
 * 		getDetectedObjects();
 *
 */


/*
 * TODO:
 * Correct the displacement of RBG and Depth camera
 * 		this problem causes the contour to be slightly displaced in the visualization
 */


#include "pplDetect.h"

#include <iostream>
#include<vector>

using namespace cv;
using namespace std;

int capture_source;
CvCapture* capture = NULL;
int g_slider_position = 0;

const char *video_file = "/home/rr/Desktop/PI_CG/trunk/workspace/PI-CG/Debug/Resources/IMAG0009.AVI";
const char *video_file2 = "./Resources/test_vid1.wmv";
const char *video_file3 = "/home/rr/Desktop/PI_CG/trunk/workspace/PI-CG/Debug/Resources/test_vid3.wmv";

CvMemStorage* contourStorage; //storage for contours
CvSeq *contourSeq; 	// sequence of detected contours
CvSeq *approxContourSeq; // sequence of approximated detected contours

CvScalar color1 = CV_RGB( 255, 0, 0); // contour colors
CvScalar color2 = CV_RGB( 0, 255, 0);
CvScalar color3 = CV_RGB( 0, 0, 255);
CvRect bRect;	// bounding rectangle
CvBox2D bBox;	// bounding box

CvPoint* PointArray;	// CloudPoints of the contours
Point3D* PointArray3D; // CloudPoints of the contours in 3D
/*
 * Collection of PointArray3D
 * At each frame this vector is populated with arrays of 3D positions of the contours of the detected objects
 */
std::vector<DetectedObject> * DetectedObjects;

// these images will hold kinect RBG and depth data
static IplImage *frame ;
static IplImage *depth ;
freenect_context * kinect_context;
// these images will serve as fg/bg depth model
static IplImage *fg_kinect_depth;
static IplImage *bg_kinect_depth;

static int IMG_WIDTH = 640;
static int IMG_HEIGHT = 480;

#define MIN_FEATURE_AREA 4608 //640*480*0.015 : 1,5% of pixel area


/********************************/


/*  for segmentation */
CvBGStatModel* bg_model = 0;
unsigned int nframes=1;
unsigned int nframesToLearnBG = 50;
IplImage* fg_regions;		// foreground binary mask, the detected regions 

/*
 * Background Subtraction, using Gaussian Model
 */
void doSegm(IplImage * imgCam)
{
	if(!bg_model)
	{
		//create BG model
		bg_model = cvCreateGaussianBGModel( imgCam );
		
		fg_regions = cvCreateImage(cvSize(imgCam->width, imgCam->height), imgCam->depth, 1);
		cvSet(fg_regions,cvScalar(255));
	}

	if(nframes-1 < nframesToLearnBG)
	{
		cvUpdateBGStatModel( imgCam, bg_model,  -1 );
		printf("Training the model, please hold %d frames\n", nframesToLearnBG-nframes);
		++nframes;
	}
	else
	{
		cvUpdateBGStatModel( imgCam, bg_model,  0 );
	}
	
	cvCopy(bg_model->foreground, fg_regions);

	// clean small disturbances and emphatize our foreground
	//cvErode(fg_regions, fg_regions, NULL, 5);
	//cvDilate(fg_regions, fg_regions, NULL, 5);
}
void onTrackBarSlide(int pos){
	cvSetCaptureProperty(
		capture,
		CV_CAP_PROP_POS_FRAMES,
		pos );
}
void clean()
{
	//cleanup
	DetectedObjects->clear();

	cvReleaseImage(&fg_kinect_depth);
	cvReleaseImage(&bg_kinect_depth);
	cvReleaseImage( &frame);
	cvReleaseImage( &fg_regions);
	cvReleaseBGStatModel( &bg_model);
	cvReleaseMemStorage( &contourStorage);
    cvReleaseCapture( &capture );
	cvDestroyAllWindows();
}
/*
 * http://nicolas.burrus.name/index.php/Research/KinectCalibration
 * http://graphics.stanford.edu/~mdfisher/Kinect.html
 */
float RawDepthToMeters(int depthValue)
{
	if (depthValue < 2047)
	{
		return float(1.0 / (double(depthValue) * -0.0030711016 + 3.3309495161));
	}
	return 0.0f;
}
/*
 * Returns the minimal area rectangle that bounds the detected person in the scene
 */
CvRect getBoundingRect() { return bRect; }
/*
 *  Returns minimum area rotated rectangle bounding the detected object in the scene
 */
CvBox2D getBoundingBox() { return bBox; }
/*
 * Return an array of CvPoint that represente the silhouette of the detected objects
 */
CvPoint * getPointArray() { return PointArray; }
/*
 * Return an array of CvPoint that representes the silhouette of the detected objects, along with depth (z) info
 */
Point3D * getPointArray3D(){ return PointArray3D; }
/*
 * Generate 3D point Array using the 2D Point array
 */
void genPointArray3D(int size)
{
    PointArray3D = (Point3D*)((malloc(size * sizeof (Point3D))));
    for(int i = 0; i < size; i++)
    {
    	int x = PointArray[i].x;
    	int y = PointArray[i].y;
    	PointArray3D[i].x = x;
    	PointArray3D[i].y = y;
    	if(capture_source == CAP_FROM_KINECT)
    		PointArray3D[i].z = RawDepthToMeters(((unsigned short *)depth->imageData)[y*IMG_WIDTH + x]);
    	else
    		PointArray3D[i].z = -1;
    	//printf(" %.4f\n", PointArray3D[i].z);
    }
}
/*
 * Add the 3D point array to the collection of detected objects
 * seqSize : size of 3D point Array
 */
void addDetectedObject(int seqSize)
{
	DetectedObject n;
	n.size = seqSize;
	n.pointArray = PointArray3D;
	n.bRect = bRect;
	DetectedObjects->push_back(n);
}
/*
 * Frees memory used by each contour and clears the DetectedObjects vector
 */
void cleanDetectedObjects()
{
	for(int i = 0; i< DetectedObjects->size(); i++)
		free(DetectedObjects->at(i).pointArray );

	DetectedObjects->clear();
}
/*
 * returns a vector of the detected objects in the scene
 * it is populated with arrays of 3D positions of the contours of the detected objects
 */
std::vector<DetectedObject> * getDetectedObjects(){ return DetectedObjects; };
IplImage * getFrame(){ return frame; };

/*
 * Finds countours,
 * approximates them,
 * creates an array of points of the silhouette
 * also creates a bounding box for that silhouette
 */
void getSilhouette(IplImage* in, IplImage* out)
{
	contourStorage = cvCreateMemStorage(0);
	cleanDetectedObjects();

    // get the contours from the detected foreground regions
    cvFindContours(in, contourStorage, &contourSeq, sizeof (CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
    if(contourSeq)
    {
        // approximate to reduce the number of points 
        approxContourSeq = cvApproxPoly(contourSeq, sizeof (CvContour), contourStorage, CV_POLY_APPROX_DP, 2, 1);
        for(;approxContourSeq != 0;approxContourSeq = approxContourSeq->h_next)
        {
            // maybe clean the small contours
            if(cvContourArea(approxContourSeq) < MIN_FEATURE_AREA)
                continue;
            cvDrawContours(out, approxContourSeq, color1, color2, -1);
			// get a bounding rectangle
		    bRect = cvBoundingRect(approxContourSeq, 1);
		    cvRectangle(out,
            	cvPoint(bRect.x, bRect.y),
				cvPoint((bRect.x + bRect.width), (bRect.y + bRect.height)),
            	color3);

            // just to get central coordinate
            bBox = cvMinAreaRect2(approxContourSeq, contourStorage);
            CvBox2D b = bBox;
            b.size = cvSize2D32f(5,5);
            //cvEllipseBox(out, b, color3, 5);

            /*
             * Cloud point generation
             */
            // Allocate memory for contour point set + the center point
            PointArray = (CvPoint*)((malloc( (approxContourSeq->total + 1) * sizeof (CvPoint))));
            // insert the center of the contour at the beggining of point array
            PointArray[0].x = bBox.center.x;
            PointArray[0].y = bBox.center.y;
			// Get contour point set.
			cvCvtSeqToArray(approxContourSeq, PointArray+1, CV_WHOLE_SEQ);

			// generate array of 3D points (by adding z co-ordinate to 'PointArray')
			genPointArray3D(approxContourSeq->total + 1);
			// add the 3D point array to the collection of detected objects
			addDetectedObject(approxContourSeq->total + 1);

			// print the points
//			printf("\nContours: %d\n", approxContourSeq->total);
//			for(int i = 0;i < approxContourSeq->total;i++){
//				printf("%d,%d\t", PointArray[i].x, PointArray[i].y);
//			}
//			printf("\n");
			free(PointArray);


        }
    }
    cvReleaseMemStorage(&contourStorage);
}

/*
 * colorize the depth image form kinect
 */
IplImage *GlViewColor(IplImage *depth)
{
	static IplImage *image = 0;
	if (!image) image = cvCreateImage(cvSize(640,480), 8, 3);
	unsigned char *depth_mid = (unsigned char*)(image->imageData);
	int i;
	for (i = 0; i < 640*480; i++)
	{
		int lb = ((short *)depth->imageData)[i] % 256;
		int ub = ((short *)depth->imageData)[i] / 256;
		switch (ub)
		{
			case 0:
				depth_mid[3*i+2] = 255;
				depth_mid[3*i+1] = 255-lb;
				depth_mid[3*i+0] = 255-lb;
				break;
			case 1:
				depth_mid[3*i+2] = 255;
				depth_mid[3*i+1] = lb;
				depth_mid[3*i+0] = 0;
				break;
			case 2:
				depth_mid[3*i+2] = 255-lb;
				depth_mid[3*i+1] = 255;
				depth_mid[3*i+0] = 0;
				break;
			case 3:
				depth_mid[3*i+2] = 0;
				depth_mid[3*i+1] = 255;
				depth_mid[3*i+0] = lb;
				break;
			case 4:
				depth_mid[3*i+2] = 0;
				depth_mid[3*i+1] = 255-lb;
				depth_mid[3*i+0] = 255;
				break;
			case 5:
				depth_mid[3*i+2] = 0;
				depth_mid[3*i+1] = 0;
				depth_mid[3*i+0] = 255-lb;
				break;
			default:
				depth_mid[3*i+2] = 0;
				depth_mid[3*i+1] = 0;
				depth_mid[3*i+0] = 0;
				break;
		}
	}
	return image;
}


/*
 * Captures a single frame from camera
 */
void captureIplImage(void)
{
	// create the window
	cvNamedWindow("Image", CV_WINDOW_AUTOSIZE);

	frame = cvQueryFrame(capture);
	if(!frame)
		frame=0;

	doSegm(frame);
	cvShowImage("Detected regions", fg_regions);
	getSilhouette(fg_regions, frame);
	//cvShowImage("Detected regions2", fg_regions);
	cvShowImage("Image", frame);

    cvWaitKey(10);
}

/*
 * Start a loop on a generic capture to detect silhouette
 */
void doLoop()
{
	// create the window
	cvNamedWindow("Image", CV_WINDOW_AUTOSIZE);

    while(27 != (char)cvWaitKey(10) )
    {
        frame = cvQueryFrame(capture);
        if(!frame)
            continue;

        doSegm(frame);
        cvShowImage("Detected regions", fg_regions);
        getSilhouette(fg_regions, frame);
        //cvShowImage("Detected regions2", fg_regions);
        cvShowImage("Image", frame);
    }
}

/*
 * Start a loop on a capture from Kinect to detect silhouette
 */
void doLoopKinect()
{
	freenect_sync_set_led(LED_GREEN, 0);

	// auxiliar iplimage to hold the depth data but with 8 bit
	IplImage *i8bit = cvCreateImage(cvGetSize(depth), 8, 1);

	while (27 != (char)cvWaitKey(10) )
	{
//		double wt1 = omp_get_wtime();

		// get new frames from kinect
		freenect_sync_get_rgb_cv(0);
		freenect_sync_get_depth_cv(0);

		cvCvtColor(frame, frame, CV_RGB2BGR);

		// perform image subtraction on depth
		// the result should filter out the farther and keep only the closest
		// each pixel of depth has 16 bits so I cast to ushort to that indexing is properly done
		unsigned int pixels = depth->width * depth->height ;
		for (unsigned int i = 0; i < pixels; i++)
		{
			((unsigned short *)fg_kinect_depth->imageData)[i] =
					((unsigned short *)depth->imageData)[i] - ((unsigned short *)bg_kinect_depth->imageData)[i];
		}

		// clean small disturbances
		//cvErode(fg_kinect_depth, fg_kinect_depth, NULL, 2);
		//cvDilate(fg_kinect_depth, fg_kinect_depth, NULL, 2);

		// show our stuff
		//cvShowImage("RAW Depth", depth);
		cvShowImage("Depth", GlViewColor(depth));
		cvShowImage("FG", fg_kinect_depth);
		//cvShowImage("BG", GlViewColor(bg));
		//cvShowImage("RGB", image);

		// manual convertion to 8 bit so that normal contour detection can be done
		for (unsigned int i = 0; i < pixels; i++)
		{
			((char *)i8bit->imageData)[i] =  ((unsigned short *)fg_kinect_depth->imageData)[i] * 256/65536;
		}
		// i am getting a "shadow" on depth information (cameras displacement?) so this will clean it up
		cvThreshold(i8bit, i8bit, 240, 255,CV_THRESH_BINARY );
		cvShowImage("8bitFG", i8bit);
		getSilhouette(i8bit, frame);
		cvShowImage("8BitRGB", frame);

//		double wt2 = omp_get_wtime();
//		printf("time: %.3f \n", wt2-wt1);
	}
	cvReleaseImage( &i8bit);
	freenect_sync_set_led(LED_YELLOW, 0);
}
/*
 * Get a frame from either video or camera and detect silhouette
 * use getPointArray3D() to access the silhouette, the 'z' value will be -1
 */
void processFrame()
{
	frame = cvQueryFrame(capture);
	if(!frame)
		return;

	doSegm(frame);
	getSilhouette(fg_regions, frame);
	cvShowImage("Image", frame);
}
/*
 * Get a Kinect frame and detect silhouette based on depth information
 * use getPointArray3D() to access the silhouette
 */
void processKinectFrame()
{
	// auxiliar iplimage to hold the depth data but with 8 bit
	IplImage *i8bit = cvCreateImage(cvGetSize(depth), 8, 1);

	// get new frames from kinect
	freenect_sync_get_rgb_cv(0);
	freenect_sync_get_depth_cv(0);

	cvCvtColor(frame, frame, CV_RGB2BGR);

	// perform image subtraction on depth
	// the result should filter out the farther and keep only the closest
	// each pixel of depth has 16 bits
	unsigned int pixels = depth->width * depth->height ;
	for (unsigned int i = 0; i < pixels; i++)
	{
		((unsigned short *)fg_kinect_depth->imageData)[i] =
				((unsigned short *)depth->imageData)[i] - ((unsigned short *)bg_kinect_depth->imageData)[i];
	}

	// show our stuff
	//cvShowImage("Depth", GlViewColor(depth));
	//cvShowImage("FG", fg_kinect_depth);

	// manual convertion to 8 bit
	for (unsigned int i = 0; i < pixels; i++)
	{
		((char *)i8bit->imageData)[i] =  ((unsigned short *)fg_kinect_depth->imageData)[i] * 256/65536;
	}

	// i am getting a "shadow" so this will clean it up
	cvThreshold(i8bit, i8bit, 240, 255,CV_THRESH_BINARY );
	//cvShowImage("8bitFG", i8bit);

	getSilhouette(i8bit, frame);
	//cvShowImage("8BitRGB", frame);

	cvReleaseImage( &i8bit);
}
/*
 * Initializes variables to use file
 */
void initFromFile()
{
	printf("Capturing from AVI\n");
	// get the file
	capture = cvCreateFileCapture(video_file3);
	if(!capture) { printf("Fail to get vid\n"); getchar(); abort(); }

	// get the number of frames in video
	nframes = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_COUNT);
	if(!nframes) { printf("Fail to get property\n"); getchar(); abort(); }

	// get the 1st frame
	frame = cvQueryFrame( capture );
	if(!frame) { printf("Fail to get frame\n"); getchar(); abort(); }

	if(!DetectedObjects) DetectedObjects = new std::vector<DetectedObject>;

	capture_source = CAP_FROM_FILE;
}
/*
 * Initializes variables to use camera
 */
void initCamera()
{
	printf("Capturing from Camera\n");
	capture = cvCaptureFromCAM(0);
	if(!capture){ printf("Failed to capture from camera"); getchar(); abort(); }

	frame = cvQueryFrame( capture );
	if(!frame){ printf("Failed to capture from camera"); getchar(); abort(); }

	if(!DetectedObjects) DetectedObjects = new std::vector<DetectedObject>;

	capture_source = CAP_FROM_CAM;
}
/*
 * Create kinect contex and initialize it
 * Get one frame from RBG and depth camera to test comunication
 */
void initKinect()
{
	if(0 != freenect_init(&kinect_context, NULL) )
	{
		printf("Kinect context initialization error.\n");
		abort();
	}

	if (!freenect_num_devices(kinect_context))
	{
		printf("Error no Kinect devices found.\n");
		abort();
	}

	// omg its alive! :D
	freenect_sync_set_led(LED_BLINK_GREEN, 0);

	if (!freenect_sync_get_rgb_cv(0))
	{
	    printf("Error acquiring RBG image. Kinect not connected?\n");
	    abort();
	}

	if (!freenect_sync_get_depth_cv(0))
	{
	    printf("Error acquiring depth image. Kinect not connected?\n");
	    abort();
	}

	frame = freenect_sync_get_rgb_cv(0);
	depth = freenect_sync_get_depth_cv(0);

	// this will hold our foreground depth (objects that are closer)
	fg_kinect_depth = cvCreateImage(cvGetSize(depth), 16, 1);
	// get a depth frame from kinect to set our background
	bg_kinect_depth = cvCreateImage(cvGetSize(depth), 16, 1);
	cvCopy( freenect_sync_get_depth_cv(0), bg_kinect_depth);
	// subtract proximity value so that the background is farther away
	cvAddS(bg_kinect_depth, cvScalar(-10, -10, -10, -10), bg_kinect_depth);

	if(!DetectedObjects) DetectedObjects = new std::vector<DetectedObject>;

	capture_source = CAP_FROM_KINECT;

	// ready, set, go!
	freenect_sync_set_led(LED_GREEN, 0);
}

/*
 * Start opencv loop to find people silhouettes
 * cap_src: either from file (0), camera (1) or kinect (2)
 */
int start_pplDetect(int cap_src )
{
	int nframes = 0;
	capture_source = cap_src;

	if(!DetectedObjects) DetectedObjects = new std::vector<DetectedObject>;

	switch(capture_source)
	{
	case CAP_FROM_FILE:
		initFromFile();

		// create trackbar to allow video seek
		// sadly stoped working with the new record we made
		// cvCreateTrackbar("Position","Image", &g_slider_position, nframes, onTrackBarSlide);

		// start loop on video frames
		doLoop();
		break;

	case CAP_FROM_CAM:
		initCamera();
		// start loop on camera frames
		doLoop();
		break;

	case CAP_FROM_KINECT:
		if(!kinect_context) initKinect();
		doLoopKinect();
		freenect_shutdown(kinect_context);
		break;
	}

	clean();
	return 0;
}

