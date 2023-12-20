#include "MotionSystemForm.h"
#include "ui_MotionSystemForm.h"

MotionSystemForm::MotionSystemForm(QWidget *parent, T_DPDLL *dpdll) : QWidget(parent), ui(new Ui::MotionSystemForm)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint);
    this->setFixedSize(this->width(),this->height());
    this->setWindowTitle("Motion System");

    _dpdll = dpdll;

    UpdateTimer = new QTimer(this);
    UpdateTimer->setInterval(50);
    connect(UpdateTimer,SIGNAL(timeout()),this, SLOT(slotMSParamsUpdate()));
    UpdateTimer->stop();
}

MotionSystemForm::~MotionSystemForm()
{
    delete UpdateTimer;
    delete ui;
}

void MotionSystemForm::closeEvent(QCloseEvent *event)
{
    event->ignore();
    UpdateTimer->stop();
    this->hide();
}

void MotionSystemForm::slotMSParamsUpdate()
{
    ui->labelDegreesX->setText("X: " + QString::number(_dpdll->_Imager->ImagerState->Roll));
    ui->labelDegreesY->setText("Y: " + QString::number(_dpdll->_Imager->ImagerState->Pitch));
    ui->labelDegreesH->setText("H: " + QString::number(_dpdll->_Imager->ImagerState->Course));

    ui->labelRadiansX->setText("X: " + QString::number(_dpdll->_Imager->ImagerState->Roll * M_PI / 180));
    ui->labelRadiansY->setText("Y: " + QString::number(_dpdll->_Imager->ImagerState->Pitch * M_PI / 180));
    ui->labelRadiansH->setText("H: " + QString::number(_dpdll->_Imager->ImagerState->Course * M_PI / 180));
}
