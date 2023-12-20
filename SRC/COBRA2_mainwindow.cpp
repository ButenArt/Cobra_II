#include "COBRA2_mainwindow.h"
#include "ui_COBRA2_mainwindow.h"

COBRA2_MainWindow::COBRA2_MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::COBRA2_MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint);
    this->setFixedSize(this->width(),this->height());
    this->setWindowTitle("COBRA2_Server");

    systemTray = new SystemTray();
    systemTray->setToolTip(this->windowTitle());

    COBRA2 = new T_COBRA2_manager();

    NetStatTimer = new QTimer(this);
    NetStatTimer->setInterval(1000);
    connect(NetStatTimer,SIGNAL(timeout()),this, SLOT(slotUpdateNetStat()));
    NetStatTimer->start();

    ControlTimer = new QTimer(this);
    ControlTimer->setInterval(20);
    connect(ControlTimer,SIGNAL(timeout()),this, SLOT(slotCANControl()));
    ControlTimer->start();

    if(COBRA2->can->BoardCount() > 0)
    {
        ui->labelMsg->setText("Moxa CAN Card OK !");
        ui->labelMsg->setStyleSheet("background-color: green");
    }
    else
    {
        ui->labelMsg->setText("There is no Moxa CAN Card.........!");
        ui->labelMsg->setStyleSheet("background-color: red");
    }

    connect(this,SIGNAL(signalPlaceToTray()),this,SLOT(slotPlaceToTray()),Qt::QueuedConnection);
    connect(systemTray,SIGNAL(signalShow()),this,SLOT(slotShowMainWindow()),Qt::QueuedConnection);
    connect(systemTray,SIGNAL(signalQuit()),this,SLOT(slotApplicationQuit()),Qt::QueuedConnection);

    ui->lblImagerStatus->setMask(QRegion(0,0,14,14, QRegion::Ellipse));
    ui->lblInstructorStatus->setMask(QRegion(0,0,14,14, QRegion::Ellipse));
    ui->lblDPDLLDriverStatus->setMask(QRegion(0,0,14,14, QRegion::Ellipse));
    ui->lblResidentDriverStatus->setMask(QRegion(0,0,14,14, QRegion::Ellipse));
    ui->lblMotionSystemDriverStatus->setMask(QRegion(0,0,14,14, QRegion::Ellipse));
    ui->lblScriptServerStatus->setMask(QRegion(0,0,14,14, QRegion::Ellipse));
    ui->lblOUStatus->setMask(QRegion(0,0,14,14, QRegion::Ellipse));

    msform = new MotionSystemForm(this, COBRA2->dpdll);
    msform->hide();

    canform = new COBRA2_CANWindow(this, COBRA2->controls);
    canform->hide();

    errorswindow = new COBRA2_ErrorsWindow;
    errorswindow->_COBRA2 = COBRA2;
    errorswindow->hide();
}

COBRA2_MainWindow::~COBRA2_MainWindow()
{
    delete errorswindow;
    delete canform;
    delete msform;
    delete ControlTimer;
    delete NetStatTimer;
    delete COBRA2;
    delete systemTray;
    delete ui;
}

void COBRA2_MainWindow::closeEvent(QCloseEvent *event)
{
    event->ignore();
    emit signalPlaceToTray();
}

void COBRA2_MainWindow::slotPlaceToTray()
{
    systemTray->showIconTray();
    this->hide();
}

void COBRA2_MainWindow::slotShowMainWindow()
{
    systemTray->showIconTray();
    this->show();
}

void COBRA2_MainWindow::slotApplicationQuit()
{
    systemTray->hideIconTray();
    COBRA2->log->WriteLog(LogEvents::Application_close, "");
    QApplication::quit();
}

void COBRA2_MainWindow::Init()
{
    COBRA2->instructor->Init();
    COBRA2->imager->Init();
    COBRA2->driver->Init();
    COBRA2->move->Init_position(COBRA2->instructor->Environment->temperature);
    COBRA2->errors->Init();
}

