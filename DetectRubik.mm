//
//  DetectRubik.cpp
//  Cocoa OpenGL
//
//  Created by Jean-Simon Lapointe on 2013-02-01.
//
//

#include "DetectRubik.h"
#include "RubikUtil.h"


#include <algorithm>
#include <set>

//#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

const std::string window_name("MyWin");
const std::string window_name2("MyWin2");


typedef std::map <RubikColor, Mat> colImgMap;
typedef std::map <RubikColor, int> colIntMap;
RubikColor allColorStat[] = {white, green, red , blue ,orange,    yellow};
std::vector< RubikColor > allCol(allColorStat, allColorStat+6);

Mat detectColor(Mat src, RubikColor col);
Mat detectColorYUV(Mat src, RubikColor col);

int isolateSquares(Mat &src);

void saveImageMap(const colImgMap& imMap, const std::string & prefix);
bool LineIntersectsLine(cv::Point l1p1, cv::Point l1p2, cv::Point l2p1, cv::Point l2p2, cv::Point &res);

cv::Rect keepLargestComponent(Mat &img)
{
    std::vector<std::vector<cv::Point> > contours;

    findContours(img, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);
    
    int maxContourInd = 0;
    double maxContourVal = -1.0;
    for (int i = 0; i< contours.size(); i++)
    {
        double curArea = contourArea(contours[i]);
        if (curArea > maxContourVal)
        {
            maxContourVal = curArea;
            maxContourInd = i;
        }
    }
    
    return cv::boundingRect(contours[maxContourInd]);
}



std::vector<cv::Point> LineIntersectsRect2(cv::Point p1, cv::Point p2, cv::Rect r)
{
    std::vector<cv::Point> retVal;
    cv::Point interP;
    if (LineIntersectsLine(p1, p2, cv::Point(r.x, r.y), cv::Point(r.x + r.width, r.y), interP) )
        retVal.push_back(interP);
    
    if (LineIntersectsLine(p1, p2, cv::Point(r.x + r.width, r.y), cv::Point(r.x + r.width, r.y + r.height), interP) )
        retVal.push_back(interP);
    
    if (LineIntersectsLine(p1, p2, cv::Point(r.x + r.width, r.y + r.height), cv::Point(r.x , r.y + r.height), interP) )
        retVal.push_back(interP);
    
    if (LineIntersectsLine(p1, p2, cv::Point(r.x, r.y + r.height), cv::Point(r.x, r.y), interP) )
        retVal.push_back(interP);
    
    return retVal;
}

std::vector<cv::Point> LineIntersectsRect(const cv::Vec2f& line,const cv::Rect &r)
{
    float rho = line[0];
    float theta = line[1];
    
    
    double a = cos(theta), b = sin(theta);
    double x0 = a*rho, y0 = b*rho;
    cv::Point pt1(cvRound(x0 + 1000*(-b)),
              cvRound(y0 + 1000*(a)));
    cv::Point pt2(cvRound(x0 - 1000*(-b)),
              cvRound(y0 - 1000*(a)));
    
    return LineIntersectsRect2(pt1, pt2, r);
}


bool LineIntersectsLine(cv::Point l1p1, cv::Point l1p2, cv::Point l2p1, cv::Point l2p2, cv::Point &res)
{
    cv::Point l1Vec = l1p2-l1p1;
    cv::Point l2Vec = l2p2-l2p1;
    
    double denom = l1Vec.cross(l2Vec);
    if (denom == 0.0)
        return false;
    double t = (l2p1-l1p1).cross(l2Vec)/l1Vec.cross(l2Vec);
    double u = (l1p1-l2p1).cross(l1Vec)/l2Vec.cross(l1Vec);
    
    if( t < 0 || t > 1 || u < 0 || u > 1 )
    {
        return false;
    }
    
    res = cv::Point(l1p1.x + t*l1Vec.x, l1p1.y + t*l1Vec.y);
    return true;
}

