#-------------------------------------------------
#
# Project created by QtCreator 2012-05-22T13:23:54
#
#-------------------------------------------------

TARGET = malachite
TEMPLATE = lib
QT += core gui opengl

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
    private/mlimagerenderer.cpp \
    private/mlimagepaintengine.cpp \
    private/mlfillgenerator.cpp \
    mlacceleratedimage.cpp \
    mlacceleratedimagefilter.cpp \
    mlargb.cpp \
    mlvector.cpp

HEADERS += malachite.h\
    mlglobal.h \
    mlblendmode.h \
    mlblendop.h \
    mlcolor.h \
    mlcurves.h \
    mlgenericimage.h \
    mlimage.h \
    mlintdivision.h \
    mlmisc.h \
    mlpaintable.h \
    mlpaintengine.h \
    mlpainter.h \
    mlsurface.h \
    mlsurfacepainter.h \
    mlsurfaceselection.h \
    mlbrush.h \
    mlbitmap.h \
    mlsimdvector.h \
    mlargb.h \
    mlimageio.h \
    mlgenericwrapperimage.h \
    mlgenericsharedimage.h \
    mlgenericgradient.h \
    mlcolorgradient.h \
    mlpixelgenerator.h \
    mlcurvesubdivision.h \
    private/mlsurfacepaintengine.h \
    private/mlimagerenderer.h \
    private/mlimagepaintengine.h \
    private/mlfillgenerator.h \
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
    mlacceleratedimage.h \
    mlacceleratedimagefilter.h \
    private/mlimageioprivate.h \
    mlvector.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE0E95D76
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = Malachite.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

OTHER_FILES += \
    README.md \
    resources/shader/default.vert \
    resources/shader/blend/src_over.frag \
    resources/shader/test.vert \
    resources/shader/test.frag

LIBS += -lfreeimage
QMAKE_CXXFLAGS += -fpermissive
#QMAKE_CXXFLAGS += -std=c++0x

RESOURCES += \
    resources.qrc

