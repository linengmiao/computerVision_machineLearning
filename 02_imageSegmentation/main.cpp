#include <iostream>
#include<opencv2/opencv.hpp>

/*
*
*   DIT IS LABO 2!!!!!!!
*
*
******************************/

using namespace std;
using namespace cv;

int a;

void callBackTrackBar(int a, void*)
{
       cerr<<a<<endl;
}

//./labo3 -p=../../images/sign.png
/*
int main(int argc, char** argv)
{

        Mat image;
        Mat hsv_color;
        vector<Mat>channels;
        vector<Mat> channelHSV;
        Mat R_th, G_th, B_th;
        Mat H_1, H_2;
        Mat img_hsv;
        Mat canvas = image.clone();
        vector<Mat>contours;
        vector<Vec4i> hierarchy;
        vector<vector<Point> >hull(contours.size());

        CommandLineParser parser(argc, argv,
        "{help usage ?  | |wrote help usage or ?}"
        "{path p        | |path to image}"
        "{threshold t   | |segments skin pixels image using given formula(github)}"
                        );

    if(parser.has("help"))
    {
        parser.printMessage();
        cerr<<"typed help"<<endl;
        return 0;
    }

    if(parser.has("p")) //path
    {


        //image locatie bepalen
        string image_location(parser.get<string>("p"));
        if(image_location.empty()){cerr<<"fout in pad"<<endl; return 0;} //WERKT NIET
        cout<<image_location<<endl;

        //image binnenlezen
        image = imread(image_location,IMREAD_COLOR);

        //split image to RGB
        split(image, channels);
        cerr<<"splitting image in R,G and, B component"<<endl;

        Mat Blue = channels[0];
        Mat Green = channels[1];
        Mat Red = channels[2];

        //
        // opdracht 1:
        //      voordelen:
        //      nadelen: threshold waardes moeten voor elke foto
        //      telkens ingseteld worden
        //
        threshold(Red, R_th, 155, 255, THRESH_BINARY);
        threshold(Green, G_th, 155, 255, THRESH_BINARY);
        threshold(Blue, B_th, 155, 255, THRESH_BINARY);

        //conversion from rgb to hsv
        //
        // opdracht 2:
        //      Voordelen: HSV als de belichting verandert heb je daar geen last van daar enkel de S en V channels
        //                 veranderen.
        //      Nadelen: image is niet visualiseerbaar in HSV (kleuren kloppen niet meer)
        //

        //convert RGB->HSV
        cvtColor(image,img_hsv,COLOR_BGR2HSV);
        imshow("HSV image", img_hsv);
        waitKey(0);

        //split in H,S and V-channel
        split(img_hsv, channelHSV);
        Mat H = channelHSV[0];
        Mat S = channelHSV[1];
        Mat V = channelHSV[2];

        //ranges manueel instellen om zo het bord eruit te halen
        // inrange returnt een binair masker: 1 als het in de range is, 0 als het erbuiten valt.
        // het masker heet H1, H2 etc...
        imshow("H", H);waitKey(0);
        inRange(H,160,180, H_1);
        imshow("H_1", H_1);waitKey(0);
        inRange(H,0,10, H_2);
        imshow("H_2", H_2);waitKey(0);
        inRange(V,150,255, V);

        //masker samenstellen
        H = (H_1|H_2) & V;  //------------------>NUT??<------------------

        //noise eruit halen. Masker bewerken
        erode(H, H, Mat(), Point(-1,-1), 1);
        dilate(H, H, Mat(), Point(-1,-1), 3);

        imshow("H mask", H); waitKey(0);


    }

}
*/

/**********
*
* Opdracht 2
*
*************************************************/



