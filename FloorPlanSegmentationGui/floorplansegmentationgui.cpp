#include <QFileDialog>
#include <QGraphicsView>
#include <QLineEdit>

#include <QComboBox>



#include <QMouseEvent>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QHeaderView>



//#include <Python.h>
#include <string>

#include <fstream>



#include "floorplansegmentationgui.h"


cv::RNG rng(12345);


FloorPlanSegmentationGui::FloorPlanSegmentationGui(QWidget *parent)
	: QMainWindow(parent)
{
	m_countOfHeightMapPoints = 0;
	m_mainWidget = new QWidget;
	m_segmentDataWidget = new QWidget;
	m_parametersWidget = new QDialog();
	m_parametersWidget->installEventFilter(this);

	//QString filename = "E:/upworkProjs/FloorPlanAnalyzing/Venue_Images/TuckerStadium/APLayout.PNG";
	//QImage image(filename);

	m_mainGraphicsScene = new CustomScene();
	m_mainGraphicsScene->setSceneRect(0, 0, 1000, 1000);


	QPolygonF poly;
	poly << QPointF(10, 10) << QPointF(10, 50) << QPointF(30, 70) << QPointF(60, 50) << QPointF(50, 10);
	QBrush brush;
	brush.setColor(Qt::green);
	brush.setStyle(Qt::SolidPattern);
	
	QPen pen(Qt::green);
	QGraphicsScene graphics_scene_ = new QGraphicsScene(0, 0, 200, 200);
	//m_mainGraphicsScene->addPolygon(poly, pen, brush);
	//setScene(graphics_scene_);

	m_mainGraphicsView = new QGraphicsView(m_mainGraphicsScene);
	m_mainGraphicsView->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	m_mainGraphicsView->setMouseTracking(true);


	m_selectImageButton = new QPushButton("Select Image");
	m_runSegmentationButton = new QPushButton("Run Segmentation");
	m_runHeightMapGenerationButton = new QPushButton("Run HeighMap Generation");



	m_applyHeightMapData = new QPushButton("ApplyHeightMapSlection");
	m_applyHeightMapData->setEnabled(false);

	m_pixmapItem = new QGraphicsPixmapItem(/*QPixmap::fromImage(image)*/);
	m_mainGraphicsScene->addItem(m_pixmapItem);
	m_pixmapItem->setPos(0, 0);
	

	m_indexOfPolygonPoint = 0;
	m_lineForTypeOfRoom =  new QLineEdit;

	//m_segmentDataWidget->addAction(m_lineForTypeOfRoom);

	QVBoxLayout* layoutForParameterWidget = new QVBoxLayout;

	QPushButton *btn1 = new QPushButton("Apply");

    m_tableWidget = new QTableWidget;
	m_tableWidget->setObjectName("ParametersTableWidget");
	m_tableWidget->setRowCount(2);
	m_tableWidget->setColumnCount(2);

	m_DbmShowingButton = new QCheckBox("Show Dbm values");
	m_SinrShowingButton = new QCheckBox("Show Sinr values");
	m_contourShowingButton = new QCheckBox("Show HeightMap Contour");
	m_isDbmBoxChecked = false;
	m_isSinrBoxChecked = false;

	m_selectComponentWidget = new QComboBox();
	m_selectComponentWidget->setFixedSize(100, 25);
	m_selectComponentWidget->addItem("Room");
	m_selectComponentWidget->addItem("Window");
	m_selectComponentWidget->addItem("Door");
	m_selectComponentWidget->addItem("Wall");
	m_selectComponentWidget->addItem("HeightMap");
	
	


	m_roomColorComboBox = new QComboBox();
	m_roomColorComboBox->addItem("Green");
	m_roomColorComboBox->addItem("Yellow");
	m_roomColorComboBox->addItem("Red");
	m_roomColorComboBox->addItem("Gray");
	m_roomColorComboBox->addItem("Black");
	m_roomColorComboBox->addItem("Blue");
	m_roomColorComboBox->addItem("DarkRed");
	m_roomColorComboBox->addItem("DarkGreen");
	m_roomColorComboBox->addItem("DarkBlue");
	m_roomColorComboBox->addItem("DarkYellow");
	


	m_tableWidget->setCellWidget(0, 1, m_roomColorComboBox);

	//m_isPythonModuleInitialized = false;
	m_graphicsRectItem = new QGraphicsRectItem;







	m_graphicsTextItem = new QGraphicsTextItem;
	
	m_graphicsTextItem->setDefaultTextColor(QColor(0, 255, 0));
	


	QTableWidgetItem *item = new QTableWidgetItem("Color", QTableWidgetItem::Type);
	item->setFlags(item->flags() ^ Qt::ItemIsEditable);
	m_tableWidget->setItem(0, 0, item);

   
	QTableWidgetItem *item1 = new QTableWidgetItem("Name of Room", QTableWidgetItem::Type);
	item1->setFlags(item1->flags() ^ Qt::ItemIsEditable);
	m_tableWidget->setItem(1, 0, item1);

	m_tableWidget->verticalHeader()->setVisible(false);
	m_tableWidget->horizontalHeader()->setVisible(false);


	
	layoutForParameterWidget->addWidget(m_tableWidget);
	layoutForParameterWidget->addWidget(btn1);

	m_parametersWidget->setLayout(layoutForParameterWidget);



	std::string imageSizeFileName = "imageSize.txt";
	std::ofstream fileSize;
	fileSize.open(imageSizeFileName);
	fileSize.close();

	/*QLineEdit *le = new QLineEdit;
	le->setProperty("row", 0);
	le->setProperty("column", 0);
	m_tableWidget->setCellWidget(0, 0, le);
	m_tableWidget->setCellWidget(0, 1, le);
	*/


	/*PyObject* pName;

	Py_Initialize();

	pName = PyUnicode_FromString("MainRf_rayTracingCurrentState");
	


	m_pModule = PyImport_Import(pName);
	
	m_pFunc = PyObject_GetAttrString(m_pModule, "finalFunctionHeightMap");
	std::cout << "before crash \n";
	//	}
	std::cout << "Prepare for python call111111111111111111111 !!!!! \n";
	Py_DECREF(pName);*/


	//m_tableWidget->setFlags(Qt::ItemIsUserCheckable);
	//m_tableWidget->setCheckState(Qt::Unchecked);
	//tableWidget->setItem(row, column, item);

	m_mainLayout= new QVBoxLayout();
	m_mainLayout->addWidget(m_selectComponentWidget);
	m_mainLayout->addWidget(m_DbmShowingButton);
	m_mainLayout->addWidget(m_SinrShowingButton);
	m_mainLayout->addWidget(m_contourShowingButton);
	m_mainLayout->addWidget(m_applyHeightMapData);
	m_mainLayout->addWidget(m_mainGraphicsView);
	m_mainLayout->addWidget(m_selectImageButton);
	m_mainLayout->addWidget(m_runSegmentationButton);
	m_mainLayout->addWidget(m_runHeightMapGenerationButton);

	QWidget * w = new QWidget();
	m_mainWidget->setLayout(m_mainLayout);


	ui.setupUi(this);
	this->setCentralWidget(m_mainWidget);
	connect(m_selectImageButton, SIGNAL(clicked()), this, SLOT(setSelectedImagePath()));
	connect(m_runSegmentationButton, SIGNAL(clicked()), this, SLOT(runSegmentation()));
	connect(m_runHeightMapGenerationButton, SIGNAL(clicked()), this, SLOT(runHeightMapGeneration()));
	connect(m_mainGraphicsScene, SIGNAL(selectPointOnScene()), SLOT(getSelectedPoints()));
	connect(m_mainGraphicsScene, SIGNAL(finishSelectingPoints()), SLOT(drawPolygonFromSelectedPoints()));
	connect(m_mainGraphicsScene, SIGNAL(selectMovedPoint()), SLOT(drawHeightMapValue()));
	connect(m_mainGraphicsScene, SIGNAL(removeLastSelectedPoint()), SLOT(onRemoveLastSelectedPoint()));
    //connect(m_parametersWidget, SIGNAL(WidgetClosed()),  SLOT(coloringOfSelectedPolygonSlot()));
	connect(btn1, &QPushButton::clicked, m_parametersWidget, &QDialog::accept);
	connect(m_DbmShowingButton, SIGNAL(clicked()), this, SLOT(dbmShowingSlot()));
	connect(m_SinrShowingButton, SIGNAL(clicked()), this, SLOT(sinrShowingSlot()));
	connect(m_selectComponentWidget, SIGNAL(currentIndexChanged(const QString&)),this, SLOT(switchComponentType(const QString&)));
	connect(m_applyHeightMapData, SIGNAL(clicked()), this, SLOT(writeHeightMapPointsDataSlot()));
	connect(m_contourShowingButton, SIGNAL(clicked()), this, SLOT(drawHeightMapContourSlot()));

	//m_parametersWidget
}




 const wchar_t *  FloorPlanSegmentationGui::GetWC(const char *c)
{
	const size_t cSize = strlen(c) + 1;
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs(wc, c, cSize);
	return wc;

}


 void  FloorPlanSegmentationGui::callPythonFunction(/*const std::vector<int>& imageSizes*/)
{
	PyObject *pName, *pModule, *pFunc;
	PyObject *pArgs, *pValue;
	int i;
	pArgs = NULL;
	std::cout << "Prepare for python call !!!!! \n";
	/*if (argc < 3) {
	fprintf(stderr, "Usage: call pythonfile funcname [args]\n");
	return 1;
	}*/

////	Py_Initialize();

////	pName = PyUnicode_FromString("MainRf_rayTracingCurrentState");
	
	/* Error checking of pName left out */
	//if (!m_isPythonModuleInitialized) {
		//m_pModule = new PyObject;
		
	////	m_pModule = PyImport_Import(pName);
	
//	}
	std::cout << "Prepare for python call111111111111111111111 !!!!! \n";
////	Py_DECREF(pName);
	//if (m_pModule == NULL) {
	//	std::cout << "Module is null \n";
	

		///Py_Initialize();
		Py_Initialize();
		//pName = PyUnicode_FromString("MainRf_rayTracingCurrentState");

	
		//if (!m_isPythonModuleInitialized) {
		//m_pModule = new PyObject;

	//	m_pModule = PyImport_Import(pName);
		m_pFunc = PyObject_GetAttrString(m_pModule, "finalFunctionHeightMap");

	//}
	if (m_pModule != NULL) {
		//if (!m_isPythonModuleInitialized) {
		    std::cout << "Before Pfunction \n";
		//	m_pFunc = PyObject_GetAttrString(m_pModule, "finalFunctionHeightMap");
			std::cout << "After PFunction \n";
		//	m_isPythonModuleInitialized = true;
	//	}
		/* pFunc is a new reference */

		if (m_pFunc && PyCallable_Check(m_pFunc)) {
			//pArgs = PyTuple_New(2);
			/*for (i = 0; i < imageSizes.size(); ++i) {
				//	pValue = PyInt_FromLong(atoi(argv[i + 3]));
				std::cout << "Inside for cycle \n";
				pValue = PyLong_FromLong(imageSizes[i]);

				if (!pValue) {
					Py_DECREF(pArgs);
					Py_DECREF(m_pModule);
					fprintf(stderr, "Cannot convert argument\n");
					return;
				
				PyTuple_SetItem(pArgs, i, pValue);
			}*/
			std::cout << "Python function calll final !!!!! \n";
			pValue = PyObject_CallObject(m_pFunc, pArgs);
			Py_DECREF(pArgs);
			if (pValue != NULL) {

				printf("Result of call: %ld\n", PyLong_AsLong(pValue));
				Py_DECREF(pValue);
			}
			else {
				Py_DECREF(m_pFunc);
				Py_DECREF(m_pModule);
				PyErr_Print();
				fprintf(stderr, "Call failed\n");
				return;
			}
		}
		else {
			if (PyErr_Occurred())
				PyErr_Print();
			//fprintf(stderr, "Cannot find function \"%s\"\n", argv[2]);
		}
		Py_XDECREF(m_pFunc);
		Py_DECREF(m_pModule);
	}
	else {
		PyErr_Print();
		//fprintf(stderr, "Failed to load \"%s\"\n", argv[1]);
		return;
	}
	Py_Finalize();
	return;

}


