include(../tt3.pri)

TEMPLATE = lib
DEFINES += TT3_REPORT_WORKSUMMARY_LIBRARY

SOURCES += \
    Component.cpp \
    DataTypes.cpp \
    ReportConfiguration.cpp \
    ReportConfigurationEditor.cpp \
    ReportGenerator.cpp \
    ReportType.cpp \
    SelectUsersDialog.cpp

HEADERS += \
    API.hpp \
    Component.hpp \
    DataTypes.hpp \
    Linkage.hpp \
    ReportConfiguration.hpp \
    ReportConfigurationEditor.hpp \
    ReportGenerator.hpp \
    ReportType.hpp \
    SelectUsersDialog.hpp

PRECOMPILED_HEADER = API.hpp

LIBS += \
    -ltt3-report$$TARGET_SUFFIX \
    -ltt3-gui$$TARGET_SUFFIX \
    -ltt3-ws$$TARGET_SUFFIX \
    -ltt3-db-api$$TARGET_SUFFIX \
    -ltt3-util$$TARGET_SUFFIX

RESOURCES += \
    tt3-report-worksummary.qrc

FORMS += \
    ReportConfigurationEditor.ui \
    SelectUsersDialog.ui
