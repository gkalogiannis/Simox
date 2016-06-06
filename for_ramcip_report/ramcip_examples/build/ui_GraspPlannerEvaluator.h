/********************************************************************************
** Form generated from reading UI file 'GraspPlannerEvaluator.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GRASPPLANNEREVALUATOR_H
#define UI_GRASPPLANNEREVALUATOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GraspPlannerEvaluator
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QFrame *frameViewer;
    QGroupBox *groupBox_1;
    QPushButton *pushButtonReset;
    QPushButton *pushButtonPlan;
    QGroupBox *groupBox_2;
    QCheckBox *checkBoxFoceClosure;
    QSpinBox *spinBoxGraspNumber;
    QLabel *label_11;
    QLabel *label_12;
    QSpinBox *spinBoxTimeOut;
    QLabel *label_13;
    QDoubleSpinBox *doubleSpinBoxQuality;
    QPushButton *pushButtonSave;
    QPushButton *pushButtonClose;
    QPushButton *pushButtonOpen;
    QFrame *line;
    QGroupBox *groupBox;
    QLabel *label_4;
    QLabel *label_5;
    QPushButton *pushButtonNormalGrasp;
    QSpinBox *spinBoxGraspNum;
    QLabel *labelGraspNum;
    QFrame *line_2;
    QPushButton *pushButtonResetPose;
    QPushButton *pushButtonPerturbedGrasp;
    QLabel *label_14;
    QLabel *label_15;
    QDoubleSpinBox *doubleSpinBoxPertDistanceX;
    QDoubleSpinBox *doubleSpinBoxPertDistanceY;
    QLabel *label_16;
    QDoubleSpinBox *doubleSpinBoxPertDistanceZ;
    QLabel *label_17;
    QDoubleSpinBox *doubleSpinBoxPertAngleX;
    QDoubleSpinBox *doubleSpinBoxPertAngleY;
    QDoubleSpinBox *doubleSpinBoxPertAngleZ;
    QLabel *label_18;
    QLabel *label_19;
    QLabel *label_20;
    QLabel *label_21;
    QFrame *line_5;
    QLabel *label_22;
    QLabel *label_23;
    QDoubleSpinBox *doubleSpinBoxStep;
    QFrame *line_6;
    QGroupBox *groupBox_4;
    QLabel *labelInfo;
    QFrame *line_4;
    QFrame *line_3;
    QDoubleSpinBox *doubleSpinBoxVDelay;
    QGroupBox *groupBox_3;
    QCheckBox *checkBoxHighlight;
    QCheckBox *checkBoxCones;
    QCheckBox *checkBoxGrasps;
    QCheckBox *checkBoxColModel;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *GraspPlannerEvaluator)
    {
        if (GraspPlannerEvaluator->objectName().isEmpty())
            GraspPlannerEvaluator->setObjectName(QString::fromUtf8("GraspPlannerEvaluator"));
        GraspPlannerEvaluator->resize(1209, 882);
        centralwidget = new QWidget(GraspPlannerEvaluator);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        frameViewer = new QFrame(centralwidget);
        frameViewer->setObjectName(QString::fromUtf8("frameViewer"));
        frameViewer->setAutoFillBackground(false);
        frameViewer->setFrameShape(QFrame::StyledPanel);
        frameViewer->setFrameShadow(QFrame::Raised);

        gridLayout->addWidget(frameViewer, 0, 0, 1, 1);

        groupBox_1 = new QGroupBox(centralwidget);
        groupBox_1->setObjectName(QString::fromUtf8("groupBox_1"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox_1->sizePolicy().hasHeightForWidth());
        groupBox_1->setSizePolicy(sizePolicy);
        groupBox_1->setMaximumSize(QSize(250, 16777215));
        pushButtonReset = new QPushButton(groupBox_1);
        pushButtonReset->setObjectName(QString::fromUtf8("pushButtonReset"));
        pushButtonReset->setGeometry(QRect(50, 0, 171, 28));
        pushButtonPlan = new QPushButton(groupBox_1);
        pushButtonPlan->setObjectName(QString::fromUtf8("pushButtonPlan"));
        pushButtonPlan->setGeometry(QRect(60, 183, 141, 31));
        pushButtonPlan->setAutoDefault(false);
        pushButtonPlan->setDefault(true);
        groupBox_2 = new QGroupBox(groupBox_1);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(40, 27, 181, 151));
        checkBoxFoceClosure = new QCheckBox(groupBox_2);
        checkBoxFoceClosure->setObjectName(QString::fromUtf8("checkBoxFoceClosure"));
        checkBoxFoceClosure->setGeometry(QRect(23, 135, 171, 17));
        spinBoxGraspNumber = new QSpinBox(groupBox_2);
        spinBoxGraspNumber->setObjectName(QString::fromUtf8("spinBoxGraspNumber"));
        spinBoxGraspNumber->setGeometry(QRect(30, 37, 111, 22));
        spinBoxGraspNumber->setMinimum(1);
        spinBoxGraspNumber->setMaximum(10000000);
        label_11 = new QLabel(groupBox_2);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(10, 20, 151, 16));
        label_12 = new QLabel(groupBox_2);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(0, 58, 211, 16));
        spinBoxTimeOut = new QSpinBox(groupBox_2);
        spinBoxTimeOut->setObjectName(QString::fromUtf8("spinBoxTimeOut"));
        spinBoxTimeOut->setGeometry(QRect(30, 76, 111, 22));
        spinBoxTimeOut->setMinimum(0);
        spinBoxTimeOut->setMaximum(10000000);
        spinBoxTimeOut->setValue(30);
        label_13 = new QLabel(groupBox_2);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(10, 97, 171, 16));
        doubleSpinBoxQuality = new QDoubleSpinBox(groupBox_2);
        doubleSpinBoxQuality->setObjectName(QString::fromUtf8("doubleSpinBoxQuality"));
        doubleSpinBoxQuality->setGeometry(QRect(30, 115, 111, 22));
        doubleSpinBoxQuality->setDecimals(3);
        doubleSpinBoxQuality->setMaximum(1);
        doubleSpinBoxQuality->setSingleStep(0.001);
        pushButtonSave = new QPushButton(groupBox_1);
        pushButtonSave->setObjectName(QString::fromUtf8("pushButtonSave"));
        pushButtonSave->setGeometry(QRect(60, 241, 141, 31));
        pushButtonClose = new QPushButton(groupBox_1);
        pushButtonClose->setObjectName(QString::fromUtf8("pushButtonClose"));
        pushButtonClose->setGeometry(QRect(60, 212, 71, 31));
        pushButtonOpen = new QPushButton(groupBox_1);
        pushButtonOpen->setObjectName(QString::fromUtf8("pushButtonOpen"));
        pushButtonOpen->setGeometry(QRect(130, 212, 71, 31));
        line = new QFrame(groupBox_1);
        line->setObjectName(QString::fromUtf8("line"));
        line->setGeometry(QRect(7, 265, 241, 20));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        groupBox = new QGroupBox(groupBox_1);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(9, 273, 241, 361));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(136, 40, 21, 17));
        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(155, 62, 51, 20));
        pushButtonNormalGrasp = new QPushButton(groupBox);
        pushButtonNormalGrasp->setObjectName(QString::fromUtf8("pushButtonNormalGrasp"));
        pushButtonNormalGrasp->setGeometry(QRect(-1, 29, 121, 41));
        pushButtonNormalGrasp->setLayoutDirection(Qt::LeftToRight);
        pushButtonNormalGrasp->setDefault(false);
        pushButtonNormalGrasp->setFlat(false);
        spinBoxGraspNum = new QSpinBox(groupBox);
        spinBoxGraspNum->setObjectName(QString::fromUtf8("spinBoxGraspNum"));
        spinBoxGraspNum->setGeometry(QRect(154, 34, 60, 31));
        labelGraspNum = new QLabel(groupBox);
        labelGraspNum->setObjectName(QString::fromUtf8("labelGraspNum"));
        labelGraspNum->setGeometry(QRect(204, 57, 41, 31));
        line_2 = new QFrame(groupBox);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setGeometry(QRect(49, 85, 141, 20));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);
        pushButtonResetPose = new QPushButton(groupBox);
        pushButtonResetPose->setObjectName(QString::fromUtf8("pushButtonResetPose"));
        pushButtonResetPose->setGeometry(QRect(130, 299, 121, 31));
        pushButtonPerturbedGrasp = new QPushButton(groupBox);
        pushButtonPerturbedGrasp->setObjectName(QString::fromUtf8("pushButtonPerturbedGrasp"));
        pushButtonPerturbedGrasp->setGeometry(QRect(0, 300, 121, 31));
        pushButtonPerturbedGrasp->setCursor(QCursor(Qt::ArrowCursor));
        pushButtonPerturbedGrasp->setDefault(true);
        label_14 = new QLabel(groupBox);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(2, 95, 101, 16));
        label_15 = new QLabel(groupBox);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setGeometry(QRect(76, 125, 51, 16));
        doubleSpinBoxPertDistanceX = new QDoubleSpinBox(groupBox);
        doubleSpinBoxPertDistanceX->setObjectName(QString::fromUtf8("doubleSpinBoxPertDistanceX"));
        doubleSpinBoxPertDistanceX->setGeometry(QRect(6, 120, 69, 27));
        doubleSpinBoxPertDistanceX->setMaximum(500);
        doubleSpinBoxPertDistanceY = new QDoubleSpinBox(groupBox);
        doubleSpinBoxPertDistanceY->setObjectName(QString::fromUtf8("doubleSpinBoxPertDistanceY"));
        doubleSpinBoxPertDistanceY->setGeometry(QRect(6, 146, 69, 27));
        doubleSpinBoxPertDistanceY->setMaximum(500);
        label_16 = new QLabel(groupBox);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setGeometry(QRect(76, 150, 51, 16));
        doubleSpinBoxPertDistanceZ = new QDoubleSpinBox(groupBox);
        doubleSpinBoxPertDistanceZ->setObjectName(QString::fromUtf8("doubleSpinBoxPertDistanceZ"));
        doubleSpinBoxPertDistanceZ->setGeometry(QRect(6, 170, 69, 27));
        doubleSpinBoxPertDistanceZ->setMaximum(500);
        label_17 = new QLabel(groupBox);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setGeometry(QRect(76, 172, 51, 16));
        doubleSpinBoxPertAngleX = new QDoubleSpinBox(groupBox);
        doubleSpinBoxPertAngleX->setObjectName(QString::fromUtf8("doubleSpinBoxPertAngleX"));
        doubleSpinBoxPertAngleX->setGeometry(QRect(125, 120, 69, 27));
        doubleSpinBoxPertAngleY = new QDoubleSpinBox(groupBox);
        doubleSpinBoxPertAngleY->setObjectName(QString::fromUtf8("doubleSpinBoxPertAngleY"));
        doubleSpinBoxPertAngleY->setGeometry(QRect(125, 146, 69, 27));
        doubleSpinBoxPertAngleZ = new QDoubleSpinBox(groupBox);
        doubleSpinBoxPertAngleZ->setObjectName(QString::fromUtf8("doubleSpinBoxPertAngleZ"));
        doubleSpinBoxPertAngleZ->setGeometry(QRect(125, 172, 69, 27));
        label_18 = new QLabel(groupBox);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setGeometry(QRect(195, 173, 51, 16));
        label_19 = new QLabel(groupBox);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        label_19->setGeometry(QRect(195, 151, 51, 16));
        label_20 = new QLabel(groupBox);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        label_20->setGeometry(QRect(126, 95, 101, 16));
        label_21 = new QLabel(groupBox);
        label_21->setObjectName(QString::fromUtf8("label_21"));
        label_21->setGeometry(QRect(195, 126, 51, 16));
        line_5 = new QFrame(groupBox);
        line_5->setObjectName(QString::fromUtf8("line_5"));
        line_5->setGeometry(QRect(50, 274, 141, 30));
        line_5->setFrameShape(QFrame::HLine);
        line_5->setFrameShadow(QFrame::Sunken);
        label_22 = new QLabel(groupBox);
        label_22->setObjectName(QString::fromUtf8("label_22"));
        label_22->setGeometry(QRect(10, 210, 51, 16));
        label_23 = new QLabel(groupBox);
        label_23->setObjectName(QString::fromUtf8("label_23"));
        label_23->setGeometry(QRect(10, 238, 101, 16));
        doubleSpinBoxStep = new QDoubleSpinBox(groupBox);
        doubleSpinBoxStep->setObjectName(QString::fromUtf8("doubleSpinBoxStep"));
        doubleSpinBoxStep->setGeometry(QRect(110, 206, 111, 27));
        line_6 = new QFrame(groupBox);
        line_6->setObjectName(QString::fromUtf8("line_6"));
        line_6->setGeometry(QRect(0, 331, 241, 20));
        line_6->setFrameShape(QFrame::HLine);
        line_6->setFrameShadow(QFrame::Sunken);
        groupBox_4 = new QGroupBox(groupBox_1);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setGeometry(QRect(9, 730, 241, 81));
        labelInfo = new QLabel(groupBox_4);
        labelInfo->setObjectName(QString::fromUtf8("labelInfo"));
        labelInfo->setGeometry(QRect(0, 20, 221, 61));
        QFont font;
        font.setBold(false);
        font.setWeight(50);
        labelInfo->setFont(font);
        labelInfo->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        line_4 = new QFrame(groupBox_1);
        line_4->setObjectName(QString::fromUtf8("line_4"));
        line_4->setGeometry(QRect(10, 720, 241, 20));
        line_4->setFrameShape(QFrame::HLine);
        line_4->setFrameShadow(QFrame::Sunken);
        line_3 = new QFrame(groupBox_1);
        line_3->setObjectName(QString::fromUtf8("line_3"));
        line_3->setGeometry(QRect(60, 467, 141, 20));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);
        doubleSpinBoxVDelay = new QDoubleSpinBox(groupBox_1);
        doubleSpinBoxVDelay->setObjectName(QString::fromUtf8("doubleSpinBoxVDelay"));
        doubleSpinBoxVDelay->setGeometry(QRect(119, 505, 111, 27));
        doubleSpinBoxVDelay->setDecimals(0);
        doubleSpinBoxVDelay->setMaximum(1000);
        groupBox_3 = new QGroupBox(groupBox_1);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(10, 619, 241, 111));
        checkBoxHighlight = new QCheckBox(groupBox_3);
        checkBoxHighlight->setObjectName(QString::fromUtf8("checkBoxHighlight"));
        checkBoxHighlight->setGeometry(QRect(10, 86, 141, 17));
        checkBoxCones = new QCheckBox(groupBox_3);
        checkBoxCones->setObjectName(QString::fromUtf8("checkBoxCones"));
        checkBoxCones->setGeometry(QRect(10, 46, 141, 17));
        checkBoxGrasps = new QCheckBox(groupBox_3);
        checkBoxGrasps->setObjectName(QString::fromUtf8("checkBoxGrasps"));
        checkBoxGrasps->setGeometry(QRect(10, 66, 141, 17));
        checkBoxColModel = new QCheckBox(groupBox_3);
        checkBoxColModel->setObjectName(QString::fromUtf8("checkBoxColModel"));
        checkBoxColModel->setGeometry(QRect(10, 26, 141, 17));

        gridLayout->addWidget(groupBox_1, 0, 1, 1, 1);

        GraspPlannerEvaluator->setCentralWidget(centralwidget);
        menubar = new QMenuBar(GraspPlannerEvaluator);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1209, 25));
        GraspPlannerEvaluator->setMenuBar(menubar);
        statusbar = new QStatusBar(GraspPlannerEvaluator);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        GraspPlannerEvaluator->setStatusBar(statusbar);

        retranslateUi(GraspPlannerEvaluator);

        QMetaObject::connectSlotsByName(GraspPlannerEvaluator);
    } // setupUi

    void retranslateUi(QMainWindow *GraspPlannerEvaluator)
    {
        GraspPlannerEvaluator->setWindowTitle(QApplication::translate("GraspPlannerEvaluator", "GraspStudio - GraspPlanner", 0, QApplication::UnicodeUTF8));
        groupBox_1->setTitle(QString());
        pushButtonReset->setText(QApplication::translate("GraspPlannerEvaluator", "Reset", 0, QApplication::UnicodeUTF8));
        pushButtonPlan->setText(QApplication::translate("GraspPlannerEvaluator", "Plan grasp(s)", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("GraspPlannerEvaluator", "Grasp Planning Options", 0, QApplication::UnicodeUTF8));
        checkBoxFoceClosure->setText(QApplication::translate("GraspPlannerEvaluator", "Force closure", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("GraspPlannerEvaluator", "Number of Grasps", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("GraspPlannerEvaluator", "Timeout, seconds (0=endless)", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("GraspPlannerEvaluator", "Min quality", 0, QApplication::UnicodeUTF8));
        pushButtonSave->setText(QApplication::translate("GraspPlannerEvaluator", "Save XML", 0, QApplication::UnicodeUTF8));
        pushButtonClose->setText(QApplication::translate("GraspPlannerEvaluator", "Close", 0, QApplication::UnicodeUTF8));
        pushButtonOpen->setText(QApplication::translate("GraspPlannerEvaluator", "Open", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("GraspPlannerEvaluator", "Pertubed Options", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("GraspPlannerEvaluator", "ID", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("GraspPlannerEvaluator", "Out of", 0, QApplication::UnicodeUTF8));
        pushButtonNormalGrasp->setText(QApplication::translate("GraspPlannerEvaluator", "Execute Normal \n"
"Grasp", 0, QApplication::UnicodeUTF8));
        labelGraspNum->setText(QApplication::translate("GraspPlannerEvaluator", "0", 0, QApplication::UnicodeUTF8));
        pushButtonResetPose->setText(QApplication::translate("GraspPlannerEvaluator", "Reset Poses", 0, QApplication::UnicodeUTF8));
        pushButtonPerturbedGrasp->setText(QApplication::translate("GraspPlannerEvaluator", "Perturbed Grasp", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("GraspPlannerEvaluator", "Translation", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("GraspPlannerEvaluator", "X axis", 0, QApplication::UnicodeUTF8));
        label_16->setText(QApplication::translate("GraspPlannerEvaluator", "Y axis", 0, QApplication::UnicodeUTF8));
        label_17->setText(QApplication::translate("GraspPlannerEvaluator", "Z axis", 0, QApplication::UnicodeUTF8));
        label_18->setText(QApplication::translate("GraspPlannerEvaluator", "Z axis", 0, QApplication::UnicodeUTF8));
        label_19->setText(QApplication::translate("GraspPlannerEvaluator", "Y axis", 0, QApplication::UnicodeUTF8));
        label_20->setText(QApplication::translate("GraspPlannerEvaluator", "Rotation", 0, QApplication::UnicodeUTF8));
        label_21->setText(QApplication::translate("GraspPlannerEvaluator", "X axis", 0, QApplication::UnicodeUTF8));
        label_22->setText(QApplication::translate("GraspPlannerEvaluator", "Step", 0, QApplication::UnicodeUTF8));
        label_23->setText(QApplication::translate("GraspPlannerEvaluator", "Vis. Delay (ms)", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QApplication::translate("GraspPlannerEvaluator", "Information", 0, QApplication::UnicodeUTF8));
        labelInfo->setText(QApplication::translate("GraspPlannerEvaluator", "Grasps: 0", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("GraspPlannerEvaluator", "Visualization Options", 0, QApplication::UnicodeUTF8));
        checkBoxHighlight->setText(QApplication::translate("GraspPlannerEvaluator", "Highlight", 0, QApplication::UnicodeUTF8));
        checkBoxCones->setText(QApplication::translate("GraspPlannerEvaluator", "Friction Cones", 0, QApplication::UnicodeUTF8));
        checkBoxGrasps->setText(QApplication::translate("GraspPlannerEvaluator", "Show grasps", 0, QApplication::UnicodeUTF8));
        checkBoxColModel->setText(QApplication::translate("GraspPlannerEvaluator", "Collision Model", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class GraspPlannerEvaluator: public Ui_GraspPlannerEvaluator {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GRASPPLANNEREVALUATOR_H