void FloorPlanSegmentationGui::mousePressEvent(QGraphicsSceneMouseEvent * e)
{


	std::cout << "PRESSSSSSSSSSSSSSSSSSSSS " << std::endl;
	double rad = 1;

	//std::cout << "XXXXXXXXXXXXXXXXXXXXX   " << e->pos().x()<<std::endl;
	//	QPointF pt = m_mainGraphicsView->mapFromParent(e->pos());
	//std::cout << "POint coors " << std::endl;
	//m_mainGraphicsScene->addEllipse(pt.x() - rad, pt.y() - rad, rad, rad,
	//QPen(), QBrush(Qt::SolidPattern));

}

void FloorPlanSegmentationGui::switchComponentType(const QString& compName)
{
	/*std::cout << "Value changed \n";
	if (compName.toStdString() != "HeightMap") {
		m_fs.release();
	}*/


	if (m_selectComponentWidget->currentText() != "HeightMap") {
		std::string imageSizeFileName = "imageSize.txt";
		std::ofstream fileSize;
		fileSize.open(imageSizeFileName);
		fileSize.close();
	}



	if (compName.toStdString() == "HeightMap") {
		std::cout << "Opened new file !!!!!!!!!!!! \n";
		std::string filename = "testHeightMapData.xml";
		cv::FileStorage fs(filename, cv::FileStorage::WRITE);
		fs.release();
		QList<QGraphicsItem*> all = m_mainGraphicsScene->items();

		std::cout << "After getting items " << all.size() << " \n";
		//m_mainGraphicsScene->clear();
		//if (all.size() > 0) {
		for (int i = 0; i < all.size(); i++)
		{
			QGraphicsItem *gi = all[i];
			m_mainGraphicsScene->removeItem(gi);
			//delete gi;
			m_mainGraphicsScene->update();
		}
		m_pixmapItem = new QGraphicsPixmapItem();
		m_mainGraphicsScene->addItem(m_pixmapItem);
		m_pixmapItem->setPixmap(QPixmap::fromImage(m_image));
		m_pixmapItem->setPos(0, 0);


		m_poly.clear();
		for (int i = 0; i < m_elipseItemsVec.size(); ++i) {
			m_mainGraphicsScene->removeItem(m_elipseItemsVec[i]);

		}
		for (int i = 0; i < m_elipseItemsVec.size(); ++i) {
			//m_mainGraphicsScene->removeItem(m_elipseItemsVec[i]);
			delete m_elipseItemsVec[i];
		}
		m_elipseItemsVec.clear();

		qDebug() << "Dialog closed imid";

		m_countOfHeightMapPoints = 0;
		m_pointsVecForHeightMap.clear();
		std::cout << "Selected heightmap mode \n";
		m_applyHeightMapData->setEnabled(true);
	}
	else {
		m_pixmapItem = new QGraphicsPixmapItem();
		m_mainGraphicsScene->addItem(m_pixmapItem);
		
		m_pixmapItem->setPixmap(QPixmap::fromImage(m_image));
		m_pixmapItem->setPos(0, 0);
	}




	
}