template <typename T>
T clip(const T& n, const T& lower, const T& upper) {
    return std::max(lower, std::min(n, upper));
}

int handleError( int status, const char* func_name,
                const char* err_msg, const char* file_name,
                int line, void* userdata )
{
    //Do nothing -- will suppress console output
    return 0;   //Return value is not used
}

void drawLine(Vec2f line, Mat &img, Scalar rgb = CV_RGB(0,0,255), int thickness = 1)
{
    if(line[1]!=0)
    {
        float m = -1/tan(line[1]);
        
        float c = line[0]/sin(line[1]);
        
        cv::line(img, cv::Point(0, c), cv::Point(img.size().width, m*img.size().width+c), rgb, thickness);
    }
    else
    {
        cv::line(img, cv::Point(line[0], 0), cv::Point(line[0], img.size().height), rgb, thickness);
    }
    
}

std::pair<cv::Point, cv::Point> getPoints(const Vec2f& line, const cv::Rect &bBox)
{
    float p = line[0];
    float theta = line[1];
    
    cv::Point pt1, pt2;
    if(line[1]>CV_PI*45/180 && line[1]<CV_PI*135/180)
    {
        pt1.x=bBox.tl().x;
        pt1.y=-pt1.x/tan(theta) + p/sin(theta);
        pt2.x=bBox.br().x;
        pt2.y=-pt2.x/tan(theta) + p/sin(theta);
    }
    else
    {
        pt1.y=bBox.tl().y;
        pt1.x=-pt1.y/tan(theta) + p/cos(theta);
        pt2.y=bBox.br().y;
        pt2.x=-pt2.y/tan(theta) + p/cos(theta);
    }
    return std::make_pair(pt1, pt2);
}

bool lineIsValid(const cv::Vec2f & l)
{
    return !(l[0]==0 && l[1]==-100);
}
void setInvalid(cv::Vec2f & l)
{
    l[0] = 0;
    l[1] = -100;
}

void mergeLines(const std::vector<cv::Vec2f> & inputLines, cv::Rect bbox, std::vector<cv::Vec2f> & outpuLines)
{
    std::vector<cv::Vec2f> lineCopy = inputLines;
    for(int i=0;i<lineCopy.size();i++)
    {
        for(int j=i+1;j<lineCopy.size();j++)
        {
            Vec2f & l1 = lineCopy[i];
            Vec2f & l2 = lineCopy[j];
            
            if(!lineIsValid(l1)) continue;
            if(!lineIsValid(l2)) continue;
            
            auto ppair1 = LineIntersectsRect(l1, bbox);
            auto ppair2 = LineIntersectsRect(l2, bbox);
            if (ppair1.size() != 2 || ppair2.size() != 2)
            {
                printf("Error\n");
                continue;
            }
            cv::Point & pt1 = ppair1[0];
            cv::Point & pt2 = ppair1[1];
            cv::Point & pt1current = ppair2[0];
            cv::Point & pt2current = ppair2[1];
            
            if(((double)(pt1.x-pt1current.x)*(pt1.x-pt1current.x) + (pt1.y-pt1current.y)*(pt1.y-pt1current.y)<64*64) &&
               ((double)(pt2.x-pt2current.x)*(pt2.x-pt2current.x) + (pt2.y-pt2current.y)*(pt2.y-pt2current.y)<64*64))
            {
                setInvalid(l2);
            }
        }
    }

    outpuLines.clear();
    for(int i=0;i<lineCopy.size();i++)
    {
        if (lineIsValid(lineCopy[i]))
            outpuLines.push_back(lineCopy[i]);
    }
}

