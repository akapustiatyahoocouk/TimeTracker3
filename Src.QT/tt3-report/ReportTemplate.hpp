//
//  tt3-report/ReportTemplate.hpp - tt3-report template API
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
    /// \class IReportTemplate tt3-report/API.hpp
    /// \brief
    ///     An abstract template that can be chosen to format a
    ///     printed report.
    /// \details
    ///     ReportTemplates are stock objects managed by a
    ///     ReportTemplateManager.
    class TT3_REPORT_PUBLIC IReportTemplate
    {
        //////////
        //  This is an interface
    protected:
        /// \brief
        ///     The default [interface] constructor.
        IReportTemplate() = default;

        /// \brief
        ///     The default [interface] destructor.
        virtual ~IReportTemplate() = default;

        //////////
        //  Operations
    public:
        /// \brief A type alias to improve code readability.
        using Mnemonic = tt3::util::Mnemonic;

        virtual auto    mnemonic() const -> Mnemonic = 0;
        virtual auto    displayName() const -> QString = 0;
        virtual auto    pageSetup() const -> PageSetup = 0;
        virtual auto    defaultFontSpecs() const -> FontSpecs = 0;
        virtual auto    defaultFontSize() const -> TypographicSize = 0;
        virtual auto    defaultFontStyle() const -> FontStyle = 0;
        virtual auto    defaultTextColor() const -> ColorSpec = 0;
        virtual auto    defaultBackgroundColor() const -> ColorSpec = 0;
        virtual auto    defaultListIndent() const -> TypographicSize = 0;
        virtual auto    defaultTableBorderType() const -> BorderType = 0;
        virtual auto    defaultLinkUnderlineMode() const -> UnderlineMode = 0;
        virtual auto    defaultPageNumberPlacement() const -> PageNumberPlacement = 0;

        virtual auto    styles() const -> Styles = 0;
        virtual auto    characterStyles() const -> CharacterStyles;
        virtual auto    blockStyles() const -> BlockStyles;
        virtual auto    paragraphStyles() const -> ParagraphStyles;
        virtual auto    listStyles() const -> ListStyles;
        virtual auto    tableStyles() const -> TableStyles;
        virtual auto    linkStyles() const -> LinkStyles;
        virtual auto    sectionStyles() const -> SectionStyles;

        virtual auto    findStyleByName(
                                const QString & name
                            ) const -> IStyle * = 0;
        virtual auto    findCharacterStyleByName(
                                const QString & name
                            ) const -> ICharacterStyle *;
        virtual auto    findBlockStyleByName(
                                const QString & name
                            ) const -> IBlockStyle *;
        virtual auto    findParagraphStyleByName(
                                const QString & name
                            ) const -> IParagraphStyle *;
        virtual auto    findListStyleByName(
                                const QString & name
                            ) const -> IListStyle *;
        virtual auto    findTableStyleByName(
                                const QString & name
                            ) const -> ITableStyle *;
        virtual auto    findLinkStyleByName(
                                const QString & name
                            ) const -> ILinkStyle *;
        virtual auto    findSectionStyleByName(
                                const QString & name
                            ) const -> ISectionStyle *;

        /// \brief
        ///     Returns the XML DOM document representing this report template.
        /// \return
        ///     The XML DOM document representing this report template.
        QDomDocument    toXmlDocument() const;

        /// \brief
        ///     Returns the formatted XML text representing this report template.
        /// \return
        ///     The formatted XML text representing this report template.
        QString         toXmlString() const;
    };

    /// \class IStyle tt3-report/API.hpp
    /// \brief An abstract interface to a "formatting style" of report eement.
    class TT3_REPORT_PUBLIC IStyle
    {
        //////////
        //  This is an interface
    protected:
        /// \brief
        ///     The default [interface] constructor.
        IStyle() = default;

        /// \brief
        ///     The default [interface] destructor.
        virtual ~IStyle() = default;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the ReportTemplate to which this Style belongs.
        /// \details
        ///     The Style is owned by the ReportTemplate and is deleted
        ///     when the ReportTemplate is deleted.
        /// \return
        ///     The ReportTemplate to which this Style belongs.
        virtual auto    reportTemplate() const -> IReportTemplate * = 0;

        /// \brief
        ///     Returns the name of this Style.
        /// \details
        ///     All Styles within a ReportTemplate must have distinct names.
        /// \return
        ///     The name of this Style.
        virtual QString name() const = 0;

        /// \brief
        ///     Returns the list of FontSpecs to use.
        /// \return
        ///     The list of FontSpecs to use; no value ==
        ///     inherit from parent, empty list == inherit
        ///     from ReportTemplate.
        virtual auto    fontSpecs() const -> FontSpecsOpt = 0;

        /// \brief
        ///     Returns the font size to use.
        /// \return
        ///     The font size to use; no value ==
        ///     inherit from parent.
        virtual auto    fontSize() const -> TypographicSizeOpt = 0;

        /// \brief
        ///     Returns the font style to use.
        /// \return
        ///     The font style to use; no value ==
        ///     inherit from parent.
        virtual auto    fontStyle() const -> FontStyleOpt = 0;

        /// \brief
        ///     Returns the text color to use.
        /// \return
        ///     The text color to use; no value == i
        ///     nherit from parent, Default == inherit
        ///     from ReportTemplate.
        virtual auto    textColor() const -> ColorSpecOpt = 0;

        /// \brief
        ///     Returns the background color to use.
        /// \return
        ///     The background color to use; no value == inherit
        ///     from parent, Default == inherit from ReportTemplate.
        virtual auto    backgroundColor() const -> ColorSpecOpt = 0;

        /// \brief
        ///     Returns the underline mode to use.
        /// \return
        ///     The underline mode to use; no value == inherit
        ///     from parent, Default == inherit from ReportTemplate.
        virtual auto    underlineMode() const -> UnderlineModeOpt = 0;
    };

    class TT3_REPORT_PUBLIC ICharacterStyle
        :   public virtual IStyle
    {
        //////////
        //  This is an interface
    protected:
        /// \brief
        ///     The default [interface] constructor.
        ICharacterStyle() = default;
    };

    class TT3_REPORT_PUBLIC IBlockStyle
        :   public virtual IStyle
    {
        //////////
        //  This is an interface
    protected:
        /// \brief
        ///     The default [interface] constructor.
        IBlockStyle() = default;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the left margin of the block (the distance between the left edge
        ///     of the containing block and the left edge of the block).
        /// \return
        ///     The left margin of the block; no value == inherit from parent.
        virtual auto    leftMargin() const -> TypographicSizeOpt = 0;

        /// \brief
        ///     Returns the left margin of the block (the distance between the left edge
        ///     of the block and the left edge of the containing block).
        /// \return
        ///     The left margin of the block; no value == inherit from parent.
        virtual auto    rightMargin() const -> TypographicSizeOpt = 0;

        /// \brief
        ///     Returns the vertical gap before the block.
        /// \return
        ///     The vertical gap before the block; no value == inherit from parent.
        virtual auto    gapAbove() const -> TypographicSizeOpt = 0;

        /// \brief
        ///     Returns the vertical after before the block.
        /// \return
        ///     The vertical gap after the block; no value == inherit from parent.
        virtual auto    gapBelow() const -> TypographicSizeOpt = 0;
    };

    class TT3_REPORT_PUBLIC IParagraphStyle
        :   public virtual IBlockStyle
    {
        //////////
        //  This is an interface
    protected:
        /// \brief
        ///     The default [interface] constructor.
        IParagraphStyle() = default;

        //////////
        //  Constants
    public:
        /// \brief
        ///     The "document title" paragraph style; must be defined by every valid report template.
        /// \details
        ///     When formatting the report, the "document title" paragraph style receives special
        ///     handling - any "section" of a "document" which has one or more paragraphs of that
        ///     style is considered a "title page" section and, if the section content is shorter
        ///     than the page length, it is vertically centred within the page.
        inline static const QString TitleStyleName = "Paragraph.Title";

        /// \brief
        ///     The "document subtitle" paragraph style; must be defined by every valid report template.
        inline static const QString SubtitleStyleName = "Paragraph.Subtitle";

        /// \brief
        ///     The default paragraph style; must be defined by every valid report template.
        inline static const QString DefaultStyleName = "Paragraph.Default";

        /// \brief
        ///     The paragraph style for level 1 headings; must be defined by every valid report template.
        inline static const QString Heading1StyleName = "Paragraph.Heading1";

        /// \brief
        ///     The paragraph style for level 2 headings; must be defined by every valid report template.
        inline static const QString Heading2StyleName = "Paragraph.Heading2";

        /// \brief
        ///     The paragraph style for level 3 headings; must be defined by every valid report template.
        inline static const QString Heading3StyleName = "Paragraph.Heading3";

        /// \brief
        ///     The paragraph style for level 4 headings; must be defined by every valid report template.
        inline static const QString Heading4StyleName = "Paragraph.Heading4";

        /// \brief
        ///     The paragraph style for level 5 headings; must be defined by every valid report template.
        inline static const QString Heading5StyleName = "Paragraph.Heading5";

        /// \brief
        ///     The paragraph style for level 6 headings; must be defined by every valid report template.
        inline static const QString Heading6StyleName = "Paragraph.Heading6";

        /// \brief
        ///     The paragraph style for level 7 headings; must be defined by every valid report template.
        inline static const QString Heading7StyleName = "Paragraph.Heading7";

        /// \brief
        ///     The paragraph style for level 8 headings; must be defined by every valid report template.
        inline static const QString Heading8StyleName = "Paragraph.Heading8";

        /// \brief
        ///     The paragraph style for level 9 headings; must be defined by every valid report template.
        inline static const QString Heading9StyleName = "Paragraph.Heading9";

        /// \brief
        ///     The paragraph style for level 1 table-of-content entries; must be defined by every valid report template.
        inline static const QString Toc1StyleName = "Paragraph.Toc1";

        /// \brief
        ///     The paragraph style for level 2 table-of-content entries; must be defined by every valid report template.
        inline static const QString Toc2StyleName = "Paragraph.Toc2";

        /// \brief
        ///     The paragraph style for level 3 table-of-content entries; must be defined by every valid report template.
        inline static const QString Toc3StyleName = "Paragraph.Toc3";

        /// \brief
        ///     The paragraph style for level 4 table-of-content entries; must be defined by every valid report template.
        inline static const QString Toc4StyleName = "Paragraph.Toc4";

        /// \brief
        ///     The paragraph style for level 5 table-of-content entries; must be defined by every valid report template.
        inline static const QString Toc5StyleName = "Paragraph.Toc5";

        /// \brief
        ///     The paragraph style for level 6 table-of-content entries; must be defined by every valid report template.
        inline static const QString Toc6StyleName = "Paragraph.Toc6";

        /// \brief
        ///     The paragraph style for level 7 table-of-content entries; must be defined by every valid report template.
        inline static const QString Toc7StyleName = "Paragraph.Toc7";

        /// \brief
        ///     The paragraph style for level 8 table-of-content entries; must be defined by every valid report template.
        inline static const QString Toc8StyleName = "Paragraph.Toc8";

        /// \brief
        ///     The paragraph style for level 9 table-of-content entries; must be defined by every valid report template.
        inline static const QString Toc9StyleName = "Paragraph.Toc9";

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the text alignment within the paragraph.
        /// \return
        ///     The text alignment within the paragraph; no value ==
        ///     inherit from parent, Default == inherit from ReportTemplate.
        virtual auto    textAlignment() const -> HorizontalAlignmentOpt = 0;

        /// \brief
        ///     Returns the type of border around the paragraph.
        /// \return
        ///     The type of border around the paragraph; no value ==
        ///     inherit from parent, Default == inherit from ReportTemplate.
        virtual auto    borderType() const -> BorderTypeOpt = 0;
    };

    class TT3_REPORT_PUBLIC IListStyle
        :   public virtual IBlockStyle
    {
        //////////
        //  This is an interface
    protected:
        /// \brief
        ///     The default [interface] constructor.
        IListStyle() = default;

        //////////
        //  Constants
    public:
        /// \brief
        ///     The default list style; must be defined by every valid report template.
        inline static const QString DefaultStyleName = "List.Default";

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the indent between the left edge of the
        ///     list and the left edge of list item texts.
        /// \return
        ///     The indent between the left edge of the list and
        ///     the left edge of list item texts; no value == inherit
        ///     from parent.
        virtual auto    indent() const -> TypographicSizeOpt = 0;

        /// \brief
        ///     Returns the resolved indent between the left edge
        ///     of the list and the left edge of list item texts.
        /// \return
        ///     If this style itself does not specify the indent, the
        ///     default list style from the same report template is
        ///     used, then the report template itself.
        /// \return
        ///     The resolved indent between the left edge of the
        ///     list and the left edge of list item texts.
        TypographicSize resolveIndent() const;
    };

    class TT3_REPORT_PUBLIC ITableStyle
        :   public virtual IBlockStyle
    {
        //////////
        //  This is an interface
    protected:
        /// \brief
        ///     The default [interface] constructor.
        ITableStyle() = default;

        //////////
        //  Constants
    public:
        /// \brief
        ///     The default table style; must be defined by every valid report template.
        inline static const QString DefaultStyleName = "Table.Default"; //$NON-NLS-1$

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the type of border around the table.
        /// \@return
        ///     The type of border around the table;
        ///     no value == inherit from parent,
        ///     Default == inherit from ReportTemplate.
        virtual auto    tableBorderType() const -> BorderTypeOpt = 0;

        /// \brief
        ///     Returns the type of border around the
        ///     table cells.
        /// \return
        ///     The type of border around the table cells;
        ///     mo value == inherit from parent, Default ==
        ///     inherit from ReportTemplate.
        virtual auto    cellBorderType() const -> BorderTypeOpt = 0;
    };

    class TT3_REPORT_PUBLIC ILinkStyle
        :   public virtual IStyle
    {
        //////////
        //  This is an interface
    protected:
        /// \brief
        ///     The default [interface] constructor.
        ILinkStyle() = default;

        //////////
        //  Constants
    public:
        /// \brief
        ///     The default link style; must be defined by every valid report template.
        inline static const QString DefaultStyleName = "Link.Default"; //$NON-NLS-1$
    };

    class TT3_REPORT_PUBLIC ISectionStyle
        :   public virtual IStyle
    {
        //////////
        //  This is an interface
    protected:
        /// \brief
        ///     The default [interface] constructor.
        ISectionStyle() = default;

        //////////
        //  Constants
    public:
        /// \brief
        ///     The default section style; must be defined by every valid report template.
        inline static const QString DefaultStyleName = "Section.Default"; //$NON-NLS-1$

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the placement rule for page numbers for
        ///     pages in a section.
        /// \return
        ///     The placement rule for page numbers for pages in
        ///     a section; no value == inherit from parent,
        ///     Default == inherit from ReportTemplate.
        virtual auto    pageNumberPlacement() const -> PageNumberPlacementOpt = 0;
    };
}

//  End of tt3-report/ReportTemplate.hpp
