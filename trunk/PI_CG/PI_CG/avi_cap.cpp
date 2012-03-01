/*
*	Capture and reproduce a video file, allowing seek & play
*
*	Nuno Silva
*
*	Note: strange bug, sometimes it fails to aquire the frame from video.
*		maybe codec problems?
*/

#include <cv.h>
#include <highgui.h>


CvCapture* g_capture = NULL;
IplImage* frame = NULL;
int g_slider_position = 0;

void onTrackBarSlide(int pos){
	cvSetCaptureProperty(
		g_capture,
		CV_CAP_PROP_POS_FRAMES,
		pos );
}


int main(int argc, char * argv[])
{
	// get the file
	g_capture = cvCreateFileCapture("..\\IMAG0009.AVI");
	if(!g_capture) { printf("Fail to get vid\n"); return 0; }

	// get the number of frames in video
	int nframes = (int) cvGetCaptureProperty(g_capture, CV_CAP_PROP_FRAME_COUNT);
	if(!nframes) { printf("Fail to get property\n"); return 0; }

	// get the 1st frame
	frame = cvQueryFrame( g_capture );
	if(!frame) { printf("Fail to get frame\n"); return 0; }

	// create the window
	cvNamedWindow("AVI", CV_WINDOW_AUTOSIZE);

	// create trackbar to allow video seek
	cvCreateTrackbar("Position","AVI", &g_slider_position, nframes, onTrackBarSlide);

	while( cvWaitKey(25) != 27 ) 
	{
		frame = cvQueryFrame( g_capture );
		
		if(!frame) continue;

		cvShowImage("AVI", frame);
	}
	
	cvReleaseCapture( &g_capture );
	cvDestroyWindow("AVI");

	return 0;
}

