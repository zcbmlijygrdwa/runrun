#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
//#include <X11/extensions/XTest.h>

#include <cstdint>
#include <cstring>
#include <vector>
#include <iostream>

#include <stdlib.h>     /* system, NULL, EXIT_FAILURE */


#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;
using namespace std;
struct ScreenShot
{
    ScreenShot(int x, int y, int width, int height):
        x(x),
        y(y),
        width(width),
        height(height)
    {
        display = XOpenDisplay(nullptr);
        root = DefaultRootWindow(display);

        init = true;
    }

    void operator() (Mat& cvImg)
    {
        if(init == true)
            init = false;
        else
            XDestroyImage(img);

        img = XGetImage(display, root, x, y, width, height, AllPlanes, ZPixmap);

        cvImg = Mat(height, width, CV_8UC4, img->data);
    }

    ~ScreenShot()
    {
        if(init == false)
            XDestroyImage(img);

        XCloseDisplay(display);
    }

    Display* display;
    Window root;
    int x,y,width,height;
    XImage* img;

    bool init;
};


int main(int argc, char** argv)
{
    if(argc!=3)
    {
    cout<<"Usage: ./main 100 140"<<endl;
    return 1;
    }
    int startX = 0;
    int startY = 0;

    startX = atoi(argv[1]);
    startY = atoi(argv[2]);

    cout<<"startX = "<<startX<<endl;
    cout<<"startY = "<<startY<<endl;

    ScreenShot screen(startX,startY,300,200);

    Mat img;
    Mat img_gray;

    int intensity1 = 0;
    int intensity2 = 0;
    Point2f kp1(80,100);
    Point2f kp2(kp1.x,130);

    bool isJump = false;
    for(int i = 0 ; i < 100000000;i++)
    {
        isJump = false;

        screen(img);
        cvtColor(img, img_gray, cv::COLOR_RGB2GRAY);
        imshow("img_gray",img_gray);

        circle(img, kp1, 3, Scalar(255,0,0),CV_FILLED, 8,0);
        circle(img, kp2, 3, Scalar(255,0,0),CV_FILLED, 8,0);
        imshow("img",img);

        
        //process image
        intensity1 = (int)img_gray.at<uchar>(kp1.y,kp1.x);
        intensity2 = (int)img_gray.at<uchar>(kp2.y,kp2.x);
        cout<<intensity1<<endl;  

        if((intensity1<90) || (intensity2<90))
        {
            isJump = true;
        }

        if(isJump)
        {
            //simulate key press
            system ("../run.sh");
            cout<<"Jump!!"<<endl;
        }
        waitKey(1);

    }
    return 0;
}
