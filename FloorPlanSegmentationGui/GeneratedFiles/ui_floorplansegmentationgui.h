/********************************************************************************
** Form generated from reading UI file 'floorplansegmentationgui.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FLOORPLANSEGMENTATIONGUI_H
#define UI_FLOORPLANSEGMENTATIONGUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FloorPlanSegmentationGuiClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *FloorPlanSegmentationGuiClass)
    {
        if (FloorPlanSegmentationGuiClass->objectName().isEmpty())
            FloorPlanSegmentationGuiClass->setObjectName(QStringLiteral("FloorPlanSegmentationGuiClass"));
        FloorPlanSegmentationGuiClass->resize(600, 400);
        menuBar = new QMenuBar(FloorPlanSegmentationGuiClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        FloorPlanSegmentationGuiClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(FloorPlanSegmentationGuiClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        FloorPlanSegmentationGuiClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(FloorPlanSegmentationGuiClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        FloorPlanSegmentationGuiClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(FloorPlanSegmentationGuiClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        FloorPlanSegmentationGuiClass->setStatusBar(statusBar);

        retranslateUi(FloorPlanSegmentationGuiClass);

        QMetaObject::connectSlotsByName(FloorPlanSegmentationGuiClass);
    } // setupUi

    void retranslateUi(QMainWindow *FloorPlanSegmentationGuiClass)
    {
        FloorPlanSegmentationGuiClass->setWindowTitle(QApplication::translate("FloorPlanSegmentationGuiClass", "FloorPlanSegmentationGui", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class FloorPlanSegmentationGuiClass: public Ui_FloorPlanSegmentationGuiClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FLOORPLANSEGMENTATIONGUI_H
