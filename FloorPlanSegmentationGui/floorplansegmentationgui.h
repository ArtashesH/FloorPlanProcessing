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
				emit removeLastSelectedPoint();
			
		}

	
	}
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
		std::cout << "DOUBLE CLICKED \n";
		emit finishSelectingPoints();
	}
public:

	QPoint transferPickedPointsPosition()
	{
		return m_pickedPoint;
	}
private:
	void joinCloseRects();
	bool isRectsIntersecting();
private:
	QPoint m_pickedPoint;


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
	void coloringOfSelectedPolygonSlot();
	void coloringOfSelectedWalls();
	void removingSelectedPointsSlot();
	void onRemoveLastSelectedPoint();


public:
	FloorPlanSegmentationGui(QWidget *parent = 0);
	
	~FloorPlanSegmentationGui();


protected:
	bool eventFilter(QEvent *event);

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
	QString m_selectedImagePath;
	QImage m_image;
	cv::Mat m_cvImage;
	QPolygonF m_poly;
	QComboBox * m_roomColorComboBox;
	QComboBox* m_roomTypeComboBoBox;
	std::vector<QGraphicsEllipseItem*> m_elipseItemsVec;
	//QGraphicsPolygonItem
	std::vector<QPointF> m_elipseItemsVecMain;


	int m_indexOfPolygonPoint;
	

};


#endif // FLOORPLANSEGMENTATIONGUI_H
