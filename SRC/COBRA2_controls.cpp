#include "COBRA2_controls.h"

T_COBRA2_controls::T_COBRA2_controls( T_COBRE2_CAN* can, T_COBRA2_Server* Server, T_COBRA2_Imager* Imager, T_COBRA2_Instructor* Instructor, T_COBRA2_driver* driver, COBRA2_Move* move, TSettingsFile* ini, TReplay* replay, T_COBRA2_ServiceErrors* errors, T_COBRA2_MotionSystem* MotionSystem, T_DPDLL* dpdll )
	: T_Controls( can )
{
	_can = can;
	_Server = Server;
	_Imager = Imager;
	_Instructor = Instructor;
	_driver = driver;
	_move = move;
	_ini = ini;
	_replay = replay;
	_errors = errors;
	_MotionSystem = MotionSystem;
	_dpdll = dpdll;

	//ParamsList = new T_ParamsList;
	PARAM_IN.resize( N_PARAM_IN );
	PARAM_OUT.resize( N_PARAM_OUT );
	PARAM_MAB.resize( N_PARAM_MAB );
	PARAM_VID.resize( N_PARAM_VID );
	PARAM_POT.resize( N_PARAM_POT );
	PARAM_ENUM.resize( N_PARAM_ENUM );

	_OnBoard = new T_COBRA2_Onboard( _driver, _move, _errors );
	_OnBoard->_audio->SoundsEnabled = _ini->SoundsEnabled( );

	ReadLogicINI( QCoreApplication::applicationDirPath( ).append( "/Logic_IN_OUT.ini" ) );

	connect( OnBoardTimer, SIGNAL( timeout( ) ), this, SLOT( slotValidateParameters( ) ) );
	OnBoardTimer->start( );

	manualControl = false;
}

T_COBRA2_controls::~T_COBRA2_controls( )
{
	OnBoardTimer->stop( );
	delete _OnBoard;
}

void T_COBRA2_controls::slotValidateParameters( )
{
	if ( !_can->CAN_Ready( ) ) return;

	ValidateParameters( );

	ComputeParameters( );
	PrintParametrs( );
}

