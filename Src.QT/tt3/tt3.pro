include(../tt3.pri)

SOURCES += \
    Application.cpp \
    Component.cpp \
    Main.cpp

HEADERS += \
    API.hpp \
    Application.hpp \
    Components.hpp

PRECOMPILED_HEADER = API.hpp

FORMS += \

RESOURCES += \
    tt3.qrc

RC_FILE = tt3.rc

LIBS += \
    -ltt3-help-client$$TARGET_SUFFIX \
    -ltt3-gui$$TARGET_SUFFIX \
    -ltt3-ws$$TARGET_SUFFIX \
    -ltt3-db-api$$TARGET_SUFFIX \
    -ltt3-util$$TARGET_SUFFIX

DISTFILES += \
    tt3.rc

QMAKE_POST_LINK += pwd $$escape_expand(\\n\\t)
QMAKE_POST_LINK += cd $$shell_path($${PWD}/Help) $$escape_expand(\\n\\t)
QMAKE_POST_LINK += pwd $$escape_expand(\\n\\t)
QMAKE_POST_LINK += rm -f $$shell_path($${DESTDIR}/Help/$${TARGET}.zip) $$escape_expand(\\n\\t)
QMAKE_POST_LINK += $$sprintf($${QMAKE_MKDIR_CMD}, $$shell_path($${DESTDIR}/Help)) $$escape_expand(\\n\\t)
QMAKE_POST_LINK += "zip -r $$shell_path($${DESTDIR}/Help/$${TARGET}.zip) . -x \"_vti_cnf/*\" -x \"*/_vti_cnf/*\" -x \"_vti_pvt/*\" -x \"*/_vti_pvt/*\" -x desktop.ini" $$escape_expand(\\n\\t)
QMAKE_POST_LINK += cd $${OUT_PWD} $$escape_expand(\\n\\t)
QMAKE_POST_LINK += pwd $$escape_expand(\\n\\t)