void keepBiggestBlob(const cv::Mat &blobsImage, cv::Mat &resultImage, cv::Rect &blobRect)
{
    Mat imageCpy = blobsImage.clone();
    Mat kernel = (Mat_<uchar>(3,3) << 0,1,0,1,1,1,0,1,0);
    dilate(imageCpy, imageCpy, kernel);
    
    Mat labels, stats, centers;
    int nbLabels = connectedComponentsWithStats(imageCpy, labels, stats, centers);
    int biggestBlob = 0;
    int biggestArea = 0;
    for (int i = 1; i< nbLabels; ++i)
    {
        int curArea = stats.at<unsigned int>(i, CC_STAT_AREA);
        if (curArea > biggestArea)
        {
            biggestBlob = i;
            biggestArea = curArea;
        }
    }
    
    blobRect = cv::Rect(cv::Point(stats.at<unsigned int>(biggestBlob, CC_STAT_LEFT), stats.at<unsigned int>(biggestBlob, CC_STAT_TOP)),
                    cv::Size(stats.at<unsigned int>(biggestBlob, CC_STAT_WIDTH), stats.at<unsigned int>(biggestBlob, CC_STAT_HEIGHT)) );
    imageCpy = (labels == biggestBlob);
    
    erode(imageCpy, resultImage, kernel);
}


void convertToSegments(const Vec2f& line, cv::Mat binImage, std::pair<cv::Point, cv::Point> & segment)
{
    float rho = line[0];
    float theta = line[1];

    // dilate to keep continuity, dont forget to remove that length from the segment.
    Mat dilatedBinImage;
    const int dilateIter = 8;
    cv::dilate(binImage, dilatedBinImage, Mat(), cv::Point(-1, -1), dilateIter);
    
    Mat drawnLine(binImage.size(), CV_8UC1);
    drawnLine = Scalar(0);
    drawLine(line, drawnLine, Scalar(255), 10);
    drawnLine = drawnLine&dilatedBinImage;
    
    Mat lineBigPiece;
    cv::Rect cubeBB;
    keepBiggestBlob(drawnLine, lineBigPiece, cubeBB);

    //cv::erode(lineBigPiece, lineBigPiece, Mat(), cv::Point(-1, -1), dilateIter);

    cv::Point2d lineP(rho*cos(theta), rho*sin(theta));
    float lineAngle = theta + CV_PI/2;
    cv::Point2d lineVec(cos(lineAngle), sin(lineAngle));
    
    vector<cv::Point> locations;
    cv::findNonZero(lineBigPiece, locations);
    float minProj = 999999;
    float maxProj = -999999;
    for (int i = 0; i< locations.size(); ++i)
    {
        cv::Point2d p = cv::Point2d(locations[i])-lineP;
        float curProj = p.dot(lineVec);
        minProj = std::min(minProj, curProj);
        maxProj = std::max(maxProj, curProj);
    }
    
    minProj += dilateIter;
    maxProj -= dilateIter;
    
    cv::Point2d minPos = lineP + minProj*lineVec;
    cv::Point2d maxPos = lineP + maxProj*lineVec;
    
    segment.first = minPos;
    segment.second = maxPos;

//    Mat dispIm(binImage.size(), CV_8UC3);
//    dispIm = Scalar(0);
//    dispIm.setTo(Scalar(255, 255, 255), binImage);
//    dispIm.setTo(Scalar(255, 0, 0), lineBigPiece);
//    drawLine(line, dispIm, Scalar(0, 0, 255));
//    circle(dispIm, cv::Point(minPos), 6, Scalar(0, 255, 0), 2 );
//    circle(dispIm, cv::Point(maxPos), 6, Scalar(0, 255, 0), 2 );
//    
//    imshow( window_name2, dispIm );                   // Show our image inside it.
//    waitKey(0);

}

