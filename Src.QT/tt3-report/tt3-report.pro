include(../tt3.pri)

TEMPLATE = lib
DEFINES += TT3_REPORT_LIBRARY

SOURCES += \
    Component.cpp

HEADERS += \
    API.hpp \
    Components.hpp \
    Linkage.hpp

LIBS += \
    -ltt3-gui$$TARGET_SUFFIX \
    -ltt3-ws$$TARGET_SUFFIX \
    -ltt3-db-api$$TARGET_SUFFIX \
    -ltt3-util$$TARGET_SUFFIX

RESOURCES += \
    tt3-report.qrc
