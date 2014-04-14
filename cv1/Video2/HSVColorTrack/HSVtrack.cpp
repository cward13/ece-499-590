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
    namedWindow("RGBFEED", 1);
    namedWindow("HSVFEED", 1);
    namedWindow("UneditedRGBFEED", 1);
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
		for(x=0;x<hsvframe.cols;x++)
		{
			int b = hsvframe.data[hsvframe.channels()*(hsvframe.cols*y + x) + 0];
			int g = hsvframe.data[hsvframe.channels()*(hsvframe.cols*y + x) + 1];
			int r = hsvframe.data[hsvframe.channels()*(hsvframe.cols*y + x) + 2];
			//int center=frame.data[frame.channels()*(frame.cols*y + x)];
			//if((140<b&&b<149) && (79<g&&g<86) && (40<r&&r<55))
			float sv=(float)g/r;
			//printf("%f",bluepercent);
			if(b>90 && b<120 && r>110 && r<250 && g>110 && g<210)// &&g>10 && r>185 && g<30 && r<215)
			{
				//If the specific blue are looking for is detected we will either put a square around the object
				//or mark it with a green dot or both  
				//printf("Blue detected");
				int xtemp=x;
				int counterx=0;
				CvPoint center = cvPoint(x,y);
				cv::Scalar color = cv::Scalar( 90, 200, 160 );
				circle( frame, center, 1, color, 3, 8, 0 );
				//NOTE: H=>b S=>g V=>r below we will find the largest x length and y length of an object and the location from which it was found
				//This will be useful later when we wish to put a box around the desired object 
				while(b>90 && b<120 && r>110 && r<250 && g>110 && g<210)
				{
					b = hsvframe.data[hsvframe.channels()*(hsvframe.cols*y + (xtemp+counterx)) + 0];
					g = hsvframe.data[hsvframe.channels()*(hsvframe.cols*y + (xtemp+counterx)) + 1];
					r = hsvframe.data[hsvframe.channels()*(hsvframe.cols*y + (xtemp+counterx)) + 2];
				
					counterx=counterx+1;
					//Update largest width when found
					if(counterx>maxlengthx)
					{
						maxlengthx=counterx;
						maxlengthlocx=xtemp;
						maxlengthlocxy=y;
					
					}
					int ytemp=y;
					int ycounter=0;
					int by=b;
					int gy=g;
					int ry=r;
					while(by>90 && by<120 && ry>110 && ry<250 && gy>160 && gy<230)
					{
						by = hsvframe.data[hsvframe.channels()*(hsvframe.cols*(ytemp+ycounter) + xtemp) + 0];
						gy = hsvframe.data[hsvframe.channels()*(hsvframe.cols*(ytemp+ycounter) + xtemp) + 1];
						ry = hsvframe.data[hsvframe.channels()*(hsvframe.cols*(ytemp+ycounter) + xtemp) + 2];
				
						ycounter=ycounter+1;
						//Update largest height when found
						if(ycounter>maxlengthy)
						{
							maxlengthy=ycounter;
							maxlengthlocy=ytemp;
						
						}
				
					}
				
				}
			
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
		rectangle(frame, point1, point2, color, 3, 8, 0);
		rectangle(hsvframe, point1, point2, color, 3, 8, 0);
		//delete P1;
                //printf("\n blue: %d %d green: %d %d red: %d %d",bt,btbgr,gt,gtbgr,rt,rtbgr);


	if(frame.empty()) break;
        //imshow("video", blurdst);
	imshow("RGBFEED", frame);
	imshow("HSVFEED", hsvframe);
	imshow("UneditedRGBFEED", oframe);
	
        if(waitKey(30) >= 0) break;
    }

    return 0;
}


