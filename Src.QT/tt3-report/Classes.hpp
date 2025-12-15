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
    class IReportTemplate;
    class IStyle;
    class ICharacterStyle;
    class IBlockStyle;
    class IParagraphStyle;
    class IListStyle;
    class ITableStyle;
    class ITableCellStyle;
    class ILinkStyle;
    class ISectionStyle;

    class BasicReportTemplate;
    class BasicStyle;
    class BasicCharacterStyle;
    class BasicBlockStyle;
    class BasicParagraphStyle;
    class BasicListStyle;
    class BasicTableStyle;
    class BasicTableCellStyle;
    class BasicLinkStyle;
    class BasicSectionStyle;

    class CustomReportTemplate;
    class CustomStyle;
    class CustomCharacterStyle;
    class CustomBlockStyle;
    class CustomParagraphStyle;
    class CustomListStyle;
    class CustomTableStyle;
    class CustomTableCellStyle;
    class CustomLinkStyle;
    class CustomSectionStyle;

    class Report;
    class ReportElement;
    class ReportFlowElement;
    class ReportBlockElement;
    class ReportSection;
    class ReportParagraph;
    class ReportSpanElement;
    class ReportText;
    class ReportPicture;
    class ReportList;
    class ReportListItem;
    class ReportTable;
    class ReportTableCell;
    class ReportAnchor;
    class ReportLink;
    class ReportInternalLink;
    class ReportExternalLink;
    class ReportTableOfContent;

    class IReportFormat;
    class HtmlReportFormat;
    
    class IReportType;
    
    //  Collections
    using ReportTemplates = QSet<IReportTemplate*>;
    using Styles = QSet<IStyle*>;
    using CharacterStyles = QSet<ICharacterStyle*>;
    using BlockStyles = QSet<IBlockStyle*>;
    using ParagraphStyles = QSet<IParagraphStyle*>;
    using ListStyles = QSet<IListStyle*>;
    using TableStyles = QSet<ITableStyle*>;
    using TableCellStyles = QSet<ITableCellStyle*>;
    using LinkStyles = QSet<ILinkStyle*>;
    using SectionStyles = QSet<ISectionStyle*>;

    using BasicReportTemplates = QSet<BasicReportTemplate*>;
    using BasicStyles = QSet<BasicStyle*>;
    using BasicCharacterStyles = QSet<BasicCharacterStyle*>;
    using BasicBlockStyles = QSet<BasicBlockStyle*>;
    using BasicParagraphStyles = QSet<BasicParagraphStyle*>;
    using BasicListStyles = QSet<BasicListStyle*>;
    using BasicTableStyles = QSet<BasicTableStyle*>;
    using BasicTableCellStyles = QSet<BasicTableCellStyle*>;
    using BasicLinkStyles = QSet<BasicLinkStyle*>;
    using BasicSectionStyles = QSet<BasicSectionStyle*>;

    using CustomReportTemplates = QSet<CustomReportTemplate*>;
    using CustomStyles = QSet<CustomStyle*>;
    using CustomCharacterStyles = QSet<CustomCharacterStyle*>;
    using CustomBlockStyles = QSet<CustomBlockStyle*>;
    using CustomParagraphStyles = QSet<CustomParagraphStyle*>;
    using CustomListStyles = QSet<CustomListStyle*>;
    using CustomTableStyles = QSet<CustomTableStyle*>;
    using CustomTableCellStyles = QSet<CustomTableCellStyle*>;
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