int main(int argc, char** argv)
{

        Mat image;
        Mat hsv_color;
        vector<Mat>channels;
        vector<Mat> channelHSV;
        Mat H_th, S_th, V_th;
        Mat H_1, H_2;
        Mat img_hsv;
        Mat canvas = image.clone();
        vector<Mat>contours;
        vector<Vec4i> hierarchy;

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

    if(parser.has("p")) //path
    {

        //image locatie bepalen
        //string image_location(parser.get<string>("p"));
		string image_location = "../../images/sign.png";


        //image binnenlezen
        image = imread(image_location,IMREAD_COLOR);

        //convert RGB->HSV
        cvtColor(image,img_hsv,COLOR_BGR2HSV);

        //---> Waarom thresholden?<------
        //maskers zijn output
		//INRANGE: deze functie vergelijkt met een array die de lower en upperbound voorstellen
		// returnt 0 of 255. Als het in de range is returnt het 255 else 0.
		//threshold: in tegenstelling tot inrange vergelijkt deze functie met een of meerdere(?) double waarden.
        Mat mask1, mask2;
		//    threshold(img_hsv, H_th, sliderUpperH,255,THRESH_BINARY);
        inRange(img_hsv, Scalar(0, 70, 0), Scalar(10, 255, 255), mask1); //zwart - rood
        inRange(img_hsv, Scalar(170, 70, 0), Scalar(180, 255, 255), mask2);//zwart  -magenta
        Mat masker = mask1 | mask2;


        //erode en dilate enkel op kleurenkanaal
        //(I, Q, kernel, anker?, #iteraties)
        erode(masker.clone(), masker, Mat(), Point(-1,-1), 1);
        dilate(masker.clone(), masker, Mat(), Point(-1,-1), 2 );
        erode(masker.clone(), masker, Mat(), Point(-1,-1), 1);

        // hierarchy contains information about the image topology
        // CV_RETR_TREE retrieves all of the contours and reconstructs a full hierarchy of nested contours.
        // CV_RETR_EXTERNAL retrieves only the extreme outer contours.
        // Each contour is stored as a vector of points.
        // clone v H meegeven daar findcountours inputafbeelding wijzigt!!
       // findContours( res_hsv.clone(), contours, CV_RETR_TREE, CHAIN_APPROX_NONE);

        //vector<vector<Point> >hull(contours.size());
        //bepaalt de convex hull ven een reeks punten (bepaald adhv findContours)
        //for( int i = 0; i < contours.size(); i++ )
        //{convexHull( Mat(contours[i]), hull[i], false );} //parameters: input, output,

        // Draw contour
        // convex hull = binnenkant vd edges
       // Mat contoursMask = Mat::zeros( res_hsv.size(), res_hsv.type() );
       // drawContours( contoursMask, contours, -1, (0,0,255));
       // drawContours( contoursMask, hull, -1, (0,0,255));  //-1 als contourIndex tekent alle contours
       // for(int i=0;i<contours.size();i++){
       //     drawContours( contoursMask, contours, i, Scalar(0,200,255), 2, FILLED);
       // }


        split(img_hsv, channelHSV);

        Mat H = channelHSV[0];
        Mat S = channelHSV[1];
        Mat V = channelHSV[2];

        cerr<<"H type"<<endl;
        cerr<<H.type()<<endl;

        cerr<<"masker type"<<endl;
        cerr<<masker.type()<<endl;

        Mat enkelBord = Mat::zeros(H.size(), CV_8UC1);

        enkelBord = H & masker;
        S = S & masker;
        V = V & masker;


        vector<Mat> array_to_merge;
        array_to_merge.push_back(enkelBord);
        array_to_merge.push_back(S);
        array_to_merge.push_back(V);

        Mat finalOutput;
        merge(array_to_merge, finalOutput);

        imshow("masker", masker);waitKey(0); //1 kanaals
        imshow("img_hsv", img_hsv);waitKey(0); //3 kanaals
        cvtColor(finalOutput,finalOutput,COLOR_HSV2BGR);
        imshow("finalOutput", finalOutput);waitKey(0);


        //HSV is niet deftig visualiseerbaar, dus terug cvt in RGB
        //cvtColor(andedMatricesQ,andedMatricesQ,COLOR_HSV2RGB);
        //imshow("andedMatricesQ", andedMatricesQ);waitKey(0);
        //imshow("myOutput", myOutput);waitKey(0);

        // Show in a window
        // namedWindow( "windowName", 1 );
        //add trackbar


       // imshow("windowName", contoursMask);
       // createTrackbar("set colors", "windowName", &a, 255, callBackTrackBar );
       // waitKey(0);

       }
}




