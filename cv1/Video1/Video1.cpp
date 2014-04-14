//Name: Colin Ward
//Date: 3/17/14
//The following are the websites had code I used for the first video and thus all videos after this one
//http://opencv-srf.blogspot.com/2011/09/capturing-images-videos.html (For capturing video)
//http://docs.opencv.org/modules/imgproc/doc/miscellaneous_transformations.html (Converting to b/w)
//http://docs.opencv.org/modules/imgproc/doc/geometric_transformations.html (Resizing Frame)
//http://docs.opencv.org/modules/imgproc/doc/filtering.html (Blurring image)
#include "opencv2/opencv.hpp"
#include <stdio.h>
#include <iostream>
using namespace cv;
int main(int argc, char *argv[])
{
    //Create Operating Frame
    Mat frame;
    //Create windows for each portion
    namedWindow("Black and White", 1);
    namedWindow("Resized and Black and White",1);
    namedWindow("Blurred B/W and resized",1);
    //Capture from default camera
    VideoCapture cap(0);
    while ( cap.isOpened() )
    {
	//load operating frame
        cap >> frame;
	Mat greyout;
	Mat blurdst;
	Mat resizer;
	//set default Mats to operate on
	cvtColor(frame, greyout, CV_RGB2GRAY);
	//gray the frame i.e. make it black and white
	imshow("Black and White",greyout);
	//set a size for the new frame
	Size s( greyout.size().width / 2, greyout.size().height / 2 );
	//resize image
	resize(greyout, resizer, s, .5, .5, INTER_NEAREST);
	imshow("Resized and Black and White",resizer);
	//Blur the image
	blur( resizer, blurdst, Size( 10, 10));
	if(frame.empty()) break;
        imshow("Blurred B/W and resized", blurdst);
        if(waitKey(30) >= 0) break;
    }

    return 0;
}

