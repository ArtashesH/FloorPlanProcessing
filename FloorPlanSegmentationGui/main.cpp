#include "floorplansegmentationgui.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{

	/*cv::Mat initImg = cv::imread("C:/Users/Julia/Documents/Visual Studio 2015/Projects/FloorPlanSegmentationGui/x64/Release/testfileRes.png");
	for (int i = 0; i < initImg.rows; ++i) {
		for (int j = 0; j < initImg.cols; ++j) {
			if (!(initImg.at<cv::Vec3b>(i, j)[2] == 0 && initImg.at<cv::Vec3b>(i, j)[0] == 100   &&  initImg.at<cv::Vec3b>(i, j)[1] ==  0)) {
				cv::Vec3b tmpVal = initImg.at<cv::Vec3b>(i, j);
				
				initImg.at<cv::Vec3b>(i, j)[1] = 100;
			}
		}
	}
	cv::imshow("InitImg", initImg);
	cv::waitKey(0);
	return 0;*/
	/*std::string widthVal = "1024";
	std::string heightVal = "724";
	int retCode = system("HeightMapGeneration.exe ");
	return 0;*/
	QApplication a(argc, argv);
	Py_Initialize();
	FloorPlanSegmentationGui w;
	w.show();
	return a.exec();
}















//#include <Python.h>
//#include <string>
//#include <iostream>
//#include <opencv2/core/core.hpp>
//#include <opencv2/imgcodecs/imgcodecs.hpp>
//
//#if PY_MAJOR_VERSION >= 3
//#define IS_PY3K
//#endif
//
//
//const wchar_t *GetWC(const char *c)
//{
//	const size_t cSize = strlen(c) + 1;
//	wchar_t* wc = new wchar_t[cSize];
//	mbstowcs(wc, c, cSize);
//
//	return wc;
//}
//
//
//void callPythonFunction(/*const std::vector<int>& imageSizes*/)
//{
//	
//	PyObject *pName, *pModule, *pFunc;
//	PyObject *pArgs, *pValue;
//	int i;
//	pArgs = NULL;
//
//	/*if (argc < 3) {
//	fprintf(stderr, "Usage: call pythonfile funcname [args]\n");
//	return 1;
//	}*/
//
//	Py_Initialize();
//
//	pName = PyUnicode_FromString("MainRf_rayTracingCurrentState");
//
//	/* Error checking of pName left out */
//
//	pModule = PyImport_Import(pName);
//	Py_DECREF(pName);
//
//	if (pModule != NULL) {
//		pFunc = PyObject_GetAttrString(pModule, "finalFunctionHeightMap");
//		/* pFunc is a new reference */
//
//		if (pFunc && PyCallable_Check(pFunc)) {
//			//pArgs = PyTuple_New(2);
//			/*for (i = 0; i < imageSizes.size(); ++i) {
//				//	pValue = PyInt_FromLong(atoi(argv[i + 3]));
//				
//				pValue = PyLong_FromLong(imageSizes[i]);
//
//				if (!pValue) {
//					Py_DECREF(pArgs);
//					Py_DECREF(pModule);
//					fprintf(stderr, "Cannot convert argument\n");
//					return ;
//				}
//			
//				PyTuple_SetItem(pArgs, i, pValue);
//			}*/
//			std::cout << "Call something \n";
//			pValue = PyObject_CallObject(pFunc,pArgs);
//			Py_DECREF(pArgs);
//			if (pValue != NULL) {
//
//				printf("Result of call: %ld\n", PyLong_AsLong(pValue));
//				Py_DECREF(pValue);
//			}
//			else {
//				Py_DECREF(pFunc);
//				Py_DECREF(pModule);
//				PyErr_Print();
//				fprintf(stderr, "Call failed\n");
//				return ;
//			}
//		}
//		else {
//			if (PyErr_Occurred())
//				PyErr_Print();
//			//fprintf(stderr, "Cannot find function \"%s\"\n", argv[2]);
//		}
//		Py_XDECREF(pFunc);
//		Py_DECREF(pModule);
//	}
//	else {
//		PyErr_Print();
//		//fprintf(stderr, "Failed to load \"%s\"\n", argv[1]);
//		return ;
//	}
//	Py_Finalize();
//	return ;
//
//}
//////
//////
//////
//int
//main(int argc, char *argv[])
//{
//	callPythonFunction();
//	return 0;
//	/*if (argc != 3) {
//		std::cout << "Input arguments are not correct \n";
//		return 0;
//	}
//	std::vector<int> imageSizesVec;
//	imageSizesVec.push_back( std::atoi(argv[1]));
//	imageSizesVec.push_back(std::atoi(argv[2]));
//	callPythonFunction(imageSizesVec);
//	return 0;*/
//	PyObject *pName, *pModule, *pFunc;
//	PyObject *pArgs, *pValue;
//	int i;
//	
//
//	/*if (argc < 3) {
//		fprintf(stderr, "Usage: call pythonfile funcname [args]\n");
//		return 1;
//	}*/
//
//	Py_Initialize();
//	
//	pName = PyUnicode_FromString( "MainRf_rayTracingCurrentState");
//	
//	/* Error checking of pName left out */
//
//	pModule = PyImport_Import(pName);
//	Py_DECREF(pName);
//
//	if (pModule != NULL) {
//		pFunc = PyObject_GetAttrString(pModule, "finalFunctionHeightMap");
//		/* pFunc is a new reference */
//
//		if (pFunc && PyCallable_Check(pFunc)) {
//			pArgs = PyTuple_New(argc - 1);
//			for (i = 0; i < argc - 1; ++i) {
//			//	pValue = PyInt_FromLong(atoi(argv[i + 3]));
//				pValue =   PyLong_FromLong(atol(argv[i + 1]));
//				
//				if (!pValue) {
//					Py_DECREF(pArgs);
//					Py_DECREF(pModule);
//					fprintf(stderr, "Cannot convert argument\n");
//					return 1;
//				}
//				/* pValue reference stolen here: */
//				PyTuple_SetItem(pArgs, i, pValue);
//			}
//			pValue = PyObject_CallObject(pFunc, pArgs);
//			Py_DECREF(pArgs);
//			if (pValue != NULL) {
//				
//				printf("Result of call: %ld\n", PyLong_AsLong(pValue));
//				Py_DECREF(pValue);
//			}
//			else {
//				Py_DECREF(pFunc);
//				Py_DECREF(pModule);
//				PyErr_Print();
//				fprintf(stderr, "Call failed\n");
//				return 1;
//			}
//		}
//		else {
//			if (PyErr_Occurred())
//				PyErr_Print();
//			//fprintf(stderr, "Cannot find function \"%s\"\n", argv[2]);
//		}
//		Py_XDECREF(pFunc);
//		Py_DECREF(pModule);
//	}
//	else {
//		PyErr_Print();
//		//fprintf(stderr, "Failed to load \"%s\"\n", argv[1]);
//		return 1;
//	}
//	Py_Finalize();
//	return 0;
//}

