include(../tt3.pri)

SOURCES += \
    ApplicationComponent.cpp \
    ApplicationSettings.cpp \
    Main.cpp

HEADERS += \
    API.hpp \
    Components.hpp

FORMS += \

RESOURCES += \
    tt3.qrc

LIBS += -ltt3-gui -ltt3-util
