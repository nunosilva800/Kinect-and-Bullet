/*
 * Several OpenCV utility functions
 * This file also serves as trash can since most of these functions aren't used already.... :)
 */

#include "pplDetect.h"

/*
 * Either convert the image to greyscale, or make a copy of the existing greyscale image.
 * Make sure that to call cvReleaseImage() on the output when it is not needed.
 */
IplImage* convertImageToGreyscale(const IplImage *imageSrc)
{
	IplImage *imageGrey;

	if (imageSrc->nChannels == 3) {
		imageGrey = cvCreateImage( cvGetSize(imageSrc), IPL_DEPTH_8U, 1 );
		cvCvtColor( imageSrc, imageGrey, CV_BGR2GRAY );
	}
	else {
		imageGrey = cvCloneImage(imageSrc);
	}

	return imageGrey;
}



// Haar Cascade file, used for people detection.
const char *bodyCascadeFilename = "../Resources/haarcascade_fullbody.xml";
CvHaarClassifierCascade* bodyCascade = NULL;
CvMemStorage* bodyStorage = cvCreateMemStorage(0); //storage for body detection
CvSeq* rects = 0; // the bounding rectangles for the bodies

// scale down factor for detection speed up (dont change it for now)
int pyrDownScale = 2;

// Perform a detection on the input image, using the given Haar Cascade.
// Returns a rectangle for each detected region in the given image.
CvSeq* detectInImage(IplImage *imageProcessed, CvHaarClassifierCascade* cascade)
{
	// possibly convert to gray and equalize to improve detection
	//convert to gray
	IplImage * greyImg = convertImageToGreyscale(imageProcessed);

	const CvSize minFeatureSize = cvSize(100/pyrDownScale, 100/pyrDownScale);
	const int flags = CV_HAAR_DO_ROUGH_SEARCH | CV_HAAR_DO_CANNY_PRUNING;
	float search_scale_factor =  1.5f ;

	double t;
	unsigned int ms = 0;

	cvClearMemStorage( bodyStorage );

	// down-scale the image to get performance boost
	IplImage * small_img = cvCreateImage( 
		cvSize(greyImg->width/pyrDownScale,greyImg->height/pyrDownScale),
		IPL_DEPTH_8U, greyImg->nChannels 
		);
	cvPyrDown(greyImg, small_img, CV_GAUSSIAN_5x5);
	
	// Detect all the faces in the greyscale image.
	t = (double)cvGetTickCount();
	rects = cvHaarDetectObjects( small_img, cascade, bodyStorage,
				search_scale_factor, 2, flags, minFeatureSize);
	t = (double)cvGetTickCount() - t;
	ms = cvRound( t / ((double)cvGetTickFrequency() * 1000.0) );
	printf("Detection took %d ms and found %d objects\n", ms, rects->total);

	cvReleaseImage(&small_img);
	cvReleaseImage(&greyImg);

	return rects;
}


// VARIABLES for CODEBOOK METHOD: 
//CvBGCodeBookModel* codebook_model = 0;
//const int NCHANNELS = 3;
//bool ch[NCHANNELS]={true,true,true}; // This sets what channels should be adjusted for background bounds
//unsigned int nframes=1;
//int nframesToLearnBG = 80;
//IplImage* codebookAuxImage = 0; 
//IplImage* fg_regions;		// foreground binary mask, the detected regions 
//
//void doCodeBook(IplImage * imgCam)
//{
//	//1st frame
//	if (!codebook_model)
//	{
//		codebook_model = cvCreateBGCodeBookModel();
//
//		//Set color thresholds
//		codebook_model->modMin[0] = 1;
//		codebook_model->modMin[1] = codebook_model->modMin[2] = 1;
//		codebook_model->modMax[0] = 10;
//		codebook_model->modMax[1] = codebook_model->modMax[2] = 10;
//		codebook_model->cbBounds[0] = codebook_model->cbBounds[1] = codebook_model->cbBounds[2] = 10;
//
//		codebookAuxImage = cvCloneImage(imgCam);
//
//		fg_regions = cvCreateImage(cvSize(imgCam->width, imgCam->height), imgCam->depth, 1);
//		cvSet(fg_regions,cvScalar(255));
//
//	}
//
//	cvCvtColor( imgCam, codebookAuxImage, CV_BGR2HSV );//YUV For codebook method
//
//	//This is where we build our background model
//	if( nframes-1 < nframesToLearnBG  )
//	{
//		cvBGCodeBookUpdate( codebook_model, codebookAuxImage );
//		printf("Training the model, please hold %d frames\n", nframesToLearnBG-nframes);
//		++nframes;
//	}
//
//	if( nframes-1 == nframesToLearnBG  )
//	{
//		cvBGCodeBookClearStale( codebook_model, codebook_model->t/2 );
//		++nframes;
//	}
//	
//	//Find the foreground if any
//	if( nframes-1 >= nframesToLearnBG  )
//	{
//		// Find foreground by codebook method
//		cvBGCodeBookDiff( codebook_model, codebookAuxImage, fg_regions );
//		cvSegmentFGMask( fg_regions );
//
//	}
//
//}

