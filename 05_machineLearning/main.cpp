#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
using namespace cv::ml;

void mouseCallback(int event, int x, int y, int flags, void *userdata);

vector<Point> trainingPos, trainingNeg;
bool trainingState = true;

int main(int argc, char **argv)
{
    // Parse Input
    CommandLineParser parser(argc, argv,
    "{help h usage ?    |   | show this message}"
    "{image i           |   | (required) path to image }"
    );

    if (parser.has("help"))
    {
        parser.printMessage();
        return 0;
    }

    // Check for valid input
    //string imagepath(parser.get<string>("image"));
    string imagepath = "../../images/strawberry1.tif";
    if (imagepath.empty())
    {
        parser.printMessage();
        return -1;
    }

    // Read Images
    Mat img = imread(imagepath, IMREAD_COLOR);
    GaussianBlur(img, img, Size(5,5), 0, 0);

    if (img.empty())
    {
        cerr << "Could not read images" << endl;
        return -1;
    }
    Mat HSVimg;
    cvtColor(img, HSVimg, COLOR_RGB2HSV);

    // Create window
    namedWindow("Afbeelding", 1);
    setMouseCallback("Afbeelding", mouseCallback, NULL);
    imshow("Afbeelding", img);

    // Get Points
    cout << "********************************" << endl << "Click Positive training Points" << endl << "********************************" << endl;
    waitKey(0);
    trainingState = false;
    cout << "********************************" << endl << "Click Negative training Points" << endl << "********************************" << endl;

    waitKey(0);
    setMouseCallback("Afbeelding", NULL, NULL);

    // Create Descriptors & labels
    Mat Desc = Mat::zeros(trainingNeg.size()+trainingPos.size(), 3, CV_32FC1);
    Mat Label = Mat::zeros(trainingNeg.size()+trainingPos.size(), 1, CV_32SC1);
    int posSize = trainingPos.size();
    for(int i=0; i<posSize; i++)
    {
        //hsv waarde wordt opgevraard op die specifieke x-y coordinaten
        //deze hsv waarden dienen als descriptoren
        Vec3b px = HSVimg.at<Vec3b>(trainingPos[i]);

        Label.at<int>(i,0) = 1;
        Desc.at<float>(i,0) = px[0];
        Desc.at<float>(i,1) = px[1];
        Desc.at<float>(i,2) = px[2];
    }
    for (int i=0; i<trainingNeg.size(); i++)
    {
        Vec3b px = HSVimg.at<Vec3b>(trainingNeg[i]);
        Label.at<int>(posSize+i,0) = -1;
        Desc.at<float>(posSize+i,0) = px[0];
        Desc.at<float>(posSize+i,1) = px[1];
        Desc.at<float>(posSize+i,2) = px[2];
    }
    cout << "********************************" << endl << "Descriptors & Labels" << endl << "********************************" << endl;
    cout << Desc << endl << Label << endl << endl;

    waitKey(250);

    /// -------------------
    /// SVM
    /// -------------------
    Ptr<SVM> svm = SVM::create();
    svm->setType(SVM::C_SVC);
    svm->setKernel(SVM::LINEAR);
    svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 100, 1e-6));
    svm->train(Desc, ROW_SAMPLE, Label);

    Mat SVMMask = Mat::zeros(HSVimg.rows, HSVimg.cols, CV_8UC1);
    Mat SVNtempMat(1,3,CV_32FC1);
    Mat SVNtempLbl;
    for (int i = 0; i < HSVimg.rows; ++i)
        for (int j = 0; j < HSVimg.cols; ++j)
        {
            SVNtempMat.at<float>(0,0) = HSVimg.at<Vec3b>(i,j)[0];
            SVNtempMat.at<float>(0,1) = HSVimg.at<Vec3b>(i,j)[1];
            SVNtempMat.at<float>(0,2) = HSVimg.at<Vec3b>(i,j)[2];
            svm->predict(SVNtempMat, SVNtempLbl);

            if (SVNtempLbl.at<float>(0,0) == 1)
                SVMMask.at<uchar>(i,j) = 255;
        }
    imshow("SVM Mask", SVMMask);

    waitKey(0);

    /// -------------------
    /// K-nearest Neighbor
    /// -------------------
    Ptr<KNearest> knn = KNearest::create();
    knn->setIsClassifier(true);
    knn->setAlgorithmType(KNearest::BRUTE_FORCE);
    knn->setDefaultK(1);
    knn->train(Desc, ROW_SAMPLE, Label);

    Mat KNNMask = Mat::zeros(HSVimg.rows, HSVimg.cols, CV_8UC1);
    Mat KNNtempMat(1,3,CV_32FC1);
    Mat KNNtempLbl, KNNDistance;
    for (int i = 0; i < HSVimg.rows; ++i)
        for (int j = 0; j < HSVimg.cols; ++j)
        {
            KNNtempMat.at<float>(0,0) = HSVimg.at<Vec3b>(i,j)[0];
            KNNtempMat.at<float>(0,1) = HSVimg.at<Vec3b>(i,j)[1];
            KNNtempMat.at<float>(0,2) = HSVimg.at<Vec3b>(i,j)[2];
            knn->findNearest(KNNtempMat, 1, KNNtempLbl, KNNDistance);

            if (KNNtempLbl.at<float>(0,0) == 1)
                KNNMask.at<uchar>(i,j) = 255;
        }
    imshow("KNN Mask", KNNMask);


    waitKey(0);
    return 0;
}


void mouseCallback(int event, int x, int y, int flags, void *userdata)
{
    switch(event)
    {
        case EVENT_LBUTTONDOWN:
            if (trainingState)
                trainingPos.push_back(Point(x,y));
            else
                trainingNeg.push_back(Point(x,y));
            break;

        case EVENT_RBUTTONDOWN:
            if (trainingState)
            {
                if (trainingPos.size() != 0)
                    trainingPos.pop_back();
                else
                    cout << "No point to remove" << endl;
            }
            else
            {
                if (trainingNeg.size() != 0)
                    trainingNeg.pop_back();
                else
                    cout << "No point to remove" << endl;
            }
            break;

        case EVENT_MBUTTONDOWN:
            if (trainingState)
                cout << "Positive TrainingsPoints (" << trainingPos.size() << "):"  << endl << "  " << trainingPos << endl;
            else
                cout << "Negative TrainingsPoints (" << trainingNeg.size() << "):" << endl << "  " << trainingNeg << endl;
            break;

        default:
            break;
    }
}