//int main()
//{
//	cv::FileStorage fileStorage;
//	fileStorage.open("test.xml", cv::FileStorage::READ);
//	cv::FileNode n = fileStorage["vector0"];
//
//	cv::FileNodeIterator it = n.begin(), it_end = n.end(); // Go through the node
//	for (; it != it_end; ++it)
//		std::cout << "Value  " << (int)*it << std::endl;
//	return 0;
//
//}



//#include <opencv2/core/core.hpp>
//#include <opencv2/imgcodecs/imgcodecs.hpp>
//
//
//int main() {
//	std::vector<std::vector<cv::Point>> myvector;
//	for (int i = 0; i < 3; ++i) {
//		std::vector<cv::Point> tmpVec;
//		myvector.push_back(tmpVec);
//	}
//
//	cv::Point point11 = cv::Point(10, 20);
//	cv::Point point12 = cv::Point(10, 50);
//	myvector[0].push_back(point11);
//	myvector[0].push_back(point12);
//
//	cv::Point point21 = cv::Point(50, 50);
//	cv::Point point22 = cv::Point(100, 50);
//	myvector[1].push_back(point21);
//	myvector[1].push_back(point22);
//
//	cv::Point point31 = cv::Point(150, 150);
//	cv::Point point32 = cv::Point(200, 200);
//	myvector[2].push_back(point31);
//	myvector[2].push_back(point32);
//
//
//
//	std::string filename = "test.xml";
//	cv::FileStorage fs(filename, cv::FileStorage::WRITE);
//	fs << "data" << "{";
//	for (int i = 0; i < myvector.size(); ++i)
//	{
//		//Write each vector
//		std::string vectorName = "vector";// +std::to_string((long)i);
//		fs <<  vectorName<< "{";
//		//fs << pointName << "{";
//		for (int j = 0; j < myvector[i].size(); ++j)
//		{
//			std::string pointNameX = "x";// +std::to_string(long(j));
//			std::string pointNameY = "y";// +std::to_string(long(j));
//			//Write each point
//			fs << pointNameX << myvector[i][j].x << pointNameY << myvector[i][j].y ;
//			//fs << "}"; // close point
//		}
//		fs << "}"; // close vector
//	}
//	fs << "}"; // close data
//	fs.release();
//
//}