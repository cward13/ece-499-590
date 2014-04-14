//Name: Colin Ward
//Date: 3/17/2014
//Course: ECE 499
//Note: functions and libraries belong to opencv
//in this code, I recieved most of my help from opencv websites below
//http://docs.opencv.org/doc/tutorials/imgproc/erosion_dilatation/erosion_dilatation.html
//http://docs.opencv.org/doc/tutorials/imgproc/imgtrans/canny_detector/canny_detector.html
//Declare Necessary Libraries
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdio.h>
#include <iostream>
using namespace cv;
//Declare main functions
int main(int argc, char *argv[])
{
    //Declare the frame we will be operating on
    Mat frame;
    //Create windows for each operation
    namedWindow("CANNY", 1);
    namedWindow("ERODE", 1);
    namedWindow("DILATE", 1);
    //Begin video Capture from default camera
    VideoCapture cap(0);
    //While there is a feed
    while ( cap.isOpened() )
    {
	//Declare necessary Mat frames and load a frame from the
	//camera into the video frame 
        cap >> frame;
	Mat greyout;
	Mat blurdst;
	Mat resizer;
	Mat finalframe;
	//Convert the RGB frame to a black and white frame so we can perform
	//operations on it
	cvtColor(frame, greyout, CV_RGB2GRAY);
	//Perform Canny operation
	Canny(greyout, blurdst,150,150,3);
	//Put the cannied image on the video frame
	imshow("CANNY",blurdst);
	//This basically sets the features of the erosion/dilation
	Mat element = getStructuringElement( MORPH_RECT,Size(  2.3, 1.5),Point( 1, .7) );
	erode(blurdst,resizer,element);
	imshow("ERODE",resizer);
	element = getStructuringElement( MORPH_CROSS,Size(  2.8, 2.8),Point( .9, .9));
	dilate(resizer,finalframe,element);
	if(frame.empty()) break;
        imshow("DILATE",finalframe);
        if(waitKey(30) >= 0) break;
    }

    return 0;
}
