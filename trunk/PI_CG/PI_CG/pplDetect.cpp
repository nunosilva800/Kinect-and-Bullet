/*
*	PI CG
*	detect people in a scene
*	put balls in it
*	watch interactions
*	
*/


#include "pplDetect.h"

using namespace cv;
using namespace std;

CvCapture* capture = NULL;
IplImage* frame = NULL;
int g_slider_position = 0;

const char *video_file = "../Resources/IMAG0009.AVI";
const char *video_file2 = "../Resources/test_vid1.wmv";
const char *video_file3 = "../Resources/test_vid3.wmv";

CvMemStorage* contourStorage = cvCreateMemStorage(0); //storage for contours
CvSeq *contourSeq; // sequence of detected contours
CvSeq *approxContourSeq; // sequence of approximated detected contours
CvPoint* PointArray;	// CloudPoints of the contours
CvScalar color1 = CV_RGB( 255, 0, 0); // contour colors
CvScalar color2 = CV_RGB( 0, 255, 0);
CvScalar color3 = CV_RGB( 0, 0, 255);
CvRect bRect;	// bounding rectangle





/********************************/

char showMenu()
{
	printf(" 1: Capture from AVI\n");
	printf(" 2: Capture from Camera\n");
	flushall();
	return getchar();
}

void onTrackBarSlide(int pos){
	cvSetCaptureProperty(
		capture,
		CV_CAP_PROP_POS_FRAMES,
		pos );
}


/*  for segmentation */
CvBGStatModel* bg_model = 0;
unsigned int nframes=1;
int nframesToLearnBG = 80;
IplImage* fg_regions;		// foreground binary mask, the detected regions 

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
	cvErode(fg_regions, fg_regions, NULL, 5);
	cvDilate(fg_regions, fg_regions, NULL, 5);
}


int main(int argc, char * argv[])
{
	char menuOption = 0;
	int nframes = 0;

	menuOption = showMenu();

	// create the window
	cvNamedWindow("Image", CV_WINDOW_AUTOSIZE);

	switch(menuOption)
	{
		case '1':
			printf("Capturing from AVI\n");
			// get the file
			capture = cvCreateFileCapture(video_file3);
			if(!capture) { printf("Fail to get vid\n"); getchar(); return 0; }

			// get the number of frames in video
			nframes = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_COUNT);
			if(!nframes) { printf("Fail to get property\n"); getchar(); return 0; }

			// get the 1st frame
 			frame = cvQueryFrame( capture );
			if(!frame) { printf("Fail to get frame\n"); getchar(); return 0; }

			// create trackbar to allow video seek
			cvCreateTrackbar("Position","Image", &g_slider_position, nframes, onTrackBarSlide);
			break;
		case '2':
			printf("Capturing from Camera\n");
			capture = cvCaptureFromCAM(0);
			if(!capture){ printf("Failed to capture from camera"); getchar(); return 0; }

			frame = cvQueryFrame( capture );
			if(!frame){ printf("Failed to capture from camera"); getchar(); return 0; }
			break;
		default:
			return 0;
	}

	while( cvWaitKey(10) != 27 ) 
	{
		frame = cvQueryFrame( capture );
		if(!frame) continue;

		doSegm(frame);
		
		cvShowImage("Detected regions", fg_regions);

		// get the contours from the detected foreground regions
		cvFindContours(fg_regions, contourStorage, &contourSeq,
			sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
		
		if(contourSeq)
		{
			// approximate to reduce the number of points 
			approxContourSeq = cvApproxPoly(contourSeq, sizeof(CvContour),
				contourStorage,	CV_POLY_APPROX_DP, 2, 1) ;

			for( ; approxContourSeq != 0; approxContourSeq = approxContourSeq->h_next )
			{
				// maybe clean the small contours
				if(cvContourArea(approxContourSeq) < 500.0)
					continue;

				// Allocate memory for contour point set.
				PointArray = (CvPoint*)malloc( approxContourSeq->total*sizeof(CvPoint) );
		
				// Get contour point set.
				cvCvtSeqToArray(approxContourSeq, PointArray, CV_WHOLE_SEQ);
		
				// print the points
				printf("\nContours: %d\n", approxContourSeq->total);
				for(int i = 0; i < approxContourSeq->total; i++)
				{
					printf("%d,%d\t", PointArray[i].x,PointArray[i].y  );
				}
				free(PointArray);
				printf("\n");
				
				// get a bounding rectangle
				bRect = cvBoundingRect(approxContourSeq, 1);
				
				cvRectangle(frame, 
					cvPoint(bRect.x, bRect.y),
					cvPoint( (bRect.x+bRect.width),(bRect.y+bRect.height) ),
					color3);

				cvDrawContours(frame, approxContourSeq, color1, color2, -1);
			
			}		
		}


		cvShowImage("Image", frame);
	}


	cvReleaseCapture( &capture );
	cvDestroyAllWindows();

	return 0;
}

/* *** Old code
	!-- HogDescriptor - Very slow, not usable for real-time --!

	//HOGDescriptor hog;
	//hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
	//vector<Rect> found, found_filtered;


	//found.clear();
	//hog.detectMultiScale(frame, found, 0, Size(25,25), Size(50,50), 1.05, 2);

	//for( int i = 0; i < found.size(); i++ )
	//{
	//    Rect r = found[i];
	//    // the HOG detector returns slightly larger rectangles than the real objects.
	//    // so we slightly shrink the rectangles to get a nicer output.
	//    r.x += cvRound(r.width*0.1);
	//    r.width = cvRound(r.width*0.8);
	//    r.y += cvRound(r.height*0.07);
	//    r.height = cvRound(r.height*0.8);
	//    cvRectangle(frame, r.tl(), r.br(), cv::Scalar(0,255,0), 3);
	//}




	!-- HaarCascades - Too many false positives, and low detection rate --!
	// Load the HaarCascade classifier for body detection.
	bodyCascade = (CvHaarClassifierCascade*) cvLoad(bodyCascadeFilename, 0, 0, 0);
	if( !bodyCascade ) {
		printf("Couldnt load body detector '%s'\n", bodyCascadeFilename);
		return 0;
	}
	CvSeq * bodies = NULL;
	bodies = detectInImage(frame, bodyCascade);

	if(bodies->total > 0)
	{
		CvRect boundingBox = *(CvRect*)cvGetSeqElem( bodies, 0 );
		boundingBox.height *= pyrDownScale;
		boundingBox.width *= pyrDownScale;
		boundingBox.x *= pyrDownScale;
		boundingBox.y *= pyrDownScale;

		cvRectangle( frame, cvPoint(boundingBox.x, boundingBox.y),
			cvPoint((boundingBox.x+boundingBox.width),
					(boundingBox.y+boundingBox.height)),
			CV_RGB(255,0,0));
	}
*/