void T_COBRA2_controls::ReadLogicINI( QString INIfilename )
{
	QString GPIO_IN( "GPIO_IN" );
	QString GPIO_OUT( "GPIO_OUT" );
	QString MAB( "MAB" );
	QString ENUM( "ENUM" );
	QString POTENTIOMETER( "POTENTIOMETER" );
	QString VID( "VID" );

	QSettings* ini = new QSettings( INIfilename, QSettings::IniFormat );

	if ( QFileInfo( INIfilename ).exists( ) )
	{
		ini->beginGroup( GPIO_IN );
		for ( int i = 1; i <= N_PARAM_IN; i++ )
		{
			PARAM_IN[i].ParameterCaption = ini->value( "NAME#" + QString::number( i ), "NC" ).toString( );
			PARAM_IN[i].UnitNum = ini->value( "UNIT#" + QString::number( i ), 0 ).toInt( );
			PARAM_IN[i].RemapBitNum = ini->value( "BIT#" + QString::number( i ), 0 ).toInt( );
			PARAM_IN[i].InverseBit = ini->value( "INV#" + QString::number( i ), 0 ).toBool( );
			qDebug( ) << i
				<< PARAM_IN[i].ParameterCaption
				<< PARAM_IN[i].UnitNum
				<< PARAM_IN[i].RemapBitNum
				<< PARAM_IN[i].InverseBit;
		}
		ini->endGroup( );

		ini->beginGroup( GPIO_OUT );
		for ( int i = 1; i <= N_PARAM_OUT; i++ )
		{
			PARAM_OUT[i].ParameterCaption = ini->value( "NAME#" + QString::number( i ), "NC" ).toString( );
			PARAM_OUT[i].UnitNum = ini->value( "UNIT#" + QString::number( i ), 0 ).toInt( );
			PARAM_OUT[i].RemapBitNum = ini->value( "BIT#" + QString::number( i ), 0 ).toInt( );
			PARAM_OUT[i].InverseBit = ini->value( "INV#" + QString::number( i ), 0 ).toBool( );
			qDebug( ) << i
				<< PARAM_OUT[i].ParameterCaption
				<< PARAM_OUT[i].UnitNum
				<< PARAM_OUT[i].RemapBitNum
				<< PARAM_OUT[i].InverseBit;
		}
		ini->endGroup( );

		ini->beginGroup( MAB );
		for ( int i = 1; i <= N_PARAM_MAB; i++ )
		{
			PARAM_MAB[i].Ratio = 1.0f;
			PARAM_MAB[i].Caption = ini->value( "NAME#" + QString::number( i ), "NC" ).toString( );
			PARAM_MAB[i].UnitNum = ini->value( "UNIT#" + QString::number( i ), 0 ).toInt( );
			PARAM_MAB[i].RemapPosition = ini->value( "WORD#" + QString::number( i ), 0 ).toInt( );
			PARAM_MAB[i].Reverse = ini->value( "REVERSE#" + QString::number( i ), 0 ).toBool( );
			PARAM_MAB[i].OffSet = ini->value( "OFFSET#" + QString::number( i ), 0 ).toInt( );
			PARAM_MAB[i].CutOff = ini->value( "CUTOFF#" + QString::number( i ), 0 ).toInt( );
			PARAM_MAB[i].Ratio = ini->value( "RATIO#" + QString::number( i ), 1.0 ).toFloat( );
			PARAM_MAB[i].MaxValue = ini->value( "MAXVAL#" + QString::number( i ), 0 ).toInt( );
			PARAM_MAB[i].NormalValue = ini->value( "NORMVAL#" + QString::number( i ), 0 ).toInt( );
			PARAM_MAB[i].OutOfRange = false;
			PARAM_MAB[i].Value = 0;
		}
		ini->endGroup( );

		ini->beginGroup( ENUM );
		for ( int i = 1; i <= N_PARAM_ENUM; i++ )
		{
			PARAM_ENUM[i].Caption = ini->value( "NAME#" + QString::number( i ), "NC" ).toString( );
			PARAM_ENUM[i].UnitNum = ini->value( "UNIT#" + QString::number( i ), 0 ).toInt( );
			PARAM_ENUM[i].RemapPosition = ini->value( "WORD#" + QString::number( i ), 0 ).toInt( );
			PARAM_ENUM[i].Reverse = ini->value( "REVERSE#" + QString::number( i ), 0 ).toBool( );
			PARAM_ENUM[i].OffSet = ini->value( "OFFSET#" + QString::number( i ), 0 ).toInt( );
			PARAM_ENUM[i].CutOff = ini->value( "CUTOFF#" + QString::number( i ), 0 ).toInt( );
			PARAM_ENUM[i].NormalValue = ini->value( "NORMVAL#" + QString::number( i ), 0 ).toInt( );
			PARAM_ENUM[i].OutOfRange = false;
			PARAM_ENUM[i].Value = 0;
		}
		ini->endGroup( );

		ini->beginGroup( POTENTIOMETER );
		for ( int i = 1; i <= N_PARAM_POT; i++ )
		{
			PARAM_POT[i].Caption = ini->value( "NAME#" + QString::number( i ), "NC" ).toString( );
			PARAM_POT[i].UnitNum = ini->value( "UNIT#" + QString::number( i ), 0 ).toInt( );
			PARAM_POT[i].RemapPosition = ini->value( "WORD#" + QString::number( i ), 0 ).toInt( );
			PARAM_POT[i].Reverse = ini->value( "REVERSE#" + QString::number( i ), 0 ).toBool( );
			PARAM_POT[i].OffSet = ini->value( "OFFSET#" + QString::number( i ), 0 ).toInt( );
			PARAM_POT[i].CutOff = ini->value( "CUTOFF#" + QString::number( i ), 0 ).toInt( );
			PARAM_POT[i].MaxValue = ini->value( "MAXVAL#" + QString::number( i ), 0 ).toInt( );
			PARAM_POT[i].NormalValue = ini->value( "NORMVAL#" + QString::number( i ), 0 ).toInt( );
			PARAM_POT[i].OutOfRange = false;
			PARAM_POT[i].Value = 0;
		}
		ini->endGroup( );

		ini->beginGroup( VID );
		for ( int i = 1; i <= N_PARAM_VID; i++ )
		{
			PARAM_VID[i].Caption = ini->value( "NAME#" + QString::number( i ), "NC" ).toString( );
			PARAM_VID[i].UnitNum = ini->value( "UNIT#" + QString::number( i ), 0 ).toInt( );
			PARAM_VID[i].RemapPosition = ini->value( "WORD#" + QString::number( i ), 0 ).toInt( );
			for ( int j = 1; j <= N_STEP_POINTS_COUNT; j++ )
			{
				PARAM_VID[i].Value_Scale_Point[j - 1] = ini->value( "SCALE#" + QString::number( i ) + ": Point" + QString::number( j ), 0 ).toInt( );
				PARAM_VID[i].Value_Steps_Point[j - 1] = ini->value( "STEPS#" + QString::number( i ) + ": Point" + QString::number( j ), 0 ).toInt( );
			}
			PARAM_VID[i].Value = 0;
			PARAM_VID[i].MIN_VAL_SCALE_POINT = PARAM_VID[i].MAX_VAL_SCALE_POINT = PARAM_VID[i].Value_Scale_Point[0]; // мин и макс равны первому значению
			for ( int j = 0; j <= N_STEP_POINTS_COUNT; j++ ) // выясняем макимальную по значению прибора точку
			{
				if ( PARAM_VID[i].Value_Scale_Point[j] > PARAM_VID[i].MAX_VAL_SCALE_POINT )
				{
					PARAM_VID[i].MAX_VAL_SCALE_POINT = PARAM_VID[i].Value_Scale_Point[j];
					PARAM_VID[i].MAX_VAL_ID = j;
				}
			}
			qDebug( ) << i << PARAM_VID[i].Caption << PARAM_VID[i].UnitNum;
		}
		ini->endGroup( );

	}
	ini->destroyed( );
}

