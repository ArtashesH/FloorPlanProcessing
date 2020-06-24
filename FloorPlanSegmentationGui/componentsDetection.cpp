#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include "componentsDetection.h"
#include "NaturalBinarization.h"

cv::RNG rng1(12345);

componentsDetector::componentsDetector(const cv::Mat& inputImage)
{
	m_pointsForLines.clear();
	m_image = inputImage.clone();

}

void componentsDetector::setCurrentImage(const cv::Mat& tmpImg)
{
	m_pointsForLines.clear();
	m_image = tmpImg.clone();
}


void componentsDetector::setInputImage(const std::string& inputImagePath) {
	m_image = cv::imread(inputImagePath);
}




void componentsDetector::setUpdatedDetectedWalls(const std::vector<std::vector<cv::Point>>& updatedWallsVec)
{
	m_pointsForWalls = updatedWallsVec;
}

void componentsDetector::ThinSubiteration1(cv::Mat & pSrc, cv::Mat & pDst)
{

	int rows = pSrc.rows;
	int cols = pSrc.cols;
	pSrc.copyTo(pDst);
	cv::cvtColor(pDst, pDst, CV_BGR2GRAY);
	for (int i = 0; i < pDst.rows; ++i) {
		for (int j = 0; j < pDst.cols; ++j) {
			if (pDst.at<uchar>(i, j) != 0) {
				pDst.at<uchar>(i, j) = 255;
			}
		}
	}
/*	cv::erode(pDst, pDst, cv::Mat());
	cv::erode(pDst, pDst, cv::Mat());
	cv::erode(pDst, pDst, cv::Mat());
	cv::erode(pDst, pDst, cv::Mat());
	cv::erode(pDst, pDst, cv::Mat());*/

	for (int i = 1; i < pDst.rows - 1; ++i) {
		for (int j = 1; j < pDst.cols - 1; ++j) {
			if (pDst.at<uchar>(i - 1, j - 1) != 0 && pDst.at<uchar>(i - 1, j) != 0 && pDst.at<uchar>(i - 1, j + 1) != 0 &&
				pDst.at<uchar>(i, j + 1) != 0 && pDst.at<uchar>(i + 1, j + 1) != 0 && pDst.at<uchar>(i + 1, j) != 0 && pDst.at<uchar>(i + 1, j - 1) != 0 && pDst.at<uchar>(i, j - 1) != 0) {
				;/*pDst.at<uchar>(i - 1, j - 1) = 0;
				pDst.at<uchar>(i - 1, j) = 0; 
				pDst.at<uchar>(i - 1, j + 1) = 0;
				pDst.at<uchar>(i, j + 1) = 0; 
				pDst.at<uchar>(i + 1, j + 1) = 0; 
				pDst.at<uchar>(i + 1, j) = 0; 
				pDst.at<uchar>(i + 1, j - 1) = 0; 
				pDst.at<uchar>(i, j - 1) = 0;*/
				
			}
		}
	}
	/*for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (pSrc.at<float>(i, j) == 1.0f) {
				/// get 8 neighbors
				/// calculate C(p)
				int neighbor0 = (int)pSrc.at<float>(i - 1, j - 1);
				int neighbor1 = (int)pSrc.at<float>(i - 1, j);
				int neighbor2 = (int)pSrc.at<float>(i - 1, j + 1);
				int neighbor3 = (int)pSrc.at<float>(i, j + 1);
				int neighbor4 = (int)pSrc.at<float>(i + 1, j + 1);
				int neighbor5 = (int)pSrc.at<float>(i + 1, j);
				int neighbor6 = (int)pSrc.at<float>(i + 1, j - 1);
				int neighbor7 = (int)pSrc.at<float>(i, j - 1);
				int C = int(~neighbor1 & (neighbor2 | neighbor3)) +
					int(~neighbor3 & (neighbor4 | neighbor5)) +
					int(~neighbor5 & (neighbor6 | neighbor7)) +
					int(~neighbor7 & (neighbor0 | neighbor1));
				if (C == 1) {
					/// calculate N
					int N1 = int(neighbor0 | neighbor1) +
						int(neighbor2 | neighbor3) +
						int(neighbor4 | neighbor5) +
						int(neighbor6 | neighbor7);
					int N2 = int(neighbor1 | neighbor2) +
						int(neighbor3 | neighbor4) +
						int(neighbor5 | neighbor6) +
						int(neighbor7 | neighbor0);
					int N = std::min(N1, N2);
					if ((N == 2) || (N == 3)) {
						int E = (neighbor5 | neighbor6 | ~neighbor0) & neighbor7;
						if (E == 0) {
							;//	pDst.at<float>(i, j) = 0.0f;
						}
					}
				}
			}
		}
	}*/

}