void FloorPlanSegmentationGui::writeHeightMapPointsDataSlot()
{
	std::cout << "HeightMap points count " << m_pointsVecForHeightMap.size() << std::endl;

	std::string filename = "testHeightMapData.xml";
	cv::FileStorage fs(filename, cv::FileStorage::APPEND);



	std::vector<std::vector<cv::Point>> myvector;
		for (int i = 0; i < 3; ++i) {
			std::vector<cv::Point> tmpVec;
			myvector.push_back(tmpVec);
		}
	
		cv::Point point11 = cv::Point(10, 20);
		cv::Point point12 = cv::Point(10, 50);
		myvector[0].push_back(point11);
		myvector[0].push_back(point12);
	
		cv::Point point21 = cv::Point(50, 50);
		cv::Point point22 = cv::Point(100, 50);
		myvector[1].push_back(point21);
		myvector[1].push_back(point22);
	
		cv::Point point31 = cv::Point(150, 150);
		cv::Point point32 = cv::Point(200, 200);
		myvector[2].push_back(point31);
		myvector[2].push_back(point32);
	
	
	
//		std::string filename = "testHeightMapData.xml";
	//	cv::FileStorage fs(filename, cv::FileStorage::WRITE);
		//fs << "data" << "{";
		for (int i = 0; i < m_pointsVecForHeightMap.size(); ++i)
		{
			//Write each vector
			std::string vectorName ="vector" + std::to_string((long)i) ;
			fs <<  vectorName<< "{";
			//fs << pointName << "{";
			//for (int j = 0; j < m_pointsVecForHeightMap[i].size(); ++j)
		//	{
				std::string pointNameX0 = "x" + std::to_string(0);
				std::string pointNameY0 = "y" + std::to_string(0);
				std::string pointNameX1 = "x" + std::to_string(1);
				std::string pointNameY1 = "y" + std::to_string(1);
				fs << pointNameX0 << m_pointsVecForHeightMap[i].first.x << pointNameY0 << m_pointsVecForHeightMap[i].first.y;
				if (i == m_pointsVecForHeightMap.size() - 1 && m_countOfHeightMapPoints % 2 == 1) {
					fs << pointNameX1 << m_pointsVecForHeightMap[i].first.x << pointNameY1 << m_pointsVecForHeightMap[i].first.y;
				}
				else {
					fs << pointNameX1 << m_pointsVecForHeightMap[i].second.x << pointNameY1 << m_pointsVecForHeightMap[i].second.y;
				}

				//fs << "}"; // close point
	//		}
			fs << "}"; // close vector
		}
		//fs << "}"; // close data
		fs.release();


		m_pointsVecForHeightMap.clear();
		m_countOfHeightMapPoints = 0;
	
}


void FloorPlanSegmentationGui::drawHeightMapContourSlot()
{

	std::cout << "contour drawing slot !!!!!!!!!!!!!  \n";

	cv::Mat heightmapImage;
	heightmapImage = m_heightMapImage.clone();
	//QImage image;
	if (m_contourShowingButton->isChecked()) {
		heightmapImage = heightmapImage + m_contourImage.clone();
	} 
	QImage image(heightmapImage.data, heightmapImage.cols, heightmapImage.rows, QImage::Format_RGB888);




	//	QImage image(fileName);
	//image = image.scaled(image.width()/1.3,image.height()/1.3);
	/////	m_image = image;
	QList<QGraphicsItem*> all = m_mainGraphicsScene->items();

	std::cout << "After getting items " << all.size() << " \n";
	//m_mainGraphicsScene->clear();
	//if (all.size() > 0) {
	for (int i = 0; i < all.size(); i++)
	{
		QGraphicsItem *gi = all[i];
		m_mainGraphicsScene->removeItem(gi);
		//delete gi;
		m_mainGraphicsScene->update();
	}
	m_pixmapItem = new QGraphicsPixmapItem();
	m_mainGraphicsScene->addItem(m_pixmapItem);
	m_pixmapItem->setPixmap(QPixmap::fromImage(image));
	m_pixmapItem->setPos(0, 0);




}

void FloorPlanSegmentationGui::setSelectedImagePath() {


	std::string imageSizeFileName = "imageSize.txt";
	std::ofstream fileSize;
	fileSize.open(imageSizeFileName);
	fileSize.close();

	std::string filename = "testHeightMapData.xml";
	cv::FileStorage fs(filename, cv::FileStorage::WRITE);
	fs.release();


	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Open Image"), "/home/jana", tr("Image Files (*.png *.jpg *.bmp)"));
	std::cout << "CLICKED !!!!" << std::endl;
	//QString filename = "E:/upworkProjs/FloorPlanAnalyzing/Venue_Images/TuckerStadium/1.png";
	QImage image(fileName);
	//image = image.scaled(image.width()/1.3,image.height()/1.3);
	m_image = image;




	QList<QGraphicsItem*> all = m_mainGraphicsScene->items();

	std::cout << "After getting items " << all.size() << " \n";
	//m_mainGraphicsScene->clear();
	//if (all.size() > 0) {
	for (int i = 0; i < all.size(); i++)
	{
		QGraphicsItem *gi = all[i];
		m_mainGraphicsScene->removeItem(gi);
		//delete gi;
		m_mainGraphicsScene->update();
	}
	m_pixmapItem = new QGraphicsPixmapItem();
	m_mainGraphicsScene->addItem(m_pixmapItem);
	m_pixmapItem->setPixmap(QPixmap::fromImage(m_image));
	m_pixmapItem->setPos(0, 0);

	
	m_poly.clear();
	for (int i = 0; i < m_elipseItemsVec.size(); ++i) {
		m_mainGraphicsScene->removeItem(m_elipseItemsVec[i]);

	}
	for (int i = 0; i < m_elipseItemsVec.size(); ++i) {
		//m_mainGraphicsScene->removeItem(m_elipseItemsVec[i]);
		delete m_elipseItemsVec[i];
	}
	m_elipseItemsVec.clear();






	//fileSize.open(imageSizeFileName);
	//fileSize << m_image.width() << " " << m_image.height() << " " << 10 << " " << 10;
	//fileSize.close();



	m_mainGraphicsScene->setSceneRect(0,0,image.width(), image.height());
//	m_mainGraphicsScene->clear();
	QImage temp = m_image.copy();
	cv::Mat res(temp.height(), temp.width(), CV_8UC4, (uchar*)temp.bits(), temp.bytesPerLine());
	cvtColor(res, res, CV_BGRA2BGR);
	m_cvImage = res.clone();

	m_pixmapItem->setPixmap(QPixmap::fromImage(m_image));
	m_pixmapItem->setPos(0, 0);


	 all = m_mainGraphicsScene->items();
	
	std::cout << "After getting items " << all.size() << " \n";
	//m_mainGraphicsScene->clear();
	//if (all.size() > 0) {
		for (int i = 0; i < all.size(); i++)
		{
			QGraphicsItem *gi = all[i];
			m_mainGraphicsScene->removeItem(gi);
			delete gi;
			m_mainGraphicsScene->update();
		}
	//}

		m_pixmapItem = new QGraphicsPixmapItem(/*QPixmap::fromImage(image)*/);
		m_mainGraphicsScene->addItem(m_pixmapItem);
		m_pixmapItem->setPixmap(QPixmap::fromImage(m_image));
		m_pixmapItem->setPos(0, 0);
	
		
}


