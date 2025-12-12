//
//  tt3-report/CustomReportTemplate.hpp - tt3-report external (XML file) report template
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
    /// \class CustomReportTemplate tt3-report/API.hpp
    /// \brief The report template stored externally in an XML file.
    class TT3_REPORT_PUBLIC CustomReportTemplate final
        :   public virtual IReportTemplate
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CustomReportTemplate)

        friend class CustomBlockStyle;
        friend class CustomStyle;
        friend class CustomParagraphStyle;
        friend class CustomListStyle;
        friend class CustomTableStyle;
        friend class CustomSectionStyle;

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs a custom report template by analyzing
        ///     the specified XML string.
        /// \param xml
        ///     The XML string to analyze.
        /// \exception ReportException
        ///     If an error occurs.
        explicit CustomReportTemplate(const QString & xml);

        /// \brief
        ///     Constructs a custom report template by analyzing
        ///     the specified XML DOM document.
        /// \param document
        ///     The XML document to analyze.
        /// \exception ReportException
        ///     If an error occurs.
        explicit CustomReportTemplate(const QDomDocument & document);

        /// \brief
        ///     The class destructor.
        virtual ~CustomReportTemplate();

        //////////
        //  IReportTemplate
    public:
        virtual auto    mnemonic() const -> Mnemonic override;
        virtual auto    displayName() const -> QString override;
        virtual auto    pageSetup() const -> PageSetup override;
        virtual auto    defaultFontSpecs() const -> FontSpecs override;
        virtual auto    defaultFontSize() const -> TypographicSize override;
        virtual auto    defaultFontStyle() const -> FontStyle override;
        virtual auto    defaultTextColor() const -> ColorSpec override;
        virtual auto    defaultBackgroundColor() const -> ColorSpec override;
        virtual auto    defaultListIndent() const -> TypographicSize override;
        virtual auto    defaultTableBorderType() const -> BorderType override;
        virtual auto    defaultCellBorderType() const -> BorderType override;
        virtual auto    defaultLinkUnderlineMode() const -> UnderlineMode override;
        virtual auto    defaultPageNumberPlacement() const -> PageNumberPlacement override;

        virtual auto    styles() const -> Styles override;
        virtual auto    findStyle(
                                const Mnemonic & name
                            ) const -> IStyle * override;

        virtual Report *createNewReport() const override;

        //////////
        //  Implementation
    private:
        Mnemonic            _mnemonic;
        QString             _displayName;
        PageSetup           _pageSetup;                 //  never null
        FontSpecs           _defaultFontSpecs;      //  never null; unmodifiable list
        TypographicSize     _defaultFontSize;           //  never null
        FontStyle           _defaultFontStyle;          //  java.awt.Font - style bit mask
        ColorSpec           _defaultTextColor;          //  never null
        ColorSpec           _defaultBackgroundColor;    //  never null
        TypographicSize     _defaultListIndent;         //  never null
        BorderType          _defaultTableBorderType;    //  never null
        BorderType          _defaultCellBorderType;    //  never null
        UnderlineMode       _defaultLinkUnderlineMode;  //  never null
        PageNumberPlacement _defaultPageNumberPlacement;

        QMap<Mnemonic, CustomStyle*>_styles; //  all styles "owned" by this report template

        QString         _emptyReportXml;

        //  Helpers
        void            _construct(const QDomDocument & document);
        void            _addStyle(CustomStyle * style);

        QDomElement     _findChildElement(
                                const QDomElement & parentElement,
                                const QString & childTagName
                            );
        QDomElement     _getChildElement(
                                const QDomElement & parentElement,
                                const QString & childTagName
                            );

        template<class T>
        void            _parseAttribute(
                                const QDomElement & element,
                                const QString & attributeName,
                                T & attributeValue
                            )
        {
            QDomElement e = _findChildElement(element, attributeName);
            if (!e.isNull())
            {
                QString text = e.text();
                qsizetype scan = 0;
                try
                {
                    attributeValue = tt3::util::fromString<T>(text, scan);
                }
                catch (tt3::util::Exception & ex)
                {   //  OOPS! Log & translate
                    qCritical() << ex;
                    throw InvalidReportTemplateException();
                }
                if (scan < text.length())
                {   //  OOPS!
                    throw InvalidReportTemplateException();
                }
            }
        }
        template<class T>
        void            _parseAttribute(
                                const QDomElement & element,
                                const QString & attributeName,
                                std::optional<T> & attributeValue
                            )
        {
            QDomElement e = _findChildElement(element, attributeName);
            if (!e.isNull())
            {
                QString text = e.text();
                qsizetype scan = 0;
                try
                {
                    attributeValue = tt3::util::fromString<T>(text, scan);
                }
                catch (tt3::util::Exception & ex)
                {   //  OOPS! Log & translate
                    qCritical() << ex;
                    throw InvalidReportTemplateException();
                }
                if (scan < text.length())
                {   //  OOPS!
                    throw InvalidReportTemplateException();
                }
            }
        }
    };

    /// \class CustomStyle tt3-report/API.hpp
    /// \brief A custom (loaded from XML file) stylw.
    class TT3_REPORT_PUBLIC CustomStyle
        :   public virtual IStyle
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CustomStyle)

        friend class CustomReportTemplate;
        friend class CustomCharacterStyle;
        friend class CustomBlockStyle;
        friend class CustomParagraphStyle;
        friend class CustomLinkStyle;
        friend class CustomTableStyle;
        friend class CustomListStyle;
        friend class CustomSectionStyle;

        //////////
        //  Construction/destruction - from friends only
    private:
        CustomStyle(
                CustomReportTemplate * reportTemplate,
                const Name & name,
                const FontSpecsOpt & fontSpecs,
                const TypographicSizeOpt & fontSize,
                const FontStyleOpt & fontStyle,
                const ColorSpecOpt & textColor,
                const ColorSpecOpt & backgroundColor,
                const UnderlineModeOpt & underlineMode
            );
        virtual ~CustomStyle();

        //////////
        //  IStyle
    public:
        virtual auto    reportTemplate() const -> IReportTemplate * override;
        virtual Name    name() const override;
        virtual auto    fontSpecs() const -> FontSpecsOpt override;
        virtual auto    fontSize() const -> TypographicSizeOpt override;
        virtual auto    fontStyle() const -> FontStyleOpt override;
        virtual auto    textColor() const -> ColorSpecOpt override;
        virtual auto    backgroundColor() const -> ColorSpecOpt override;
        virtual auto    underlineMode() const -> UnderlineModeOpt override;

        //////////
        //  Implementation
    private:
        CustomReportTemplate *_reportTemplate;
        Name                  _name;
        FontSpecsOpt          _fontSpecs;
        TypographicSizeOpt    _fontSize;
        FontStyleOpt          _fontStyle;
        ColorSpecOpt          _textColor;
        ColorSpecOpt          _backgroundColor;
        UnderlineModeOpt      _underlineMode;

        //////////
        //  Serialization
    private:
        explicit CustomStyle(
                CustomReportTemplate * reportTemplate
            );
        virtual void    _deserialize(const QDomElement & styleElement);
    };

    /// \class CustomCharacterStyle tt3-report/API.hpp
    /// \brief A custom (loaded from XML file) character stylw.
    class TT3_REPORT_PUBLIC CustomCharacterStyle
        :   public CustomStyle,
            public virtual ICharacterStyle
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CustomCharacterStyle)

        //////////
        //  Construction/destruction - from friends only
    private:
        CustomCharacterStyle(
                CustomReportTemplate * reportTemplate,
                const Name & name,
                const FontSpecsOpt & fontSpecs,
                const TypographicSizeOpt & fontSize,
                const FontStyleOpt & fontStyle,
                const ColorSpecOpt & textColor,
                const ColorSpecOpt & backgroundColor,
                const UnderlineModeOpt & underlineMode
            );
        virtual ~CustomCharacterStyle();

        //////////
        //  Serialization
    private:
        explicit CustomCharacterStyle(
                CustomReportTemplate * reportTemplate
            );
        virtual void    _deserialize(const QDomElement & styleElement) override;
    };

    /// \class CustomBlockStyle tt3-report/API.hpp
    /// \brief A custom (loaded from XML file) block stylw.
    class TT3_REPORT_PUBLIC CustomBlockStyle
        :   public CustomStyle,
            public virtual IBlockStyle
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CustomBlockStyle)

        friend class CustomParagraphStyle;
        friend class CustomListStyle;
        friend class CustomTableStyle;

        //////////
        //  Construction/destruction - from friends only
    private:
        CustomBlockStyle(
                CustomReportTemplate * reportTemplate,
                const Name & name,
                const FontSpecsOpt & fontSpecs,
                const TypographicSizeOpt & fontSize,
                const FontStyleOpt & fontStyle,
                const ColorSpecOpt & textColor,
                const ColorSpecOpt & backgroundColor,
                const UnderlineModeOpt & underlineMode,
                const TypographicSizeOpt & leftMargin,
                const TypographicSizeOpt & rightMargin,
                const TypographicSizeOpt & gapAbove,
                const TypographicSizeOpt & gapBelow
            );
        virtual ~CustomBlockStyle();

        //////////
        //  IBlockStyle
    public:
        virtual auto    leftMargin() const -> TypographicSizeOpt override;
        virtual auto    rightMargin() const -> TypographicSizeOpt override;
        virtual auto    gapAbove() const -> TypographicSizeOpt override;
        virtual auto    gapBelow() const -> TypographicSizeOpt override;

        //////////
        //  Implementation
    private:
        TypographicSizeOpt  _leftMargin;
        TypographicSizeOpt  _rightMargin;
        TypographicSizeOpt  _gapAbove;
        TypographicSizeOpt  _gapBelow;

        //////////
        //  Serialization
    private:
        explicit CustomBlockStyle(
                CustomReportTemplate * reportTemplate
            );
        virtual void    _deserialize(const QDomElement & styleElement) override;
    };

    /// \class CustomParagraphStyle tt3-report/API.hpp
    /// \brief A custom (loaded from XML file) paragraph stylw.
    class TT3_REPORT_PUBLIC CustomParagraphStyle
        :   public CustomBlockStyle,
            public virtual IParagraphStyle
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CustomParagraphStyle)

        friend class CustomReportTemplate;

        //////////
        //  Construction/destruction - from friends only
    private:
        CustomParagraphStyle(
                CustomReportTemplate * reportTemplate,
                const Name & name,
                const FontSpecsOpt & fontSpecs,
                const TypographicSizeOpt & fontSize,
                const FontStyleOpt & fontStyle,
                const ColorSpecOpt & textColor,
                const ColorSpecOpt & backgroundColor,
                const UnderlineModeOpt & underlineMode,
                const TypographicSizeOpt & leftMargin,
                const TypographicSizeOpt & rightMargin,
                const TypographicSizeOpt & gapAbove,
                const TypographicSizeOpt & gapBelow,
                const HorizontalAlignmentOpt & textAlignment,
                const BorderTypeOpt & borderType
            );
        virtual ~CustomParagraphStyle();

        //////////
        //  IParagraphStyle
    public:
        virtual auto    textAlignment() const -> HorizontalAlignmentOpt override;
        virtual auto    borderType() const -> BorderTypeOpt override;

        //////////
        //  Implementation
    private:
        HorizontalAlignmentOpt  _textAlignment;
        BorderTypeOpt           _borderType;

        //////////
        //  Serialization
    private:
        explicit CustomParagraphStyle(
                CustomReportTemplate * reportTemplate
            );
        virtual void    _deserialize(const QDomElement & styleElement) override;
    };

    /// \class CustomListStyle tt3-report/API.hpp
    /// \brief A custom (loaded from XML file) list stylw.
    class TT3_REPORT_PUBLIC CustomListStyle
        :   public CustomBlockStyle,
            public virtual IListStyle
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CustomListStyle)

        friend class CustomReportTemplate;

        //////////
        //  Construction/destruction - from friends only
    private:
        CustomListStyle(
                CustomReportTemplate * reportTemplate,
                const Name & name,
                const FontSpecsOpt & fontSpecs,
                const TypographicSizeOpt & fontSize,
                const FontStyleOpt & fontStyle,
                const ColorSpecOpt & textColor,
                const ColorSpecOpt & backgroundColor,
                const UnderlineModeOpt & underlineMode,
                const TypographicSizeOpt & leftMargin,
                const TypographicSizeOpt & rightMargin,
                const TypographicSizeOpt & gapAbove,
                const TypographicSizeOpt & gapBelow,
                const TypographicSizeOpt & indent
            );
        virtual ~CustomListStyle();

        //////////
        //  IListStyle
    public:
        virtual auto    indent() const -> TypographicSizeOpt override;

        //////////
        //  Implementation
    private:
        TypographicSizeOpt  _indent;

        //////////
        //  Serialization
    private:
        explicit CustomListStyle(
                CustomReportTemplate * reportTemplate
            );
        virtual void    _deserialize(const QDomElement & styleElement) override;
    };

    /// \class CustomTableStyle tt3-report/API.hpp
    /// \brief A custom (loaded from XML file) Table stylw.
    class TT3_REPORT_PUBLIC CustomTableStyle
        :   public CustomBlockStyle,
            public virtual ITableStyle
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CustomTableStyle)

        friend class CustomReportTemplate;

        //////////
        //  Construction/destruction - from friends only
    private:
        CustomTableStyle(
                CustomReportTemplate * reportTemplate,
                const Name & name,
                const FontSpecsOpt & fontSpecs,
                const TypographicSizeOpt & fontSize,
                const FontStyleOpt & fontStyle,
                const ColorSpecOpt & textColor,
                const ColorSpecOpt & backgroundColor,
                const UnderlineModeOpt & underlineMode,
                const TypographicSizeOpt & leftMargin,
                const TypographicSizeOpt & rightMargin,
                const TypographicSizeOpt & gapAbove,
                const TypographicSizeOpt & gapBelow,
                const BorderTypeOpt & tableBorderType,
                const BorderTypeOpt & cellBorderType
            );
        virtual ~CustomTableStyle();

        //////////
        //  ITableStyle
    public:
        virtual auto    tableBorderType() const -> BorderTypeOpt override;
        virtual auto    cellBorderType() const -> BorderTypeOpt override;

        //////////
        //  Implementation
    private:
        BorderTypeOpt   _tableBorderType;
        BorderTypeOpt   _cellBorderType;

        //////////
        //  Serialization
    private:
        explicit CustomTableStyle(
                CustomReportTemplate * reportTemplate
            );
        virtual void    _deserialize(const QDomElement & styleElement) override;
    };

    /// \class CustomLinkStyle tt3-report/API.hpp
    /// \brief A custom (loaded from XML file) link stylw.
    class TT3_REPORT_PUBLIC CustomLinkStyle
        :   public CustomStyle,
            public virtual ILinkStyle
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CustomLinkStyle)

        friend class CustomReportTemplate;

        //////////
        //  Construction/destruction - from friends only
    private:
        CustomLinkStyle(
                CustomReportTemplate * reportTemplate,
                const Name & name,
                const FontSpecsOpt & fontSpecs,
                const TypographicSizeOpt & fontSize,
                const FontStyleOpt & fontStyle,
                const ColorSpecOpt & textColor,
                const ColorSpecOpt & backgroundColor,
                const UnderlineModeOpt & underlineMode
            );
        virtual ~CustomLinkStyle();

        //////////
        //  Serialization
    private:
        explicit CustomLinkStyle(
                CustomReportTemplate * reportTemplate
            );
        virtual void    _deserialize(const QDomElement & styleElement) override;
    };

    /// \class CustomSectionStyle tt3-report/API.hpp
    /// \brief A custom (loaded from XML file) section stylw.
    class TT3_REPORT_PUBLIC CustomSectionStyle
        :   public CustomStyle,
            public virtual ISectionStyle
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CustomSectionStyle)

        friend class CustomReportTemplate;

        //////////
        //  Construction/destruction - from friends only
    private:
        CustomSectionStyle(
                CustomReportTemplate * reportTemplate,
                const Name & name,
                const FontSpecsOpt & fontSpecs,
                const TypographicSizeOpt & fontSize,
                const FontStyleOpt & fontStyle,
                const ColorSpecOpt & textColor,
                const ColorSpecOpt & backgroundColor,
                const UnderlineModeOpt & underlineMode,
                const PageNumberPlacementOpt & pageNumberPlacement
            );
        virtual ~CustomSectionStyle();

        //////////
        //  ISectionStyle
    public:
        virtual auto    pageNumberPlacement() const -> PageNumberPlacementOpt override;

        //////////
        //  Implementation
    private:
        PageNumberPlacementOpt  _pageNumberPlacement;

        //////////
        //  Serialization
    private:
        explicit CustomSectionStyle(
                CustomReportTemplate * reportTemplate
            );
        virtual void    _deserialize(const QDomElement & styleElement) override;
    };
}

//  End of tt3-report/CustomReportTemplate.hpp
