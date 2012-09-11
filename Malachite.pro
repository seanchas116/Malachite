#-------------------------------------------------
#
# Project created by QtCreator 2012-05-22T13:23:54
#
#-------------------------------------------------

TARGET = malachite
TEMPLATE = lib
CONFIG += staticlib
QT += core gui

DEFINES += MALACHITE_LIBRARY

SOURCES += \
    mlblendmode.cpp \
    mlblendop.cpp \
    mlcolor.cpp \
    mlcurves.cpp \
    mlimage.cpp \
    mlpaintengine.cpp \
    mlpainter.cpp \
    mlsurface.cpp \
    mlsurfacepainter.cpp \
    mlsurfaceselection.cpp \
    mlmisc.cpp \
    mlbrush.cpp \
    mlimageio.cpp \
    mlcolorgradient.cpp \
    mlcurvesubdivision.cpp \
    private/mlsurfacepaintengine.cpp \
    #mlacceleratedimage.cpp \
    #mlacceleratedimagefilter.cpp \
    mlvector.cpp \
    mlpolygon.cpp \
    private/mlrenderer.cpp \
    private/mlimagepaintengine.cpp \
    mlfixedpolygon.cpp \
    private/clipper.cpp

HEADERS +=\
    mlglobal.h \
    mlblendmode.h \
    mlblendop.h \
    mlcolor.h \
    mlcurves.h \
    mlgenericimage.h \
    mlimage.h \
    mlmisc.h \
    mlpaintable.h \
    mlpaintengine.h \
    mlpainter.h \
    mlsurface.h \
    mlsurfacepainter.h \
    mlsurfaceselection.h \
    mlbrush.h \
    mlbitmap.h \
    mlimageio.h \
    mlcolorgradient.h \
    mlcurvesubdivision.h \
    private/mlsurfacepaintengine.h \
    private/agg_scanline_p.h \
    private/agg_rasterizer_sl_clip.h \
    private/agg_rasterizer_scanline_aa.h \
    private/agg_rasterizer_cells_aa.h \
    private/agg_math.h \
    private/agg_gamma_functions.h \
    private/agg_config.h \
    private/agg_clip_liang_barsky.h \
    private/agg_basics.h \
    private/agg_array.h \
    #mlacceleratedimage.h \
    #mlacceleratedimagefilter.h \
    private/mlimageioprivate.h \
    mlvector.h \
    mlpolygon.h \
    mlpixelconversion.h \
    mlmemory.h \
    private/mlscalinggenerator.h \
    private/mlgradientgenerator.h \
    private/mlfiller.h \
    private/mlrenderer.h \
    private/mlimagepaintengine.h \
    mldivision.h \
    mlfixedpolygon.h \
    private/clipper.hpp

OTHER_FILES += \
    resources/shader/default.vert \
    resources/shader/blend/src_over.frag \
    resources/shader/test.vert \
    resources/shader/test.frag \
    readme

RESOURCES += \
    resources.qrc

CONFIG(debug, debug|release) {
	DEFINES += QT_DEBUG
} else {
	DEFINES += QT_NO_DEBUG
}

QMAKE_CFLAGS_X86_64 = -mmacosx-version-min=10.7
QMAKE_CXXFLAGS_X86_64 = $$QMAKE_CFLAGS_X86_64
QMAKE_OBJECTIVE_CFLAGS_X86_64 = $$QMAKE_CFLAGS_X86_64
QMAKE_LFLAGS_X86_64 = $$QMAKE_CFLAGS_X86_64

LIBS += -lfreeimage
#QMAKE_CXXFLAGS += -fpermissive -ffast-math
QMAKE_CXXFLAGS +=  -std=c++0x -m64
CONFIG += sse2

target.path = /usr/local/lib
INSTALLS += target

