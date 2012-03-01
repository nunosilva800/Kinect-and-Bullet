#include <cv.h>
#include <cvaux.h>
#include <highgui.h>

#include<omp.h>

#include "libfreenect.h"
#include "libfreenect_sync.h"
#include "libfreenect_cv.h"


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
	Point3D * pointArray; // the coordinates (x,y,z)
	int size;			  // the size of the point array
};


IplImage* convertImageToGreyscale(const IplImage *imageSrc);

CvRect getBoundingRect();
CvPoint * getPointArray();
Point3D * getPointArray3D();
std::vector<DetectedObject> * getDetectedObjects();

void initKinect();
void getKinectFrame();
int start_pplDetect(int capture_source = 0);
