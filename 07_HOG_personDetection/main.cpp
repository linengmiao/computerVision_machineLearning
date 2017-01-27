#include <iostream>
#include<opencv2/opencv.hpp>
#include <fstream>

using namespace std;
using namespace cv;

Mat calculateHistogram(Mat inputImage);

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
    string imgLocation = "../../images/persoon1";
    cerr<<imgLocation<<endl;
    Mat input1 = imread(imgLocation);
    imshow("persoon 1", input1);waitKey(0);

    imgLocation = "../../images/persoon2";
    Mat input2 = imread(imgLocation);
    imshow("persoon 2", input2);waitKey(0);

    //SETUP HOG
    HOGDescriptor hog;
    hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
/2.4/modules/core/doc/drawing_functions.html

        vector<Rect> found, found_filtered;
        hog.detectMultiScale(input1, found, 0, Size(8,8), Size(32,32), 1.05, 2);
        size_t i, j;
        for (i=0; i<found.size(); i++)
        {
            Rect r = found[i];
            for (j=0; j<found.size(); j++)
                if (j!=i && (r & found[j]) == r)
                    break;
            if (j== found.size())
                found_filtered.push_back(r);
        }
            Rect r = found_filtered[0];
            r.x += cvRound(r.width*0.1);
		    r.width = cvRound(r.width*0.8);
		    r.y += cvRound(r.height*0.07);
		    r.height = cvRound(r.height*0.8);
		    rectangle(input1, r.tl(), r.br(), Scalar(0,255,0), 3);

            Mat roi = Mat(input1,r);
            imshow("roi",roi); waitKey(0);

            imshow("opencv", input1); waitKey(0);

            cvtColor(roi.clone(),roi,COLOR_BGR2HSV);
            Mat histRoi = calculateHistogram(roi);

            Rect r2;
            r2.x += cvRound(r.width*0.1);
		    r2.width = cvRound(r.width*0.8);
		    r2.y += cvRound(r.height*0.07);
		    r2.height = cvRound(r.height*0.8);

            for (int i=0;i<input2.rows;i+=1){
                for(int j=0;j<input2.cols-r2.width;j+=5){

                    Mat tempCloneWithMovRec = input2.clone();
                    Point coord1(r2.x , r2.y);
                    Point coord2(r2.x+r2.width , r2.y+r2.height);
                    /*rectangle(tempCloneWithMovRec, coord1, coord2, Scalar(0,0,255));
                    imshow("match", tempCloneWithMovRec); waitKey(0);*/

                    Mat schuifWindow = Mat(input2, r2); //returnt rechthoek uit afbeelding
                    Mat histWindowSchuif = calculateHistogram(schuifWindow);
                    r2.x =  r2.x+5;

                    double resultComparison = compareHist(histWindowSchuif, histRoi, CV_COMP_CORREL);
                    cerr<<resultComparison<<endl;

                    if((resultComparison>-0.01)&&(resultComparison<0.01))
                    {cerr<<"found match"<<endl;
                    rectangle(tempCloneWithMovRec, coord1, coord2, Scalar(0,0,255));
                    imshow("match", tempCloneWithMovRec);
                    waitKey(0);}

                }
                 r2.x = 0;
                 r2.y  = r2.y +1;
            }




}


Mat calculateHistogram(Mat inputImage)
{

        bool uniform = true; bool accumulateParam = false;

        Mat b_hist;
        int histSize = 256;
        float range[] = { 0, 256 } ;
        const float* histRange = { range };
        int hist_w = 512; int hist_h = 400;
        int bin_w = cvRound( (double) hist_w/histSize );
        int channels[] = {0, 1};

        // Compute the histograms:

        calcHist(&inputImage, 1, channels, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulateParam );
        Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );


        // Normalize the result to [ 0, histImage.rows ]
        normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
        for( int i = 1; i < histSize; i++ )
        {
         line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
                       Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
                       Scalar( 255, 0, 0), 2, 8, 0  );
        }
        namedWindow("calcHist Demo", CV_WINDOW_AUTOSIZE );
        //imshow("calcHist Demo", histImage );

       // waitKey(0);


        return b_hist;
}
