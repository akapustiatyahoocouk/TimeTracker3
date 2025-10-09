QT += core gui widgets xml

CONFIG += c++latest strict_c++
gcc {
    QMAKE_CXXFLAGS += -ansi -Wall -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Weffc++ -Wextra -Wformat=2 -Winit-self -Wmissing-noreturn -Wno-long-long -Wold-style-cast -Woverloaded-virtual -Wpointer-arith -Wredundant-decls -Wsign-promo -Wswitch-default -Wswitch-enum -Wundef -Wunused -Wconversion -Wformat-security -Wformat-y2k -Wmissing-declarations -Wmissing-field-initializers -Wmissing-include-dirs -Wpacked -Wshadow -Wvariadic-macros -Wwrite-strings -fno-strict-aliasing -Wno-c++0x-compat
}
msvc {
    QMAKE_CXXFLAGS += /Wall /D_CRT_SECURE_NO_WARNINGS
}
INCLUDEPATH += ..

CONFIG(debug, debug|release) {
    DESTDIR = ../../../Bin.QT/Debug
    unix:LIBS += -L../../../Bin.QT/Debug
    win32:LIBPATH += ../../../Bin.QT/Debug
} else {
    DESTDIR = ../../../Bin.QT/Release
    unix:LIBS += -L../../../Bin.QT/Release
    win32:LIBPATH += ../../../Bin.QT/Release
}