void COBRA2_MainWindow::on_ExStartButton_clicked()
{
    COBRA2->instructor->ExerciseParameters->ReStart = true;
    COBRA2->instructor->ExerciseParameters->Pause = false;
    COBRA2->instructor->ExerciseParameters->Stop = false;
    COBRA2->instructor->ExerciseParameters->ExerciseNum = 0; //7
    COBRA2->instructor->ExerciseParameters->MapType = 0;     //1
    COBRA2->instructor->ExerciseParameters->night = false;
    COBRA2->instructor->ExerciseParameters->season = static_cast<quint8>(Seasons::Summer);
    COBRA2->instructor->ExerciseParameters->Uuid = QUuid::createUuid().toByteArray();
    COBRA2->instructor->ExerciseParameters->PacketReady = true;
    COBRA2->instructor->ExerciseReStart();
}

void COBRA2_MainWindow::on_ExStopButton_clicked()
{
    COBRA2->instructor->ExerciseParameters->ReStart = false;
    COBRA2->instructor->ExerciseParameters->Pause = false;
    COBRA2->instructor->ExerciseParameters->Stop = true;
    COBRA2->instructor->ExerciseParameters->ExerciseNum = 0;
    COBRA2->instructor->ExerciseParameters->MapType = 0;
    COBRA2->instructor->ExerciseParameters->night = false;
    COBRA2->instructor->ExerciseParameters->season = static_cast<quint8>(Seasons::Summer);
    COBRA2->instructor->ExerciseParameters->PacketReady = true;
    COBRA2->instructor->ExerciseStop();
}

void COBRA2_MainWindow::on_GoButton_clicked()
{
    //COBRA2->instructor->Messages->MessageType = mt_Warning;
    //COBRA2->instructor->Messages->MessageText = "qwErty polk123$ я дЭбил и туплю ғаз қағаз";
    //COBRA2->instructor->Messages->PacketReady = true;

    //COBRA2->instructor->Failures->failureType = 15;
    //COBRA2->instructor->Failures->failureActive = true;
    //COBRA2->instructor->Failures->PacketReady = true;

    if(COBRA2->instructor->exerciseStarted)
    {
        CommandsDataPacket Commands;
        Commands.PacketReady = true;
        Commands.MoveForward = true;
        COBRA2->instructor->CommandsEnqueuePacket(Commands);
    }
}

void COBRA2_MainWindow::on_IgnitionSlider_sliderReleased()
{
    if(COBRA2->controls->ManualControl())
    {
        COBRA2->driver->Switches->Ignition_II = false;
    }
    if(ui->IgnitionSlider->value() == 2) ui->IgnitionSlider->setValue(1);
}

void COBRA2_MainWindow::on_IgnitionSlider_valueChanged(int value)
{
    if(COBRA2->controls->ManualControl())
    {
        COBRA2->driver->Switches->Ignition_I = false;
        COBRA2->driver->Switches->Ignition_II = false;
        COBRA2->driver->Switches->Ignition_III = false;
        if(value == 1) COBRA2->driver->Switches->Ignition_I = true; else COBRA2->driver->Switches->Ignition_I = false;
        if(COBRA2->driver->OnBoard->EngineStarted && value == 2) COBRA2->driver->Switches->Ignition_I = true;
        if(!COBRA2->driver->OnBoard->EngineStarted && value == 2) COBRA2->driver->Switches->Ignition_II = true;
    }
}

void COBRA2_MainWindow::on_BatteryButton_pressed()
{
   if(COBRA2->controls->ManualControl()) COBRA2->driver->Switches->Battery = true;
}

void COBRA2_MainWindow::on_BatteryButton_released()
{
   if(COBRA2->controls->ManualControl()) COBRA2->driver->Switches->Battery = false;
}

