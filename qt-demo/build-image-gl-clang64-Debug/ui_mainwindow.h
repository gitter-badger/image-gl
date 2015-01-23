/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_3;
    QTabWidget *tabWidget;
    QWidget *tab;
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QLabel *labelImageSelected;
    QLabel *labelDimension;
    QComboBox *comboBoxDimension;
    QLabel *labelOutputFormat;
    QComboBox *comboBoxFormat;
    QLabel *labelImageLoaded;
    QPushButton *pushButtonSelectImage;
    QPushButton *pushButtonLoadImage;
    QLabel *labelLoadedImage;
    QLabel *labelSelectedImage;
    QPushButton *pushButtonSplit;
    QPushButton *pushButtonDisplay;
    QPushButton *pushButtonOsteotomy;
    QPushButton *pushButton;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_4;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_2;
    QLabel *labelAP;
    QPushButton *pushButtonDemo2AP;
    QLabel *labelLAT;
    QPushButton *pushButtonDemo2LAT;
    QPushButton *pushButtonDemo2Run;
    QSpacerItem *verticalSpacer;
    QTextEdit *textEditLog;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(673, 531);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout_3 = new QVBoxLayout(centralWidget);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        verticalLayout = new QVBoxLayout(tab);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        labelImageSelected = new QLabel(tab);
        labelImageSelected->setObjectName(QStringLiteral("labelImageSelected"));

        gridLayout->addWidget(labelImageSelected, 0, 0, 1, 1);

        labelDimension = new QLabel(tab);
        labelDimension->setObjectName(QStringLiteral("labelDimension"));

        gridLayout->addWidget(labelDimension, 3, 0, 1, 1);

        comboBoxDimension = new QComboBox(tab);
        comboBoxDimension->setObjectName(QStringLiteral("comboBoxDimension"));

        gridLayout->addWidget(comboBoxDimension, 3, 1, 1, 1);

        labelOutputFormat = new QLabel(tab);
        labelOutputFormat->setObjectName(QStringLiteral("labelOutputFormat"));
        labelOutputFormat->setEnabled(false);

        gridLayout->addWidget(labelOutputFormat, 4, 0, 1, 1);

        comboBoxFormat = new QComboBox(tab);
        comboBoxFormat->setObjectName(QStringLiteral("comboBoxFormat"));
        comboBoxFormat->setEnabled(false);

        gridLayout->addWidget(comboBoxFormat, 4, 1, 1, 1);

        labelImageLoaded = new QLabel(tab);
        labelImageLoaded->setObjectName(QStringLiteral("labelImageLoaded"));

        gridLayout->addWidget(labelImageLoaded, 1, 0, 1, 1);

        pushButtonSelectImage = new QPushButton(tab);
        pushButtonSelectImage->setObjectName(QStringLiteral("pushButtonSelectImage"));

        gridLayout->addWidget(pushButtonSelectImage, 0, 3, 1, 1);

        pushButtonLoadImage = new QPushButton(tab);
        pushButtonLoadImage->setObjectName(QStringLiteral("pushButtonLoadImage"));

        gridLayout->addWidget(pushButtonLoadImage, 1, 3, 1, 1);

        labelLoadedImage = new QLabel(tab);
        labelLoadedImage->setObjectName(QStringLiteral("labelLoadedImage"));

        gridLayout->addWidget(labelLoadedImage, 1, 1, 1, 2);

        labelSelectedImage = new QLabel(tab);
        labelSelectedImage->setObjectName(QStringLiteral("labelSelectedImage"));

        gridLayout->addWidget(labelSelectedImage, 0, 1, 1, 2);

        pushButtonSplit = new QPushButton(tab);
        pushButtonSplit->setObjectName(QStringLiteral("pushButtonSplit"));
        pushButtonSplit->setEnabled(true);

        gridLayout->addWidget(pushButtonSplit, 3, 3, 1, 1);


        verticalLayout->addLayout(gridLayout);

        pushButtonDisplay = new QPushButton(tab);
        pushButtonDisplay->setObjectName(QStringLiteral("pushButtonDisplay"));

        verticalLayout->addWidget(pushButtonDisplay);

        pushButtonOsteotomy = new QPushButton(tab);
        pushButtonOsteotomy->setObjectName(QStringLiteral("pushButtonOsteotomy"));
        pushButtonOsteotomy->setEnabled(true);

        verticalLayout->addWidget(pushButtonOsteotomy);

        pushButton = new QPushButton(tab);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        verticalLayout->addWidget(pushButton);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        verticalLayout_4 = new QVBoxLayout(tab_2);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        groupBox = new QGroupBox(tab_2);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        gridLayout_2 = new QGridLayout(groupBox);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        labelAP = new QLabel(groupBox);
        labelAP->setObjectName(QStringLiteral("labelAP"));

        gridLayout_2->addWidget(labelAP, 0, 0, 1, 1);

        pushButtonDemo2AP = new QPushButton(groupBox);
        pushButtonDemo2AP->setObjectName(QStringLiteral("pushButtonDemo2AP"));

        gridLayout_2->addWidget(pushButtonDemo2AP, 0, 1, 1, 1);

        labelLAT = new QLabel(groupBox);
        labelLAT->setObjectName(QStringLiteral("labelLAT"));

        gridLayout_2->addWidget(labelLAT, 1, 0, 1, 1);

        pushButtonDemo2LAT = new QPushButton(groupBox);
        pushButtonDemo2LAT->setObjectName(QStringLiteral("pushButtonDemo2LAT"));

        gridLayout_2->addWidget(pushButtonDemo2LAT, 1, 1, 1, 1);


        verticalLayout_4->addWidget(groupBox);

        pushButtonDemo2Run = new QPushButton(tab_2);
        pushButtonDemo2Run->setObjectName(QStringLiteral("pushButtonDemo2Run"));

        verticalLayout_4->addWidget(pushButtonDemo2Run);

        verticalSpacer = new QSpacerItem(20, 25, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer);

        tabWidget->addTab(tab_2, QString());

        verticalLayout_3->addWidget(tabWidget);

        textEditLog = new QTextEdit(centralWidget);
        textEditLog->setObjectName(QStringLiteral("textEditLog"));

        verticalLayout_3->addWidget(textEditLog);

        MainWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);
        comboBoxDimension->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        labelImageSelected->setText(QApplication::translate("MainWindow", "Image Selected", 0));
        labelDimension->setText(QApplication::translate("MainWindow", "Dimension of Tile", 0));
        comboBoxDimension->clear();
        comboBoxDimension->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "256", 0)
         << QApplication::translate("MainWindow", "512", 0)
         << QApplication::translate("MainWindow", "1024", 0)
         << QApplication::translate("MainWindow", "2048", 0)
         << QApplication::translate("MainWindow", "4096", 0)
         << QApplication::translate("MainWindow", "8192", 0)
        );
        labelOutputFormat->setText(QApplication::translate("MainWindow", "Output File Format", 0));
        comboBoxFormat->clear();
        comboBoxFormat->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "png", 0)
         << QApplication::translate("MainWindow", "jpg", 0)
        );
        labelImageLoaded->setText(QApplication::translate("MainWindow", "Image Loaded", 0));
        pushButtonSelectImage->setText(QApplication::translate("MainWindow", "Select Image...", 0));
        pushButtonLoadImage->setText(QApplication::translate("MainWindow", "Load Image", 0));
        labelLoadedImage->setText(QApplication::translate("MainWindow", "_", 0));
        labelSelectedImage->setText(QApplication::translate("MainWindow", "_", 0));
        pushButtonSplit->setText(QApplication::translate("MainWindow", "Create Tiles", 0));
        pushButtonDisplay->setText(QApplication::translate("MainWindow", "Run Demo 1", 0));
        pushButtonOsteotomy->setText(QApplication::translate("MainWindow", "Run Osteotomy Simulation", 0));
        pushButton->setText(QApplication::translate("MainWindow", " Triangle Test", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "Tile Creator", 0));
        groupBox->setTitle(QApplication::translate("MainWindow", "Image Selection", 0));
        labelAP->setText(QApplication::translate("MainWindow", "AP", 0));
        pushButtonDemo2AP->setText(QApplication::translate("MainWindow", "AP select", 0));
        labelLAT->setText(QApplication::translate("MainWindow", "LAT", 0));
        pushButtonDemo2LAT->setText(QApplication::translate("MainWindow", "Lat Select", 0));
        pushButtonDemo2Run->setText(QApplication::translate("MainWindow", "Run demo 2", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MainWindow", "AP/Lat Grid", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
