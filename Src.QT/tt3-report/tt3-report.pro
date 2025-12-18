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
    BasicTableCellStyle.cpp \
    BasicTableStyle.cpp \
    BorderType.cpp \
    ColorSpec.cpp \
    Component.cpp \
    CreateReportDialog.cpp \
    CustomBlockStyle.cpp \
    CustomCharacterStyle.cpp \
    CustomLinkStyle.cpp \
    CustomListStyle.cpp \
    CustomParagraphStyle.cpp \
    CustomReportTemplate.cpp \
    CustomSectionStyle.cpp \
    CustomStyle.cpp \
    CustomTableCellStyle.cpp \
    CustomTableStyle.cpp \
    Exceptions.cpp \
    FontFlag.cpp \
    FontSpec.cpp \
    FontSpecs.cpp \
    FontStyle.cpp \
    HorizontalAlignment.cpp \
    HtmlReportFormat._CssBuilder.cpp \
    HtmlReportFormat._HtmlBuilder.cpp \
    HtmlReportFormat._HtmlGenerator.cpp \
    HtmlReportFormat.cpp \
    IBlockStyle.cpp \
    ICharacterStyle.cpp \
    ILinkStyle.cpp \
    IListStyle.cpp \
    IParagraphStyle.cpp \
    IReportTemplate.cpp \
    ISectionStyle.cpp \
    IStyle.cpp \
    ITableCellStyle.cpp \
    ITableStyle.cpp \
    KnownCustomReportTemplate.cpp \
    ManageReportTemplatesDialog.cpp \
    PageNumberPlacement.cpp \
    PageOrientation.cpp \
    PageSetup.cpp \
    Report.cpp \
    ReportAnchor.cpp \
    ReportBlockElement.cpp \
    ReportConfigurationEditor.cpp \
    ReportElement.cpp \
    ReportExternalLink.cpp \
    ReportFlowElement.cpp \
    ReportFormatManager.cpp \
    ReportInternalLink.cpp \
    ReportLink.cpp \
    ReportList.cpp \
    ReportListItem.cpp \
    ReportParagraph.cpp \
    ReportPicture.cpp \
    ReportSection.cpp \
    ReportSpanElement.cpp \
    ReportTable.cpp \
    ReportTableCell.cpp \
    ReportTableOfContent.cpp \
    ReportTemplateManager.cpp \
    ReportTemplateManagerTool.cpp \
    ReportText.cpp \
    ReportTypeManager.cpp \
    TypographicSize.cpp \
    TypographicUnit.cpp \
    UnderlineMode.cpp \
    VerticalAlignment.cpp

HEADERS += \
    API.hpp \
    BasicReportTemplate.hpp \
    Classes.hpp \
    Component.hpp \
    CreateReportDialog.hpp \
    CustomReportTemplate.hpp \
    DataTypes.hpp \
    Exceptions.hpp \
    HtmlReportFormat.hpp \
    Linkage.hpp \
    ManageReportTemplatesDialog.hpp \
    Report.hpp \
    ReportConfiguration.hpp \
    ReportConfigurationEditor.hpp \
    ReportFormat.hpp \
    ReportTemplate.hpp \
    ReportTemplateManagerTool.hpp \
    ReportType.hpp

PRECOMPILED_HEADER = API.hpp

RESOURCES += \
    tt3-report.qrc

FORMS += \
    CreateReportDialog.ui \
    ManageReportTemplatesDialog.ui

LIBS += \
    -ltt3-gui$$TARGET_SUFFIX \
    -ltt3-ws$$TARGET_SUFFIX \
    -ltt3-db-api$$TARGET_SUFFIX \
    -ltt3-util$$TARGET_SUFFIX
