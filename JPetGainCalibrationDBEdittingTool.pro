QT+= core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QMAKE_CXXFLAGS += -std=c++11
TARGET = JPetGainCalibrationDBEdittingTool.exe
TEMPLATE = app
INCLUDEPATH += $$PWD/JPET-HV-adjust
INCLUDEPATH += $$PWD/JPET-HV-adjust/math_h/include
LIBS+=-L$$PWD/JPET-HV-adjust -ldataaccess -ljpetdata -lhvadjust -lpg_interface
LIBS+= -lpqxx
SOURCES  += main.cpp mainwindow.cpp models.cpp
HEADERS  += mainwindow.h models.h
FORMS    += mainwindow.ui