void COBRA2_MainWindow::on_SignalButton_pressed()
{
   if(COBRA2->controls->ManualControl()) COBRA2->driver->CompositeSwitch->Signal = true;
}

void COBRA2_MainWindow::on_SignalButton_released()
{
   if(COBRA2->controls->ManualControl()) COBRA2->driver->CompositeSwitch->Signal = false;
}

void COBRA2_MainWindow::on_WheelsPressureModeButton_pressed()
{
   COBRA2->driver->Switches->WheelsAirPressureMode = true;
}

void COBRA2_MainWindow::on_WheelsPressureModeButton_released()
{
   COBRA2->driver->Switches->WheelsAirPressureMode = false;
}

void COBRA2_MainWindow::on_TestButton_pressed()
{
   COBRA2->driver->Switches->Diagnostics = true;
}

void COBRA2_MainWindow::on_TestButton_released()
{
   COBRA2->driver->Switches->Diagnostics = false;
}

void COBRA2_MainWindow::on_RearFogLampButton_pressed()
{
   COBRA2->driver->Switches->RearFogLight = true;
}

void COBRA2_MainWindow::on_RearFogLampButton_released()
{
   COBRA2->driver->Switches->RearFogLight = false;
}

void COBRA2_MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
        case Qt::Key_0:
            {
            msform->UpdateTimer->start();
            msform->show();
            }
            break;
        case Qt::Key_9:
            {
            canform->UpdateTimer->start();
            canform->show();
            }
            break;
        case Qt::Key_8:
            {
            errorswindow->UpdateTimer->start();
            errorswindow->show();
            }
            break;
        case Qt::Key_D:
            ui->SteeringSlider->setValue(70);
            break;
        case Qt::Key_A:
            ui->SteeringSlider->setValue(30);
            break;
        case Qt::Key_W:
            ui->GasSlider->setValue(ui->GasSlider->value() + 5);
            break;
        case Qt::Key_S:
            ui->GasSlider->setValue(ui->GasSlider->value() - 5);
            break;
        case Qt::Key_C:
            ui->BrakeSlider->setValue(ui->BrakeSlider->value() + 5);
            break;
        case Qt::Key_Z:
            ui->BrakeSlider->setValue(ui->BrakeSlider->value() - 5);
            break;
        case Qt::Key_E:
            ui->CouplingSlider->setValue(ui->CouplingSlider->value() + 5);
            break;
        case Qt::Key_Q:
            ui->CouplingSlider->setValue(ui->CouplingSlider->value() - 5);
            break;
        case Qt::Key_N:
            ui->GearSlider->setValue(0);
            break;
        case Qt::Key_R:
            ui->GearSlider->setValue(-1);
            break;
        case Qt::Key_1:
            ui->GearSlider->setValue(1);
            break;
        case Qt::Key_2:
            ui->GearSlider->setValue(2);
            break;
        case Qt::Key_3:
            ui->GearSlider->setValue(3);
            break;
        case Qt::Key_4:
            ui->GearSlider->setValue(4);
            break;
        case Qt::Key_5:
            ui->GearSlider->setValue(5);
            break;
    }
}

void COBRA2_MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    switch(event->key())
    {
        case Qt::Key_D:
        case Qt::Key_A:
            ui->SteeringSlider->setValue(50);
            break;
    }
}