void  FloorPlanSegmentationGui::runSegmentation() {

	int maximumContSize = 0;
	int indexOfBiggestCont = -1;
	cv::Mat canny_output;
	std::vector<std::vector<cv::Point> > contours;
	std::vector<std::vector<cv::Point> > filteredContours;
	std::vector<cv::Vec4i> hierarchy;
	//cv::Mat initialMat = cv::imread("E:/upworkProjs/FloorPlanAnalyzing/2dFloorPlanSimple.jpg");
	//cv::mat initialmat = cv::imread("e:/upworkprojs/floorplananalyzing/venue_images/columbus circle/2ndfloor.png");
	//cv::blur(initialmat, initialmat, cv::size(3, 3));
	//cv::resize(initialmat, initialmat, cv::size(initialmat.cols / 1.3, initialmat.rows / 1.3));
	cv::Mat imgfordraw =  m_cvImage.clone();
	cv::Mat initialMat = m_cvImage.clone();
	cv::cvtColor(initialMat, initialMat, CV_BGR2GRAY);
	//cv::imshow("FIrstImage", initialMat);
//	cv::waitKey(0);
	//cv::dilate(initialmat, initialmat, cv::mat());
	////	cv::dilate(initialmat, initialmat, cv::mat());
	////	cv::dilate(initialmat, initialmat, cv::mat());
	//
	

	cv::erode(initialMat, initialMat, cv::Mat());
		
	//	cv::erode(initialmat, initialmat, cv::mat());
	//	cv::erode(initialmat, initialmat, cv::mat());
	//	cv::blur(initialmat, initialmat, cv::size(3, 3));
	//	cv::erode(initialmat, initialmat, cv::mat());
		
	
	cv::Mat cannyImg;
	cv::Canny(initialMat, cannyImg, 150, 170);

	cv::findContours(cannyImg, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
	std::cout << "contours sizwe " << contours.size() << std::endl;
	
	
	cv::Mat drawing = cv::Mat::zeros(initialMat.size(), CV_8UC3);
	
	for (int i = 0; i < contours.size(); ++i) {
			cv::Scalar color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			if (contours[i].size() > 10) {
				cv::drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, cv::Point());
			}
		}
		//cv::imshow("drawinginit", drawing);
	
		//cv::waitKey(0);
	
	
		for (int i = 0; i< contours.size(); i++)       
		{
			//drawing = imgfordraw.clone(); 
			if (contours[i].size() > maximumContSize) {
				maximumContSize = contours[i].size();
				indexOfBiggestCont = i;
			}
			cv::Scalar color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			//if (contours[i][0] == contours[i][contours[i].size() - 1]) {
				//drawcontours(drawing, contours, i, color, 2, 8, hierarchy, 0, cv::point());
			//} 
		//	cv::waitkey(0);
		}
	
		cv::Rect bigestrect = cv::boundingRect(contours[indexOfBiggestCont]);
		for (int j = 0; j < contours.size(); ++j) {
			cv::Rect tmprect = cv::boundingRect(contours[j]);
			if ( 1 ||   bigestrect.area() / tmprect.area() >= 1.  && bigestrect.area() / tmprect.area() < 100) {
				filteredContours.push_back(contours[j]);
				//cv::scalar color = cv::scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
				//drawcontours(drawing, contours, j, color, 2, 8, hierarchy, 0, cv::point());
			
			}
	
		}
		std::cout << "filtered contours size " << filteredContours.size() << std::endl;
		cv::Mat contMat = cv::Mat(imgfordraw.rows, imgfordraw.cols, CV_8UC3, cv::Scalar::all(0));

		for (int i = 0; i < filteredContours.size(); ++i) {
			cv::Scalar color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			drawContours(imgfordraw, filteredContours, i, color, 2, 8, hierarchy, 0, cv::Point());
			drawContours(contMat, filteredContours, i, color, 2, 8, hierarchy, 0, cv::Point());
		//	cv::pointpolygontest(filteredcontours[i],)
		//	cv::fillpoly(contmat, contours, cv::scalar(255, 255, 255));
		//	ispointinsidecontour(filteredcontours[i], contmat);
			std::cout << "current contour initiasl " << std::endl;
			
		//	cv::imshow("drawing", imgfordraw);
		//	cv::waitkey(0);
		}
		
		for (int g = 0; g < filteredContours.size(); ++g) {
	
			//ispointinsidecontour(filteredcontours[g], contmat);
			//continue;
			std::cout << "current contour" << std::endl;
			cv::Rect tmprect = cv::boundingRect(filteredContours[g]);
			cv::Scalar color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		//	cv::scalar color = cv::scalar(0, 255, 0);
			for (int i = 0; i < imgfordraw.rows; ++i) {
				for (int j = 0; j < imgfordraw.cols; ++j) {
					//imgfordraw.at<cv::vec3b>(i, j) = cv::vec3b(255, 0, 0);
					
					if ( cv::pointPolygonTest(filteredContours[g],cv::Point2f(j,i),false) ==  1 /*tmprect.contains(cv::point2i(j,i))*//*cv::pointpolygontest(filteredcontours[g],cv::point2f(j,i),false) == 1 || cv::pointpolygontest(filteredcontours[g], cv::point2f(j, i), false) == 0*/) {
						contMat.at<cv::Vec3b>(i, j)[0] = color[0] ;
						contMat.at<cv::Vec3b>(i, j)[1] = color[1];
						contMat.at<cv::Vec3b>(i, j)[2] = color[2];
	
					}
					/*else if (cv::pointpolygontest(filteredcontours[g], cv::point2f(j, i), true) > -5) {
						contmat.at<cv::vec3b>(i, j)[0] = color[0];
						contmat.at<cv::vec3b>(i, j)[1] = color[1];
						contmat.at<cv::vec3b>(i, j)[2] = color[2];
					}*/
					
				//	cv::imshow("imgfordraw", imgfordraw);
				//	cv::waitkey(10);
					
				}
			}
			
		
		}
	
	//	cv::namedWindow("contmatfinal");
		//cv::moveWindow("contmatfinal", 100, 100);
		//cv::imshow("contmatfinal", contMat);
		//cv::imwrite("e:/upworkprojs/floorplananalyzing/goodworkingexamples/2dfloorplansimpleres8.jpg",contmat);
		//cv::imshow("imgfordraw", imgfordraw);
		
		//cv::waitKey(0);
	//	drawcontours(drawing, contours, indexofbiggestcont, cv::scalar(0,255,0), 2, 8, hierarchy, 0, cv::point());
	
		/*for (int i = 0; i< contours.size(); i++)
		{
			if (contours[i].size() > maximumcontsize) {
				maximumcontsize = contours[i].size();
				indexofbiggestcont = i;
			}
			if (i != maximumcontsize) {
				cv::scalar color = cv::scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
				drawcontours(drawing, contours, i, color, 2, 8, hierarchy, 0, cv::point());
			}
		}*/


		QImage imgIn = QImage((uchar*)contMat.data, contMat.cols, contMat.rows, contMat.step, QImage::Format_RGB888);
		m_pixmapItem->setPixmap(QPixmap::fromImage(imgIn));
		m_pixmapItem->setPos(0, 0);
		


		
	//	cv::imshow("initimg", initialMat);		
		//cv::waitKey(0); 
	;
}


