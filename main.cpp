#include <vector>
#include <iostream>
#include <opencv2/opencv.hpp>

#include "pose.hpp"
#include "poseEstimation.hpp"

using namespace std;

int main(){

	poseEstimation::poseEstimation pe("../models/poseEstimationModel.onnx");
	poseEstimation::poseTracker pt;

	cv::Mat frame;
	cv::VideoCapture cap("../video.avi");
	cap.set(cv::CAP_PROP_FRAME_WIDTH, 1280);
	cap.set(cv::CAP_PROP_FRAME_HEIGHT, 720);
	cap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
	cap.set(cv::CAP_PROP_FPS, 30);
	
	while(true){
		cap.read(frame);
		while(!frame .empty()){
			double t0 = cv::getTickCount();
			cv::namedWindow("test", 0);
			cv::resizeWindow("test", 1280, 720);
			cap.read(frame);
			std::vector<poseEstimation::Pose> poses = pe.run(frame);
			pt.track(poses);
			for(int i = 0; i < poses.size(); i++){
				poses[i].draw(frame, true);
			}
			double t1 = cv::getTickCount();
			cv::imshow("test", frame);
			double FPS = cv::getTickFrequency() / (t1 - t0);
			cout << "FPS: " << FPS << endl;
			if(cv::waitKey(30)=='q')
			{
				cv::destroyWindow("test");
				cap.release();
				break;
			}
		}
		cap.release();
	}
	return 0;
}