void COBRA2_MainWindow::slotUpdateNetStat()
{
    if(COBRA2->can->CAN_Ready())
    {
        if(COBRA2->can->ReadBuffer(0) > 0) ui->label_CANbuffer_0->setStyleSheet("color: red"); else ui->label_CANbuffer_0->setStyleSheet("color: black");
        ui->label_CANbuffer_0->setText("Buffer 0: " + QString::number(COBRA2->can->ReadBuffer(0)));
        ui->label_CANbuffer_0->adjustSize();
        if(COBRA2->can->ReadBuffer(1) > 0) ui->label_CANbuffer_1->setStyleSheet("color: red"); else ui->label_CANbuffer_1->setStyleSheet("color: black");
        ui->label_CANbuffer_1->setText("Buffer 1: " + QString::number(COBRA2->can->ReadBuffer(1)));
        ui->label_CANbuffer_1->adjustSize();
        ui->label_TXcnt->setStyleSheet("color: black");
        ui->label_TXcnt->setText("TX count: " + QString::number(COBRA2->can->TXpacktsCNT()));
        ui->label_TXcnt->adjustSize();
        ui->label_RXcnt->setStyleSheet("color: black");
        ui->label_RXcnt->setText("RX count: " + QString::number(COBRA2->can->RXpacktsCNT()));
        ui->label_RXcnt->adjustSize();
        ui->label_TXpps->setStyleSheet("color: black");
        ui->label_TXpps->setText("TX pps: " + QString::number(COBRA2->can->TXpacktsPPS()));
        ui->label_TXpps->adjustSize();
        ui->label_RXpps->setStyleSheet("color: black");
        ui->label_RXpps->setText("RX pps: " + QString::number(COBRA2->can->RXpacktsPPS()));
        ui->label_RXpps->adjustSize();
        if(COBRA2->can->RXpacktsPPS() != 0)
        {
            ui->NetStatGroupBox->setTitle("CAN statistics | Conection OK");
            ui->NetStatGroupBox->setStyleSheet("color: green");
        }
        else
        {
            ui->NetStatGroupBox->setTitle("CAN statistics | Conection Err");
            ui->NetStatGroupBox->setStyleSheet("color: red");
        }
    }

    ui->lblMotionSystemQueue->setText("Motion: " + QString::number(COBRA2->motionsystem->GetDriverQueueBuffer()));
    ui->lblMotionSystemQueue->adjustSize();
    ui->lblInstructorQueue->setText("Instructor: " + QString::number(COBRA2->instructor->GetQueueBuffer()));
    ui->lblInstructorQueue->adjustSize();
    ui->lblImagerQueue->setText("Imager: " + QString::number(COBRA2->imager->GetQueueBuffer()));
    ui->lblImagerQueue->adjustSize();

    if(COBRA2->can->CAN_Active()) COBRA2->server->CANBusState = ElementsStateTypes::Good;
        else COBRA2->server->CANBusState = ElementsStateTypes::CanBlockNoAnswer;
    if(COBRA2->ini->DPDLL_Driver_Enable())
    {
        if(COBRA2->dpdll->ResidentDriverActive()) COBRA2->server->ResidentDriverState = ElementsStateTypes::Good;
            else COBRA2->server->ResidentDriverState = ElementsStateTypes::NoSoftwareAnswer;
        if(COBRA2->dpdll->MotionSystemDriverActive()) COBRA2->server->MotionSystemDriverState = ElementsStateTypes::Good;
            else COBRA2->server->MotionSystemDriverState = ElementsStateTypes::NoSoftwareAnswer;
        if(COBRA2->dpdll->DPDLLDriverActive()) ui->lblDPDLLDriverStatus->setStyleSheet("background-color: green");
            else ui->lblDPDLLDriverStatus->setStyleSheet("background-color: red");
        if(COBRA2->dpdll->ResidentDriverActive()) ui->lblResidentDriverStatus->setStyleSheet("background-color: green");
            else ui->lblResidentDriverStatus->setStyleSheet("background-color: red");
        if(COBRA2->dpdll->MotionSystemDriverActive()) ui->lblMotionSystemDriverStatus->setStyleSheet("background-color: green");
            else ui->lblMotionSystemDriverStatus->setStyleSheet("background-color: red");
    }
    else
    {
        if(COBRA2->motionsystem->DriverMotionSystemActive) COBRA2->server->MotionSystemDriverState = ElementsStateTypes::Good;
            else COBRA2->server->MotionSystemDriverState = ElementsStateTypes::NoSoftwareAnswer;
        if(COBRA2->tcpclient_driver_motionsystem->ClientConnected()) ui->lblMotionSystemDriverStatus->setStyleSheet("background-color: green");
            else ui->lblMotionSystemDriverStatus->setStyleSheet("background-color: red");
    }

    if(COBRA2->tcpclient_imager->ClientConnected()) ui->lblImagerStatus->setStyleSheet("background-color: green");
        else ui->lblImagerStatus->setStyleSheet("background-color: red");
    if(COBRA2->tcpclient_instructor->ClientConnected()) ui->lblInstructorStatus->setStyleSheet("background-color: green");
        else ui->lblInstructorStatus->setStyleSheet("background-color: red");
    if(COBRA2->tcpclient_scriptserver->ClientConnected()) ui->lblScriptServerStatus->setStyleSheet("background-color: green");
        else ui->lblScriptServerStatus->setStyleSheet("background-color: red");
    if(COBRA2->tcpclient_controlsimitator->ClientConnected()) ui->lblOUStatus->setStyleSheet("background-color: green");
        else ui->lblOUStatus->setStyleSheet("background-color: red");

    ui->lst_Fault->clear();
    ui->lst_Fault->addItem(COBRA2->server->CANBusStateMsg);
}