void FloorPlanSegmentationGui::runHeightMapGeneration()
{

	int retCode = system("HeightMapGeneration.exe ");

	cv::namedWindow("HeightMap1");
	cv::Mat heightmapImage = cv::imread("resultImageFull.png");

	QString fileName = "resultImageFull.png";
	

	QString fileName1 = "maskImgHeight.png";
	//QImage image1(fileName1);
	

	cv::Mat maskImg = cv::imread(fileName1.toStdString());
	cv::Mat resContourImage;
	findOuterContour(maskImg, resContourImage);

	cv::cvtColor(resContourImage, resContourImage, CV_BGR2RGB);
	QImage imageContour(resContourImage.data, resContourImage.cols, resContourImage.rows, QImage::Format_RGB888);

	

	cv::cvtColor(heightmapImage, heightmapImage, CV_BGR2RGB);
	m_heightMapImage = heightmapImage.clone();
	m_contourImage = resContourImage.clone();


	if (m_contourShowingButton->isChecked()) {
		heightmapImage = heightmapImage + resContourImage;
	}

	QImage image(heightmapImage.data, heightmapImage.cols, heightmapImage.rows, QImage::Format_RGB888);

   QImage imageTr(heightmapImage.data, heightmapImage.cols, heightmapImage.rows, QImage::Format_RGBA8888);
   QImage imageTr1 = QImage(heightmapImage.cols, heightmapImage.rows, QImage::Format_RGBA8888);

 


   for (int i = 0; i < imageTr1.height(); ++i) {
	   for (int j = 0; j < imageTr1.width(); ++j) {
		  // QRgb apixel = image.pixel(j, i);
		   //imageTr.setPixel(j, i,
			  // qRgba(image.pixel(j,i

		   imageTr1.setPixel(j, i,
			   qRgba(heightmapImage.at<cv::Vec3b>(i,j)[0],
				   heightmapImage.at<cv::Vec3b>(i, j)[1],
				   heightmapImage.at<cv::Vec3b>(i, j)[2],
				   230));
				//   qRgba valueTmp1 = QRgba(apixel.red(), apixel.green(), apixel.blue(), 200);
		   
		  // p2.setPixel(i, j, value);
	   }
	}

	
    
//	QImage image(fileName);
	//image = image.scaled(image.width()/1.3,image.height()/1.3);
/////	m_image = image;
	QList<QGraphicsItem*> all = m_mainGraphicsScene->items();

	std::cout << "After getting items " << all.size() << " \n";
	//m_mainGraphicsScene->clear();
	//if (all.size() > 0) {
	for (int i = 0; i < all.size(); i++)
	{
		QGraphicsItem *gi = all[i];
		m_mainGraphicsScene->removeItem(gi);
		//delete gi;
		m_mainGraphicsScene->update();
	}



	m_pixmapItem = new QGraphicsPixmapItem();
	m_mainGraphicsScene->addItem(m_pixmapItem);
	m_pixmapItem->setPixmap(QPixmap::fromImage(m_image));
	m_pixmapItem->setPos(0, 0);

	

	m_pixmapItem = new QGraphicsPixmapItem();
	m_mainGraphicsScene->addItem(m_pixmapItem);
	m_pixmapItem->setPixmap(QPixmap::fromImage(imageTr1));
//	m_mainGraphicsScene->addItem(m_pixmapItem);
	m_pixmapItem->setPos(0, 0);
	







/*	QList<QGraphicsItem*> all = m_mainGraphicsScene->items();

	std::cout << "After getting items " << all.size() << " \n";


	QImage image("testfileRes.png");
	//image = image.scaled(image.width()/1.3,image.height()/1.3);
	
	m_mainGraphicsScene->setSceneRect(0, 0, image.width(), image.height());
	m_pixmapItem = new QGraphicsPixmapItem();
	m_mainGraphicsScene->addItem(m_pixmapItem);
	m_pixmapItem->setPixmap(QPixmap::fromImage(image));
	m_pixmapItem->setPos(0, 0);

*/













	/*std::cout << "Called heihjt map generation \n";
	std::vector<int> imageSizeVec;
	imageSizeVec.push_back(m_image.width());
	imageSizeVec.push_back(m_image.height());*/
//	callPythonFunction(/*imageSizeVec*/);
		//PyObject *pName, *pModule, *pFunc;
		//PyObject *pArgs, *pValue;
		//int i;
	
	
		///*if (argc < 3) {
		//	fprintf(stderr, "Usage: call pythonfile funcname [args]\n");
		//	return 1;
		//}*/
	
		//Py_Initialize();
		//
		//pName = PyUnicode_FromString( "MainRf_rayTracingCurrentState");
		//
		///* Error checking of pName left out */
	
		//pModule = PyImport_Import(pName);
		//Py_DECREF(pName);
	
		//if (pModule != NULL) {
		//	pFunc = PyObject_GetAttrString(pModule, "finalFunctionHeightMap");
		//	/* pFunc is a new reference */
	
		//	if (pFunc && PyCallable_Check(pFunc)) {
		//		//pArgs = PyTuple_New(argc - 3);
		//		//for (i = 0; i < argc - 3; ++i) {
		//		////	pValue = PyInt_FromLong(atoi(argv[i + 3]));
		//		//	pValue =   PyLong_FromLong(atol(argv[i + 3]));
		//		//	
		//		//	if (!pValue) {
		//		//		Py_DECREF(pArgs);
		//		//		Py_DECREF(pModule);
		//		//		fprintf(stderr, "Cannot convert argument\n");
		//		//		return 1;
		//		//	}
		//		//	/* pValue reference stolen here: */
		//		//	PyTuple_SetItem(pArgs, i, pValue);
		//		//}
		//		std::cout << "Before calling function \n";
		//		//printf(pFunc.c);
		//		pValue = PyObject_CallObject(pFunc, pArgs);
		//		std::cout << "After calling functio n  \n";
		//		Py_DECREF(pArgs);
		//		if (pValue != NULL) {
		//			
		//			printf("Result of call: %ld\n", PyLong_AsLong(pValue));
		//			Py_DECREF(pValue);
		//		}
		//		else {
		//			Py_DECREF(pFunc);
		//			Py_DECREF(pModule);
		//			PyErr_Print();
		//			fprintf(stderr, "Call failed\n");
		//			return ;
		//		}
		//	}
		//	else {
		//		if (PyErr_Occurred())
		//			PyErr_Print();
		//		//fprintf(stderr, "Cannot find function \"%s\"\n", argv[2]);
		//	}
		//	Py_XDECREF(pFunc);
		//	Py_DECREF(pModule);
		//}
		//else {
		//	PyErr_Print();
		//	//fprintf(stderr, "Failed to load \"%s\"\n", argv[1]);
		//	return ;
		//}
		//Py_Finalize();	
}
 


