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

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs a custom report template by analyzing
        ///     the specified XML string.
        /// \param xml
        ///     The XML string to analyze.
        /// \exception Exception
        ///     If an error occurs.
        explicit CustomReportTemplate(const QString & xml);

        /// \brief
        ///     Constructs a custom report template by analyzing
        ///     the specified XML DOM document.
        /// \param document
        ///     The XML document to analyze.
        /// \exception Exception
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
        virtual auto    findStyleByName(
                                const Mnemonic & name
                            ) const -> IStyle * override;

        //////////
        //  Implementation
    private:
        Mnemonic                _mnemonic;
        QMap<QLocale, QString>  _displayNames;
        PageSetup               _pageSetup;                 //  never null
        FontSpecs               _defaultFontSpecs;      //  never null; unmodifiable list
        TypographicSize         _defaultFontSize;           //  never null
        FontStyle               _defaultFontStyle;          //  java.awt.Font - style bit mask
        ColorSpec               _defaultTextColor;          //  never null
        ColorSpec               _defaultBackgroundColor;    //  never null
        TypographicSize         _defaultListIndent;         //  never null
        BorderType              _defaultTableBorderType;    //  never null
        BorderType              _defaultCellBorderType;    //  never null
        UnderlineMode           _defaultLinkUnderlineMode;  //  never null
        PageNumberPlacement     _defaultPageNumberPlacement;

        QMap<Mnemonic, CustomStyle*>_styles; //  all styles "owned" by this report template

        //  Helpers
        void            _construct(const QDomDocument & document);
    };

    /// \class CustomStyle tt3-report/API.hpp
    /// \brief A custom (loaded from XML file) stylw.
    class TT3_REPORT_PUBLIC CustomStyle
        :   public virtual IStyle
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CustomStyle)

        friend class CustomReportTemplate;

        //////////
        //  Construction/destruction - from friends only
    private:
        CustomStyle(
                CustomReportTemplate * reportTemplate,
                const Mnemonic & name,
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
        virtual auto    name() const -> Mnemonic override;
        virtual auto    fontSpecs() const -> FontSpecsOpt override;
        virtual auto    fontSize() const -> TypographicSizeOpt override;
        virtual auto    fontStyle() const -> FontStyleOpt override;
        virtual auto    textColor() const -> ColorSpecOpt override;
        virtual auto    backgroundColor() const -> ColorSpecOpt override;
        virtual auto    underlineMode() const -> UnderlineModeOpt override;

        //////////
        //  Implementation
    private:
        CustomReportTemplate *const _reportTemplate;
        const Mnemonic              _name;
        const FontSpecsOpt          _fontSpecs;
        const TypographicSizeOpt    _fontSize;
        const FontStyleOpt          _fontStyle;
        const ColorSpecOpt          _textColor;
        const ColorSpecOpt          _backgroundColor;
        const UnderlineModeOpt      _underlineMode;
    };
}

//  End of tt3-report/CustomReportTemplate.hpp
