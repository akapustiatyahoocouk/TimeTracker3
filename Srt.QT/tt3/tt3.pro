include(../tt3.pri)

SOURCES += \
    Application.cpp \
    Component.cpp \
    Main.cpp

HEADERS += \
    API.hpp \
    Application.hpp \
    Components.hpp

FORMS += \

RESOURCES += \
    tt3.qrc

LIBS += -ltt3-gui -ltt3-ws -ltt3-db-api -ltt3-util