void T_COBRA2_controls::ComputeParameters( )
{
	quint16 ResetPowerTickCount = 6000 / _ini->CanTxInterval( );
	quint16 ResetPowerTickStart = 5500 / _ini->CanTxInterval( ); // параметр для того чтобы успеть послать сигнал на реле сброса питания до отключения КАНа
	_Server->ResetPower = false;
	if ( _Instructor->exerciseReInit )
	{
		_driver->Init( );
		_move->Init_position( _Instructor->Environment->temperature );
		_OnBoard->_audio->Init( );
		_errors->Init( );
		_driver->OnBoard->T_init = _Instructor->Environment->temperature;
		_Instructor->exerciseReInit = false;
		_Server->ResetPower = true;
		_Server->ResetPowerTickCounter = ResetPowerTickCount;
	}
	// алгоритм задержки работы платформы и TX пакетов после срабатывания реле сброса питания
	if ( _Server->ResetPowerTickCounter > 0 )
	{
		_Server->ResetPowerTickCounter--;
		if ( _Server->ResetPowerTickCounter < ResetPowerTickStart ) _can->setTXEnabled( false );
	}
	else
	{
		if ( _can->BoardCount( ) > 0 && !_can->TXEnabled( ) ) _can->setTXEnabled( true );
		_Server->ResetPowerTickCounter = 0;
	}

	// если упражнение не запущено дальше не обрабатываем
	if ( !_Instructor->exerciseStarted )
	{
		MotionSystemStateDataPacket MotionState;
		MotionState.PacketReady = true;
		MotionState.Roll = 0;
		MotionState.Pitch = 0;
		MotionState.Course = 0;
		MotionState.matmodelZ = _ini->DownPosition_MotionSystemDriver( );
		_MotionSystem->DriverMotionStateEnqueuePacket( MotionState );
		return;
	}

	if ( _Instructor->commandMoveForward )
	{
		_Instructor->commandMoveForward = false;
	}

	// коробка передач
	if ( _driver->Controls->Gear_R ) { _driver->SetGear( -1, manualControl ); _move->_transmission->Transmission_SetGear( -1 ); }          // задний ход
	if ( _driver->Controls->Gear_5 ) { _driver->SetGear( 5, manualControl ); _move->_transmission->Transmission_SetGear( 5 ); }            // 5 передача
	if ( _driver->Controls->Gear_4 ) { _driver->SetGear( 4, manualControl ); _move->_transmission->Transmission_SetGear( 4 ); }            // 4 передача
	if ( _driver->Controls->Gear_3 ) { _driver->SetGear( 3, manualControl ); _move->_transmission->Transmission_SetGear( 3 ); }            // 3 передача
	if ( _driver->Controls->Gear_2 ) { _driver->SetGear( 2, manualControl ); _move->_transmission->Transmission_SetGear( 2 ); }            // 2 передача
	if ( _driver->Controls->Gear_1 ) { _driver->SetGear( 1, manualControl ); _move->_transmission->Transmission_SetGear( 1 ); }            // 1 передача
	if ( !_driver->Controls->Gear_R && !_driver->Controls->Gear_5 && !_driver->Controls->Gear_4 && !_driver->Controls->Gear_3 && !_driver->Controls->Gear_2 && !_driver->Controls->Gear_1 ) { _driver->SetGear( 0, manualControl ); _move->_transmission->Transmission_SetGear( 0 ); } // нейтральная передача

	// раздат. коробка
	if ( _driver->Controls->TransferGear_High ) { _driver->SetTransferGear( 1, manualControl ); _move->_transmission->Transmission_SetTransferGear( 1 ); }   // повышенная
	if ( _driver->Controls->TransferGear_Low ) { _driver->SetTransferGear( -1, manualControl ); _move->_transmission->Transmission_SetTransferGear( -1 ); }  // пониженная
	if ( !_driver->Controls->TransferGear_High && !_driver->Controls->TransferGear_Low ) { _driver->SetTransferGear( 0, manualControl ); _move->_transmission->Transmission_SetTransferGear( 0 ); } // раздат. коробка - нейтральная

	// блокировка межосевого дифференциала
	_move->_transmission->Transmission_SetDiff( _driver->Controls->DiffLock );

	// газ (с учетом топливного крана, МОД и уровня топлива)
	if ( ( !_driver->EdgeContacts->FuelTapLeftTank && !_driver->EdgeContacts->FuelTapRightTank ) || _driver->OnBoard->StopEngine || ( _driver->OnBoard->LeftFuelTankLevel + _driver->OnBoard->RightFuelTankLevel == 0 ) ) _move->_engine->Motor_SetGas( 0 ); else _move->_engine->Motor_SetGas( _driver->Controls->Gas / 100.0f );

	// сцепление
	_move->_transmission->Transmission_SetCoupling( _driver->Controls->Coupling / 100.0f );

	// тормоз
	_move->_transmission->Transmission_SetBrake( _driver->Controls->Brake / 100.0f );

	// тангаж
	_move->Move->Pitch = _Imager->ImagerState->Pitch;

	// скорость колес
	if ( !_Imager->ImagerState->WheelsList.isEmpty( ) && _Imager->ImagerState->WheelsList.count( ) == _Imager->ImagerState->WheelsCount )  // перестраховываемся
	{
		_move->Move->WheelsSpeed[0] = _Imager->ImagerState->WheelsList[0].wheelSpeed;
		_move->Move->WheelsSpeed[1] = _Imager->ImagerState->WheelsList[1].wheelSpeed;
		_move->Move->WheelsSpeed[2] = _Imager->ImagerState->WheelsList[2].wheelSpeed;
		_move->Move->WheelsSpeed[3] = _Imager->ImagerState->WheelsList[3].wheelSpeed;
	}

	// скорость
	_move->Move->bodyVelocity = RemoveSpeedVibrations( _Imager->ImagerState->Speed );

	// противомомент двигателя
	_move->Move->engineTorque = _Imager->ImagerState->EngineTorque;

	// вычисление показателей бортовых систем
	_OnBoard->Onboard_Compute( COMPUTING_INTERVAL / 1000.0f );

	// температура двигателя
	_move->_engine->Motor->T_Motor = _driver->OnBoard->T_Motor;

	_move->Movement( COMPUTING_INTERVAL / 1000.0f, _driver->OnBoard->Starter_f );

	_driver->OnBoard->EngineStarted = _move->_engine->Motor->Started;

	// крутящий момент на колесах
	_driver->OnBoard->WheelsTorque[0] = _move->Move->WheelsTorque[0];
	_driver->OnBoard->WheelsTorque[1] = _move->Move->WheelsTorque[1];
	_driver->OnBoard->WheelsTorque[2] = _move->Move->WheelsTorque[2];
	_driver->OnBoard->WheelsTorque[3] = _move->Move->WheelsTorque[3];

	// компенсирующий момент тормоза на колесах
	_driver->OnBoard->brakeTorque[0] = _move->Move->brakeTorque[0];
	_driver->OnBoard->brakeTorque[1] = _move->Move->brakeTorque[1];
	_driver->OnBoard->brakeTorque[2] = _move->Move->brakeTorque[2];
	_driver->OnBoard->brakeTorque[3] = _move->Move->brakeTorque[3];
}