void FloorPlanSegmentationGui::getSelectedPoints()
{
	std::cout << "GET SELECTED POINTS  " << std::endl;
	QPoint selectedPoint = m_mainGraphicsScene->transferPickedPointsPosition();
	std::cout << "Select points " << selectedPoint.x() << "   " << selectedPoint.y() << std::endl;


	QBrush brush;
	brush.setColor(Qt::red);
	brush.setStyle(Qt::SolidPattern);

	QPen pen(Qt::green);


	QPainter qPainter(&m_image);
	qPainter.setBrush(brush);
	qPainter.setPen(Qt::red);
	//qPainter.drawRect(selectedPoint.x(), selectedPoint.y(), 150, 150);
	//qPainter.drawEllipse(selectedPoint, 20, 20);


	//m_pixmapItem->setPixmap(QPixmap::fromImage(m_image));
	//m_pixmapItem->setPos(0, 0);


	QImage temp = m_image.copy();
	cv::Mat res(temp.height(), temp.width(), CV_8UC4, (uchar*)temp.bits(), temp.bytesPerLine());
	//cv::circle(res, cv::Point(selectedPoint.x(), selectedPoint.y()), 20, cv::Scalar(0, 255, 0), 5);
	cvtColor(res, res, CV_BGRA2BGR); // make convert colort to BGR ! 
									 //cv::imshow("Initimg", res);
									 //cv::waitKey(0);
	cv::Point tmpSelectedPoint = cv::Point(selectedPoint.x(), selectedPoint.y());





	if (  m_selectComponentWidget->currentText() == "HeightMap" &&  m_countOfHeightMapPoints % 2 == 0) {
		std::pair<cv::Point, cv::Point> tmpHeightSelPoint;
		tmpHeightSelPoint.first = tmpSelectedPoint;
		m_pointsVecForHeightMap.push_back(tmpHeightSelPoint);
		++m_countOfHeightMapPoints;
		std::cout << "Added one time " << m_countOfHeightMapPoints << std::endl;
	 
		
	}
	else if (m_selectComponentWidget->currentText() == "HeightMap" &&  m_countOfHeightMapPoints % 2 != 0)  {
		++m_countOfHeightMapPoints;
		std::cout << "Added second point " << m_countOfHeightMapPoints << std::endl;
		m_pointsVecForHeightMap[m_pointsVecForHeightMap.size() - 1].second = tmpSelectedPoint;

		QBrush brush;
		brush.setColor(Qt::green);

		brush.setStyle(Qt::SolidPattern);
		QPen pen(Qt::green);
		pen.setBrush(brush);
		pen.setWidth(5);
		
		//QLineF tmpLine;

		QGraphicsLineItem* tmpLineitem = new QGraphicsLineItem;
		QLineF tmpLine;
		tmpLine.setLine(m_pointsVecForHeightMap[m_pointsVecForHeightMap.size() - 1].first.x, m_pointsVecForHeightMap[m_pointsVecForHeightMap.size() - 1].first.y, 
				m_pointsVecForHeightMap[m_pointsVecForHeightMap.size() - 1].second.x, m_pointsVecForHeightMap[m_pointsVecForHeightMap.size() - 1].second.y);
		tmpLineitem->setLine(tmpLine);
		tmpLineitem->setPen(pen);
		m_mainGraphicsScene->addItem(tmpLineitem);
	
	}
	//m_mainGraphicsScene->clear();
	//m_mainGraphicsScene->addEllipse(tmpSelectedPoint.x, tmpSelectedPoint.y, 10, 10, pen, brush);
	QGraphicsEllipseItem* elipseItem = new QGraphicsEllipseItem;
//	elipseItem->setPos(tmpSelectedPoint.x, tmpSelectedPoint.y);
	elipseItem->setPen(pen);
	elipseItem->setBrush(brush);
	elipseItem->setRect(tmpSelectedPoint.x, tmpSelectedPoint.y, 10, 10);
	m_mainGraphicsScene->addItem(elipseItem);
	m_elipseItemsVec.push_back(elipseItem);
	//	m_mainGraphicsScene->addEllipse(100, 100, 10, 10, pen, brush);
//if (m_poly.size() == 0 && m_indexOfPolygonPoint > 0) {
		m_poly << QPointF(tmpSelectedPoint.x, tmpSelectedPoint.y);
	//}
      
	++m_indexOfPolygonPoint;

	
	//	detectAxisPoints(res, tmpSelectedPoint);




}

void FloorPlanSegmentationGui::drawPolygonFromSelectedPoints()
{
   
	QBrush brush;
	brush.setColor(Qt::red);
	brush.setStyle(Qt::SolidPattern);

	QPen pen(Qt::green);

//	m_mainGraphicsScene->addPolygon(m_poly, pen, brush);
	//setScene(graphics_scene_);
	//m_poly.clear();
//	m_tableWidget->show();
	//coloringOfSelectedPolygonSlot();

	QPushButton* button = m_parametersWidget->findChild<QPushButton*>("Apply");
//	qDebug() << "COMPBOXOBOX TEST  " << m_roomTypeComboBoBox->currentText();
	int compRes  = QString::compare("Room", m_selectComponentWidget->currentText(), Qt::CaseInsensitive);
	int compResHeightMap = QString::compare("HeightMap", m_selectComponentWidget->currentText(), Qt::CaseInsensitive);
	if (m_parametersWidget->exec() == QDialog::Accepted)
	{
		if (!compRes ) {
			std::cout << "Selected rooom !!!!!!!!!!!!!" << std::endl;
			coloringOfSelectedPolygonSlot();
		}
		else{
			coloringOfSelectedWalls();
		}
		qDebug() << "Apply button pressed!!";
		m_poly.clear();

		for (int j = 0; j < m_elipseItemsVec.size(); ++j) {
			m_elipseItemsVecMain.push_back(QPointF(m_elipseItemsVec[j]->x(), m_elipseItemsVec[j]->y()));

		}

		for (int i = 0; i < m_elipseItemsVec.size(); ++i) {
			m_mainGraphicsScene->removeItem(m_elipseItemsVec[i]);
			

		}
		for (int i = 0; i < m_elipseItemsVec.size(); ++i) {
			//m_mainGraphicsScene->removeItem(m_elipseItemsVec[i]);
			delete m_elipseItemsVec[i];
		}
		m_elipseItemsVec.clear();



	}
	else
	{
	
		m_poly.clear();
		for (int i = 0; i < m_elipseItemsVec.size(); ++i) {
			m_mainGraphicsScene->removeItem(m_elipseItemsVec[i]);
		
		}
		for (int i = 0; i < m_elipseItemsVec.size(); ++i) {
			//m_mainGraphicsScene->removeItem(m_elipseItemsVec[i]);
			delete m_elipseItemsVec[i];
		}
		m_elipseItemsVec.clear();
		
		qDebug() << "Dialog closed imid";
	}
	//coloringOfSelectedPolygonSlot();
	//m_segmentDataWidget->show();
//	m_tableWidget->show();	
	//setCentralWidget(m_tableWidget);
	
}



bool FloorPlanSegmentationGui::parseHeightMapTxtFile(cv::Mat& resHeightMap, cv::Mat& resSINRMap)
{
	int number_of_lines = 0;
	int countOfColums = 0;
	std::string line;
	std::ifstream myfile("rfMapFull.txt");
	std::ifstream myfileSINR("sinrData.txt");

	std::vector<std::vector<int>> heighMapFileData;
	bool countOfColumscalculated = false;
	while (std::getline(myfile, line)) {
		std::vector<int> tmpLineData;
		++number_of_lines;
		std::vector<std::string> tokens;
		//if (!countOfColumscalculated) {
			//countOfColumscalculated = true;
		countOfColums = 0;
		std::string buf;                 // Have a buffer string
		std::stringstream ss(line);       // Insert the string into a stream

		//std::vector<std::string> tokens; // Create vector to hold our words

		while (ss >> buf)
			tmpLineData.push_back(std::atoi(buf.c_str()));
		if (!countOfColumscalculated) {
			countOfColums = tmpLineData.size();
		}
		else {
			if (countOfColums != tmpLineData.size()) {
				return false;
			}
		}
		heighMapFileData.push_back(tmpLineData);
				
	}
	resHeightMap = cv::Mat(number_of_lines, countOfColums, CV_32SC1);
	for (int i = 0; i < heighMapFileData.size(); ++i) {
		for (int j = 0; j < heighMapFileData[i].size(); ++j) {
			resHeightMap.at<int>(i, j) = heighMapFileData[i][j];
		}
	}
	number_of_lines = 0;
	countOfColums = 0;
   
	std::vector<std::vector<float>> sinrFileData;
	countOfColumscalculated = false;
	while (std::getline(myfileSINR, line)) {
		std::vector<float> tmpLineData;
		++number_of_lines;
		std::vector<std::string> tokens;
		//if (!countOfColumscalculated) {
		//countOfColumscalculated = true;
		countOfColums = 0;
		std::string buf;                 // Have a buffer string
		std::stringstream ss(line);       // Insert the string into a stream

										  //std::vector<std::string> tokens; // Create vector to hold our words

		while (ss >> buf)
			tmpLineData.push_back(std::atoi(buf.c_str()));
		if (!countOfColumscalculated) {
			countOfColums = tmpLineData.size();
		}
		else {
			if (countOfColums != tmpLineData.size()) {
				return false;
			}
		}
		sinrFileData.push_back(tmpLineData);

	}

	
	resSINRMap = cv::Mat(number_of_lines, countOfColums, CV_32FC1);
	if (resSINRMap.cols != resHeightMap.cols || resSINRMap.rows != resHeightMap.rows) {
		return false;
	}
	for (int i = 0; i < sinrFileData.size(); ++i) {
		for (int j = 0; j < sinrFileData[i].size(); ++j) {
			resSINRMap.at<float>(i, j) = sinrFileData[i][j];
		}
	}


	return true;

//	std::cout << "Number of lines in text file: " << number_of_lines<<"\n";
	//std::cout << "Count of colums " << tokens.size() << "\n";
}



