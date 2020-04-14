#ifndef FLOORPLANSEGMENTATIONGUI_H
#define FLOORPLANSEGMENTATIONGUI_H

#include <QtWidgets/QMainWindow>


#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPushbutton>
#include <QLayout>
#include <QPixmap>
#include <QEvent>
#include <QAction>

#include <QMouseEvent>
#include <QGraphicsPixmapItem>
#include <QDebug>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QGraphicsSceneMouseEvent>
#include <QTableWidget>
#include <QCheckBox>
#include <QRadioButton>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>



#include <iostream>

#include <Python.h>



#define PI 3.14159265
#define radiantoDeg 57.2958



#if PY_MAJOR_VERSION >= 3
#define IS_PY3K
#endif



#include "ui_floorplansegmentationgui.h"
#include "componentsDetection.h"




class CustomScene : public QGraphicsScene
{
	Q_OBJECT
		signals :
	void selectPointOnScene();
	void finishSelectingPoints();
	void removeLastSelectedPoint();
	void selectMovedPoint();

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event)
	{
		
		qDebug() << "Custom scene clicked.";
		QGraphicsScene::mousePressEvent(event);
		if (!event->isAccepted() && event->button() == Qt::LeftButton) {
			
				qDebug() << "Pressed left button \n";
				// add a custom item to the scene
				QPointF pt = event->scenePos();
				m_pickedPoint.setX(pt.x());
				m_pickedPoint.setY(pt.y());
				//std::cout << "POint position  " << pt.x() << "   " << pt.y() << std::endl;
				emit selectPointOnScene();

			

		}
		else if (!event->isAccepted() && event->button() == Qt::RightButton) {
			
				qDebug() << "Custom scene RIGHT clicked.";
				QPointF pt = event->scenePos();
				m_pickedPoint.setX(pt.x());
				m_pickedPoint.setY(pt.y());
				emit removeLastSelectedPoint();
			
		}

	
	}
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
		std::cout << "DOUBLE CLICKED \n";
		emit finishSelectingPoints();
	}
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override{
		//if (event->type() == QGraphicsSceneMouseEvent::MouseMove)
		
		QPointF pt = event->scenePos();
		m_currentMovedPoint.setX(pt.x());
		m_currentMovedPoint.setY(pt.y());
	//	qDebug() << "MOVE EVENT !!!!";
	//	std::cout << m_currentMovedPoint.x() << "   " << m_currentMovedPoint.y() << std::endl;
		emit selectMovedPoint();
		//emit removeLastSelectedPoint();

	}
public:

	QPoint transferPickedPointsPosition()
	{
		return m_pickedPoint;
	}

	QPoint transferMovedPointsPosition()
	{
		return m_currentMovedPoint;
	}

private:
	void joinCloseRects();
	bool isRectsIntersecting();
private:
	QPoint m_pickedPoint;
	QPoint m_currentMovedPoint;


};









class FloorPlanSegmentationGui : public QMainWindow
{
	Q_OBJECT


signals:
	void WidgetClosed();

public slots:
	void setSelectedImagePath();
	void runSegmentation();
	void runHeightMapGeneration();
	void mousePressEvent(QGraphicsSceneMouseEvent* e);
	void getSelectedPoints();
	void drawPolygonFromSelectedPoints();
	void dbmShowingSlot();
	void sinrShowingSlot();

	void drawHeightMapValue();
	void coloringOfSelectedPolygonSlot();
	void coloringOfSelectedWalls();
	void removingSelectedPointsSlot();
	void onRemoveLastSelectedPoint();
	void switchComponentType(const QString& compName);
	void writeHeightMapPointsDataSlot();
	void drawHeightMapContourSlot();
	void resetSceneSlot();
	void detectComponentsSlot();


public:
	FloorPlanSegmentationGui(QWidget *parent = 0);
	
	~FloorPlanSegmentationGui();


protected:
	bool eventFilter(QEvent *event);
//	bool eventFilter(QObject * obj, QEvent * ev);
//	void mouseMoveEvent(QMouseEvent* event);


public:
	const wchar_t *GetWC(const char *c);
	void callPythonFunction(/*const std::vector<int>& imageSizes*/);
	bool parseHeightMapTxtFile(cv::Mat& resHeightMap, cv::Mat& resSINRMat);
	void findOuterContour(const cv::Mat& inputMaskImg, cv::Mat& resImg);
	void rotateCurrentImage();
	void findRotationAngleAndDistance(const float& latitude1, const float& longtitude1, const float& latitude2, const float& longtitude2, float& rotationAngle, float& finalDist);
	float getAngleBetweenPoints(const float& x1, const float&  y1, const float& x2, const float& y2);
	void calculateLatAndLong(const float& initLat, const float& initLong, const float&  meterPerX, const float& meterPerY, float& finalLat, float& finalLong);
	void generateGeojsonFileLast();
	void generateGeojsonFile();
	//void calculateRotationAngle();


