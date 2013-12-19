#include <iostream>                              //	for standard I/O
#include <string>							     // for strings
#include <opencv2/core/core.hpp>				 // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/imgproc/imgproc.hpp>           // for image pre-processing
#include <opencv2/highgui/highgui.hpp>           // OpenCV window I/O
#include <opencv2/calib3d/calib3d.hpp>           // OpenCV calibration
#include <opencv2/video/video.hpp>           // OpenCV calibration
#include <opencv2/features2d/features2d.hpp>

using namespace std;
using namespace cv;

void initial(Mat& mRgb,Mat& imgHSV,Mat& des_object,vector<KeyPoint>& kp_object,vector<Point2f>& obj_corners){

	Mat object = imread( "/sdcard/credit_card.png", CV_LOAD_IMAGE_GRAYSCALE );

	//Detect the keypoints using SURF Detector
	int minHessian = 500;
	OrbFeatureDetector detector( minHessian );
	detector.detect( object, kp_object );

	//Calculate descriptors (feature vectors)
	OrbDescriptorExtractor extractor;
	extractor.compute( object, kp_object, des_object );

	//Get the corners from the object
	obj_corners[0] = cvPoint(0,0);
	obj_corners[1] = cvPoint( object.cols, 0 );
	obj_corners[2] = cvPoint( object.cols, object.rows );
	obj_corners[3] = cvPoint( 0, object.rows );

	cvtColor(mRgb, imgHSV, CV_BGR2HSV);
}


void calibrate(Mat& mRgb,Mat& warpMat){

    rectangle(mRgb, Point2i(0,0), Point2i(mRgb.cols,mRgb.rows), Scalar(255,255,255),CV_FILLED);
    int dots_cols=mRgb.cols/128;
    int dots_rows=mRgb.rows/128;
    vector<Point2f> points;
    for(int j=0;j<dots_rows;j++){
        for(int i=0;i<dots_cols;i++){
                Point point=Point2f(128*i+64,128*j+64);
                points.push_back(point);
                circle(mRgb, point, 48, Scalar(0,0,0),CV_FILLED);
        }
    }

    vector<Point2f> realPoints;
    Mat grayImage;
    cvtColor(mRgb, grayImage, CV_BGR2GRAY);
    bool found=findCirclesGrid(grayImage, Size(dots_cols,dots_rows),realPoints,CALIB_CB_SYMMETRIC_GRID);
    if(found){
 //       cout<< "realPoints" << realPoints<<endl;
 //       cout<< "points" << points<<endl;

//        imshow("corneers", rawImage);
//        sort(points.begin(), points.end(), xsorter);
//        sort(points.begin(), points.end(), ysorter);
//        sort(realPoints.begin(), realPoints.end(), xsorter);
//        sort(realPoints.begin(), realPoints.end(), ysorter);
        warpMat=findHomography(realPoints, points);
//        warpMat=findHomography(points,realPoints);
       //cout<< "Warp mat " << warpMat <<endl;
        //status=Projection;
    }
//    waitKey(2000);
//    if(i++<50) return;
//    std::vector<std::vector<cv::Point> > contours;
//    findContours(redBinary.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
//
//    std::vector<cv::Point> approx;
//
//    for (int i = 0; i < contours.size(); i++)
//    {
//        // Approximate contour with accuracy proportional
//        // to the contour perimeter
//        cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true)*0.02, true);
//
//        // Skip small or non-convex objects
//        if (std::fabs(cv::contourArea(contours[i])) < 100 || !cv::isContourConvex(approx))
//            continue;
//
//        if (approx.size() == 4)
//        {
//            cout<< "Rectange found " << approx <<endl;
//
//            std::vector<cv::Point2f> corners(4);
//            corners[0] = Point2f(0,0);
//            corners[1] = Point2f(0,outputImage.rows);
//            corners[2] = Point2f(outputImage.cols,0);
//            corners[3] = Point(outputImage.cols,outputImage.rows);
//            std::vector<cv::Point2f> realCorners(4);
//            realCorners[0] = Point2f(approx[0].x,approx[0].y);
//            realCorners[1] = Point2f(approx[1].x,approx[1].y);
//            realCorners[2] = Point2f(approx[2].x,approx[2].y);
//            realCorners[3] = Point2f(approx[3].x,approx[3].y);
//
//            stable_sort(realCorners.begin(),realCorners.end(),xsorter);
//            stable_sort(realCorners.begin(),realCorners.end(),ysorter);
//            stable_sort(corners.begin(),corners.end(),xsorter);
//            stable_sort(corners.begin(),corners.end(),ysorter);
//            cout<< "Corners " << corners <<endl;
//            cout<< "Real corners " << realCorners <<endl;
//            warpMat=findHomography(realCorners, corners);
//            cout<< "Warp mat " << warpMat <<endl;
//
//            status=Projection;
//        }
//    }

}
