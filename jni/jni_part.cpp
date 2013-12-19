#include <jni.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>           // OpenCV window I/O
#include "opencv2/calib3d/calib3d.hpp"
#include <vector>
#include <time.h>

using namespace std;
using namespace cv;
Mat image1;
Mat image2;
vector<KeyPoint> keypoints_old1,keypoints_new1,keypoints_old2,keypoints_new2,keypoints_old3,keypoints_new3;
Mat descriptors_old1,descriptors_new1,descriptors_old2,descriptors_new2,descriptors_old3,descriptors_new3;
cv::BFMatcher matcher1(cv::NORM_HAMMING, true),matcher2(cv::NORM_HAMMING, true),matcher3(cv::NORM_HAMMING, true);
std::vector<cv::DMatch> matches1,matches2,matches3;
std::vector<cv::Point2f> initial_pts1,initial_pts2,initial_pts3;
std::vector<cv::Point2f> new_pts1,new_pts2,new_pts3;
cv::Mat homography1,homography2,homography3;
std::vector<uchar> mask1,mask2,mask3;
bool capture=false;
bool display=false;
double inliner_ratio1=0,inliner_ratio2=0,inliner_ratio3=0;
int nbinliner1=0,nbinliner2=0,nbinliner3=0;
int a=0;


double seconds_elapsedmatch3,seconds_elapsedmatch2,seconds_elapsedmatch1,seconds_elapsedhomo3,seconds_elapsedhomo2,seconds_elapsedhomo1,seconds_elapsed_fast,seconds_elapsedbrisk,seconds_elapsedbrief, seconds_elapsedorb, seconds_elapsed_star,seconds_elapsed_orb,seconds_elapsed_brisk,seconds_elapsed_mser,seconds_elapsed_good,seconds_elapsed_dense,seconds_elapsed_blob,seconds_elapsed_grid;
bool display1=false;
bool display2=false;
bool display3=false;
bool capture1=false;
int b=0;
Mat image_runtime;
vector<KeyPoint> keypoints_fast,keypoints_star,keypoints_orb,keypoints_brisk,keypoints_mser,keypoints_good,keypoints_dense,keypoints_blob,keypoints_grid;
Mat descriptors_orb,descriptors_brief,descriptors_brisk;




