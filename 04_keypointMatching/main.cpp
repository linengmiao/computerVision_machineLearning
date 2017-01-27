#include <iostream>
#include<opencv2/opencv.hpp>


using namespace std;
using namespace cv;


Mat detectKeypoints(Mat input, int choiseAlgorithm);
Mat mergeImages(vector<Mat>& input);
Mat bruteForce(Mat inputImg, Mat imgTemplate);

// ./labo4  -p=../../images/kinderbueno_image.png -t=../../images/kinderbueno_object.png
/*int main(int argc, char** argv)
{

        Mat inputImg, imgTemplate;
        Mat imgInKypts, imgTemplKypts;
        Mat output;
        Mat outAkaze, outBrisk;

        vector<KeyPoint> kp;
        Mat image;
        vector<Mat> imagesToMerge;

        CommandLineParser parser(argc, argv,
        "{help usage ?  | |wrote help usage or ?}"
        "{path p        | |path to image}"
        "{template t   | | template}"
                        );

    if(parser.has("help"))
    {
        parser.printMessage();
        cerr<<"typed help"<<endl;
        return 0;
    }

    if(parser.has("p")) //path
    {


        string image_location(parser.get<string>("p"));
        if(image_location.empty()){cerr<<"fout in pad"<<endl; return -1;} //WERKT NIET

        cerr<<image_location<<endl;

        inputImg = imread(image_location,IMREAD_COLOR);


        if(parser.has("t"))
        {
          string image_location(parser.get<string>("t"));
          imgTemplate=imread(image_location,IMREAD_COLOR);
          cerr<<image_location<<endl;
          imshow("img template", imgTemplate); waitKey(0);

           imgInKypts = detectKeypoints(inputImg,2);
           imgTemplKypts = detectKeypoints(imgTemplate,2);

           imagesToMerge.push_back(imgTemplKypts);
           imagesToMerge.push_back(imgInKypts);

           output = mergeImages(imagesToMerge);


        //imshow("canvasImage",canvasImage);waitKey(0);
        //imshow("canvasTemplate",canvasTemplate); waitKey(0);
        //ORB zoekt naar zeer sterke edges

*/

        /*
        *
        *******


        detectand compute(im1, Mat(), keypoints1, descriptor_1);
        detectand compute(im1, Mat(), keypoints2, descriptor_2);

        //meest accurate distances hebben kleinste dinstance
        BFMatcher matcher(NORM_L2);
        vector<DMatch>matches;
        matcher.match(descriptor_1,descriptor_2,matches);

        double max_dist, min dist;

        //localize the object
        for(<descriptor_1)
        {double dist = matches}
        }
        ...
        vector<Point2f>obj;
        vector<Point2f>scene;
        for(<goodmatches)
        {
    obj,push_back
    scene.push_back()
        }
        Mat H= findHomography(obj, scene, RANSAC);

        //get corners from image 1


        //draw lines to connect
        line


    //RANSAK werkt op basis van rico
*******************/

//}
//}
//}


/*
* oefening 2 brute force matching
*
***********************************/
// ./labo4  -p=../../images/kinderbueno_image.png -t=../../images/kinderbueno_object.png


int main(int argc, char** argv)
{

        Mat inputImg, imgTemplate;
        Mat output;

        CommandLineParser parser(argc, argv,
        "{help usage ?  | |wrote help usage or ?}"
        "{path p        | |path to image}"
        "{template t   | | template}"
                        );

    if(parser.has("help"))
    {
        parser.printMessage();
        cerr<<"typed help"<<endl;
        return 0;
    }

//    if(parser.has("p")) //path
//    {


        //string image_location(parser.get<string>("p"));
        string image_location = "../../images/kinderbueno_image.png";
        if(image_location.empty()){cerr<<"fout in pad"<<endl; return -1;} //WERKT NIET

        cerr<<image_location<<endl;

        inputImg = imread(image_location,IMREAD_COLOR);


       // if(parser.has("t"))
       // {
          //string image_location(parser.get<string>("t"));
          image_location = "../../images/kinderbueno_object.png";
          imgTemplate=imread(image_location,IMREAD_COLOR);
          cerr<<image_location<<endl;
          imshow("img template", imgTemplate); waitKey(0);
          output = bruteForce(inputImg, imgTemplate);
       // }
//}
}

