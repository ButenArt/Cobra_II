#include "PCAN.h"

CAN_PCAN::CAN_PCAN( )
{
	auto stsResult = CAN_Initialize( PcanHandle[0], Bitrate );
	if ( stsResult != PCAN_ERROR_OK )
	{
		qDebug() << "USB1: Can not initialize. Please check the defines in the code.";
		qDebug() << "Error code: " << hex << stsResult;
		//return;
	}
	stsResult = CAN_Initialize( PcanHandle[1], Bitrate );
	if ( stsResult != PCAN_ERROR_OK )
	{
		qDebug() << "USB2: Can not initialize. Please check the defines in the code.";
		qDebug() << "Error code: " << hex << stsResult;
		//return;
	}
}

CAN_PCAN::~CAN_PCAN( )
{
	qDebug( ) << "Can uninitialize.";
	CAN_Uninitialize( PcanHandle[0] );
	CAN_Uninitialize( PcanHandle[1] );
}

size_t CAN_PCAN::ReadBuffer( int port )
{
	TPCANMsg CANMsg;
	TPCANStatus stsResult;
	//stsResult = CAN_Read( PcanHandle[port], &CANMsg, nullptr );
	while ( stsResult = CAN_Read( PcanHandle[port], &CANMsg, nullptr ) == PCAN_ERROR_OK )
	{
		message[port].push( CANMsg );
	}
	return message[port].size( );
}

int CAN_PCAN::SendPacket(DWORD addr, BYTE FrameType, BYTE Length, BYTE frame[], int port)
{
	TPCANMsg msgCanMessage;
	msgCanMessage.ID = addr;
	msgCanMessage.LEN = Length;
	msgCanMessage.MSGTYPE = PCAN_MESSAGE_EXTENDED;
	for (size_t i = 0; i < Length; ++i)
	{
		msgCanMessage.DATA[i] = frame[i];
	}
	//const TPCANHandle PcanHandle0 = PCAN_USBBUS1;
	auto stsResult = CAN_Write(PcanHandle[port], &msgCanMessage);
	if (stsResult == PCAN_ERROR_INITIALIZE)
		stsResult = CAN_Initialize(PcanHandle[port], Bitrate); // переподключение

	return stsResult == PCAN_ERROR_OK ? CNIO_SUCCESS : 0;
}

// данные уже считаны функцией ReadBuffer и положены в очередь
int CAN_PCAN::ReadPacket( DWORD& addr, BYTE& FrameType, BYTE& Length, BYTE frame[ ], int port )
{
	if ( message[port].empty( ) ) 
		return 0;

	auto m = message[port].front( );
	message[port].pop( );
	addr = m.ID;
	FrameType = CNIO_EXTENDED_FRAME;
	Length = m.LEN;
	for ( size_t i = 0; i < Length; ++i )
	{
		frame[i] = m.DATA[i];
	}
	return CNIO_SUCCESS;
}

