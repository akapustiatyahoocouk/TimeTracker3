include(../tt3.pri)

TEMPLATE = lib
DEFINES += TT3_GUI_LIBRARY

SOURCES += \
    GuiComponent.cpp \
    GuiSettings.cpp \
    SkinRegistry.cpp

HEADERS += \
    API.hpp \
    Components.hpp \
    Linkage.hpp \
    Skin.hpp

LIBS += -ltt3-util