Mat bruteForce(Mat inputImg, Mat imgTemplate)
{

        Mat descriptor_1, descriptor_2;
        Ptr<ORB> detector = ORB::create();
        vector <KeyPoint> keypoints_1_ORB;
        vector <KeyPoint> keypoints_2_ORB;

        //detecteert keypoints en berekent overeenkomstige descriptoren
        detector-> detectAndCompute(inputImg, Mat(), keypoints_1_ORB, descriptor_1);
        detector-> detectAndCompute(imgTemplate, Mat(), keypoints_2_ORB, descriptor_2);

        //matching descriptor vectors wirh a brute force algortihm
		//The type of norm tells the BFMatcher how to compute the distances between every two features.
		//The NORM L1 is in general much faster to compute (mostly because you don't compute the square root). The NORM L2 is more accurate
        BFMatcher matcher(NORM_L2 );
        vector<DMatch> matches;
       // matcher.match(descriptor_1, matches, descriptor_2 ); //parameters: object-scene
        matcher.match(descriptor_2, descriptor_1, matches );

         cerr<<"in brute force"<<endl;

        //draw matches
        Mat output_matches;
        //drawMatches(inputImg, keypoints_1_ORB,imgTemplate, keypoints_2_ORB, matches, output_matches);
        drawMatches(imgTemplate, keypoints_2_ORB, inputImg, keypoints_1_ORB, matches, output_matches);

        //---> keypoints_2_ORB[i].pt is [0,-nan] ?<----
        //lijnen tekenen tussen matches
        /*
            HOEFT NIET FUNCTIE DRAWMATCHES TEKENT ALLE LIJNEN VOOR U
            for(int i=0;i<keypoints_2_ORB.size();i++){
            line(output_matches,keypoints_2_ORB[i].pt,keypoints_1_ORB[i].pt,Scalar(0,0,255));
            cerr<<keypoints_2_ORB[i].pt;
            cerr<<", ";
            cerr<<keypoints_1_ORB[i].pt;
            cerr<<endl;
        }*/
        //cerr<<"keypoints 2: ";
        //cerr<<keypoints_2_ORB;
             //show matches
        imshow("image matches", output_matches);
        waitKey(0);

        Mat img_matches;
        output_matches.copyTo(img_matches);

        double max_dist = 0;
        double min_dist = 100;

        //-- Quick calculation of max and min distances between keypoints
        for( int i = 0; i < descriptor_2.rows; i++ )
        {
            double dist = matches[i].distance;
            if( dist < min_dist ) min_dist = dist;
            if( dist > max_dist ) max_dist = dist;
        }

        printf("-- Max dist : %f \n", max_dist );
        printf("-- Min dist : %f \n", min_dist );
        //-- Draw only "good" matches (i.e. whose distance is less than 3*min_dist )
        std::vector< DMatch > good_matches;
        for( int i = 0; i < descriptor_2.rows; i++ )
        {
            if( matches[i].distance < 3*min_dist )
            {
                good_matches.push_back( matches[i]);
            }
        }

        //-- Localize the object
        std::vector<Point2f> obj_loc;
        std::vector<Point2f> img_loc;
        for( size_t i = 0; i < good_matches.size(); i++ )
        {
            //-- Get the keypoints from the good matches
            obj_loc.push_back( keypoints_2_ORB[ good_matches[i].queryIdx ].pt );
            img_loc.push_back( keypoints_1_ORB[ good_matches[i].trainIdx ].pt );
        }
        Mat H = findHomography( obj_loc, img_loc, RANSAC );

        //-- Get the corners from the image_1 ( the object to be "detected" )
        std::vector<Point2f> obj_corners(4);
        obj_corners[0] = cvPoint(0,0);
        obj_corners[1] = cvPoint( imgTemplate.cols, 0 );
        obj_corners[2] = cvPoint( imgTemplate.cols, imgTemplate.rows );
        obj_corners[3] = cvPoint( 0, imgTemplate.rows );
        std::vector<Point2f> scene_corners(4);
        perspectiveTransform( obj_corners, scene_corners, H);
        //-- Draw lines between the corners (the mapped object in the scene - image_2 )
        line( img_matches, scene_corners[0] + Point2f( imgTemplate.cols, 0), scene_corners[1] + Point2f( imgTemplate.cols, 0), Scalar( 0, 255, 0), 2 );
        line( img_matches, scene_corners[1] + Point2f( imgTemplate.cols, 0), scene_corners[2] + Point2f( imgTemplate.cols, 0), Scalar( 0, 255, 0), 2 );
        line( img_matches, scene_corners[2] + Point2f( imgTemplate.cols, 0), scene_corners[3] + Point2f( imgTemplate.cols, 0), Scalar( 0, 255, 0), 2 );
        line( img_matches, scene_corners[3] + Point2f( imgTemplate.cols, 0), scene_corners[0] + Point2f( imgTemplate.cols, 0), Scalar( 0, 255, 0), 2 );
        //-- Show detected matches
        imshow("final result", img_matches );waitKey(0);



        return output_matches;

}


