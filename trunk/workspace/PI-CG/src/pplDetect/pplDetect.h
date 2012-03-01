#ifndef  _H_PPLDETECT_
#define _H_PPLDETECT_

#include <cv.h>
#include <cvaux.h>
#include <highgui.h>

#include <omp.h>

#include "libfreenect.h"
#include "libfreenect_sync.h"
#include "libfreenect_cv.h"
#include "libfreenect.h"
#include "libfreenect_sync.h"

#include <GL/gl.h>

#define CAP_FROM_FILE 0
#define CAP_FROM_CAM 1
#define CAP_FROM_KINECT 2

struct Point3D
{
	int x, y;
	float z;
};
struct DetectedObject
{
	Point3D * pointArray; // the coordinates (x,y,z) of silhouete
	CvRect bRect;		  // axis aligned bounding box
	int size;			  // the size of the point array
};

IplImage* convertImageToGreyscale(const IplImage *imageSrc);

CvRect getBoundingRect();
CvBox2D getBoundingBox();
CvPoint * getPointArray();
Point3D * getPointArray3D();
std::vector<DetectedObject> * getDetectedObjects();

void doLoop();
void initFromFile();
void initCamera();
void initKinect();
void processFrame();
void processKinectFrame();
IplImage * getFrame();
int start_pplDetect(int cap_src = 0);
void captureIplImage();


#endif
