/*
 * main.cpp
 *
 *  Created on: Jun 8, 2011
 *      Author: nuno
 */

#include "pplDetect/pplDetect.h"
#include "PhysicsGL/MApp.h"

int main(int argc, char *argv[])
{
	std::vector<DetectedObject> * detectedObjs = new std::vector<DetectedObject>;

	//initKinect();
//	while(27 != (char)cvWaitKey(10))
//	{
//		processKinectFrame();
//		detectedObjs = getDetectedObjects() ;
//
//		/*
//		cvWaitKey(0);
//		for(int i = 0; i < detectedObjs->size(); i++)
//		{
//			printf( "Contour %d/%d\t", i+1, detectedObjs->size());
//			printf( "Size: %d 3D points\n", detectedObjs->at(i).size );
//			Point3D * points = detectedObjs->at(i).pointArray;
//			for(int j=0; j < detectedObjs->at(i).size; j++ )
//				printf( "glVertex3f(%d, %d, %f);\n", points[j].x, points[j].y, points[j].z );
//		}./Resources
//		fflush(stdout);
//		*/
//	}

//	initCamera();
//	while(27 != (char)cvWaitKey(10))
//	{
//		processFrame();
//		detectedObjs = getDetectedObjects() ;
//	}

//	initFromFile();
//	while(27 != (char)cvWaitKey(10))
//	{
//		processFrame();
//		detectedObjs = getDetectedObjects() ;
//	}

	//start_pplDetect(CAP_FROM_KINECT); // this starts a loop just displying the kinect captures

	start_PhysicsGL();
}
