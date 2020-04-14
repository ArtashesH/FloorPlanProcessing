#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

int main()
{
	cv::Mat initImg = cv::imread("E:/upworkProjs/FloorPlanAnalyzing/2dFloorPlanSimple6.jpg");
	//cv::Mat initImg = cv::imread("E:/upworkProjs/FloorPlanAnalyzing/Venue_Images/Columbus Circle/1stfloor.PNG");
	cv::Mat initImgVis = initImg.clone();

	cv::Mat processedImage = initImg.clone();
	cv::cvtColor(processedImage, processedImage,CV_BGR2GRAY);


	cv::threshold(processedImage, processedImage, 150, 250, CV_THRESH_BINARY);

	enum roomType { outer, inner , innerAgresive};
	
	roomType currPr = innerAgresive;

	if (currPr == outer) {
		cv::dilate(processedImage, processedImage, cv::Mat());
		cv::erode(processedImage, processedImage, cv::Mat());
		cv::dilate(processedImage, processedImage, cv::Mat());
		cv::dilate(processedImage, processedImage, cv::Mat());
		cv::dilate(processedImage, processedImage, cv::Mat());
	}
	else if ( currPr == inner) {
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


	for (int i = 0; i < initImg.rows; ++i) {
		for (int j = 0; j < initImg.cols; ++j) {
			if (processedImage.at<uchar>(i, j) < 50 ) {
				initImg.at<cv::Vec3b>(i, j) = cv::Vec3b(0, 255, 0);

			}
		}
	}
	cv::imshow("InitialImage", initImg);
	cv::imwrite("WallDetection21.png", initImg);
	cv::imshow("initImg", initImgVis);
	cv::waitKey(0);
}