void testcv()
{
    cv::redirectError(handleError);

    Mat im3 = imread("rubik2.jpg");
    
    
    Mat im1;
    resize(im3, im1, cv::Size(), 0.25, 0.25);

    Mat grayImage;
    cv::cvtColor(im1, grayImage, COLOR_RGB2GRAY);
    
    GaussianBlur(grayImage, grayImage, cv::Size(11,11), 0);

    Mat borders(grayImage.size(), CV_8UC1);
    adaptiveThreshold(grayImage, borders, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 5, 2);

    bitwise_not(borders, borders);
    
    Mat bordersBig;
    cv::Rect cubeBB;
    keepBiggestBlob(borders, bordersBig, cubeBB);
    borders = bordersBig;

    namedWindow( window_name, WINDOW_AUTOSIZE );
    namedWindow( window_name2, WINDOW_AUTOSIZE );

    int expectedLineThickness = 12;
    
    vector<Vec2f> curLines;
    HoughLines(borders, curLines, 1, CV_PI/180, 200);


    vector<Vec2f> realLines;
    mergeLines(curLines, cubeBB, realLines);
    
    Mat dispIm = im1.clone();
    for(int i=0;i<realLines.size();i++)
    {
        Vec2f & l1 = realLines[i];
        drawLine(l1, dispIm, CV_RGB(0,0,128));
    }

    imshow( window_name, dispIm );                   // Show our image inside it.
    imshow( window_name2, borders );                   // Show our image inside it.
    waitKey(0);
    
    
    // for each line, we want to know the line segment: compute the two endpoint
    //void convertToSegments(const Vec2f& line, cv::Mat binImage, std::pair<cv::Point, cv::Point> & segment)
    std::vector <std::pair<cv::Point, cv::Point>> segments;
    vector< cv::Point2f > lineAngles;
    for(int i=0;i<realLines.size();i++)
    {
        Vec2f & l1 = realLines[i];
        
        double angle = 2*l1[1];
        lineAngles.push_back(cv::Point2f(cos(angle), sin(angle)));
        std::pair<cv::Point, cv::Point> seg;
        convertToSegments(l1, borders, seg);
        segments.push_back(seg);
    }
    
    // now, we handle the case if we see 3 faces.
    Mat labels, centers;
    kmeans(Mat(lineAngles), 3, labels, TermCriteria( TermCriteria::EPS+TermCriteria::COUNT, 10, 0.5), 3, KMEANS_PP_CENTERS, centers);
    Scalar cols[] = {
        Scalar(255, 0, 0),
        Scalar(0, 255, 0),
        Scalar(0, 0, 255)
    };
    
    dispIm = im1.clone();
    dispIm.setTo(Scalar(0), borders);
    for(int i=0;i<segments.size();i++)
    {
        cv::Point p1 = segments[i].first;
        cv::Point p2 = segments[i].second;
        line(dispIm, p1, p2, cols[labels.at<int>(i)], 2);
        circle(dispIm, p1, 5, Scalar(255, 255, 0), 2);
        circle(dispIm, p2, 5, Scalar(255, 255, 0), 2);
    }
    imshow( window_name, im1 );                   // Show our image inside it.
    imshow( window_name2, dispIm );                   // Show our image inside it.
    waitKey(0);


    
    // add segment for the borders if not present. (convex hull??)
    
    //find the four points where we have thre segments meeting.
    
    //
    
}

int isolateSquares(Mat &src)
{
    imwrite("src1.jpg", src);

    cv::Mat element = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3));
    const int nbDilate = 3;
    
    for (int i =0; i< nbDilate; i++)
        cv::dilate(cv::Mat(src), cv::Mat(src), element);
    imwrite("src2.jpg", src);
    for (int i =0; i< nbDilate; i++)
        cv::erode(cv::Mat(src), cv::Mat(src), element);
    imwrite("src3.jpg", src);

    
    // threashold specifying minimum area of a blob
    double thresholdLow = src.size().width*src.size().height*0.002;
    double thresholdHigh = src.size().width*src.size().height*0.10;
    
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    std::vector<int> invalidBlobs;
    double contour_area;
    cv::Mat temp_image;
    
    // find all contours in the binary image
    cv::Mat(src).copyTo(temp_image);
    findContours(temp_image, contours, noArray(), RETR_CCOMP,
                 CHAIN_APPROX_SIMPLE);
    
    // Find indices of contours whose area is less than `threshold`
    if ( !contours.empty()) {
        for (size_t i=0; i<contours.size(); ++i) {
            contour_area = contourArea(contours[i]) ;
            if ( contour_area < thresholdLow || contour_area > thresholdHigh)
                invalidBlobs.push_back(i);
        }
    }
    
    // fill-in all small contours with zeros
    for (size_t i=0; i < invalidBlobs.size(); ++i) {
        drawContours(src, contours, invalidBlobs[i], cv::Scalar(0),
                     FILLED, 8);
    }
    imwrite("src4.jpg", src);

    return contours.size() - invalidBlobs.size();
    
}