void FloorPlanSegmentationGui::findOuterContour(const cv::Mat& inputMaskImg, cv::Mat& resImg)
{
	cv::Mat grayImg;
	cv::cvtColor(inputMaskImg, grayImg, CV_BGR2GRAY);
	

	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;

	/// Detect edges using canny
//cv::Canny(grayImg, canny_output, 100, 150, 3);

	/// Find contours
	findContours(grayImg, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
	cv::Mat drawing = cv::Mat::zeros(grayImg.size(), CV_8UC3);
	for (int i = 0; i< contours.size(); i++)
	{
		//cv::Scalar color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		cv::Scalar color = cv::Scalar(0, 255, 0);
		drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, cv::Point());
	}
	resImg = drawing.clone();
//	cv::imshow("DrawingImag", drawing);
//	cv::waitKey(0);


}

void FloorPlanSegmentationGui::drawHeightMapValue()
{
	QPoint tmpMovePoint  = m_mainGraphicsScene->transferMovedPointsPosition();
	//m_graphicsTextItem->setRect(tmpMovePoint.x(), tmpMovePoint.y(),10,10);
	//m_mainGraphicsScene->addItem(m_graphicsTextItem);
	m_graphicsTextItem->setPos(tmpMovePoint.x(), tmpMovePoint.y());
	m_graphicsTextItem->setScale(2);
	QString tmpPositon = QString(std::to_string(tmpMovePoint.x()).c_str()) + "  " +  QString(std::to_string(tmpMovePoint.y()).c_str());
	if (tmpMovePoint.x() < m_image.width() && tmpMovePoint.y() < m_image.height()  /*&&  m_roomTypeComboBoBox->currentText() == "HeightMap"*/) {
		cv::Mat heightMap;
		cv::Mat sinrMap;
		if (m_DbmShowingButton->isChecked()  &&  parseHeightMapTxtFile(heightMap,sinrMap) ) {
			if (heightMap.rows == m_image.height() && heightMap.cols == m_image.width() ) {
				std::cout << "FINAL HEIGHTMAPPPPPPPPPPPPPPPPPPP \n";
				//m_graphicsTextItem->setRect(tmpMovePoint.x(), tmpMovePoint.y(),10,10);
				tmpPositon = QString(std::to_string(heightMap.at<int>(tmpMovePoint.y(), tmpMovePoint.x())).c_str()) + " Dbm";
				m_graphicsTextItem->setPlainText(tmpPositon);
				//m_graphicsTextItem->setItemTitle
				m_mainGraphicsScene->addItem(m_graphicsTextItem);
				//std::cout << m_roomTypeComboBoBox->currentText().toStdString() << std::endl;
			}
		}
		else if (m_SinrShowingButton->isChecked() && parseHeightMapTxtFile(heightMap, sinrMap)) {
			if (sinrMap.rows == m_image.height() && sinrMap.cols == m_image.width()) {
				std::cout << "FINAL HEIGHTMAPPPPPPPPPPPPPPPPPPP  SINR\n";
				//m_graphicsTextItem->setRect(tmpMovePoint.x(), tmpMovePoint.y(),10,10);
				tmpPositon = QString(std::to_string(sinrMap.at<float>(tmpMovePoint.y(), tmpMovePoint.x())).c_str()) + " DB";
				m_graphicsTextItem->setPlainText(tmpPositon);
				//m_graphicsTextItem->setItemTitle
				m_mainGraphicsScene->addItem(m_graphicsTextItem);
				//std::cout << m_roomTypeComboBoBox->currentText().toStdString() << std::endl;
			}
		}
		else if (!m_SinrShowingButton->isChecked() && !m_DbmShowingButton->isChecked()) {
			m_mainGraphicsScene->removeItem(m_graphicsTextItem);
		}
		
	//	std::cout << m_roomTypeComboBoBox->currentText().toStdString() << "\n";

//		m_graphicsTextItem->setPlainText(tmpPositon);
//		m_mainGraphicsScene->addItem(m_graphicsTextItem);
		//std::cout << m_roomTypeComboBoBox->currentText().toStdString() << std::endl;
	}
	else {

		QList<QGraphicsItem*> all = m_mainGraphicsScene->items();

		std::cout << "After getting items " << all.size() << " \n";
		//m_mainGraphicsScene->clear();
		//if (all.size() > 0) {
		for (int i = 0; i < all.size(); i++)
		{
			QGraphicsItem *gi = all[i];
			QGraphicsTextItem* tmpItem = dynamic_cast<QGraphicsTextItem*>(gi);
			if (tmpItem) {
				m_mainGraphicsScene->removeItem(gi);
				//delete gi;
				m_mainGraphicsScene->update();
			}
		}
	//	tmpPositon = QString("-90") + QString(" Dbm");
	//	m_graphicsTextItem->setPlainText(tmpPositon);
	//	m_mainGraphicsScene->addItem(m_graphicsTextItem);
	}




}

bool FloorPlanSegmentationGui::eventFilter(QEvent *event)
{
	if (event->type() == QEvent::Close) {
		emit WidgetClosed();
		event->accept();
		//coloringOfSelectedPolygonSlot();
		m_poly.clear();
	}
	return false;
}


/*
bool FloorPlanSegmentationGui::eventFilter(QObject * obj, QEvent * ev)
{
	if (obj == m_mainGraphicsView)
		if (ev->type() == QEvent::MouseMove)
		{
			QMouseEvent *mEvent = (QMouseEvent*)ev;
			qDebug() << "Mouse move event!!!!!!!!!!!!!!!";
		}
	return false;
}
*/






