QT += core gui widgets xml charts webenginewidgets

TT3_VERSION=1.0.0

CONFIG += c++20 strict_c++
CONFIG += force_debug_info
gcc {
    QMAKE_CXXFLAGS += -ansi -Wall -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wextra -Wformat=2 -Winit-self -Wmissing-noreturn -Wno-long-long -Wold-style-cast -Woverloaded-virtual -Wpointer-arith -Wredundant-decls -Wswitch-enum -Wundef -Wunused -Wconversion -Wformat-security -Wformat-y2k -Wmissing-declarations -Wmissing-field-initializers -Wmissing-include-dirs -Wpacked -Wshadow -Wvariadic-macros -Wwrite-strings -fno-strict-aliasing -Wno-c++0x-compat
    #QMAKE_CXXFLAGS += -Weffc++ -Wsign-promo -Wswitch-default
    TT3_BUILD_DATE=$$system(date '+%Y%m%d')
    TT3_BUILD_TIME=$$system(date '+%H%M')
}
msvc {
    QMAKE_CXXFLAGS += /Wall /D_CRT_SECURE_NO_WARNINGS
    TT3_BUILD_DATE=$$system(c:\\cygwin64\\bin\\date '+%Y%m%d')
    TT3_BUILD_TIME=$$system(c:\\cygwin64\\bin\\date '+%H%M')
    QMAKE_CXXFLAGS += /wd4996
}
TARGET_SUFFIX=-$$TT3_VERSION
DEFINES += TT3_VERSION=\\\"$$TT3_VERSION\\\"
DEFINES += TT3_BUILD_DATE=\\\"$$TT3_BUILD_DATE\\\"
DEFINES += TT3_BUILD_TIME=\\\"$$TT3_BUILD_TIME\\\"
INCLUDEPATH += ..
COMPONENT_NAME = $$TARGET
TARGET = $$TARGET$$TARGET_SUFFIX

CONFIG(debug, debug|release) {
    DESTDIR = ../../../Bin.QT/Debug
    unix:LIBS += -L../../../Bin.QT/Debug
    win32:LIBPATH += ../../../Bin.QT/Debug
} else {
    DESTDIR = ../../../Bin.QT/Release
    unix:LIBS += -L../../../Bin.QT/Release
    win32:LIBPATH += ../../../Bin.QT/Release
}

exists($${PWD}/$${COMPONENT_NAME}/Help) {
    #   Pick up project-specific help as a .zip file
    QMAKE_POST_LINK += $$sprintf($${QMAKE_MKDIR_CMD}, $$shell_path($${DESTDIR}/Help)) $$escape_expand(\\n\\t)
    QMAKE_POST_LINK += rm -f makehelp.bat $$escape_expand(\\n\\t)
    QMAKE_POST_LINK += echo pwd >> makehelp.bat $$escape_expand(\\n\\t)
    QMAKE_POST_LINK += echo cd $$shell_path($${PWD}/$${COMPONENT_NAME}/Help) >> makehelp.bat $$escape_expand(\\n\\t)
    QMAKE_POST_LINK += echo pwd >> makehelp.bat $$escape_expand(\\n\\t)
    QMAKE_POST_LINK += echo rm -f $$shell_path($${DESTDIR}/Help/$${TARGET}.zip) >> makehelp.bat $$escape_expand(\\n\\t)
    QMAKE_POST_LINK += echo "zip -r $$shell_path($${DESTDIR}/Help/$${TARGET}.zip) . -x \"_vti_cnf/*\" -x \"*/_vti_cnf/*\" -x \"_vti_pvt/*\" -x \"*/_vti_pvt/*\" -x desktop.ini" >> makehelp.bat $$escape_expand(\\n\\t)
    QMAKE_POST_LINK += echo cd $${OUT_PWD} >> makehelp.bat $$escape_expand(\\n\\t)
    QMAKE_POST_LINK += echo pwd >> makehelp.bat $$escape_expand(\\n\\t)
    win32 {
        QMAKE_POST_LINK += makehelp.bat $$escape_expand(\\n\\t)
    }
    unix {
        QMAKE_POST_LINK += bash makehelp.bat $$escape_expand(\\n\\t)
    }
    #   Schedule proper cleanup
    QMAKE_CLEAN += $$shell_path($${DESTDIR}/Help/$${TARGET}.zip)
    QMAKE_CLEAN += rm -f makehelp.bat $$escape_expand(\\n\\t)
} else {
    #   Make sure there is no project-specific help
    QMAKE_POST_LINK += rm -f $$shell_path($${DESTDIR}/Help/$${TARGET}.zip) $$escape_expand(\\n\\t)
}
QMAKE_CLEAN += $$shell_path($${DESTDIR}/$${TARGET}.*)
