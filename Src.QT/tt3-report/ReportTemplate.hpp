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
        friend class IStyle;
        friend class IBlockStyle;
        friend class IParagraphStyle;
        friend class IListStyle;
        friend class ITableStyle;
        friend class ITableCellStyle;
        friend class ISectionStyle;
        friend class BasicReportTemplate;
        friend class CustomReportTemplate;
        friend class Report;

        //////////
        //  Constants
    public:
        /// \brief
        ///     The preferred extension for [custom] report template files.
        inline static const QString PreferredExtension = ".tt3-rpt";

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

        /// \brief
        ///     Returns the mnemonic identifier of this report template.
        /// \return
        ///     The mnemonic identifier of this report template.
        virtual auto    mnemonic() const -> Mnemonic = 0;

        /// \brief
        ///     Returns the user-readable display name of this report template.
        /// \return
        ///     The user-readable display name of this report template
        ///     for the current default locale.
        virtual auto    displayName() const -> QString = 0;

        /// \brief
        ///     Returns the small (16x16) icon representing this report template.
        /// \return
        ///     The small (16x16) icon representing this report template.
        virtual QIcon       smallIcon() const;

        /// \brief
        ///     Returns the large (32x32) icon representing this report template.
        /// \return
        ///     The large (32x32) icon representing this report template.
        virtual QIcon       largeIcon() const;


        /// \brief
        ///     Returns the page setup of this report template.
        /// \return
        ///     The page setup of this report template.
        virtual auto    pageSetup() const -> PageSetup = 0;

        /// \brief
        ///     Returns the prioritized list of font specs used by
        ///     this report template.
        /// \details
        ///     For portability, it is recommended that the last element
        ///     of the font specs list is a generic font family name
        ///     (such as Serif, etc.)
        /// \return
        ///     The prioritized list of font specs used by
        ///     this report template with the best choice first,
        ///     worst choice last.
        virtual auto    defaultFontSpecs() const -> FontSpecs = 0;

        /// \brief
        ///     Returns the default report font size.
        /// \return
        ///     The default report font size.
        virtual auto    defaultFontSize() const -> TypographicSize = 0;

        /// \brief
        ///     Returns the default report font style.
        /// \return
        ///     The default report font style.
        virtual auto    defaultFontStyle() const -> FontStyle = 0;

        /// \brief
        ///     Returns the default report text color.
        /// \return
        ///     The default report text color.
        virtual auto    defaultTextColor() const -> ColorSpec = 0;

        /// \brief
        ///     Returns the default report background color.
        /// \return
        ///     The default report background color.
        virtual auto    defaultBackgroundColor() const -> ColorSpec = 0;

        /// \brief
        ///     Returns the default list indent for the report.
        /// \return
        ///     The default list indent for the report.
        virtual auto    defaultListIndent() const -> TypographicSize = 0;

        /// \brief
        ///     Returns the default table border type for the report.
        /// \return
        ///     The default table border type for the report.
        virtual auto    defaultTableBorderType() const -> BorderType = 0;

        /// \brief
        ///     Returns the default table cell border type for the report.
        /// \return
        ///     The default table cell border type for the report.
        virtual auto    defaultCellBorderType() const -> BorderType = 0;

        /// \brief
        ///     Returns the default link underline type for the report.
        /// \return
        ///     The default link underline type for the report.
        virtual auto    defaultLinkUnderlineMode() const -> UnderlineMode = 0;

        /// \brief
        ///     Returns the default page number placement for the report.
        /// \return
        ///     The default page number placement for the report.
        virtual auto    defaultPageNumberPlacement() const -> PageNumberPlacement = 0;

        /// \brief
        ///     Returns the set of all styles in this report template.
        /// \return
        ///     The set of all styles in this report template.
        virtual auto    styles() const -> Styles = 0;

        /// \brief
        ///     Returns the set of all characterstyles in this report template.
        /// \return
        ///     The set of all characterstyles in this report template.
        virtual auto    characterStyles() const -> CharacterStyles;

        /// \brief
        ///     Returns the set of all block styles in this report template.
        /// \return
        ///     The set of all block styles in this report template.
        virtual auto    blockStyles() const -> BlockStyles;

        /// \brief
        ///     Returns the set of all paragraph styles in this report template.
        /// \return
        ///     The set of all paragraph styles in this report template.
        virtual auto    paragraphStyles() const -> ParagraphStyles;

        /// \brief
        ///     Returns the set of all list styles in this report template.
        /// \return
        ///     The set of all list styles in this report template.
        virtual auto    listStyles() const -> ListStyles;

        /// \brief
        ///     Returns the set of all table styles in this report template.
        /// \return
        ///     The set of all table styles in this report template.
        virtual auto    tableStyles() const -> TableStyles;

        /// \brief
        ///     Returns the set of all table cell styles in this report template.
        /// \return
        ///     The set of all table cell styles in this report template.
        virtual auto    tableCellStyles() const -> TableCellStyles;

        /// \brief
        ///     Returns the set of all link styles in this report template.
        /// \return
        ///     The set of all link styles in this report template.
        virtual auto    linkStyles() const -> LinkStyles;

        /// \brief
        ///     Returns the set of all section styles in this report template.
        /// \return
        ///     The set of all section styles in this report template.
        virtual auto    sectionStyles() const -> SectionStyles;

        /// \brief
        ///     Finds a style defined in this report template by name.
        /// \param name
        ///     The name to look for.
        /// \return
        ///     The style with the required name; nullptr
        ///     if not found.
        virtual auto    findStyle(
                                const Mnemonic & name
                            ) const -> IStyle * = 0;

        /// \brief
        ///     Gets a style defined in this report template by name.
        /// \param name
        ///     The name to look for.
        /// \return
        ///     The style with the required name.
        /// \exception ReportException
        ///     If the style does not exist.
        virtual auto    style(
                                const Mnemonic & name
                            ) const -> IStyle *;

        /// \brief
        ///     Finds a character style defined in this report template by name.
        /// \param name
        ///     The name to look for.
        /// \return
        ///     The character style with the required name; nullptr
        ///     if not found or not a character style.
        virtual auto    findCharacterStyle(
                                const Mnemonic & name
                            ) const -> ICharacterStyle *;

        /// \brief
        ///     Gets a character style defined in this report template by name.
        /// \param name
        ///     The name to look for.
        /// \return
        ///     The character style with the required name.
        /// \exception ReportException
        ///     If the style does not exist or is not a
        ///     character style.
        virtual auto    characterStyle(
                                const Mnemonic & name
                            ) const -> ICharacterStyle *;

        /// \brief
        ///     Finds a block style defined in this report template by name.
        /// \param name
        ///     The name to look for.
        /// \return
        ///     The block style with the required name; nullptr
        ///     if not found or not a block style.
        virtual auto    findBlockStyle(
                                const Mnemonic & name
                            ) const -> IBlockStyle *;

        /// \brief
        ///     Gets a block style defined in this report template by name.
        /// \param name
        ///     The name to look for.
        /// \return
        ///     The block style with the required name.
        /// \exception ReportException
        ///     If the style does not exist or is not a
        ///     block style.
        virtual auto    blockStyle(
                                const Mnemonic & name
                            ) const -> IBlockStyle *;

        /// \brief
        ///     Finds a paragraph style defined in this report template by name.
        /// \param name
        ///     The name to look for.
        /// \return
        ///     The style with the required name; nullptr
        ///     if not found or not a paragraph style.
        virtual auto    findParagraphStyle(
                                const Mnemonic & name
                            ) const -> IParagraphStyle *;

        /// \brief
        ///     Gets a paragraph style defined in this report template by name.
        /// \param name
        ///     The name to look for.
        /// \return
        ///     The paragraph style with the required name.
        /// \exception ReportException
        ///     If the style does not exist or is not a
        ///     paragraph style.
        virtual auto    paragraphStyle(
                                const Mnemonic & name
                            ) const -> IParagraphStyle *;

        /// \brief
        ///     Finds a list style defined in this report template by name.
        /// \param name
        ///     The name to look for.
        /// \return
        ///     The list style with the required name; nullptr
        ///     if not found or not a list style.
        virtual auto    findListStyle(
                                const Mnemonic & name
                            ) const -> IListStyle *;

        /// \brief
        ///     Gets a list style defined in this report template by name.
        /// \param name
        ///     The name to look for.
        /// \return
        ///     The list list style with the required name.
        /// \exception ReportException
        ///     If the style does not exist or is not a
        ///     list style.
        virtual auto    listStyle(
                                const Mnemonic & name
                            ) const -> IListStyle *;

        /// \brief
        ///     Finds a table style defined in this report template by name.
        /// \param name
        ///     The name to look for.
        /// \return
        ///     The table style with the required name; nullptr
        ///     if not found or not a table style.
        virtual auto    findTableStyle(
                                const Mnemonic & name
                            ) const -> ITableStyle *;

        /// \brief
        ///     Gets a table style defined in this report template by name.
        /// \param name
        ///     The name to look for.
        /// \return
        ///     The table style with the required name.
        /// \exception ReportException
        ///     If the style does not exist or is not a
        ///     table style.
        virtual auto    tableStyle(
                                const Mnemonic & name
                            ) const -> ITableStyle *;

        /// \brief
        ///     Finds a table cell style defined in this report template by name.
        /// \param name
        ///     The name to look for.
        /// \return
        ///     The table cell style with the required name; nullptr
        ///     if not found or not a table cell style.
        virtual auto    findTableCellStyle(
                                const Mnemonic & name
                            ) const -> ITableCellStyle *;

        /// \brief
        ///     Gets a table cell style defined in this report template by name.
        /// \param name
        ///     The name to look for.
        /// \return
        ///     The table style with the required name.
        /// \exception ReportException
        ///     If the style does not exist or is not a
        ///     table cell style.
        virtual auto    tableCellStyle(
                                const Mnemonic & name
                            ) const -> ITableCellStyle *;

        /// \brief
        ///     Finds a link style defined in this report template by name.
        /// \param name
        ///     The name to look for.
        /// \return
        ///     The link style with the required name; nullptr
        ///     if not found or not a link style.
        virtual auto    findLinkStyle(
                                const Mnemonic & name
                            ) const -> ILinkStyle *;

        /// \brief
        ///     Gets a link style defined in this report template by name.
        /// \param name
        ///     The name to look for.
        /// \return
        ///     The link style with the required name.
        /// \exception ReportException
        ///     If the style does not exist or is not a
        ///     link style.
        virtual auto    linkStyle(
                                const Mnemonic & name
                            ) const -> ILinkStyle *;

        /// \brief
        ///     Finds a section style defined in this report template by name.
        /// \param name
        ///     The name to look for.
        /// \return
        ///     The section style with the required name; nullptr
        ///     if not found or not a section style.
        virtual auto    findSectionStyle(
                                const Mnemonic & name
                            ) const -> ISectionStyle *;

        /// \brief
        ///     Gets a section style defined in this report template by name.
        /// \param name
        ///     The name to look for.
        /// \return
        ///     The section style with the required name.
        /// \exception ReportException
        ///     If the style does not exist or is not a
        ///     section style.
        virtual auto    sectionStyle(
                                const Mnemonic & name
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

        /// \brief
        ///     Checks this report template for validity.
        /// \details
        ///     The main criterion is that all mandatory styles
        ///     are defined. Derived classes may implement extra
        ///     checks, but must always call the base version.
        /// \exception ReportException
        ///     If this report templais invalid.
        virtual void    validate() const;

        /// \brief
        ///     Creates a new report, containing only elements
        ///     specific to this report template.
        /// \details
        ///     -   The report will have one or more sections
        ///         with what content is deemed appropriate.
        ///     -   One of them will be a "body" section, where
        ///         the concrete report shall create dynamic content.
        ///     -   The report will have a TOC if the report template
        ///         demands it. The same goes for title and subtitle,
        ///         althugh the report generator will normally choose
        ///         to alter the content of these paragraphs.
        ///     -   The caller is reponsible for "delete"ing the
        ///         returned Report instance when done with it.
        /// \return
        ///     The newly created Report.
        virtual Report *createNewReport() const = 0;

        //////////
        //  Serialization
    public:
        /// \brief
        ///     The XML serialization format version.
        inline static const QString FormatVersion = "1";

        /// \brief
        ///     The tag name for an XML element representing
        ///     the report template itself in an XML document.
        inline static const QString XmlTagName = "ReportTemplate";

        /// \brief
        ///     Serializes this report template by setting attributes
        ///     and children of the specified element as necessary.
        /// \param element
        ///     The XML DOM element to serialize to.
        virtual void    serialize(QDomElement & element) const;

        //////////
        //  Implementation
    private:
        //  Reports modify this when instances are created/destroyed
        mutable qsizetype   _referenceCount = 0;    //  no. of report instances that use this ReportTemplate

        //  Helpers
        template <class T>
        static void     _setAttribute(
                                QDomElement & element,
                                QString name,
                                const T & value
                            )
        {
            QDomElement childElement = element.ownerDocument().createElement(name);
            element.appendChild(childElement);
            QDomText text = element.ownerDocument().createTextNode(tt3::util::toString(value));
            childElement.appendChild(text);
        }

        template <class T>
        static void     _setAttribute(
                QDomElement & element,
                QString name,
                const std::optional<T> & value
            )
        {
            if (value.has_value())
            {
                _setAttribute(element, name, value.value());
            }
        }
    };

    /// \class ReportTemplateManager tt3-report/API.hpp
    /// \brief The manager of known report templates.
    class TT3_REPORT_PUBLIC ReportTemplateManager final
    {
        TT3_UTILITY_CLASS(ReportTemplateManager)

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the set of all registered ReportTemplates.
        /// \return
        ///     The set of all registered ReportTemplates.
        static auto     all() -> ReportTemplates;

        /// \brief
        ///     Registers the specified ReportTemplate.
        /// \param reportTemplate
        ///     The ReportTemplate to register
        /// \return
        ///     True on success, false on failure. Repeated
        ///     registration of the same ReportTemplate is treated
        ///     as a "success".
        static bool     register(IReportTemplate * reportTemplate);

        /// \brief
        ///     Un-registers the specified ReportTemplate.
        /// \param reportTemplate
        ///     The ReportTemplate to un-register
        /// \return
        ///     True on success, false on failure.
        static bool     unregister(IReportTemplate * reportTemplate);

        /// \brief
        ///     Finds a registered ReportTemplate by its mnemonic.
        /// \param mnemonic
        ///     The mnemonic to look for.
        /// \return
        ///     The registered ReportTemplate with the required mnemonic;
        ///     nullptr if not found.
        static auto     find(const tt3::util::Mnemonic & mnemonic) -> IReportTemplate *;

        //////////
        //  Implementation
    private:
        struct _Impl;

        //  Helpers
        static _Impl *      _impl();
    };

    /// \class IStyle tt3-report/API.hpp
    /// \brief An abstract interface to a "formatting style" of report element.
    class TT3_REPORT_PUBLIC IStyle
    {
        friend class IReportTemplate;
        friend class ICharacterStyle;
        friend class IBlockStyle;
        friend class ILinkStyle;
        friend class ISectionStyle;

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
        /// \brief A type alias to improve code readability.
        using Name = tt3::util::Mnemonic;

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
        virtual Name    name() const = 0;

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

        //////////
        //  Serialization
    public:
        /// \brief
        ///     Returns the tag name for an XML element representing
        ///     the report template itself in an XML document.
        /// \return
        ///     The tag name for an XML element representing
        ///     the report template itself in an XML document.
        virtual QString xmlTagName() const = 0;

        /// \brief
        ///     Serializes this style by setting attributes
        ///     and children of the specified element as necessary.
        /// \param element
        ///     The XML DOM element to serialize to.
        virtual void    serialize(QDomElement & element) const;
    };

    /// \class ICharacterStyle tt3-report/API.hpp
    /// \brief An abstract interface to a "character formatting style" of report element.
    class TT3_REPORT_PUBLIC ICharacterStyle
        :   public virtual IStyle
    {
        //////////
        //  This is an interface
    protected:
        /// \brief
        ///     The default [interface] constructor.
        ICharacterStyle() = default;

        //////////
        //  Serialization
    public:
        virtual void    serialize(QDomElement & element) const override;
    };

    /// \class IBlockStyle tt3-report/API.hpp
    /// \brief An abstract interface to a "block character formatting style" of report element.
    class TT3_REPORT_PUBLIC IBlockStyle
        :   public virtual IStyle
    {
        friend class IParagraphStyle;
        friend class IListStyle;
        friend class ITableStyle;

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

        //////////
        //  Serialization
    public:
        virtual void    serialize(QDomElement & element) const override;
    };

    /// \class IParagraphStyle tt3-report/API.hpp
    /// \brief An abstract interface to a "paragraph character formatting style" of report element.
    class TT3_REPORT_PUBLIC IParagraphStyle
        :   public virtual IBlockStyle
    {
        friend class CustomReportTemplate;

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
        inline static const Name TitleStyleName = M(Paragraph.Title);

        /// \brief
        ///     The "document subtitle" paragraph style; must be defined by every valid report template.
        inline static const Name SubtitleStyleName = M(Paragraph.Subtitle);

        /// \brief
        ///     The default paragraph style; must be defined by every valid report template.
        inline static const Name DefaultStyleName = M(Paragraph.Default);

        /// \brief
        ///     The paragraph style for level 1 headings; must be defined by every valid report template.
        inline static const Name Heading1StyleName = M(Paragraph.Heading1);

        /// \brief
        ///     The paragraph style for level 2 headings; must be defined by every valid report template.
        inline static const Name Heading2StyleName = M(Paragraph.Heading2);

        /// \brief
        ///     The paragraph style for level 3 headings; must be defined by every valid report template.
        inline static const Name Heading3StyleName = M(Paragraph.Heading3);

        /// \brief
        ///     The paragraph style for level 4 headings; must be defined by every valid report template.
        inline static const Name Heading4StyleName = M(Paragraph.Heading4);

        /// \brief
        ///     The paragraph style for level 5 headings; must be defined by every valid report template.
        inline static const Name Heading5StyleName = M(Paragraph.Heading5);

        /// \brief
        ///     The paragraph style for level 6 headings; must be defined by every valid report template.
        inline static const Name Heading6StyleName = M(Paragraph.Heading6);

        /// \brief
        ///     The paragraph style for level 7 headings; must be defined by every valid report template.
        inline static const Name Heading7StyleName = M(Paragraph.Heading7);

        /// \brief
        ///     The paragraph style for level 8 headings; must be defined by every valid report template.
        inline static const Name Heading8StyleName = M(Paragraph.Heading8);

        /// \brief
        ///     The paragraph style for level 9 headings; must be defined by every valid report template.
        inline static const Name Heading9StyleName = M(Paragraph.Heading9);

        /// \brief
        ///     The paragraph style for level 1 table-of-content entries; must be defined by every valid report template.
        inline static const Name Toc1StyleName = M(Paragraph.Toc1);

        /// \brief
        ///     The paragraph style for level 2 table-of-content entries; must be defined by every valid report template.
        inline static const Name Toc2StyleName = M(Paragraph.Toc2);

        /// \brief
        ///     The paragraph style for level 3 table-of-content entries; must be defined by every valid report template.
        inline static const Name Toc3StyleName = M(Paragraph.Toc3);

        /// \brief
        ///     The paragraph style for level 4 table-of-content entries; must be defined by every valid report template.
        inline static const Name Toc4StyleName = M(Paragraph.Toc4);

        /// \brief
        ///     The paragraph style for level 5 table-of-content entries; must be defined by every valid report template.
        inline static const Name Toc5StyleName = M(Paragraph.Toc5);

        /// \brief
        ///     The paragraph style for level 6 table-of-content entries; must be defined by every valid report template.
        inline static const Name Toc6StyleName = M(Paragraph.Toc6);

        /// \brief
        ///     The paragraph style for level 7 table-of-content entries; must be defined by every valid report template.
        inline static const Name Toc7StyleName = M(Paragraph.Toc7);

        /// \brief
        ///     The paragraph style for level 8 table-of-content entries; must be defined by every valid report template.
        inline static const Name Toc8StyleName = M(Paragraph.Toc8);

        /// \brief
        ///     The paragraph style for level 9 table-of-content entries; must be defined by every valid report template.
        inline static const Name Toc9StyleName = M(Paragraph.Toc9);

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

        //////////
        //  Serialization
    public:
        /// \brief
        ///     The tag name for an XML element representing
        ///     this style in an XML document.
        inline static const QString XmlTagName = "ParagraphStyle";

        virtual QString xmlTagName() const override { return XmlTagName; }
        virtual void    serialize(QDomElement & element) const override;
    };

    /// \class IListStyle tt3-report/API.hpp
    /// \brief An abstract interface to a "list character formatting style" of report element.
    class TT3_REPORT_PUBLIC IListStyle
        :   public virtual IBlockStyle
    {
        friend class CustomReportTemplate;

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
        inline static const Name DefaultStyleName = M(List.Default);

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

        //////////
        //  Serialization
    public:
        /// \brief
        ///     The tag name for an XML element representing
        ///     this style in an XML document.
        inline static const QString XmlTagName = "ListStyle";

        virtual QString xmlTagName() const override { return XmlTagName; }
        virtual void    serialize(QDomElement & element) const override;
    };

    /// \class ITableStyle tt3-report/API.hpp
    /// \brief An abstract interface to a "table character formatting style" of report element.
    class TT3_REPORT_PUBLIC ITableStyle
        :   public virtual IBlockStyle
    {
        friend class CustomReportTemplate;

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
        inline static const Name DefaultStyleName = M(Table.Default);

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the type of border around the table.
        /// \return
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

        //////////
        //  Serialization
    public:
        /// \brief
        ///     The tag name for an XML element representing
        ///     this style in an XML document.
        inline static const QString XmlTagName = "TableStyle";

        virtual QString xmlTagName() const override { return XmlTagName; }
        virtual void    serialize(QDomElement & element) const override;
    };

    /// \class ITableCellStyle tt3-report/API.hpp
    /// \brief An abstract interface to a "table cell formatting style" of report element.
    class TT3_REPORT_PUBLIC ITableCellStyle
        :   public virtual IStyle
    {
        friend class CustomReportTemplate;

        //////////
        //  This is an interface
    protected:
        /// \brief
        ///     The default [interface] constructor.
        ITableCellStyle() = default;

        //////////
        //  Constants
    public:
        /// \brief
        ///     The table heading cell style; must be defined by every valid report template.
        inline static const Name HeadingStyleName = M(TableCell.Heading);

        /// \brief
        ///     The default table cell style; must be defined by every valid report template.
        inline static const Name DefaultStyleName = M(TableCell.Default);

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the horizontal alignment of the
        ///     content within the cell.
        /// \return
        ///     The horizontal alignment of the content
        ///     within the cell; no value == inherit from parent,
        ///     Default == inherit from ReportTemplate.
        virtual auto    horizontalAlignment() const -> HorizontalAlignmentOpt = 0;

        /// \brief
        ///     Returns the vertical alignment of the
        ///     content within the cell.
        /// \return
        ///     The vertical alignment of the content
        ///     within the cell; no value == inherit from parent,
        ///     Default == inherit from ReportTemplate.
        virtual auto    verticalAlignment() const -> VerticalAlignmentOpt = 0;

        //////////
        //  Serialization
    public:
        /// \brief
        ///     The tag name for an XML element representing
        ///     this style in an XML document.
        inline static const QString XmlTagName = "TableCellStyle";

        virtual QString xmlTagName() const override { return XmlTagName; }
        virtual void    serialize(QDomElement & element) const override;
    };

    /// \class ILinkStyle tt3-report/API.hpp
    /// \brief An abstract interface to a "link formatting style" of report element.
    class TT3_REPORT_PUBLIC ILinkStyle
        :   public virtual IStyle
    {
        friend class CustomReportTemplate;

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
        inline static const Name DefaultStyleName = M(Link.Default);

        //////////
        //  Serialization
    public:
        /// \brief
        ///     The tag name for an XML element representing
        ///     this style in an XML document.
        inline static const QString XmlTagName = "LinkStyle";

        virtual QString xmlTagName() const override { return XmlTagName; }
        virtual void    serialize(QDomElement & element) const override;
    };

    /// \class ISectionStyle tt3-report/API.hpp
    /// \brief An abstract interface to a "section formatting style" of report element.
    class TT3_REPORT_PUBLIC ISectionStyle
        :   public virtual IStyle
    {
        friend class CustomReportTemplate;

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
        ///     The title section style; must be defined by every valid report template.
        /// \details
        ///     The "prequel" of a report is a section which appears in the report at the
        ///     very beginning.
        inline static const Name TitleStyleName = M(Section.Title);

        /// \brief
        ///     The prequel section style; must be defined by every valid report template.
        /// \details
        ///     The "prequel" of a report is a section which appears in the report BEFORE
        ///     the generated report body.
        inline static const Name PrequelStyleName = M(Section.Prequel);

        /// \brief
        ///     The body section style; must be defined by every valid report template.
        /// \details
        ///     The "body" of a report is a section where the report generator creates
        ///     actual content during report generation.
        inline static const Name BodyStyleName = M(Section.Body);

        /// \brief
        ///     The sequel section style; must be defined by every valid report template.
        /// \details
        ///     The "sequel" of a report is a section which appears in the report AFTER
        ///     the generated report body.
        inline static const Name SequelStyleName = M(Section.Sequel);

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

        //////////
        //  Serialization
    public:
        /// \brief
        ///     The tag name for an XML element representing
        ///     this style in an XML document.
        inline static const QString XmlTagName = "SectionStyle";

        virtual QString xmlTagName() const override { return XmlTagName; }
        virtual void    serialize(QDomElement & element) const override;
    };
}

//  End of tt3-report/ReportTemplate.hpp