void T_COBRA2_controls::ValidateParameters( )
{
	SetVIDSteps( 1, _driver->Gauges->Speedometer );  // спидометр
	SetVIDSteps( 2, _driver->Gauges->Tachometer );  // тахометр
	SetVIDSteps( 3, _driver->Gauges->FrontWheelsAirPressure );  // манометр давления воздуха в шинах передняя ось (красная стрелка)
	SetVIDSteps( 4, _driver->Gauges->SystemAirPressure );  // манометр воздушной системы (белая стрелка)
	SetVIDSteps( 5, _driver->Gauges->Voltage );  // напряжение АКБ
	SetVIDSteps( 6, _driver->Gauges->EngineCoolantTemperature );  // термометр ОЖ
	SetVIDSteps( 7, _driver->Gauges->BrakeSystemAirPressure );  // манометр давления воздуха в пневмоприводе тормозов (красная стрелка)
	SetVIDSteps( 8, _driver->Gauges->EngineOilPressure );  // манометр масла двигателя
	SetVIDSteps( 9, _driver->Gauges->FuelLevel );  // указатель уровня топлива
	SetVIDSteps( 10, _driver->Gauges->RearWheelsAirPressure ); // манометр давления воздуха в шинах задняя ось (белая стрелка)

	if ( !ManualControl( ) )
	{
		CanHeartBeat( );

		_driver->Controls->Gas = MABNormalValue( 1 );                             // газ в %
		_driver->Controls->Brake = MABNormalValue( 2 );                           // тормоз в %
		if ( _errors->FaultInjection.Active[NO_BRAKE] ) _driver->Controls->Brake = 0;
		_driver->Controls->Coupling = MABNormalValue( 3 );                        // сцепление в %
		if ( _errors->FaultInjection.Active[NO_COUPLING] ) _driver->Controls->Coupling = 0;
		_driver->Controls->Steering = MABNormalValue( 4 );                        // руль в %

		_driver->Controls->Gear_R = GetGPIOINBitValue( 1 );                       // задний ход
		_driver->Controls->Gear_5 = GetGPIOINBitValue( 2 );                       // 5 передача
		_driver->Controls->Gear_4 = GetGPIOINBitValue( 3 );                       // 4 передача
		_driver->Controls->Gear_3 = GetGPIOINBitValue( 4 );                       // 3 передача
		_driver->Controls->Gear_2 = GetGPIOINBitValue( 5 );                       // 2 передача
		_driver->Controls->Gear_1 = GetGPIOINBitValue( 6 );                       // 1 передача
		_driver->Controls->HandBrake = GetGPIOINBitValue( 7 );                    // ручной тормоз
		_driver->Controls->EngineBrake = GetGPIOINBitValue( 8 );                  // моторный тормоз
		_driver->Controls->DiffLock = GetGPIOINBitValue( 9 );                     // блокировка межосевого дифференциала (сухая дорога/скользкая дорога)
		_driver->Controls->WheelsLock = GetGPIOINBitValue( 10 );                  // выключатель блокировки колес
		_driver->Controls->TransferGear_High = GetGPIOINBitValue( 11 );           // раздат. коробка - повышенная
		_driver->Controls->TransferGear_Low = GetGPIOINBitValue( 12 );            // раздат. коробка - пониженная
		_driver->Switches->Ignition_I = GetGPIOINBitValue( 13 );                  // замок зажигания в положении I
		_driver->Switches->Ignition_II = GetGPIOINBitValue( 14 );                 // замок зажигания в положении II (нефиксируемое)
		_driver->Switches->Ignition_III = GetGPIOINBitValue( 15 );                // замок зажигания в положении III (блокировка руля)
		_driver->Switches->Battery = GetGPIOINBitValue( 16 );                     // выключатель батарей
		_driver->Switches->SMU = GetGPIOINBitValue( 17 );                         // светомаскировка
		_driver->Switches->Alarm = GetGPIOINBitValue( 18 );                       // выключатель аварийной сигнализации
		_driver->Switches->Sidelights = GetGPIOINBitValue( 19 );                  // центральный переключатель света - габариты
		_driver->Switches->CentralLight = GetGPIOINBitValue( 20 );                // центральный переключатель света - ближний свет
		_driver->Switches->RearFogLight = GetGPIOINBitValue( 21 );                // выключатель задних противотуманных фонарей
		_driver->Switches->Preheater = GetGPIOINBitValue( 22 );                   // предпусковой подогреватель (ПЖД16)
		_driver->Switches->WheelsAirPressurePower = GetGPIOINBitValue( 23 );      // система подкачки шин
		_driver->Switches->WheelsAirPressureMode = GetGPIOINBitValue( 24 );       // режим системы подкачки шин
		_driver->Switches->Diagnostics = GetGPIOINBitValue( 25 );                 // диагностика
		_driver->Switches->ArticulatedLorry = GetGPIOINBitValue( 26 );            // выключатель "Автопоезд"
		_driver->Switches->LeftFuelTank = GetGPIOINBitValue( 27 );                // переключатель баков
		_driver->Doping_2->Power = GetGPIOINBitValue( 28 );                       // тумблер запуска генераторов системы пажаротушения
		_driver->CompositeSwitch->LeftTurnSwitch = GetGPIOINBitValue( 29 );       // переключатель поворота на лево
		_driver->CompositeSwitch->RightTurnSwitch = GetGPIOINBitValue( 30 );      // переключатель поворота на право
		_driver->CompositeSwitch->WiperModeA = GetGPIOINBitValue( 31 );           // выключатель стеклоочистителя первая скорость
		_driver->CompositeSwitch->WiperModeB = GetGPIOINBitValue( 32 );           // выключатель стеклоочистителя вторая скорость
		_driver->CompositeSwitch->WiperModeC = GetGPIOINBitValue( 33 );           // выключатель стеклоочистителя третья скорость
		_driver->CompositeSwitch->Washer = GetGPIOINBitValue( 34 );               // насос омывателя стекла
		_driver->CompositeSwitch->DistanceLight = GetGPIOINBitValue( 35 );        // дальний свет фар
		_driver->CompositeSwitch->Signal = GetGPIOINBitValue( 36 );               // звуковой сигнал
		_driver->TyreValves->FrontAirTapPump = GetGPIOINBitValue( 37 );           // накачка шин передняя ось
		_driver->TyreValves->FrontAirTapDischarge = GetGPIOINBitValue( 38 );      // выпуск воздуха из шин передняя ось
		_driver->TyreValves->RearAirTapPump = GetGPIOINBitValue( 39 );            // накачка шин задняя ось
		_driver->TyreValves->RearAirTapDischarge = GetGPIOINBitValue( 40 );       // выпуск воздуха из шин задняя ось

		_can->setGPIOOUT_Ready( false );
		_can->SOD_Clear_TMP_OUT_Data( );
		SetGPIOOUTBitValue( _driver->Lamps->EngineOilPressLamp, 1 );              // давление масла в двигателе
		SetGPIOOUTBitValue( _driver->Lamps->LorryLamp, 2 );                       // фонари автопоезда
		SetGPIOOUTBitValue( _driver->Lamps->WaterLevelLamp, 3 );                  // уровень ОЖ
		SetGPIOOUTBitValue( _driver->Lamps->SMULamp, 4 );                         // режим светомаскировки
		SetGPIOOUTBitValue( _driver->Lamps->ColdStartLamp, 5 );                   // холодный пуск
		SetGPIOOUTBitValue( _driver->Lamps->TurnLamp, 6 );                        // указатель поворота
		SetGPIOOUTBitValue( _driver->Lamps->SidelightsLamp, 7 );                  // габаритные огни
		SetGPIOOUTBitValue( _driver->Lamps->DiagnosticsLamp, 8 );                 // диагностика
		SetGPIOOUTBitValue( _driver->Lamps->TrailerTurnLamp, 9 );                 // указатель поворота прицепа
		SetGPIOOUTBitValue( _driver->Lamps->LowFuelLamp, 10 );                    // мало топлива
		SetGPIOOUTBitValue( _driver->Lamps->WaterInFuelLamp, 11 );                // наличие воды в топливе
		SetGPIOOUTBitValue( _driver->Lamps->AirFilterLamp, 12 );                  // засореный воздушный фильтр
		SetGPIOOUTBitValue( _driver->Lamps->BatteryLamp, 13 );                    // батарея
		SetGPIOOUTBitValue( _driver->Lamps->PreheaterLamp, 14 );                  // предпусковой подогреватель (ПЖД16)
		SetGPIOOUTBitValue( _driver->Lamps->EngineBrakeLamp, 15 );                // моторный тормоз
		SetGPIOOUTBitValue( _driver->Lamps->InteraxleDiffLockLamp, 16 );          // сигнал блокировки межосевого дифференциала
		SetGPIOOUTBitValue( _driver->Lamps->CrossaxleDiffLockLamp, 17 );          // сигнал блокировки межколесного дифференциала
		SetGPIOOUTBitValue( _driver->Lamps->HighBeamLamp, 18 );                   // дальний свет
		SetGPIOOUTBitValue( _driver->Lamps->HandBrakeLamp, 19 );                  // включение стояночной тормозной системы
		SetGPIOOUTBitValue( _driver->Lamps->BrakeFaultLamp, 20 );                 // неисправность тормозной системы
		SetGPIOOUTBitValue( _driver->Lamps->WaterOverheatLamp, 21 );              // перегрев ОЖ
		SetGPIOOUTBitValue( _driver->Lamps->WheelsAirPressure_Sand, 22 );         // режим "песок" давления воздуха в шинах
		SetGPIOOUTBitValue( _driver->Lamps->WheelsAirPressure_Terrain, 23 );      // режим "грунт" давления воздуха в шинах
		SetGPIOOUTBitValue( _driver->Lamps->WheelsAirPressure_Highway, 24 );      // режим "шоссе" давления воздуха в шинах
		SetGPIOOUTBitValue( _driver->Lamps->WheelsAirPressure_Fault, 25 );        // неисправность или низкое давление в шинах
		SetGPIOOUTBitValue( _driver->Doping_2->LeftGreenLamp, 26 );               // сигнальная лампа исправности левого генератора системы пожаротушения
		SetGPIOOUTBitValue( _driver->Doping_2->LeftRedLamp, 27 );                 // сигнальная лампа неисправности левого генератора системы пожаротушения
		SetGPIOOUTBitValue( _driver->Doping_2->RightGreenLamp, 28 );              // сигнальная лампа исправности правого генератора системы пожаротушения
		SetGPIOOUTBitValue( _driver->Doping_2->RightRedLamp, 29 );                // сигнальная лампа неисправности правого генератора системы пожаротушения
		SetGPIOOUTBitValue( _driver->OnBoard->SteeringBlock, 30 );                // блокировка руля
		SetGPIOOUTBitValue( _driver->OnBoard->TransmissionBlock, 31 );            // блокировка КПП
		SetGPIOOUTBitValue( _Server->ResetPower, 32 );                            // общий сброс питания тренажера
		SetGPIOOUTBitValue( _driver->Lamps->DashBoardLight, 33 );                 // подсветка приборной доски
		_can->setGPIOOUT_Ready( true );
	}
}

