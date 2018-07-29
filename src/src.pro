QT += core gui widgets opengl
QT += quick quickcontrols2

OBJECTS_DIR = $$PWD/build/obj
MOC_DIR = $$PWD/build/moc
RCC_DIR = $$PWD/build/rcc

TARGET = dir_compare
DESTDIR  = $$PWD/../bin
TEMPLATE = app
#TEMPLATE = console

CONFIG += c++17

QMAKE_CXXFLAGS += -std=c++17
#QMAKE_CXXFLAGS += -lstdc++fs
#QMAKE_CXXFLAGS += -ggdb3
#QMAKE_CXXFLAGS += -Og


win32{
    DEFINES +=WIN32
}
unix{
    DEFINES +=UNIX
}

debug{
    DEFINES += DEBUG
}

HEADERS += crc32.h
HEADERS += Ui.h
HEADERS += dir_tool.h

SOURCES += Ui.cpp
SOURCES += crc32.cpp
SOURCES += dir_tool.cpp
SOURCES += main.cpp

#LIBS +=  -lopengl32 -lglu32 -lpthread
LIBS += -lpthread -lstdc++fs

RESOURCES += qml.qrc


OTHER_FILES += .\main.qml
#OTHER_FILES += .\error.qml

#RC_FILE = app.rc
