#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>


#include "componentsDetection.h"

componentsDetector::componentsDetector(const cv::Mat& inputImage)
{
	m_pointsForLines.clear();
	m_image = inputImage.clone();

}



void componentsDetector::setInputImage(const std::string& inputImagePath) {
	m_image = cv::imread(inputImagePath);
}
void componentsDetector::findInnerAndOuterWallPoints()
{

	m_pointsForLines.clear();

   //  cv::Mat initImg = cv::imread("E:/upworkProjs/FloorPlanAnalyzing/2dFloorPlanSimple6.jpg");
	// m_image = initImg.clone();
	 //cv::Mat initImg = cv::imread("E:/upworkProjs/FloorPlanAnalyzing/Venue_Images/Columbus Circle/1stfloor.PNG");
	
	if (m_image.cols == 0 || m_image.rows == 0) {
		return;
	}
	cv::Mat initImgVis = m_image.clone();

	m_resultImage = m_image.clone();
	cv::Mat processedImage = m_image.clone();
	if (processedImage.channels() == 3) {
		cv::cvtColor(processedImage, processedImage, CV_BGR2GRAY);
	}

	cv::threshold(processedImage, processedImage, 150, 250, CV_THRESH_BINARY);

	enum roomType { outer, inner, innerAgresive };

	roomType currPr = outer;

	if (currPr == outer) {
		cv::dilate(processedImage, processedImage, cv::Mat());
		cv::erode(processedImage, processedImage, cv::Mat());
		cv::dilate(processedImage, processedImage, cv::Mat());
		cv::dilate(processedImage, processedImage, cv::Mat());
		cv::dilate(processedImage, processedImage, cv::Mat());
	}
	else if (currPr == inner) {
		cv::dilate(processedImage, processedImage, cv::Mat());
		cv::erode(processedImage, processedImage, cv::Mat());
		cv::dilate(processedImage, processedImage, cv::Mat());
		cv::medianBlur(processedImage, processedImage, 5);
		//cv::dilate(processedImage, processedImage, cv::Mat());
		//cv::dilate(processedImage, processedImage, cv::Mat());
	}
	else {
		cv::dilate(processedImage, processedImage, cv::Mat());
		cv::erode(processedImage, processedImage, cv::Mat());
		//cv::dilate(processedImage, processedImage, cv::Mat());
		cv::medianBlur(processedImage, processedImage, 3);
	}
	//cv::medianBlur(processedImage, processedImage, 9);

	//	cv::dilate(processedImage, processedImage, cv::Mat());


	for (int i = 0; i < m_image.rows; ++i) {
		for (int j = 0; j < m_image.cols; ++j) {
			if (processedImage.at<uchar>(i, j) < 50) {
				m_resultImage.at<cv::Vec3b>(i, j) = cv::Vec3b(0, 255, 0);
				m_pointsForLines.push_back(cv::Point(j, i));

			}
		}
	}
/*	cv::imshow("InitialImage", m_resultImage);
	cv::imwrite("WallDetection21.png", m_resultImage);
	cv::imshow("initImg", initImgVis);
	cv::waitKey(0);*/

}


void componentsDetector::visualizeDetections()
{
	cv::Mat visImg = m_image.clone();
	for (int i = 0; i < m_pointsForLines.size(); ++i) {
		cv::circle(visImg, m_pointsForLines[i], 3, cv::Scalar(0, 244, 0), -1);
	}
	cv::imshow("VisImg", visImg);
	cv::waitKey(0);

}



std::vector<cv::Point> componentsDetector::getDetectedPoints() const {
	return m_pointsForLines;
}
