include(../tt3.pri)

SOURCES += \
    ApplicationComponent.cpp \
    ApplicationSettings.cpp \
    Main.cpp

HEADERS += \
    API.hpp \
    Components.hpp

FORMS += \

LIBS += -ltt3-util

RESOURCES += \
    tt3.qrc
