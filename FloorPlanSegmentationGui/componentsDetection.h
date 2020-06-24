#include <vector>
#include <string>

#include <opencv2/core/core.hpp>


enum processingType { outer, inner, innerAgresive };



class componentsDetector {
private:
	std::vector<cv::Point> m_pointsForLines;
	std::vector<std::vector<cv::Point>> m_pointsForWalls;
	cv::Mat m_image;
	cv::Mat m_resultImage;
	//std::string m_wallDetectionAlgorithmType;
	//processingType m_wallDetectionType;


public:
	componentsDetector(const cv::Mat& inputImage = cv::Mat());
	void setCurrentImage(const cv::Mat& tmpImg);
	void setRoomDetectionAlgType(const std::string& selectedAlgType);
	void setUpdatedDetectedWalls(const std::vector<std::vector<cv::Point>>& updatedWallsVec);
	void ThinSubiteration1(cv::Mat & pSrc, cv::Mat & pDst);
	void setInputImage(const std::string& inputImagePath);
	//componentsDetector(const cv::Mat& inputImage=cv::Mat());
	void findInnerAndOuterWallPoints(const std::string& processingAlgType);
	void visualizeDetections();
	cv::Mat getResultImage() const;
	std::vector<cv::Point> getDetectedPoints() const ;
	std::vector<std::vector<cv::Point>> getDetectedWallPoints() const;

};