#-------------------------------------------------
#
# Project created by QtCreator 2013-02-05T21:17:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = demo
TEMPLATE = app

INCLUDEPATH += ../include

LIBS += -L$$OUT_PWD/../src -lmalachite

QMAKE_CXXFLAGS += -std=c++11

mac {
  QMAKE_LFLAGS_SONAME = -Wl,-install_name,$$OUT_PWD/
  QMAKE_CFLAGS_X86_64 = -mmacosx-version-min=10.7
  QMAKE_CXXFLAGS_X86_64 = $$QMAKE_CFLAGS_X86_64
  QMAKE_OBJECTIVE_CFLAGS_X86_64 = $$QMAKE_CFLAGS_X86_64
  QMAKE_LFLAGS_X86_64 = $$QMAKE_CFLAGS_X86_64

	SHAREDLIB.files = $$OUT_PWD/../src/libmalachite.1.dylib
	SHAREDLIB.path = Contents/Frameworks
	QMAKE_BUNDLE_DATA += SHAREDLIB
}

SOURCES += main.cpp \
    viewport.cpp \
    form.cpp

HEADERS  += \
    viewport.h \
    form.h

FORMS += \
    form.ui

RESOURCES += \
    demo.qrc
