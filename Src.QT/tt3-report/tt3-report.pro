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
    FontSpecs.cpp \
    FontStyle.cpp \
    HorizontalAlignment.cpp \
    HtmlReportFormat.cpp \
    IBlockStyle.cpp \
    ICharacterStyle.cpp \
    ILinkStyle.cpp \
    IListStyle.cpp \
    IParagraphStyle.cpp \
    IReportTemplate.cpp \
    ISectionStyle.cpp \
    IStyle.cpp \
    ITableStyle.cpp \
    PageNumberPlacement.cpp \
    PageOrientation.cpp \
    PageSetup.cpp \
    Report.cpp \
    ReportBlockElement.cpp \
    ReportElement.cpp \
    ReportFlowElement.cpp \
    ReportFormatManager.cpp \
    ReportList.cpp \
    ReportListItem.cpp \
    ReportParagraph.cpp \
    ReportPicture.cpp \
    ReportSection.cpp \
    ReportSpanElement.cpp \
    ReportTemplateManager.cpp \
    ReportText.cpp \
    TypographicSize.cpp \
    TypographicUnit.cpp \
    UnderlineMode.cpp \
    VerticalAlignment.cpp

HEADERS += \
    API.hpp \
    BasicReportTemplate.hpp \
    Classes.hpp \
    Component.hpp \
    DataTypes.hpp \
    HtmlReportFormat.hpp \
    Linkage.hpp \
    Report.hpp \
    ReportFormat.hpp \
    ReportTemplate.hpp

PRECOMPILED_HEADER = API.hpp

LIBS += \
    -ltt3-gui$$TARGET_SUFFIX \
    -ltt3-ws$$TARGET_SUFFIX \
    -ltt3-db-api$$TARGET_SUFFIX \
    -ltt3-util$$TARGET_SUFFIX

RESOURCES += \
    tt3-report.qrc
