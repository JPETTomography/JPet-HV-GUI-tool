QT+= core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QMAKE_CXXFLAGS += -std=c++11
TARGET = JPetGainCalibrationDBEdittingTool.exe
TEMPLATE = app
INCLUDEPATH += $$PWD/JPET-HV-adjust
LIBS+=-L$$PWD/JPET-HV-adjust -ldataaccess
LIBS+=-L$$PWD/JPET-HV-adjust -lpg_interface
LIBS+=-L$$PWD/JPET-HV-adjust -lhvadjust
LIBS+=-L$$PWD/JPET-HV-adjust -llocal_routines
LIBS+= -lpqxx
SOURCES += main.cpp mainwindow.cpp
HEADERS  += mainwindow.h
FORMS    += mainwindow.ui