extern "C" {
JNIEXPORT void JNICALL Java_org_opencv_samples_tutorial2_Tutorial2Activity_FindFeatures1(JNIEnv*, jobject, jlong addrGray, jlong addrRgba);
JNIEXPORT void JNICALL Java_org_opencv_samples_tutorial2_Tutorial2Activity_CaptureFrame(JNIEnv*, jobject, jint capture_idx);
JNIEXPORT void JNICALL Java_org_opencv_samples_tutorial2_Tutorial2Activity_CaptureFrame1(JNIEnv*, jobject, jint capture_idx);
JNIEXPORT void JNICALL Java_org_opencv_samples_tutorial2_Tutorial2Activity_CaptureFrame1(JNIEnv*, jobject, jint capture_idx){

	if(b==2){
		b=3;
		capture1=true;
	}
	if(b==1){
		b=2;
		capture1=true;
	}
	if(b==0){
		b=1;
		capture1=true;
	}


}
JNIEXPORT void JNICALL Java_org_opencv_samples_tutorial2_Tutorial2Activity_CaptureFrame(JNIEnv*, jobject, jint capture_idx){

	if(a==1){
		a=2;
		capture=true;
	}
	if(a==0){
		a=1;
		capture=true;
	}


}

JNIEXPORT void JNICALL Java_org_opencv_samples_tutorial2_Tutorial2Activity_FindFeatures1(JNIEnv*, jobject, jlong addrGray, jlong addrRgba)
{

	Mat& mGr  = *(Mat*)addrGray;
	Mat& mRgb = *(Mat*)addrRgba;

	if (b==1&&capture1==true){
		image_runtime=mRgb;

		FastFeatureDetector fastdetect( 10/*threshold*/, true/*nonmax_suppression*/ );
		StarFeatureDetector stardetect( 16/*max_size*/, 5/*response_threshold*/, 10/*line_threshold_projected*/,8/*line_threshold_binarized*/, 5/*suppress_nonmax_size*/ );
		OrbFeatureDetector orbdetect(500);
		cv::BRISK  Briskdetect(60,4,1.0f);
		Briskdetect.create("Feature2D.BRISK");

		GoodFeaturesToTrackDetector Gooddetect( 1000/*maxCorners*/, 0.01/*qualityLevel*/, 1./*minDistance*/,3/*int _blockSize*/, true/*useHarrisDetector*/, 0.04/*k*/ );
		DenseFeatureDetector Densedetect(4, 4, 1.5);


		const clock_t start_fast = clock();
		// do stuff here
		fastdetect.detect(image_runtime,keypoints_fast);
		clock_t now_fast = clock();
		clock_t delta_fast = now_fast - start_fast;
		seconds_elapsed_fast = static_cast<double>(delta_fast) / CLOCKS_PER_SEC;

		const clock_t start_star = clock();
		// do stuff here
		stardetect.detect(image_runtime,keypoints_star);
		clock_t now_star = clock();
		clock_t delta_star = now_star - start_star;
		seconds_elapsed_star = static_cast<double>(delta_star) / CLOCKS_PER_SEC;

		const clock_t start_orb = clock();
		// do stuff here
		orbdetect.detect(image_runtime,keypoints_orb);
		clock_t now_orb = clock();
		clock_t delta_orb = now_orb - start_orb;
		seconds_elapsed_orb = static_cast<double>(delta_orb) / CLOCKS_PER_SEC;

		const clock_t start_brisk = clock();
		// do stuff here
		Briskdetect.detect(image_runtime, keypoints_brisk);
		clock_t now_brisk = clock();
		clock_t delta_brisk = now_brisk - start_brisk;
		seconds_elapsed_brisk = static_cast<double>(delta_brisk) / CLOCKS_PER_SEC;


		const clock_t start_good = clock();
		// do stuff here
		Gooddetect.detect(image_runtime, keypoints_good);
		clock_t now_good = clock();
		clock_t delta_good = now_good - start_good;
		seconds_elapsed_good = static_cast<double>(delta_good) / CLOCKS_PER_SEC;

		const clock_t start_dense = clock();
		// do stuff here
		Densedetect.detect(image_runtime, keypoints_dense);
		clock_t now_dense = clock();
		clock_t delta_dense = now_dense - start_dense;
		seconds_elapsed_dense = static_cast<double>(delta_dense) / CLOCKS_PER_SEC;

		capture1=false;
		display1=true;
	}
	if (display1==true){

		std::stringstream str_stream_features1;
		str_stream_features1 << "#fast:" << seconds_elapsed_fast<<"#star:" << seconds_elapsed_star<<"#orb:" << seconds_elapsed_orb<<"#brisk:" << seconds_elapsed_brisk;
		cv::putText(mRgb, str_stream_features1.str() , cv::Point2i(50,40), CV_FONT_HERSHEY_PLAIN, 2, cv::Scalar(0, 255, 0));
		std::stringstream str_stream_features2;
		str_stream_features2 <<  "#good:" << seconds_elapsed_good<<  "#dense:" << seconds_elapsed_dense;
		cv::putText(mRgb, str_stream_features2.str() , cv::Point2i(50,80), CV_FONT_HERSHEY_PLAIN, 2, cv::Scalar(0, 255, 0));
	}

	if (b==2&&capture1==true){

		OrbFeatureDetector orb_detect(500);
		orb_detect.detect(image_runtime,keypoints_orb);
		const clock_t startorb = clock();
				// do stuff here
		orb_detect.compute(image_runtime,keypoints_orb,descriptors_orb);
		clock_t noworb = clock();
		clock_t deltaorb = noworb - startorb;
		seconds_elapsedorb = static_cast<double>(deltaorb) / CLOCKS_PER_SEC;

		BriefDescriptorExtractor extractor_brief(32);
		const clock_t startbrief = clock();
						// do stuff here
		extractor_brief.compute(image_runtime,keypoints_orb,descriptors_brief);
		clock_t nowbrief = clock();
		clock_t deltabrief = nowbrief - startbrief;
		seconds_elapsedbrief = static_cast<double>(deltabrief) / CLOCKS_PER_SEC;

		int Threshl=60;
		int Octaves=4;
		float PatternScales=1.0f;
		cv::BRISK  Brisk_detect(Threshl,Octaves,PatternScales);
		Brisk_detect.create("Feature2D.BRISK");
		Brisk_detect.detect(image_runtime, keypoints_brisk);
		const clock_t startbrisk = clock();
						// do stuff here
		Brisk_detect.compute(image_runtime, keypoints_brisk,descriptors_brisk);
		clock_t nowbrisk = clock();
		clock_t deltabrisk = nowbrisk - startbrisk;
		seconds_elapsedbrisk = static_cast<double>(deltabrisk) / CLOCKS_PER_SEC;


		capture1=false;
		display2=true;
	}

	if (display2==true){

		std::stringstream str_stream_features3;
		str_stream_features3 <<  "#orb_des:" << seconds_elapsedorb<<  "#brisk_des:" << seconds_elapsedbrisk<< "#brief_des:" << seconds_elapsedbrief ;
		cv::putText(mRgb, str_stream_features3.str() , cv::Point2i(50,700), CV_FONT_HERSHEY_PLAIN, 2, cv::Scalar(0, 255, 0));
	}


	if (capture==true&&a==1){
		image1=mRgb;
		capture=false;

		OrbFeatureDetector detector2(500);
		BriefDescriptorExtractor extractor2(32);
		detector2.detect(image1,keypoints_old2);
		extractor2.compute(image1,keypoints_old2,descriptors_old2);


		OrbFeatureDetector detector1(500);

		detector1.detect(image1,keypoints_old1);
		detector1.compute(image1,keypoints_old1,descriptors_old1);

		int Threshl=60;
		int Octaves=4;
		float PatternScales=1.0f;
		cv::BRISK  BRISKD(Threshl,Octaves,PatternScales);
		BRISKD.create("Feature2D.BRISK");
		BRISKD.detect(image1, keypoints_old3);
		BRISKD.compute(image1, keypoints_old3,descriptors_old3);
		}
	if (capture==true&&a==2){
		image2=mRgb;
		capture=false;

		OrbFeatureDetector detector1(500);
		detector1.detect(image2,keypoints_new1);
		detector1.compute(image2,keypoints_new1,descriptors_new1);

		FastFeatureDetector detector2(50);
		BriefDescriptorExtractor extractor2(32);
		detector2.detect(image2,keypoints_new2);
		extractor2.compute(image2,keypoints_new2,descriptors_new2);

		int Threshl=60;
		int Octaves=4;
		float PatternScales=1.0f;
		cv::BRISK  BRISKD(Threshl,Octaves,PatternScales);
		BRISKD.create("Feature2D.BRISK");
		BRISKD.detect(image2, keypoints_new3);
		BRISKD.compute(image2, keypoints_new3,descriptors_new3);



		a=3;
        //process of orb features

		const clock_t startmatch1 = clock();
				// do stuff here
		matcher1.match(descriptors_old1, descriptors_new1, matches1);
		clock_t nowmatch1 = clock();
		clock_t deltamatch1 = nowmatch1 - startmatch1;
		seconds_elapsedmatch1 = static_cast<double>(deltamatch1) / CLOCKS_PER_SEC;

		for(int i = 0; i < matches1.size(); i++ ){
			cv::DMatch& d = matches1[i];
			cv::KeyPoint& init_pt = keypoints_old1[d.queryIdx];
			cv::KeyPoint& new_pt = keypoints_new1[d.trainIdx];
			initial_pts1.push_back(init_pt.pt);
			new_pts1.push_back(new_pt.pt);
		}

		const clock_t starthomo1 = clock();
				// do stuff here
		homography1 = cv::findHomography(initial_pts1, new_pts1, CV_RANSAC, 2, mask1);
		clock_t nowhomo1 = clock();
		clock_t deltahomo1 = nowhomo1 - starthomo1;
		seconds_elapsedhomo1 = static_cast<double>(deltahomo1) / CLOCKS_PER_SEC;


		for(unsigned int i = 0; i<mask1.size();i++)
		{
			if (mask1[i]){//> Inlier
				nbinliner1++;
			}

		}
		inliner_ratio1=100*nbinliner1/mask1.size();

        //process of brief
		const clock_t startmatch2 = clock();
				// do stuff here
		matcher2.match(descriptors_old2, descriptors_new2, matches2);
		clock_t nowmatch2 = clock();
		clock_t deltamatch2 = nowmatch2 - startmatch2;
		seconds_elapsedmatch2 = static_cast<double>(deltamatch2) / CLOCKS_PER_SEC;

		for(int i = 0; i < matches2.size(); i++ ){
			cv::DMatch& d = matches2[i];
			cv::KeyPoint& init_pt = keypoints_old2[d.queryIdx];
			cv::KeyPoint& new_pt = keypoints_new2[d.trainIdx];
			initial_pts2.push_back(init_pt.pt);
			new_pts2.push_back(new_pt.pt);
		}

		const clock_t starthomo2 = clock();
				// do stuff here
		homography2 = cv::findHomography(initial_pts2, new_pts2, CV_RANSAC, 2, mask2);
		clock_t nowhomo2 = clock();
		clock_t deltahomo2 = nowhomo2 - starthomo2;
		seconds_elapsedhomo2 = static_cast<double>(deltahomo2) / CLOCKS_PER_SEC;

		for(unsigned int i = 0; i<mask2.size();i++)
		{
			if (mask2[i]){//> Inlier
				nbinliner2++;
			}

		}
		inliner_ratio2=100*nbinliner2/mask2.size();

        //process of brisk
		const clock_t startmatch3 = clock();
						// do stuff here
		matcher3.match(descriptors_old3, descriptors_new3, matches3);
		clock_t nowmatch3 = clock();
		clock_t deltamatch3 = nowmatch3 - startmatch3;
	    seconds_elapsedmatch3 = static_cast<double>(deltamatch3) / CLOCKS_PER_SEC;

		for(int i = 0; i < matches3.size(); i++ ){
			cv::DMatch& d = matches3[i];
			cv::KeyPoint& init_pt = keypoints_old3[d.queryIdx];
			cv::KeyPoint& new_pt = keypoints_new3[d.trainIdx];
			initial_pts3.push_back(init_pt.pt);
			new_pts3.push_back(new_pt.pt);
		}

		const clock_t starthomo3 = clock();
				// do stuff here
		homography3 = cv::findHomography(initial_pts3, new_pts3, CV_RANSAC, 2, mask3);
		clock_t nowhomo3 = clock();
		clock_t deltahomo3 = nowhomo3 - starthomo3;
		seconds_elapsedhomo3 = static_cast<double>(deltahomo3) / CLOCKS_PER_SEC;

		for(unsigned int i = 0; i<mask3.size();i++)
		{
			if (mask3[i]){//> Inlier
				nbinliner3++;
			}

		}
		inliner_ratio3=100*nbinliner3/mask3.size();
		display=true;

	}
    if (a>0&&capture==false){

    	for( unsigned int i = 0; i < keypoints_old1.size(); i++ )
    	{
    		const KeyPoint& kp = keypoints_old1[i];
    		circle(mRgb, Point(kp.pt.x, kp.pt.y), 5, Scalar(0,0,255,0));
    	}
    	for( unsigned int j = 0; j < keypoints_new1.size(); j++ )
    	{
    	    const KeyPoint& kp = keypoints_new1[j];
    	    circle(mRgb, Point(kp.pt.x, kp.pt.y), 5, Scalar(255,0,0,0));
    	}

    	std::stringstream str_stream1;
    	str_stream1 << "#nb_k1: " << keypoints_new1.size()<< "-" <<keypoints_new2.size()<< "-" <<keypoints_new3.size()<<"#nb_k2: "<<keypoints_old1.size() <<"-" <<keypoints_old2.size() <<"-" <<keypoints_old3.size() << "#nb_m: " << matches1.size()<<"-" << matches2.size()<<"-" << matches3.size(); //<< s.height;
    	cv::putText(mRgb, str_stream1.str() , cv::Point2i(50,80), CV_FONT_HERSHEY_PLAIN, 2, cv::Scalar(0, 255, 0));
    	std::stringstream str_stream2;
    	str_stream2 << "#m_size:" << mask1.size()<< "-"<< mask2.size()<< "-"<<mask3.size()<< "#in_n:" << nbinliner1<< "-"<< nbinliner2<< "-"<< nbinliner3<<"# in_r%" << inliner_ratio1<<"-" << inliner_ratio2<<"-" << inliner_ratio3; //<< s.height;
    	cv::putText(mRgb, str_stream2.str() , cv::Point2i(50,40), CV_FONT_HERSHEY_PLAIN, 2, cv::Scalar(0, 255, 0));
    	std::stringstream str_stream3;
    	str_stream3 <<  "#homography1:" << seconds_elapsedhomo1 << "#homography2:" << seconds_elapsedhomo2<< "#homography3:" << seconds_elapsedhomo3 ;
    	cv::putText(mRgb, str_stream3.str() , cv::Point2i(50,700), CV_FONT_HERSHEY_PLAIN, 2, cv::Scalar(0, 255, 0));
    	std::stringstream str_stream4;
    	str_stream4 <<  "#match1:" << seconds_elapsedmatch1 << "#match2:" << seconds_elapsedmatch2<< "#match3:" << seconds_elapsedmatch3 ;
    	cv::putText(mRgb, str_stream4.str() , cv::Point2i(50,660), CV_FONT_HERSHEY_PLAIN, 2, cv::Scalar(0, 255, 0));




    }



}
}
