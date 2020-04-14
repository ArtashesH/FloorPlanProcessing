#include <vector>
#include <string>

#include <opencv2/core/core.hpp>

class componentsDetector {
private:
	std::vector<cv::Point> m_pointsForLines;
	cv::Mat m_image;
	cv::Mat m_resultImage;


public:
	componentsDetector(const cv::Mat& inputImage = cv::Mat());
	void setInputImage(const std::string& inputImagePath);
	//componentsDetector(const cv::Mat& inputImage=cv::Mat());
	void findInnerAndOuterWallPoints();
	void visualizeDetections();
	std::vector<cv::Point> getDetectedPoints() const ;

};