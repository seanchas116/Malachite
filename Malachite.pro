#-------------------------------------------------
#
# Project created by QtCreator 2012-05-22T13:23:54
#
#-------------------------------------------------

TARGET = malachite
TEMPLATE = lib

DEFINES += MALACHITE_LIBRARY

SOURCES += \
    agg_curves.cpp \
    mlblendmode.cpp \
    mlblendop.cpp \
    mlcolor.cpp \
    mlcurves.cpp \
    mlgradientcache.cpp \
    mlimage.cpp \
    mlimagepaintengine.cpp \
    mllinegradient.cpp \
    mlpaintengine.cpp \
    mlpainter.cpp \
    mlsurface.cpp \
    mlsurfacepaintengine.cpp \
    mlsurfacepainter.cpp \
    mlsurfaceselection.cpp \
    mlmisc.cpp \
    mlbrush.cpp

HEADERS += malachite.h\
    agg_curves.h \
    agg_config.h \
    agg_basics.h \
    agg_array.h \
    mlglobal.h \
    mlblendmode.h \
    mlblendop.h \
    mlcolor.h \
    mlcurves.h \
    mlgenericimage.h \
    mlgradient.h \
    mlgradientcache.h \
    mlimage.h \
    mlimagepaintengine.h \
    mlintdivision.h \
    mllinegradient.h \
    mlmisc.h \
    mlpaintable.h \
    mlpaintengine.h \
    mlpainter.h \
    mlscanlineclipper.h \
    mlssevector.h \
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
    mlbitmap.h

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

# MacPorts
mac {
	INCLUDEPATH += /opt/local/include
	LIBS += -L/opt/local/lib
}

LIBS += -lfreeimage