void COBRA2_MainWindow::slotCANControl()
{
  if(COBRA2->can->CAN_Ready())
  {
     ui->labelBattery->setText("Батареи: " + QString::number(COBRA2->driver->OnBoard->BatteryActive));
     ui->labelBrake->setText("Тормоз: " + QString::number(COBRA2->driver->Controls->Brake));
     ui->labelGas->setText("Газ: " + QString::number(COBRA2->driver->Controls->Gas));
     ui->labelCoupling->setText("Сцепление: " + QString::number(COBRA2->driver->Controls->Coupling));
     ui->labelSteering->setText("Руль: " + QString::number(COBRA2->driver->Controls->Steering));
     ui->labelTrans->setText("Передача: " + QString::number(COBRA2->driver->OnBoard->Gear));
     ui->labelIncTrans->setText("Раздатка: " + QString::number(COBRA2->driver->OnBoard->TransferGear));
     ui->labelBlockDiff->setText("Блок. дифф.: " + QString::number(COBRA2->driver->Controls->DiffLock));
     ui->labelBlockWheels->setText("Блок. колес: " + QString::number(COBRA2->driver->Controls->WheelsLock));
     ui->labelWheelsAirSystemPower->setText("Сист.подкачки: " + QString::number(COBRA2->driver->OnBoard->WheelsAirPressurePower));
     ui->labelWheelsAirSystemMode->setText("Реж.подкачки: " + QString::number(COBRA2->driver->OnBoard->WheelsAirPressureMode));

     ui->labelSpeed->setText("Скорость: " + QString::number(COBRA2->move->Move->bodyVelocity));
     ui->labelSpeedEngine->setText("Обороты двигателя: " + QString::number(COBRA2->move->_engine->Motor->Omega));
     ui->labelTorque->setText("Крутящий момент: " + QString::number(COBRA2->move->_engine->Motor->Torque));
     ui->labelEnginePower->setText("Мощность двигателя: " + QString::number(COBRA2->move->_engine->Motor->Power));
     ui->labelVolt->setText("Заряд аккумуляттора (V): " + QString::number(COBRA2->driver->OnBoard->V_akb));
     ui->labelVboard->setText("Напряжение бортовой сети (V): " + QString::number(COBRA2->driver->OnBoard->V_board));
     ui->labelEngineTemp->setText("Температура двигателя: " + QString::number(COBRA2->driver->OnBoard->T_Motor));
     ui->labelWaterTemp->setText("Температура ОЖ: " + QString::number(COBRA2->driver->OnBoard->T_OG));
     ui->labelSparkTemp->setText("Температура свечи: " + QString::number(COBRA2->driver->OnBoard->T_Svecha));
     ui->labelEngineOilPress->setText("Давление масла двигателя: " + QString::number(COBRA2->driver->OnBoard->Pmdv));
     ui->labelTransferOilPress->setText("Давление масла РК: " + QString::number(COBRA2->driver->OnBoard->Pmkp));
     ui->labelFuel->setText("Остаток топлива (л): " + QString::number(COBRA2->driver->OnBoard->LeftFuelTankLevel+COBRA2->driver->OnBoard->RightFuelTankLevel));
     ui->labelTrip->setText("Пробег: " + QString::number(COBRA2->move->Move->Kilometrage));

     ui->labelWheelsCounterTorqueRL->setText("brakeTorque RL: " + QString::number(COBRA2->driver->OnBoard->brakeTorque[5]));
     ui->labelWheelsTorqueRL->setText("WheelsTorque RL: " + QString::number(COBRA2->driver->OnBoard->WheelsTorque[5]));

     ui->labelExerciseGuid->setText(COBRA2->instructor->ExerciseParameters->Uuid);

     ui->Driver_GB->setEnabled(COBRA2->ini->ManualMode());
     ui->TyreValvesGB->setEnabled(COBRA2->ini->ManualMode());
     //ui->ExerciseGB->setEnabled(TIGER->ini->ManualMode());
     COBRA2->controls->SetManualControl(COBRA2->ini->ManualMode());
     if(COBRA2->controls->ManualControl())
     {
        // механик-водитель
        COBRA2->driver->Controls->Brake = ui->BrakeSlider->value();
        COBRA2->driver->Controls->Gas = ui->GasSlider->value();
        COBRA2->driver->Controls->Coupling = ui->CouplingSlider->value();
        COBRA2->driver->Controls->Steering = ui->SteeringSlider->value();
        COBRA2->driver->SetGear(ui->GearSlider->value(),COBRA2->controls->ManualControl());
        COBRA2->driver->SetTransferGear(ui->TransferGearSlider->value(),COBRA2->controls->ManualControl());
        COBRA2->driver->CompositeSwitch->Washer = ui->Washer_CB->isChecked();
        COBRA2->driver->Switches->CentralLight = ui->HeadLamp_CB->isChecked();
        COBRA2->driver->Switches->Sidelights = ui->HeadLamp_CB->isChecked();
        COBRA2->driver->CompositeSwitch->DistanceLight = ui->HighBeamLamp_CB->isChecked();
        COBRA2->driver->Switches->Preheater = ui->Preheater_CB->isChecked();
        COBRA2->driver->Switches->ArticulatedLorry = ui->Lorry_CB->isChecked();
        COBRA2->driver->Switches->SMU = ui->SMU_CB->isChecked();
        COBRA2->driver->Switches->Alarm = ui->Alarm_CB->isChecked();
        COBRA2->driver->Controls->HandBrake = ui->HandBrake_CB->isChecked();
        COBRA2->driver->Controls->EngineBrake = ui->EngineBrake_CB->isChecked();
        COBRA2->driver->Controls->DiffLock = ui->DiffLock_CB->isChecked();
        COBRA2->driver->Controls->WheelsLock = ui->WheelsLock_CB->isChecked();
        COBRA2->driver->Switches->LeftFuelTank = ui->LeftFuelTank_CB->isChecked();
        COBRA2->driver->Switches->WheelsAirPressurePower = ui->WheelsAirPressurePower_CB->isChecked();
        CheckWiperSlider(ui->WiperSlider->value(),COBRA2->driver->CompositeSwitch->WiperModeA, COBRA2->driver->CompositeSwitch->WiperModeB, COBRA2->driver->CompositeSwitch->WiperModeC);
        CheckCentralLightAdjustLevelSlider(ui->CentralLightAdjustLevelSlider->value(), COBRA2->driver->Switches->CentralLightAdjustLevel0, COBRA2->driver->Switches->CentralLightAdjustLevel1, COBRA2->driver->Switches->CentralLightAdjustLevel2, COBRA2->driver->Switches->CentralLightAdjustLevel3, COBRA2->driver->Switches->CentralLightAdjustLevel4);
        CheckSlider(ui->TurnSwitchSlider->value(), COBRA2->driver->CompositeSwitch->LeftTurnSwitch, COBRA2->driver->CompositeSwitch->RightTurnSwitch);
        COBRA2->driver->TyreValves->TyreTap_1 = ui->TyreTap_1_CB->isChecked();
        COBRA2->driver->TyreValves->TyreTap_2 = ui->TyreTap_2_CB->isChecked();
        COBRA2->driver->TyreValves->TyreTap_3 = ui->TyreTap_3_CB->isChecked();
        COBRA2->driver->TyreValves->TyreTap_4 = ui->TyreTap_4_CB->isChecked();
        CheckSlider(ui->FrontTyreTapSlider->value(), COBRA2->driver->TyreValves->FrontAirTapDischarge, COBRA2->driver->TyreValves->FrontAirTapPump);
        CheckSlider(ui->RearTyreTapSlider->value(), COBRA2->driver->TyreValves->RearAirTapDischarge, COBRA2->driver->TyreValves->RearAirTapPump);
        ui->labelTyre_1->setText("Колесо 1: " + QString::number(COBRA2->driver->OnBoard->Press_tyre[0]*10) + "  Уровень: " + QString::number(COBRA2->driver->OnBoard->Press_tyre_level[0]));
        ui->labelTyre_2->setText("Колесо 2: " + QString::number(COBRA2->driver->OnBoard->Press_tyre[1]*10) + "  Уровень: " + QString::number(COBRA2->driver->OnBoard->Press_tyre_level[1]));
        ui->labelTyre_3->setText("Колесо 3: " + QString::number(COBRA2->driver->OnBoard->Press_tyre[2]*10) + "  Уровень: " + QString::number(COBRA2->driver->OnBoard->Press_tyre_level[2]));
        ui->labelTyre_4->setText("Колесо 4: " + QString::number(COBRA2->driver->OnBoard->Press_tyre[3]*10) + "  Уровень: " + QString::number(COBRA2->driver->OnBoard->Press_tyre_level[3]));
        ui->labelFrontTyres->setText("Давление шин передняя ось: " + QString::number(COBRA2->driver->OnBoard->FrontWheelsAirPressure*10));
        ui->labelRearTyres->setText("Давление шин задняя ось: " + QString::number(COBRA2->driver->OnBoard->RearWheelsAirPressure*10));
        ui->labelSystemAirPress->setText("Давление воздуха в системе: " + QString::number(COBRA2->driver->OnBoard->Pvoz));
     }
  }
}

void COBRA2_MainWindow::CheckSlider(int value, bool &variable1, bool &variable2)
{
    if(value==-1) variable1 = true; else variable1 = false;
    if(value==1) variable2 = true; else variable2 = false;
}

void COBRA2_MainWindow::CheckWiperSlider(int value, bool &variable1, bool &variable2, bool &variable3)
{
    if(value==1) variable1 = true; else variable1 = false;
    if(value==2) variable2 = true; else variable2 = false;
    if(value==3) variable3 = true; else variable3 = false;
}

void COBRA2_MainWindow::CheckCentralLightAdjustLevelSlider(int value, bool &variable0, bool &variable1, bool &variable2, bool &variable3, bool &variable4)
{
    if(value==0) variable0 = true; else variable0 = false;
    if(value==1) variable1 = true; else variable1 = false;
    if(value==2) variable2 = true; else variable2 = false;
    if(value==3) variable3 = true; else variable3 = false;
    if(value==4) variable4 = true; else variable4 = false;
}
