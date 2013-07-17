
CONFIG(debug, debug|release) {
  DEFINES += QT_DEBUG
} else {
  DEFINES += QT_NO_DEBUG
  #QMAKE_CXXFLAGS_RELEASE -= -O2
  #QMAKE_CXXFLAGS_RELEASE += -O3
  #QMAKE_LFLAGS_RELEASE -= -O1
  #QMAKE_LFLAGS_RELEASE += -O3
}

mac {
	QMAKE_LFLAGS_SONAME  = -Wl,-install_name,@executable_path/../Frameworks/
	QMAKE_CFLAGS_X86_64 = -mmacosx-version-min=10.7
	QMAKE_CXXFLAGS_X86_64 = $$QMAKE_CFLAGS_X86_64
	QMAKE_OBJECTIVE_CFLAGS_X86_64 = $$QMAKE_CFLAGS_X86_64
	QMAKE_LFLAGS_X86_64 = $$QMAKE_CFLAGS_X86_64
}

LIBS += -lfreeimage
QMAKE_CXXFLAGS += -std=c++11 -msse2
QMAKE_LFLAGS += -std=c++11

contains(QMAKE_CXX, clang++) {
	mac {
		QMAKE_CXXFLAGS += -stdlib=libc++
		QMAKE_LFLAGS += -stdlib=libc++
	}
	DEFINES += Q_COMPILER_INITIALIZER_LISTS
}

CONFIG += sse2
