include(../tt3.pri)

TEMPLATE = lib
DEFINES += TT3_REPORT_WORKSUMMARY_LIBRARY

SOURCES += \
    Component.cpp \
    ReportConfigurationEditor.cpp \
    ReportType.cpp

HEADERS += \
    API.hpp \
    Component.hpp \
    Linkage.hpp \
    ReportConfiguration.hpp \
    ReportConfigurationEditor.hpp \
    ReportType.hpp

LIBS += \
    -ltt3-report$$TARGET_SUFFIX \
    -ltt3-gui$$TARGET_SUFFIX \
    -ltt3-ws$$TARGET_SUFFIX \
    -ltt3-db-api$$TARGET_SUFFIX \
    -ltt3-util$$TARGET_SUFFIX

RESOURCES += \
    tt3-report-worksummary.qrc

FORMS += \
    ReportConfigurationEditor.ui
