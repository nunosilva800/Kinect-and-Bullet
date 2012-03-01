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
	initKinect();

	while(27 != (char)cvWaitKey(10))
	{
		getKinectFrame();
		getDetectedObjects() ;
	}

	//start_pplDetect(CAP_FROM_KINECT); // this starts a loop just displying the kinect captures

	//start_PhysicsGL();
}
