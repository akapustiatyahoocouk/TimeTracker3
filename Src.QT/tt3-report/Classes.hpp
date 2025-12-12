//
//  tt3-report/Classes.hpp - Forward declarations & typedefs.
//
//  TimeTracker3
//  Copyright (C) 2026, Andrey Kapustin
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//////////

namespace tt3::report
{
    class TT3_REPORT_PUBLIC IReportTemplate;
    class TT3_REPORT_PUBLIC IStyle;
    class TT3_REPORT_PUBLIC ICharacterStyle;
    class TT3_REPORT_PUBLIC IBlockStyle;
    class TT3_REPORT_PUBLIC IParagraphStyle;
    class TT3_REPORT_PUBLIC IListStyle;
    class TT3_REPORT_PUBLIC ITableStyle;
    class TT3_REPORT_PUBLIC ILinkStyle;
    class TT3_REPORT_PUBLIC ISectionStyle;

    class TT3_REPORT_PUBLIC BasicReportTemplate;
    class TT3_REPORT_PUBLIC BasicStyle;
    class TT3_REPORT_PUBLIC BasicCharacterStyle;
    class TT3_REPORT_PUBLIC BasicBlockStyle;
    class TT3_REPORT_PUBLIC BasicParagraphStyle;
    class TT3_REPORT_PUBLIC BasicListStyle;
    class TT3_REPORT_PUBLIC BasicTableStyle;
    class TT3_REPORT_PUBLIC BasicLinkStyle;
    class TT3_REPORT_PUBLIC BasicSectionStyle;

    class TT3_REPORT_PUBLIC CustomReportTemplate;
    class TT3_REPORT_PUBLIC CustomStyle;
    class TT3_REPORT_PUBLIC CustomCharacterStyle;
    class TT3_REPORT_PUBLIC CustomBlockStyle;
    class TT3_REPORT_PUBLIC CustomParagraphStyle;
    class TT3_REPORT_PUBLIC CustomListStyle;
    class TT3_REPORT_PUBLIC CustomTableStyle;
    class TT3_REPORT_PUBLIC CustomLinkStyle;
    class TT3_REPORT_PUBLIC CustomSectionStyle;

    class TT3_REPORT_PUBLIC Report;
    class TT3_REPORT_PUBLIC ReportElement;
    class TT3_REPORT_PUBLIC ReportFlowElement;
    class TT3_REPORT_PUBLIC ReportBlockElement;
    class TT3_REPORT_PUBLIC ReportSection;
    class TT3_REPORT_PUBLIC ReportParagraph;
    class TT3_REPORT_PUBLIC ReportSpanElement;
    class TT3_REPORT_PUBLIC ReportText;
    class TT3_REPORT_PUBLIC ReportPicture;
    class TT3_REPORT_PUBLIC ReportList;
    class TT3_REPORT_PUBLIC ReportListItem;
    class TT3_REPORT_PUBLIC ReportTable;
    class TT3_REPORT_PUBLIC ReportTableCell;
    class TT3_REPORT_PUBLIC ReportAnchor;
    class TT3_REPORT_PUBLIC ReportLink;
    class TT3_REPORT_PUBLIC ReportInternalLink;
    class TT3_REPORT_PUBLIC ReportExternalLink;
    class TT3_REPORT_PUBLIC ReportTableOfContent;

    class TT3_REPORT_PUBLIC IReportFormat;
    class TT3_REPORT_PUBLIC HtmlReportFormat;
    
    class TT3_REPORT_PUBLIC IReportType;
    
    //  Collections
    using ReportTemplates = QSet<IReportTemplate*>;
    using Styles = QSet<IStyle*>;
    using CharacterStyles = QSet<ICharacterStyle*>;
    using BlockStyles = QSet<IBlockStyle*>;
    using ParagraphStyles = QSet<IParagraphStyle*>;
    using ListStyles = QSet<IListStyle*>;
    using TableStyles = QSet<ITableStyle*>;
    using LinkStyles = QSet<ILinkStyle*>;
    using SectionStyles = QSet<ISectionStyle*>;

    using BasicReportTemplates = QSet<BasicReportTemplate*>;
    using BasicStyles = QSet<BasicStyle*>;
    using BasicCharacterStyles = QSet<BasicCharacterStyle*>;
    using BasicBlockStyles = QSet<BasicBlockStyle*>;
    using BasicParagraphStyles = QSet<BasicParagraphStyle*>;
    using BasicListStyles = QSet<BasicListStyle*>;
    using BasicTableStyles = QSet<BasicTableStyle*>;
    using BasicLinkStyles = QSet<BasicLinkStyle*>;
    using BasicSectionStyles = QSet<BasicSectionStyle*>;

    using CustomReportTemplates = QSet<CustomReportTemplate*>;
    using CustomStyles = QSet<CustomStyle*>;
    using CustomCharacterStyles = QSet<CustomCharacterStyle*>;
    using CustomBlockStyles = QSet<CustomBlockStyle*>;
    using CustomParagraphStyles = QSet<CustomParagraphStyle*>;
    using CustomListStyles = QSet<CustomListStyle*>;
    using CustomTableStyles = QSet<CustomTableStyle*>;
    using CustomLinkStyles = QSet<CustomLinkStyle*>;
    using CustomSectionStyles = QSet<CustomSectionStyle*>;

    using ReportSections = QList<ReportSection*>;
    using ReportBlockElements = QList<ReportBlockElement*>;
    using ReportSpanElements = QList<ReportSpanElement*>;
    using ReportListItems = QList<ReportListItem*>;
    using ReportTableCells = QList<ReportTableCell*>;
    using ReportAnchors = QList<ReportAnchor*>;
    using ReportLinks = QList<ReportLink*>;
    
    using ReportSectionsC = QList<const ReportSection*>;
    using ReportBlockElementsC = QList<const ReportBlockElement*>;
    using ReportSpanElementsC = QList<const ReportSpanElement*>;
    using ReportListItemsC = QList<const ReportListItem*>;
    using ReportTableCellsC = QList<const ReportTableCell*>;
    using ReportAnchorsC = QList<const ReportAnchor*>;
    using ReportLinksC = QList<const ReportLink*>;

    using ReportFormats = QSet<IReportFormat*>;
    using ReportTypes = QSet<IReportType*>;
}

//  End of tt3-report/Classes.hpp