void FloorPlanSegmentationGui::coloringOfSelectedWalls()
{
	QBrush brush;
	brush.setColor(Qt::green);
	
	brush.setStyle(Qt::SolidPattern);
	QPen pen(Qt::green);


	if (m_roomColorComboBox->currentText() == "Red") {
		brush.setColor(Qt::red);
		pen.setColor(Qt::red);
	}
	else if (m_roomColorComboBox->currentText() == "Green") {
		brush.setColor(Qt::green);
		pen.setColor(Qt::green);
	}
	else if (m_roomColorComboBox->currentText() == "Yellow") {
		brush.setColor(Qt::yellow);
		pen.setColor(Qt::yellow);
	}
	else if (m_roomColorComboBox->currentText() == "Blue") {
		brush.setColor(Qt::blue);
		pen.setColor(Qt::blue);
	}
	else if (m_roomColorComboBox->currentText() == "Black") {
		brush.setColor(Qt::black);
		pen.setColor(Qt::black);
	}
	else if (m_roomColorComboBox->currentText() == "Gray") {
		brush.setColor(Qt::gray);
		pen.setColor(Qt::gray);
	}
	else if (m_roomColorComboBox->currentText() == "DarkRed") {
		brush.setColor(Qt::darkRed);
		pen.setColor(Qt::darkRed);
	}
	else if (m_roomColorComboBox->currentText() == "DarkGreen") {
		brush.setColor(Qt::darkGreen);
		pen.setColor(Qt::darkGreen);
	}
	else if (m_roomColorComboBox->currentText() == "DarkBlue") {
		brush.setColor(Qt::darkBlue);
		pen.setColor(Qt::darkBlue);
	}
	else if (m_roomColorComboBox->currentText() == "DarkYellow") {
		brush.setColor(Qt::darkYellow);
		pen.setColor(Qt::darkYellow);
	}

	pen.setBrush(brush);
	pen.setWidth(5);
	for (int i = 0; i < m_poly.size()-1; ++i) {
		QLineF tmpLine;
		tmpLine.setLine(m_poly[i].x(), m_poly[i].y(), m_poly[i + 1].x(), m_poly[i + 1].y());
		//tmpLine.y1 = m_poly[i].y();
	//	tmpLine.x2 = m_poly[i+1].x();
		//tmpLine.y2 = m_poly[i+1].y();
		m_mainGraphicsScene->addLine(tmpLine, pen);
	}

}


void FloorPlanSegmentationGui::coloringOfSelectedPolygonSlot()
{
	std::cout << "Final coloring stuff \n";
	QBrush brush;
	QPen pen;
	//if (if  m_) {

	//}
	if (m_roomColorComboBox->currentText() == "Red") {
		brush.setColor(Qt::red);
		pen.setColor(Qt::red);
	}
	else if (m_roomColorComboBox->currentText() == "Green") {
		brush.setColor(Qt::green);
		pen.setColor(Qt::green);
	}
	else if (m_roomColorComboBox->currentText() == "Yellow") {
		brush.setColor(Qt::yellow);
		pen.setColor(Qt::yellow);
	}
	else if (m_roomColorComboBox->currentText() == "Blue") {
		brush.setColor(Qt::blue);
		pen.setColor(Qt::blue);
	}
	else if (m_roomColorComboBox->currentText() == "Black") {
		brush.setColor(Qt::black);
		pen.setColor(Qt::black);
	}
	else if (m_roomColorComboBox->currentText() == "Gray") {
		brush.setColor(Qt::gray);
		pen.setColor(Qt::gray);
	}
	else if (m_roomColorComboBox->currentText() == "DarkRed") {
		brush.setColor(Qt::darkRed);
		pen.setColor(Qt::darkRed);
	}
	else if (m_roomColorComboBox->currentText() == "DarkGreen") {
		brush.setColor(Qt::darkGreen);
		pen.setColor(Qt::darkGreen);
	}
	else if (m_roomColorComboBox->currentText() == "DarkBlue") {
		brush.setColor(Qt::darkBlue);
		pen.setColor(Qt::darkBlue);
	}
	else if (m_roomColorComboBox->currentText() == "DarkYellow") {
		brush.setColor(Qt::darkYellow);
		pen.setColor(Qt::darkYellow);
	}

	brush.setStyle(Qt::SolidPattern);	
	m_mainGraphicsScene->addPolygon(m_poly, pen, brush);
	
	m_poly.clear();

}


void FloorPlanSegmentationGui::onRemoveLastSelectedPoint()
{
	std::cout << "Calling remove last item function \n";
	if (m_selectComponentWidget->currentText() != "HeightMap") {
		if (m_elipseItemsVec.size() > 0) {
			m_poly.pop_back();
			m_mainGraphicsScene->removeItem(m_elipseItemsVec[m_elipseItemsVec.size() - 1]);
			m_elipseItemsVec.pop_back();
			m_mainGraphicsScene->update();
		}
	}
	else {
	//	m_mainGraphicsScene->removeItem(m_graphicsRectItem);
		QGraphicsRectItem* graphicsRectItem = new QGraphicsRectItem;
		QGraphicsEllipseItem* elipseItemForAxisPoint = new QGraphicsEllipseItem;
		


		QRectF tmpRect;
		
		QPoint selectedPoint = m_mainGraphicsScene->transferPickedPointsPosition();



		std::string imageSizeFileName = "imageSize.txt";
		std::ofstream fileSize;
		fileSize.open(imageSizeFileName,std::ios::app);
		fileSize << m_image.width() << " " << m_image.height() << " " << selectedPoint.x() << " " << selectedPoint.y() << "\n";
		fileSize.close();



		tmpRect.setX(selectedPoint.x());
		tmpRect.setY (selectedPoint.y());
		tmpRect.setWidth(20);
		tmpRect.setHeight(10);
	////	m_graphicsRectItem->setRect(tmpRect);
		
		elipseItemForAxisPoint->setRect(selectedPoint.x(), selectedPoint.y(), 20, 20);
		//elipseItemForAxisPoint->set
		graphicsRectItem->setRect(tmpRect);
	


		QBrush brush;
		brush.setColor(Qt::blue);
		brush.setStyle(Qt::SolidPattern);

		QPen pen(Qt::blue);
		pen.setBrush(brush);
		pen.setWidth(5);
		//QLineF tmpLine;
		////m_graphicsRectItem->setPen(pen);
		graphicsRectItem->setPen(pen);
		elipseItemForAxisPoint->setPen(pen);
		elipseItemForAxisPoint->setBrush(brush);


		///m_mainGraphicsScene->addItem(m_graphicsRectItem);
		m_mainGraphicsScene->addItem(elipseItemForAxisPoint);
		m_mainGraphicsScene->update();
	}

	

}


void FloorPlanSegmentationGui::dbmShowingSlot()
{

	if (!m_isDbmBoxChecked) {
		m_DbmShowingButton->setChecked(true);
		m_SinrShowingButton->setChecked(false);
		m_isDbmBoxChecked = true;
		this->update();
		
	}
	else {
		m_isDbmBoxChecked = false;
		m_DbmShowingButton->setChecked(false);
	
		this->update();
	}
	/*if (m_DbmShowingButton->isChecked()) {
		m_DbmShowingButton->setChecked(false);
	}
	else {
		m_DbmShowingButton->setChecked(true);
	}
	m_SinrShowingButton->setChecked(false);*/
}


void FloorPlanSegmentationGui::sinrShowingSlot()
{

	if (!m_isSinrBoxChecked) {
		m_SinrShowingButton->setChecked(true);
		m_DbmShowingButton->setChecked(false);
		m_isSinrBoxChecked = true;
		this->update();
	}
	else {
		m_isSinrBoxChecked = false;
		m_SinrShowingButton->setChecked(false);
		this->update();
	}

	/*m_DbmShowingButton->setChecked(false);
	
	if (m_SinrShowingButton->isChecked()) {
		m_SinrShowingButton->setChecked(false);
	} else {
		m_SinrShowingButton->setChecked(true);
	}*/
//	m_SinrShowingButton->setChecked(true);
}




void FloorPlanSegmentationGui::removingSelectedPointsSlot()
{
	;
}



FloorPlanSegmentationGui::~FloorPlanSegmentationGui()
{

}
