//
//  tt3-report/BasicReportTemplate.hpp - tt3-report basic report template
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
    /// \class BasicReportTemplate tt3-report/API.hpp
    /// \brief The predefined basic report template.
    class TT3_REPORT_PUBLIC BasicReportTemplate final
        :   public virtual IReportTemplate
    {
        DECLARE_SINGLETON(BasicReportTemplate)

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
        virtual auto    defaultLinkUnderlineMode() const -> UnderlineMode override;
        virtual auto    defaultPageNumberPlacement() const -> PageNumberPlacement override;

        virtual auto    styles() const -> Styles override;
        virtual auto    findStyleByMnemonic(
                                const Mnemonic & mnemonic
                            ) const -> IStyle * override;

        //////////
        //  Implementation
    private:
        const PageSetup             _pageSetup;                 //  never null
        const FontSpecs             _defaultFontSpecs;      //  never null; unmodifiable list
        const TypographicSize       _defaultFontSize;           //  never null
        const FontStyle             _defaultFontStyle;          //  java.awt.Font - style bit mask
        const ColorSpec             _defaultTextColor;          //  never null
        const ColorSpec             _defaultBackgroundColor;    //  never null
        const TypographicSize       _defaultListIndent;         //  never null
        const BorderType            _defaultTableBorderType;    //  never null
        const UnderlineMode         _defaultLinkUnderlineMode;  //  never null
        const PageNumberPlacement   _defaultPageNumberPlacement;

        QMap<Mnemonic, BasicStyle*> _mapNamesToStyles;          //  never null; unmodifiable map
    };

    /// \class BasicStyle tt3-report/API.hpp
    /// \brief A basic (predefined) stylw.
    class TT3_REPORT_PUBLIC BasicStyle
        :   public virtual IStyle
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(BasicStyle)

        friend class BasicReportTemplate;

        //////////
        //  Construction/destruction - from friends only
    private:
        BasicStyle(
                const QString & name,
                const FontSpecsOpt & fontSpecs,
                const TypographicSizeOpt & fontSize,
                const FontStyleOpt & fontStyle,
                const ColorSpecOpt & textColor,
                const ColorSpecOpt & backgroundColor,
                const UnderlineModeOpt & underlineMode
            );
        virtual ~BasicStyle();

        //////////
        //  IStyle
    public:
        virtual auto    reportTemplate() const -> IReportTemplate * override;
        virtual QString name() const override;
        virtual auto    fontSpecs() const -> FontSpecsOpt override;
        virtual auto    fontSize() const -> TypographicSizeOpt override;
        virtual auto    fontStyle() const -> FontStyleOpt override;
        virtual auto    textColor() const -> ColorSpecOpt override;
        virtual auto    backgroundColor() const -> ColorSpecOpt override;
        virtual auto    underlineMode() const -> UnderlineModeOpt override;

        //////////
        //  Implementation
    private:
        const QString               _name;
        const FontSpecsOpt          _fontSpecs;
        const TypographicSizeOpt    _fontSize;
        const FontStyleOpt          _fontStyle;
        const ColorSpecOpt          _textColor;
        const ColorSpecOpt          _backgroundColor;
        const UnderlineModeOpt      _underlineMode;
    };
}

//  End of tt3-report/BasicReportTemplate.hpp
