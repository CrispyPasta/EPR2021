/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
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
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *label_3;
    QComboBox *functionPicker;
    QLabel *label;
    QTextEdit *rpmTextEdit;
    QLabel *label_2;
    QTextEdit *durationTextEdit;
    QPushButton *startButton;
    QPushButton *stopButton;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *verticalLayout_2;
    QTextEdit *outputTextEdit;
    QLabel *rpmLabel;
    QLabel *imageLabel;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1061, 623);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayoutWidget = new QWidget(centralWidget);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(10, 10, 251, 591));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        label_3 = new QLabel(verticalLayoutWidget);
        label_3->setObjectName(QStringLiteral("label_3"));

        verticalLayout->addWidget(label_3);

        functionPicker = new QComboBox(verticalLayoutWidget);
        functionPicker->setObjectName(QStringLiteral("functionPicker"));

        verticalLayout->addWidget(functionPicker);

        label = new QLabel(verticalLayoutWidget);
        label->setObjectName(QStringLiteral("label"));

        verticalLayout->addWidget(label);

        rpmTextEdit = new QTextEdit(verticalLayoutWidget);
        rpmTextEdit->setObjectName(QStringLiteral("rpmTextEdit"));

        verticalLayout->addWidget(rpmTextEdit);

        label_2 = new QLabel(verticalLayoutWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        verticalLayout->addWidget(label_2);

        durationTextEdit = new QTextEdit(verticalLayoutWidget);
        durationTextEdit->setObjectName(QStringLiteral("durationTextEdit"));

        verticalLayout->addWidget(durationTextEdit);

        startButton = new QPushButton(verticalLayoutWidget);
        startButton->setObjectName(QStringLiteral("startButton"));
        startButton->setEnabled(true);

        verticalLayout->addWidget(startButton);

        stopButton = new QPushButton(verticalLayoutWidget);
        stopButton->setObjectName(QStringLiteral("stopButton"));

        verticalLayout->addWidget(stopButton);

        verticalLayoutWidget_2 = new QWidget(centralWidget);
        verticalLayoutWidget_2->setObjectName(QStringLiteral("verticalLayoutWidget_2"));
        verticalLayoutWidget_2->setGeometry(QRect(790, 10, 261, 591));
        verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget_2);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        outputTextEdit = new QTextEdit(verticalLayoutWidget_2);
        outputTextEdit->setObjectName(QStringLiteral("outputTextEdit"));

        verticalLayout_2->addWidget(outputTextEdit);

        rpmLabel = new QLabel(verticalLayoutWidget_2);
        rpmLabel->setObjectName(QStringLiteral("rpmLabel"));

        verticalLayout_2->addWidget(rpmLabel);

        imageLabel = new QLabel(centralWidget);
        imageLabel->setObjectName(QStringLiteral("imageLabel"));
        imageLabel->setGeometry(QRect(270, 10, 511, 511));
        MainWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        label_3->setText(QApplication::translate("MainWindow", "Image Processing Function:", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "RPM List:", Q_NULLPTR));
        label_2->setText(QApplication::translate("MainWindow", "Duration List:", Q_NULLPTR));
        startButton->setText(QApplication::translate("MainWindow", "Start", Q_NULLPTR));
        stopButton->setText(QApplication::translate("MainWindow", "Stop", Q_NULLPTR));
        rpmLabel->setText(QApplication::translate("MainWindow", "Current RPM: 0", Q_NULLPTR));
        imageLabel->setText(QApplication::translate("MainWindow", "TextLabel", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
