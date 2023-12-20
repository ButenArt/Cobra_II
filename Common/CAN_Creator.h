#pragma once
#include "CAN_Interface.h"
#include "CAN4.h"
#include "PCAN.h"
#include "MOXA.h"

class CAN_Creator
{
public:
	ICAN_Board* CreateInstance( QString name )
	{
		qDebug( ) << __FUNCTION__ << name;
		name = name.toUpper( );
		if ( name.indexOf( "MOXA" ) != -1 ) return static_cast<ICAN_Board*>( new CAN_MOXA( ) );
		if ( name.indexOf( "PCAN" ) != -1 ) return static_cast<ICAN_Board*>( new CAN_PCAN( ) );
		if ( name.indexOf( "CAN4" ) != -1 ) return static_cast<ICAN_Board*>( new CAN_CAN4( ) );
		return static_cast<ICAN_Board*>( new CAN_VIRT( ) );
	}

};

