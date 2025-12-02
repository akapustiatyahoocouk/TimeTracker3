include(../tt3.pri)

TEMPLATE = lib
DEFINES += TT3_REPORT_LIBRARY

SOURCES += \
    BasicBlockStyle.cpp \
    BasicCharacterStyle.cpp \
    BasicLinkStyle.cpp \
    BasicListStyle.cpp \
    BasicParagraphStyle.cpp \
    BasicReportTemplate.cpp \
    BasicSectionStyle.cpp \
    BasicStyle.cpp \
    BasicTableStyle.cpp \
    BorderType.cpp \
    ColorSpec.cpp \
    Component.cpp \
    FontFlag.cpp \
    FontSpec.cpp \
    FontStyle.cpp \
    HorizontalAlignment.cpp \
    IReportTemplate.cpp \
    PageNumberPlacement.cpp \
    PageOrientation.cpp \
    PageSetup.cpp \
    ReportTemplateManager.cpp \
    TypographicSize.cpp \
    TypographicUnit.cpp \
    UnderlineMode.cpp \
    VerticalAlignment.cpp

HEADERS += \
    API.hpp \
    BasicReportTemplate.hpp \
    Classes.hpp \
    Components.hpp \
    DataTypes.hpp \
    Linkage.hpp \
    ReportTemplate.hpp

PRECOMPILED_HEADER = API.hpp

LIBS += \
    -ltt3-gui$$TARGET_SUFFIX \
    -ltt3-ws$$TARGET_SUFFIX \
    -ltt3-db-api$$TARGET_SUFFIX \
    -ltt3-util$$TARGET_SUFFIX

RESOURCES += \
    tt3-report.qrc
