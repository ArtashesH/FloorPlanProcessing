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


#if PY_MAJOR_VERSION >= 3
#define IS_PY3K
#endif



#include "ui_floorplansegmentationgui.h"




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
	QGraphicsView* m_mainGraphicsView;
	CustomScene* m_mainGraphicsScene;
	QGraphicsPixmapItem* m_pixmapItem;
	QPushButton* m_selectImageButton;
	QPushButton* m_runSegmentationButton;
	QPushButton* m_runHeightMapGenerationButton;
	QPushButton* m_applyHeightMapData;
	QPushButton* m_showHeightMapImageButton;
	QString m_selectedImagePath;
	QImage m_image;
	cv::Mat m_heightMapImage;
	cv::Mat m_contourImage;
	cv::Mat m_cvImage;
	QPolygonF m_poly;
	QComboBox * m_roomColorComboBox;
	QComboBox* m_roomTypeComboBoBox;
	bool m_isDbmBoxChecked;
	bool m_isSinrBoxChecked;
	QCheckBox* m_DbmShowingButton;
	QCheckBox* m_SinrShowingButton;
	QCheckBox* m_contourShowingButton;
	QGraphicsRectItem*  m_graphicsRectItem;
	std::vector<QGraphicsEllipseItem*> m_elipseItemsVec;
	QGraphicsTextItem* m_graphicsTextItem;
	//QGraphicsPolygonItem
	std::vector<QPointF> m_elipseItemsVecMain;
	std::vector<std::pair<cv::Point, cv::Point>> m_pointsVecForHeightMap;
	int m_countOfHeightMapPoints;
	cv::FileStorage m_fs;


	int m_indexOfPolygonPoint;
	

};


#endif // FLOORPLANSEGMENTATIONGUI_H