Mat mergeImages(vector<Mat>& input)
{
/*
 * zorg ervoor dat de variabele canvasTemplate groot genoeg is! Anders failt deze functie
 */


        Mat canvasTemplate(input[1].rows, input[1].cols*2, CV_8UC3);
        cerr<<"made canvas"<<endl;
        imshow("merged images", canvasTemplate);waitKey(0);
        cerr<<"going to copy to canvas"<<endl;
        input[0].copyTo(canvasTemplate(Rect(input[1].cols, 0, input[0].cols, input[0].rows)));  // x,y, width, height
        input[1].copyTo(canvasTemplate(Rect(0,0, input[1].cols, input[1].rows)));
        cerr<<"copied canvas"<<endl;
        imshow("input[1] merged", canvasTemplate);waitKey(0);

       // Mat canvasImage(imgTemplate.rows, imgTemplate.cols *3, CV_8UC3 );

       return canvasTemplate;
}



Mat detectKeypoints(Mat input, int choiseAlgorithm)
{
        Mat output;

        switch(choiseAlgorithm){
            case 0:{
                Ptr<ORB> detector = ORB::create();// Ptr<ORB> detector2 = ORB::create();
                vector <KeyPoint> keypoints_1_ORB; //vector <KeyPoint> keypoints_2_ORB;
                detector-> detect(input, keypoints_1_ORB); //detector2-> detect(imgTemplate, keypoints_2_ORB);
                drawKeypoints(input, keypoints_1_ORB, output); //drawKeypoints(imgTemplate, keypoints_2_ORB, outTemplORB);
                //imshow("Kpts ORB image", output);waitKey(0);
                break;}
           case 1:{
                Ptr<AKAZE> detectorAkaze = AKAZE::create();
                vector <KeyPoint> keypoints_1_AKAZE;
                  detectorAkaze-> detect(input, keypoints_1_AKAZE);
                 drawKeypoints(input, keypoints_1_AKAZE, output);
                 // imshow("Kpts Akaze", out);waitKey(0);
                break;}
            case 2:{
                Ptr<BRISK> detectorBrisk = BRISK::create();
                vector <KeyPoint> keypoints_1_BRISK;
                detectorBrisk-> detect(input, keypoints_1_BRISK);
                drawKeypoints(input, keypoints_1_BRISK, output);
                // imshow("Kpts Brisk", out);waitKey(0);
                break;}
            default: {break;}

        }
        return output;
}





