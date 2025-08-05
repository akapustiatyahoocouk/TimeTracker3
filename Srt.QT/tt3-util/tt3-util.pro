include(../tt3.pri)

TEMPLATE = lib
DEFINES += TT3_UTIL_LIBRARY

SOURCES += \
    FromString.cpp \
    ToString.cpp

HEADERS += \
    API.hpp \
    Collections.hpp \
    FromString.hpp \
    Linkage.hpp \
    Macros.hpp \
    ToString.hpp