T_UNITS T_COBRA2_controls::GetUnitsData( )
{
	//	qDebug() << __FUNCTION__;
	for ( int i = 0; i < N_MAB_UNITS; i++ )
	{
		_can->UNITS.MAB[i].INFO = _can->UNITS.MAB[i].DESC +
			"Pack TX / RX : [" + QString::number( _can->UNITS.MAB[i].TX ) + "]/[" + QString::number( _can->UNITS.MAB[i].RX ) + "]";
	}

	for ( int i = 0; i < N_VID_UNITS; i++ )
	{
		_can->UNITS.VID[i].INFO = _can->UNITS.VID[i].DESC +
			"Pack TX / RX: [" + QString::number( _can->UNITS.VID[i].TX ) + "]/[" + QString::number( _can->UNITS.VID[i].RX ) + "]";
	}
	for ( int i = 0; i < N_IN_UNITS; i++ )
	{
		_can->UNITS.GPIN[i].INFO = _can->UNITS.GPIN[i].DESC +
			"Pack TX / RX: [" + QString::number( _can->UNITS.GPIN[i].TX ) + "]/[" + QString::number( _can->UNITS.GPIN[i].RX ) + "]";
	}
	for ( int i = 0; i < N_OUT_UNITS; i++ ) {
		_can->UNITS.GPOUT[i].INFO = _can->UNITS.GPOUT[i].DESC +
			"Pack TX/RX: [" + QString::number( _can->UNITS.GPOUT[i].TX ) + "]/[" + QString::number( _can->UNITS.GPOUT[i].RX ) + "]";
	}
	for ( int i = 0; i < N_ENC_UNITS; i++ ) {
		_can->UNITS.ENC[i].INFO = _can->UNITS.ENC[i].DESC +
			"Pack TX/RX: [" + QString::number( _can->UNITS.ENC[i].TX ) + "]/[" + QString::number( _can->UNITS.ENC[i].RX ) + "]";
	}
	for ( int i = 0; i < N_POT_UNITS; i++ )
	{
		_can->UNITS.POT[i].INFO = _can->UNITS.POT[i].DESC +
			"Pack TX/RX: [" + QString::number( _can->UNITS.POT[i].TX ) + "]/[" + QString::number( _can->UNITS.POT[i].RX ) + "]";
	}
	for ( int i = 0; i < N_ENUM_UNITS; i++ )
	{
		_can->UNITS.ENUM[i].INFO = _can->UNITS.ENUM[i].DESC +
			"Pack TX/RX: [" + QString::number( _can->UNITS.ENUM[i].TX ) + "]/[" + QString::number( _can->UNITS.ENUM[i].RX ) + "]";
	}
	return _can->UNITS;
}

