#-------------------------------------------------
#
# Project created by QtCreator 2012-05-22T13:23:54
#
#-------------------------------------------------

TARGET = malachite
TEMPLATE = lib

DEFINES += MALACHITE_LIBRARY

SOURCES += \
    mlblendmode.cpp \
    mlblendop.cpp \
    mlcolor.cpp \
    mlcurves.cpp \
    mlimage.cpp \
    mlimagepaintengine.cpp \
    mlpaintengine.cpp \
    mlpainter.cpp \
    mlsurface.cpp \
    mlsurfacepaintengine.cpp \
    mlsurfacepainter.cpp \
    mlsurfaceselection.cpp \
    mlmisc.cpp \
    mlbrush.cpp \
    mlimageio.cpp \
    mlimagerenderer.cpp \
    mlcolorgradient.cpp \
    mlfillgenerator.cpp

HEADERS += malachite.h\
    agg_config.h \
    agg_basics.h \
    agg_array.h \
    mlglobal.h \
    mlblendmode.h \
    mlblendop.h \
    mlcolor.h \
    mlcurves.h \
    mlgenericimage.h \
    mlimage.h \
    mlimagepaintengine.h \
    mlintdivision.h \
    mlmisc.h \
    mlpaintable.h \
    mlpaintengine.h \
    mlpainter.h \
    mlsurface.h \
    mlsurfacepaintengine.h \
    mlsurfacepainter.h \
    mlsurfaceselection.h \
    agg_math.h \
    agg_scanline_p.h \
    agg_rasterizer_sl_clip.h \
    agg_rasterizer_scanline_aa.h \
    agg_rasterizer_cells_aa.h \
    agg_gamma_functions.h \
    mlbrush.h \
    agg_clip_liang_barsky.h \
    mlbitmap.h \
    mlsimdvector.h \
    mlargb.h \
    mlimageio.h \
    mlgenericwrapperimage.h \
    mlgenericsharedimage.h \
    mlimagerenderer.h \
    mlgenericgradient.h \
    mlcolorgradient.h \
    mlpixelgenerator.h \
    mlfillgenerator.h

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
    README.md

LIBS += -lfreeimage
QMAKE_CXXFLAGS += -fpermissive
#QMAKE_CXXFLAGS += -std=c++0x

