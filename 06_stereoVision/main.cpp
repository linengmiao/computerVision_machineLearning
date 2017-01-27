#include <iostream>
#include<opencv2/opencv.hpp>
#include <fstream>

using namespace std;
using namespace cv;

float x1=0;
float x2=0;

void CallBackLeftImg(int event, int x, int y,int, void*param);
void CallBackRightImg(int event, int x, int y,int, void*param);

//./labo6
int main(int argc, char** argv)
{
    CommandLineParser parser(argc, argv,
        "{help usage ?  | |wrote help usage or ?}"
        "{path p        | |path to image}"
         );

    if(parser.has("help"))
    {
        parser.printMessage();
        cerr<<"typed help"<<endl;
        return 0;
    }

    //image linker camera
    string imgLocation = "../../imageleft.png";
    Mat imgL = imread(imgLocation);

    //image rechter camera
    imgLocation = "../../imageright.png";
    Mat imgR = imread(imgLocation);

    //rotatie matrices (2 per camera)
      FileStorage fs;
    fs.open("../../mx1.xml", FileStorage::READ);
    Mat mx1;
    fs["mx1"]>>mx1;

    fs.open("../../my1.xml", FileStorage::READ);
    Mat my1;
    fs["my1"]>>my1;

    fs.open("../../mx2.xml", FileStorage::READ);
    Mat mx2;
    fs["mx2"]>>mx2;

    fs.open("../../my2.xml", FileStorage::READ);
    Mat my2;
    fs["my2"]>>my2;


    remap(imgL, imgL, mx1, my1, CV_INTER_LINEAR);
    remap(imgR, imgR, mx2, my2, CV_INTER_LINEAR);

    namedWindow("remapped img left");
    setMouseCallback("remapped img left",CallBackLeftImg);
    imshow("remapped img left", imgL);

    cerr<<"debg"<<endl;

    //img moet zelfde naam als window hebben
    namedWindow("remapped img right");
    setMouseCallback("remapped img right",CallBackRightImg);
    imshow("remapped img right", imgR);waitKey(0);
    cerr<<"start measuring distances"<<endl;
    waitKey(0);


}

float calcDepth()
{
    float B = 12;///cm
    float f = 742.2;///pixels
    x1 = x1 - (320);
    //float x2 = pointR.x - (imL.cols/2);
    x2 = x2  - (320);
    float Z = (B*f)/(x1-x2);

    x1=0.0;
    x2=0.0;

    cerr<<Z<<endl;
    return Z;

}




void CallBackRightImg(int event, int x, int y,int, void*param)
{
    //cerr<<"callBack"<<endl;
    switch(event)
    {
        case CV_EVENT_LBUTTONDOWN:{
                cerr<<"right img"<<endl;
                x1=x;
                if(x2!=0)
                    calcDepth();
                break;
                }
    }
}

void CallBackLeftImg(int event, int x, int y,int, void*param)
{
    switch(event)
    {
        case CV_EVENT_LBUTTONDOWN:{
                cerr<<"left img"<<endl;
                x2 = x;
                if(x1!=0)
                    calcDepth();
                break;
                }
    }
}