void componentsDetector::findInnerAndOuterWallPoints(const std::string& procAlgType)
{

	m_pointsForLines.clear();
	processingType currPr;
	if (procAlgType == "Aggressive Wall") {
		currPr = processingType::innerAgresive;
		std::cout << "Aggressive case \n";
	}
	else if (procAlgType == "Inner Wall") {
		std::cout << "Inner Wall \n";
		currPr = processingType::inner;
	}
	else if (procAlgType == "Outer Wall") {
		std::cout << "Outer wall \n";
		currPr = processingType::outer;
	}
	else {
		return;
	}
   //  cv::Mat initImg = cv::imread("E:/upworkProjs/FloorPlanAnalyzing/2dFloorPlanSimple6.jpg");
	// m_image = initImg.clone();
	 //cv::Mat initImg = cv::imread("E:/upworkProjs/FloorPlanAnalyzing/Venue_Images/Columbus Circle/1stfloor.PNG");
	
	if (m_image.cols == 0 || m_image.rows == 0) {
		return;
	}
	cv::Mat initImgVis = m_image.clone();

	cv::Mat grayImg;
	cv::cvtColor(initImgVis, grayImg, CV_BGR2GRAY);
	cv::Mat sobel_X;
	cv::Mat abs_grad_x, abs_grad_y;
	cv::Sobel(grayImg, sobel_X, CV_64F, 1, 0);
	cv::convertScaleAbs(sobel_X, abs_grad_x);
	cv::Mat sobel_Y;
	cv::Sobel(grayImg, sobel_Y, CV_64F, 0, 1);
	cv::convertScaleAbs(sobel_Y, abs_grad_y);
	cv::Mat resultSobel;
	cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0,grayImg);

	NaturalBinarization natBin(40,40,0.4);

	cv::Mat binImg;

	binImg = natBin.run(initImgVis);
	cv::imshow("InitImg", initImgVis);

	cv::imshow("SobelRes", binImg);

	m_resultImage = m_image.clone();
	cv::Mat processedImage = m_image.clone();
	if (processedImage.channels() == 3) {
		cv::cvtColor(processedImage, processedImage, CV_BGR2GRAY);
	}


	//processedImage = binImg.clone();
	cv::imshow("ProcImg", processedImage);
	if (currPr == processingType::innerAgresive) {
		cv::threshold(processedImage, processedImage, 240, 250, CV_THRESH_BINARY);
	}
	else if (currPr == processingType::inner)  {
		cv::threshold(processedImage, processedImage, 230, 250, CV_THRESH_BINARY);
	}
	else {
		cv::threshold(processedImage, processedImage, 100, 110, CV_THRESH_BINARY);
	}



	if (currPr == outer) {
	//	cv::dilate(processedImage, processedImage, cv::Mat());
	//	cv::dilate(processedImage, processedImage, cv::Mat());
	//	cv::dilate(processedImage, processedImage, cv::Mat());
	//	cv::dilate(processedImage, processedImage, cv::Mat());

         





		cv::dilate(processedImage, processedImage, cv::Mat());
		cv::erode(processedImage, processedImage, cv::Mat());
		cv::dilate(processedImage, processedImage, cv::Mat());
		cv::dilate(processedImage, processedImage, cv::Mat());
		cv::dilate(processedImage, processedImage, cv::Mat());
	}
	else if (currPr == inner) {
		//cv::dilate(processedImage, processedImage, cv::Mat());
		//cv::erode(processedImage, processedImage, cv::Mat());
		//cv::dilate(processedImage, processedImage, cv::Mat());
		//cv::medianBlur(processedImage, processedImage, 5);
		//cv::dilate(processedImage, processedImage, cv::Mat());
		//cv::dilate(processedImage, processedImage, cv::Mat());
	}
	else {
	//	cv::dilate(processedImage, processedImage, cv::Mat());
	//	cv::erode(processedImage, processedImage, cv::Mat());
		//cv::dilate(processedImage, processedImage, cv::Mat());
		;// cv::medianBlur(processedImage, processedImage, 3);
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
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::Mat cannyImg;
	if (currPr == processingType::innerAgresive) {
		cv::Canny(processedImage, cannyImg, 240, 250);
	}
	else if (currPr == processingType::inner) {
		cv::Canny(processedImage, cannyImg, 230, 250);
	}
	else {
		cv::Canny(processedImage, cannyImg, 100, 150);
	}

	//processedImage = grayImg.clone();
	cv::findContours(processedImage, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE, cv::Point(0, 0));
	cv::Mat drawing = cv::Mat::zeros(processedImage.size(), CV_8UC3);
	int maxElementConoutIndex = -1;
	int maxElementCountConour = 0;
	for (int h = 0; h < contours.size(); ++h) {
		if (contours[h].size() > maxElementCountConour) {
			maxElementCountConour = contours[h].size();
			maxElementConoutIndex = h;
		}
	}
	cv::Scalar color = cv::Scalar(rng1.uniform(0, 255), rng1.uniform(0, 255), rng1.uniform(0, 255));
	//drawContours(drawing, contours, maxElementConoutIndex, color, 1, 8, hierarchy, 0, cv::Point());
	m_pointsForWalls.clear();
	for (int i = 0; i< contours.size(); i++)
	{
		cv::Scalar color = cv::Scalar(rng1.uniform(0, 255), rng1.uniform(0, 255), rng1.uniform(0, 255));
		std::vector<std::vector<cv::Point>> tmpContourDrawing(1);
		/*for (int f = 0; f < contours[i].size(); ++f) {
			tmpContourDrawing[0].push_back(contours[i][f]);
			
			cv::circle(drawing, contours[i][f], 2, cv::Scalar(0, 255, 0), -1);
		}*/
		std::cout << "Points count in contopur  " << contours[i].size() << "\n";
		cv::Rect boundingRect = cv::boundingRect(contours[i]);
		/**/
		std::vector<cv::Point> tmpWallPointsVec;
		if (currPr == processingType::outer || currPr == processingType::inner ||   contours[i].size() > 700 && (float)processedImage.cols / (float)boundingRect.width < 1.8 && (float)processedImage.rows / (float)boundingRect.height  < 1.8/* &&

			((float)processedImage.rows /  (float)boundingRect.height) >  1.05*/ ) {
			for (int r = 0; r < contours[i].size(); ++r) {
				tmpWallPointsVec.push_back(contours[i][r]);
			}
			m_pointsForWalls.push_back(tmpWallPointsVec);
			drawContours(drawing, contours, i, color, 1, 8, hierarchy, 0, cv::Point());
			std::cout << "Current contour point's size " << contours[i].size() << "\n";
		}
		/*cv::Rect boundingOfCont = cv::boundingRect(contours[i]);
		if (drawing.rows / boundingOfCont.height == 1 && drawing.cols / boundingOfCont.width == 1)
			continue;
		for (int g = 0; g < drawing.rows; ++g) {
			for (int f = 0; f < drawing.cols; ++f) {
				double resultDist = cv::pointPolygonTest(contours[i], cv::Point2f(f, g), false);
				if (resultDist > 0  ) {
					;// drawing.at<cv::Vec3b>(g, f) = cv::Vec3b(0, 255, 0);
					

				}

			}
		}
		//cv::rectangle(drawing, boundingOfCont, cv::Scalar(255, 0, 0), 1);
		cv::Mat cropedImg = drawing(boundingOfCont);
		cv::Mat resImg;
		ThinSubiteration1(cropedImg, resImg);*/
	//	cv::imshow("IngiImg", resImg);
	//	cv::waitKey(0);

		
	}




/*	std::vector<cv::Vec2f> lines; // will hold the results of the detection
	cv::HoughLines(cannyImg, lines, 1, CV_PI / 180, 50, 100, 100); // runs the actual detection
													   // Draw the lines
	for (size_t i = 0; i < lines.size(); i++)
	{
		float rho = lines[i][0], theta = lines[i][1];
		cv::Point pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a*rho, y0 = b*rho;
		pt1.x = cvRound(x0 + 1000 * (-b));
		pt1.y = cvRound(y0 + 1000 * (a));
		pt2.x = cvRound(x0 - 1000 * (-b));
		pt2.y = cvRound(y0 - 1000 * (a));
		cv::line(drawing, pt1, pt2, cv::Scalar(0, 0, 255), 3, cv::LINE_AA);
	}*/










	//cv::imshow("InitialImage", m_resultImage);
//	cv::imwrite("WallDetection21.png", m_resultImage);
//	cv::dilate(processedImage, processedImage, cv::Mat());
	//cv::dilate(processedImage, processedImage, cv::Mat());
	//cv::imshow("initImg", drawing);
//	cv::imshow("ProcImg", processedImage);
//	cv::imshow("CannyImg", cannyImg);
//	cv::imshow("DrawingImage", drawing);
	//cv::waitKey(0);

	for (int i = 0; i < drawing.rows; ++i) {
		for (int j = 0; j < drawing.cols; ++j) {
			if (drawing.at<cv::Vec3b>(i, j) == cv::Vec3b(0, 0, 0)) {
				drawing.at<cv::Vec3b>(i, j) = cv::Vec3b(255, 255, 255);
			}
			else {
				drawing.at<cv::Vec3b>(i, j) = cv::Vec3b(255, 0, 0);
			}
		}
	}
	m_resultImage = m_image.clone();
	
	//m_image = cannyImg.clone();

}



cv::Mat componentsDetector::getResultImage() const
{
	return m_resultImage;
}


std::vector<std::vector<cv::Point>> componentsDetector::getDetectedWallPoints() const
{
	return m_pointsForWalls;
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
