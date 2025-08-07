include(../tt3.pri)

TEMPLATE = lib
DEFINES += TT3_WS_LIBRARY

SOURCES += \
    WorkspaceComponent.cpp \
    WorkspaceSettings.cpp

HEADERS += \
    API.hpp \
    Components.hpp \
    Linkage.hpp

LIBS += -ltt3-db-api -ltt3-util
