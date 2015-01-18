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
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
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
    QTextEdit *textEditLog;
    QPushButton *pushButtonDisplay;
    QPushButton *pushButton;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(432, 475);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        labelImageSelected = new QLabel(centralWidget);
        labelImageSelected->setObjectName(QStringLiteral("labelImageSelected"));

        gridLayout->addWidget(labelImageSelected, 0, 0, 1, 1);

        labelDimension = new QLabel(centralWidget);
        labelDimension->setObjectName(QStringLiteral("labelDimension"));

        gridLayout->addWidget(labelDimension, 3, 0, 1, 1);

        comboBoxDimension = new QComboBox(centralWidget);
        comboBoxDimension->setObjectName(QStringLiteral("comboBoxDimension"));

        gridLayout->addWidget(comboBoxDimension, 3, 1, 1, 1);

        labelOutputFormat = new QLabel(centralWidget);
        labelOutputFormat->setObjectName(QStringLiteral("labelOutputFormat"));
        labelOutputFormat->setEnabled(false);

        gridLayout->addWidget(labelOutputFormat, 4, 0, 1, 1);

        comboBoxFormat = new QComboBox(centralWidget);
        comboBoxFormat->setObjectName(QStringLiteral("comboBoxFormat"));
        comboBoxFormat->setEnabled(false);

        gridLayout->addWidget(comboBoxFormat, 4, 1, 1, 1);

        labelImageLoaded = new QLabel(centralWidget);
        labelImageLoaded->setObjectName(QStringLiteral("labelImageLoaded"));

        gridLayout->addWidget(labelImageLoaded, 1, 0, 1, 1);

        pushButtonSelectImage = new QPushButton(centralWidget);
        pushButtonSelectImage->setObjectName(QStringLiteral("pushButtonSelectImage"));

        gridLayout->addWidget(pushButtonSelectImage, 0, 3, 1, 1);

        pushButtonLoadImage = new QPushButton(centralWidget);
        pushButtonLoadImage->setObjectName(QStringLiteral("pushButtonLoadImage"));

        gridLayout->addWidget(pushButtonLoadImage, 1, 3, 1, 1);

        labelLoadedImage = new QLabel(centralWidget);
        labelLoadedImage->setObjectName(QStringLiteral("labelLoadedImage"));

        gridLayout->addWidget(labelLoadedImage, 1, 1, 1, 2);

        labelSelectedImage = new QLabel(centralWidget);
        labelSelectedImage->setObjectName(QStringLiteral("labelSelectedImage"));

        gridLayout->addWidget(labelSelectedImage, 0, 1, 1, 2);

        pushButtonSplit = new QPushButton(centralWidget);
        pushButtonSplit->setObjectName(QStringLiteral("pushButtonSplit"));
        pushButtonSplit->setEnabled(false);

        gridLayout->addWidget(pushButtonSplit, 3, 3, 1, 1);


        verticalLayout->addLayout(gridLayout);

        textEditLog = new QTextEdit(centralWidget);
        textEditLog->setObjectName(QStringLiteral("textEditLog"));

        verticalLayout->addWidget(textEditLog);

        pushButtonDisplay = new QPushButton(centralWidget);
        pushButtonDisplay->setObjectName(QStringLiteral("pushButtonDisplay"));

        verticalLayout->addWidget(pushButtonDisplay);

        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        verticalLayout->addWidget(pushButton);

        MainWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

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
         << QApplication::translate("MainWindow", "64", 0)
         << QApplication::translate("MainWindow", "128", 0)
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
        pushButtonSplit->setText(QApplication::translate("MainWindow", "Split", 0));
        pushButtonDisplay->setText(QApplication::translate("MainWindow", "Display", 0));
        pushButton->setText(QApplication::translate("MainWindow", "Test", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
