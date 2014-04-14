//Name: Colin Ward
//Date: 3/17/14
//Basic Sources: 
//http://docs.opencv.org/doc/user_guide/ug_mat.html
//http://stackoverflow.com/questions/7899108/opencv-get-pixel-information-from-mat-image (Used to get pixel information)
//http://docs.opencv.org/modules/core/doc/drawing_functions.html (Used to draw Circles and Rectangles)
#include "opencv2/opencv.hpp"
#include <stdio.h>
#include <iostream>
using namespace cv;
int main(int argc, char *argv[])
{
    //Initialize frames and begin capturing video from the default camera 
    Mat frame;
    Mat hsvframe;
    Mat oframe;
    namedWindow("Edited Frame", 1);
    namedWindow("Unedited Frame", 1);
    VideoCapture cap(0);
    while ( cap.isOpened() )
    {
	//set cap output to the frame and initialize values
        cap >> frame;
	int y=0;
	int x=0;
	int maxlengthx=0;
	int maxlengthlocx=0;
	int maxlengthlocxy=0;
	int maxlengthy=0;
	int maxlengthlocy=0;
	//Convert BGR to HSV
	cvtColor(frame, hsvframe, CV_BGR2HSV);
	cvtColor(hsvframe, oframe, CV_HSV2BGR);
	for(y=0;y<hsvframe.rows;y++)
	{
		//These for loops navigate the frame in both the x and y dimensions
		//printf("%d",y);
		for(x=0;x<frame.cols;x++)
		{
			int b = frame.data[hsvframe.channels()*(hsvframe.cols*y + x) + 0];
			int g = frame.data[hsvframe.channels()*(hsvframe.cols*y + x) + 1];
			int r = frame.data[hsvframe.channels()*(hsvframe.cols*y + x) + 2];
			//int center=frame.data[frame.channels()*(frame.cols*y + x)];
			//if((140<b&&b<149) && (79<g&&g<86) && (40<r&&r<55))
			float sv=(float)g/r;
			//printf("%f",bluepercent);
			float bgr= (float)b/(b+g+r);
			//Basically this finds whether Blue is greater than all the other values
			//and it holds a 50% majority
			if(b>g && g>r && bgr > .5)
			{
				//If the object is blue we will 
				//mark it with a green dot  
				//printf("Blue detected");
				int xtemp=x;
				int counterx=0;
				CvPoint center = cvPoint(x,y);
				cv::Scalar color = cv::Scalar( 90, 200, 160 );
				circle( frame, center, 1, color, 3, 8, 0 );		
			}		
		}
		
	}
		CvPoint point1 = cvPoint(maxlengthlocx,maxlengthlocy);
		CvPoint point2 = cvPoint(maxlengthlocx+maxlengthx,maxlengthlocy+maxlengthy);
		//CvPoint P1;
		//P1.x=x;
		//P1.y=y;
		//Scalar color = colors[i%8];
		cv::Scalar color = cv::Scalar( 200, 0, 200 );
		//circle( frame, center, 1, color, 3, 8, 0 );
		//rectangle(frame, point1, point2, color, 3, 8, 0);
		//delete P1;
		int b = frame.data[0];
		int g = frame.data[1];
		int r = frame.data[2];
                printf("\n %d %d %d", b,g,r);


	if(frame.empty()) break;
        //imshow("video", blurdst);
	imshow("Edited Frame", frame);
	imshow("Unedited Frame", oframe);
        if(waitKey(30) >= 0) break;
    }

    return 0;
}