void T_COBRA2_controls::CanHeartBeat( )
{
	//	qDebug() << __FUNCTION__;
	ElementsStateTypes CANBusState = ElementsStateTypes::Good;
	QString CANBusStateMsg = "";
	if ( _can->BoardCount( ) > 0 )
	{
		if ( _can->Port0( ) == INVALID_HANDLE_VALUE )
		{
			CANBusState = ElementsStateTypes::CanBlockNoAnswer;
			CANBusStateMsg += "Open CAN Port0 Fail \r\n";
		}
		if ( _can->Port1( ) == INVALID_HANDLE_VALUE )
		{
			CANBusState = ElementsStateTypes::CanBlockNoAnswer;
			CANBusStateMsg += "Open CAN Port1 Fail \r\n";
		}
	}
	for ( auto& unit : _can->UNITS.MAB )
	{
		if ( unit.FAULT )
		{
			CANBusState = ElementsStateTypes::CanBlockNoAnswer;
			CANBusStateMsg += unit.sFAULT;
		}
	}
	for ( auto& unit : _can->UNITS.VID )
	{
		if ( unit.FAULT )
		{
			CANBusState = ElementsStateTypes::CanBlockNoAnswer;
			CANBusStateMsg += unit.sFAULT;
		}
	}
	for ( auto& unit : _can->UNITS.GPIN )
	{
		if ( unit.FAULT )
		{
			CANBusState = ElementsStateTypes::CanBlockNoAnswer;
			CANBusStateMsg += unit.sFAULT;
		}
	}
	for ( auto& unit : _can->UNITS.GPOUT )
	{
		if ( unit.FAULT )
		{
			CANBusState = ElementsStateTypes::CanBlockNoAnswer;
			CANBusStateMsg += unit.sFAULT;
		}
	}
	for ( auto& unit : _can->UNITS.POT )
	{
		if ( unit.FAULT )
		{
			CANBusState = ElementsStateTypes::CanBlockNoAnswer;
			CANBusStateMsg += unit.sFAULT;
		}
	}
	for ( int i = 0; i < N_PARAM_MAB; i++ )
	{
		WORD value = GetMABValue( i );
		if ( value == 4096 ) CANBusStateMsg += "MAB" + GetMABaddr( i ) + " disconnected \r\n";
	}
	_Server->CANBusState = CANBusState;
	_Server->CANBusStateMsg = CANBusStateMsg;
}

void T_COBRA2_controls::PrintParametrs( )
{
	PrintTumblers( );
}

void T_COBRA2_controls::PrintTumblers( )
{
}