#include "COBRA2_errorswindow.h"
#include "ui_COBRA2_errorswindow.h"

COBRA2_ErrorsWindow::COBRA2_ErrorsWindow(QWidget *parent) : QWidget(parent), ui(new Ui::COBRA2_ErrorsWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint);
    this->setFixedSize(this->width(),this->height());
    this->setWindowTitle("COBRA2 Driver Errors");

    UpdateTimer = new QTimer(this);
    UpdateTimer->setInterval(20);
    connect(UpdateTimer,SIGNAL(timeout()),this, SLOT(slotErrorsUpdate()));
    UpdateTimer->stop();
}

COBRA2_ErrorsWindow::~COBRA2_ErrorsWindow()
{
    delete UpdateTimer;
    delete ui;
}

void COBRA2_ErrorsWindow::closeEvent(QCloseEvent *event)
{
    event->ignore();
    UpdateTimer->stop();
    this->hide();
}

void COBRA2_ErrorsWindow::slotErrorsUpdate()
{
    ui->ErrorsListWidget->clear();
    for(int i=0; i<N_ERRORS; i++)
    {
        if(_COBRA2->errors->Errors.Send[i]) ui->ErrorsListWidget->addItem(_COBRA2->errors->Errors.Name[i]);
    }
}