	inline cv::Mat QImageToCvMat(const QImage &inImage, bool inCloneImageData = true)
	{
		switch (inImage.format())
		{
			// 8-bit, 4 channel
		case QImage::Format_ARGB32:
		case QImage::Format_ARGB32_Premultiplied:
		{
			cv::Mat  mat(inImage.height(), inImage.width(),
				CV_8UC4,
				const_cast<uchar*>(inImage.bits()),
				static_cast<size_t>(inImage.bytesPerLine())
			);

			return (inCloneImageData ? mat.clone() : mat);
		}

		// 8-bit, 3 channel
		case QImage::Format_RGB32:
		{
			if (!inCloneImageData)
			{
				qWarning() << "ASM::QImageToCvMat() - Conversion requires cloning so we don't modify the original QImage data";
			}

			cv::Mat  mat(inImage.height(), inImage.width(),
				CV_8UC4,
				const_cast<uchar*>(inImage.bits()),
				static_cast<size_t>(inImage.bytesPerLine())
			);

			cv::Mat  matNoAlpha;

			cv::cvtColor(mat, matNoAlpha, cv::COLOR_BGRA2BGR);   // drop the all-white alpha channel

			return matNoAlpha;
		}

		// 8-bit, 3 channel
		case QImage::Format_RGB888:
		{
			if (!inCloneImageData)
			{
				qWarning() << "ASM::QImageToCvMat() - Conversion requires cloning so we don't modify the original QImage data";
			}

			QImage   swapped = inImage.rgbSwapped();

			return cv::Mat(swapped.height(), swapped.width(),
				CV_8UC3,
				const_cast<uchar*>(swapped.bits()),
				static_cast<size_t>(swapped.bytesPerLine())
			).clone();
		}
		case 24:
		{
			cv::Mat  mat(inImage.height(), inImage.width(),
				CV_8UC1,
				const_cast<uchar*>(inImage.bits()),
				static_cast<size_t>(inImage.bytesPerLine())
			);

			return (inCloneImageData ? mat.clone() : mat);
		}


		// 8-bit, 1 channel
		case QImage::Format_Indexed8:
		{
			cv::Mat  mat(inImage.height(), inImage.width(),
				CV_8UC1,
				const_cast<uchar*>(inImage.bits()),
				static_cast<size_t>(inImage.bytesPerLine())
			);

			return (inCloneImageData ? mat.clone() : mat);
		}

		default:
			qWarning() << "ASM::QImageToCvMat() - QImage format not handled in switch:" << inImage.format();
			break;
		}

		return cv::Mat();
	}

	// If inPixmap exists for the lifetime of the resulting cv::Mat, pass false to inCloneImageData to share inPixmap's data
	// with the cv::Mat directly
	//    NOTE: Format_RGB888 is an exception since we need to use a local QImage and thus must clone the data regardless



	PyObject* m_pModule;
	PyObject* m_pFunc;
//	bool m_isPythonModuleInitialized;

private:
	Ui::FloorPlanSegmentationGuiClass ui;
	QWidget* m_mainWidget;
	QWidget* m_segmentDataWidget;
	QDialog* m_parametersWidget;
	QComboBox* m_selectComponentWidget;
	QTableWidget*  m_tableWidget;
	QLineEdit* m_lineForTypeOfRoom;


	QVBoxLayout* m_mainLayout;
	QHBoxLayout* m_selectSegmentLayout;
	QHBoxLayout* m_heightmapLineDetLayout;
	QGraphicsView* m_mainGraphicsView;
	CustomScene* m_mainGraphicsScene;
	QGraphicsPixmapItem* m_pixmapItem;
	QPushButton* m_selectImageButton;
	QPushButton* m_runSegmentationButton;
	QPushButton* m_runHeightMapGenerationButton;
	QPushButton* m_runComponentsDetectionButton;
	QPushButton* m_runResetButton;
	QPushButton* m_applyHeightMapData;
	QPushButton* m_showHeightMapImageButton;
	QString m_selectedImagePath;
	QImage m_image;
	cv::Mat m_heightMapImage;
	cv::Mat m_heightMapInitImage;
	cv::Mat m_sinrInitImage;
	cv::Mat m_contourImage;
	cv::Mat m_cvImage;
	QPolygonF m_poly;
	QComboBox * m_roomColorComboBox;
//	QSpinBox* m_rotationAngleSpinBox;
	QDoubleSpinBox* m_longtitudeSpinBox1;
	QDoubleSpinBox* m_latitudeSpinBox1;
	QDoubleSpinBox* m_longtitudeSpinBox2;
	QDoubleSpinBox* m_latitudeSpinBox2;
	QDoubleSpinBox* m_scaleValueSpinBoxX;
	QDoubleSpinBox* m_scaleValueSpinBoxY;
	QComboBox* m_roomTypeComboBoBox;
	bool m_isDbmBoxChecked;
	bool m_isSinrBoxChecked;
	QCheckBox* m_DbmShowingButton;
	QCheckBox* m_SinrShowingButton;
	QCheckBox* m_contourShowingButton;
	QGraphicsRectItem*  m_graphicsRectItem;
	QGraphicsEllipseItem* m_elipseItemForAxisPoint;
	//elipseItemForAxisPoint
	std::vector<QGraphicsEllipseItem*> m_elipseItemsVec;
	std::vector<QGraphicsEllipseItem>* m_elipsItemsForDetectedComponents;
	QGraphicsTextItem* m_graphicsTextItem;
	//QGraphicsPolygonItem
	std::vector<QPointF> m_elipseItemsVecMain;
	std::vector<std::pair<cv::Point, cv::Point>> m_pointsVecForHeightMap;
	int m_countOfHeightMapPoints;
	float m_rotationAngle;
	float m_longtitudeValue1;
	float m_latitudeValue1;
	float m_longtitudeValue2;
	float m_latitudeValue2;
	float m_scaleValueX;
	float m_scaleValueY;
	cv::FileStorage m_fs;

	componentsDetector m_componentDetector;

	bool m_isSelectedAxisPoint;
	int m_indexOfPolygonPoint;
	

};


#endif // FLOORPLANSEGMENTATIONGUI_H
