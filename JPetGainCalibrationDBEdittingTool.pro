QT+= core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QMAKE_CXXFLAGS += -std=c++11
TARGET = JPetGainCalibrationDBEdittingTool.exe
TEMPLATE = app
INCLUDEPATH += $$PWD/JPET-HV-adjust
LIBS+=-L$$PWD/JPET-HV-adjust -ljpetdata -lpg_interface -ldataaccess
LIBS+= -lpqxx
SOURCES += main.cpp mainwindow.cpp \
    models.cpp
HEADERS  += mainwindow.h \
    models.h
FORMS    += mainwindow.ui