void saveImageMap(const colImgMap& imMap, const std::string & prefix)
{
   // for (auto it = imMap.begin(); it != im)

}


Mat detectColor(Mat src, RubikColor col)
{
    Mat retVal;
    Mat imgHSV(src.size(), 8, 3);
    cvtColor(src, imgHSV, COLOR_BGR2HSV);
    imwrite("imgHSV.jpg", imgHSV);

    
    if (col == red)
    {
        Mat imgThreshed1(src.size(), 8, 1);
        inRange(imgHSV, Scalar(173, 100, 100), Scalar(180, 255, 255), imgThreshed1);
        
        Mat imgThreshed2(src.size(), 8, 1);
        inRange(imgHSV, Scalar(0, 100, 100), Scalar(5, 255, 255), imgThreshed2);

        Mat imgThreshedRes(src.size(), 8, 1);
        imgThreshedRes = (imgThreshed1 | imgThreshed2);
        retVal = imgThreshedRes;
    }
    else
    {
        Scalar minVal, maxVal;
        switch (col)
        {
            case blue:
                minVal = Scalar(100, 100, 100);
                maxVal = Scalar(140, 255, 255);
                break;
                
            case yellow:
                minVal = Scalar(20, 100, 100);
                maxVal = Scalar(30, 255, 255);
                break;
                
            case orange:
                minVal = Scalar(7, 100, 100);
                maxVal = Scalar(22, 255, 255);
                break;
                
            case white:
                minVal = Scalar(0, 0, 100);
                maxVal = Scalar(50, 40, 255);
                break;
                
            case green:
                minVal = Scalar(50, 80, 100);
                maxVal = Scalar(100, 255, 255);
                break;
                
        }
        
        Mat imgThreshed(src.size(), 8, 1);
        inRange(imgHSV, minVal, maxVal, imgThreshed);
        imwrite("imgThreshed.jpg", imgThreshed);
        retVal = imgThreshed;
    }
    
    
    //cvReleaseImage(&imgHSV);
    return retVal;
}

Mat detectColorYUV(Mat src, RubikColor col)
{
    Scalar minVal, maxVal;
    switch (col)
    {
        case red:
            minVal = Scalar(50, 110-38, 205-45);
            maxVal = Scalar(140, 110+38, 205+45);
            break;
            
        case orange:
            minVal = Scalar(140, 100-48, 205-45);
            maxVal = Scalar(210, 100+48, 205+45);
            break;
            
        case blue:
            minVal = Scalar(70, 195-55, 55-55);
            maxVal = Scalar(240, 195+55, 55+55);
            break;
            
            
        case green:
            minVal = Scalar(70, 70-55, 60-50);
            maxVal = Scalar(240, 70+55, 60+50);
            break;
            
        case white:
            minVal = Scalar(190, 128-12, 128-11);
            maxVal = Scalar(255, 128+12, 128+11);
            break;
            
        case yellow:
            minVal = Scalar(118, 90-25, 140-20);
            maxVal = Scalar(150, 90+25, 140+20);
            break;
            
    }
    
    Mat imgThreshed(src.size(), 8, 1);
    inRange(src, minVal, maxVal, imgThreshed);
    imwrite("imgThreshed.jpg", imgThreshed);
    Mat retVal = imgThreshed;
    return retVal;
}
