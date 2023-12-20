SOURCES += $$PWD/CAN_common.cpp \
    $$PWD/Logging.cpp \
    $$PWD/SettingsFile.cpp \
    $$PWD/torquecurve.cpp \
    $$PWD/Replay.cpp \
    $$PWD/UDP_OU.cpp \
    $$PWD/SystemTray.cpp \
    $$PWD/ServiceErrors.cpp \
    $$PWD/TCP_client.cpp \
    $$PWD/Controls.cpp \
    $$PWD/Imager.cpp \
    $$PWD/Instructor.cpp \
    $$PWD/Audio.cpp \
    $$PWD/Instructor_TCP_client.cpp \
    $$PWD/Imager_TCP_client.cpp \
    $$PWD/Server.cpp \
    $$PWD/TargetsManager.cpp \
    $$PWD/Script_TCP_client.cpp \
    $$PWD/Script.cpp \
    $$PWD/Imitator.cpp \
    $$PWD/Imitator_TCP_client.cpp \
    $$PWD/FCC.cpp \
    $$PWD/FCC_TCP_client.cpp \
    $$PWD/DriverMotion_TCP_client.cpp \
    $$PWD/GunnerMotion_TCP_client.cpp \
    $$PWD/MotionSystem.cpp

HEADERS  += \
    $$PWD/CAN_common.h \
    $$PWD/common_dictionary.h \
    $$PWD/Logging.h \
    $$PWD/SettingsFile.h \
    $$PWD/torquecurve.h \
    $$PWD/Replay.h \
    $$PWD/UDP_OU.h \
    $$PWD/SystemTray.h \
    $$PWD/ServiceErrors.h \
    $$PWD/TCP_client.h \
    $$PWD/Controls.h \
    $$PWD/Imager.h \
    $$PWD/Instructor.h \
    $$PWD/Audio.h \
    $$PWD/Instructor_TCP_client.h \
    $$PWD/Imager_TCP_client.h \
    $$PWD/Server.h \
    $$PWD/TargetsManager.h \
    $$PWD/Script_TCP_client.h \
    $$PWD/Script.h \
    $$PWD/Imitator.h \
    $$PWD/Imitator_TCP_client.h \
    $$PWD/FCC.h \
    $$PWD/FCC_TCP_client.h \
    $$PWD/DriverMotion_TCP_client.h \
    $$PWD/GunnerMotion_TCP_client.h \
    $$PWD/MotionSystem.h \
    $$PWD/queuetimer.h
	
RESOURCES += \
    $$PWD/Common.qrc

DISTFILES +=

include(MOXA/MOXA.pri)
include(PCAN/PCAN.pri)
include(Joystick/Joystick.pri)
