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

        QMap<Mnemonic, BasicStyle*> _styles; //  all styles "owned" by this report template

        //  Helpers
        void            _addStyle(BasicStyle * style);
    };

    /// \class BasicStyle tt3-report/API.hpp
    /// \brief A basic (predefined) stylw.
    class TT3_REPORT_PUBLIC BasicStyle
        :   public virtual IStyle
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(BasicStyle)

        friend class BasicReportTemplate;
        friend class BasicCharacterStyle;
        friend class BasicBlockStyle;
        friend class BasicLinkStyle;
        friend class BasicSectionStyle;

        //////////
        //  Construction/destruction - from friends only
    private:
        BasicStyle(
                const Mnemonic & name,
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
        const Mnemonic              _name;
        const FontSpecsOpt          _fontSpecs;
        const TypographicSizeOpt    _fontSize;
        const FontStyleOpt          _fontStyle;
        const ColorSpecOpt          _textColor;
        const ColorSpecOpt          _backgroundColor;
        const UnderlineModeOpt      _underlineMode;
    };

    /// \class BasicCharacterStyle tt3-report/API.hpp
    /// \brief A basic (predefined) character stylw.
    class TT3_REPORT_PUBLIC BasicCharacterStyle
        :   public BasicStyle,
            public virtual ICharacterStyle
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(BasicCharacterStyle)

        //////////
        //  Construction/destruction - from friends only
    private:
        BasicCharacterStyle(
                const Mnemonic & name,
                const FontSpecsOpt & fontSpecs,
                const TypographicSizeOpt & fontSize,
                const FontStyleOpt & fontStyle,
                const ColorSpecOpt & textColor,
                const ColorSpecOpt & backgroundColor,
                const UnderlineModeOpt & underlineMode
            );
        virtual ~BasicCharacterStyle();
    };

    /// \class BasicBlockStyle tt3-report/API.hpp
    /// \brief A basic (predefined) block stylw.
    class TT3_REPORT_PUBLIC BasicBlockStyle
        :   public BasicStyle,
            public virtual IBlockStyle
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(BasicBlockStyle)

        friend class BasicParagraphStyle;
        friend class BasicListStyle;
        friend class BasicTableStyle;

        //////////
        //  Construction/destruction - from friends only
    private:
        BasicBlockStyle(
                const Mnemonic & name,
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
        virtual ~BasicBlockStyle();

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
        const TypographicSizeOpt    _leftMargin;
        const TypographicSizeOpt    _rightMargin;
        const TypographicSizeOpt    _gapAbove;
        const TypographicSizeOpt    _gapBelow;
    };

    /// \class BasicParagraphStyle tt3-report/API.hpp
    /// \brief A basic (predefined) paragraph stylw.
    class TT3_REPORT_PUBLIC BasicParagraphStyle
        :   public BasicBlockStyle,
            public virtual IParagraphStyle
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(BasicParagraphStyle)

        friend class BasicReportTemplate;

        //////////
        //  Construction/destruction - from friends only
    private:
        BasicParagraphStyle(
                const Mnemonic & name,
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
        virtual ~BasicParagraphStyle();

        //////////
        //  IParagraphStyle
    public:
        virtual auto    textAlignment() const -> HorizontalAlignmentOpt override;
        virtual auto    borderType() const -> BorderTypeOpt override;

        //////////
        //  Implementation
    private:
        const HorizontalAlignmentOpt    _textAlignment;
        const BorderTypeOpt             _borderType;
    };

    /// \class BasicListStyle tt3-report/API.hpp
    /// \brief A basic (predefined) list stylw.
    class TT3_REPORT_PUBLIC BasicListStyle
        :   public BasicBlockStyle,
            public virtual IListStyle
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(BasicListStyle)

        friend class BasicReportTemplate;

        //////////
        //  Construction/destruction - from friends only
    private:
        BasicListStyle(
            const Mnemonic & name,
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
        virtual ~BasicListStyle();

        //////////
        //  IListStyle
    public:
        virtual auto    indent() const -> TypographicSizeOpt override;

        //////////
        //  Implementation
    private:
        const TypographicSizeOpt _indent;
    };

    /// \class BasicTableStyle tt3-report/API.hpp
    /// \brief A basic (predefined) Table stylw.
    class TT3_REPORT_PUBLIC BasicTableStyle
        :   public BasicBlockStyle,
            public virtual ITableStyle
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(BasicTableStyle)

        friend class BasicReportTemplate;

        //////////
        //  Construction/destruction - from friends only
    private:
        BasicTableStyle(
                const Mnemonic & name,
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
        virtual ~BasicTableStyle();

        //////////
        //  ITableStyle
    public:
        virtual auto    tableBorderType() const -> BorderTypeOpt override;
        virtual auto    cellBorderType() const -> BorderTypeOpt override;

        //////////
        //  Implementation
    private:
        const BorderTypeOpt     _tableBorderType;
        const BorderTypeOpt     _cellBorderType;
    };

    /// \class BasicLinkStyle tt3-report/API.hpp
    /// \brief A basic (predefined) link stylw.
    class TT3_REPORT_PUBLIC BasicLinkStyle
        :   public BasicStyle,
            public virtual ILinkStyle
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(BasicLinkStyle)

        friend class BasicReportTemplate;

        //////////
        //  Construction/destruction - from friends only
    private:
        BasicLinkStyle(
                const Mnemonic & name,
                const FontSpecsOpt & fontSpecs,
                const TypographicSizeOpt & fontSize,
                const FontStyleOpt & fontStyle,
                const ColorSpecOpt & textColor,
                const ColorSpecOpt & backgroundColor,
                const UnderlineModeOpt & underlineMode
            );
        virtual ~BasicLinkStyle();
    };

    /// \class BasicSectionStyle tt3-report/API.hpp
    /// \brief A basic (predefined) section stylw.
    class TT3_REPORT_PUBLIC BasicSectionStyle
        :   public BasicStyle,
            public virtual ISectionStyle
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(BasicSectionStyle)

        friend class BasicReportTemplate;

        //////////
        //  Construction/destruction - from friends only
    private:
        BasicSectionStyle(
                const Mnemonic & name,
                const FontSpecsOpt & fontSpecs,
                const TypographicSizeOpt & fontSize,
                const FontStyleOpt & fontStyle,
                const ColorSpecOpt & textColor,
                const ColorSpecOpt & backgroundColor,
                const UnderlineModeOpt & underlineMode,
                const PageNumberPlacementOpt & pageNumberPlacement
            );
        virtual ~BasicSectionStyle();

        //////////
        //  ISectionStyle
    public:
        virtual auto    pageNumberPlacement() const -> PageNumberPlacementOpt override;

        //////////
        //  Implementation
    private:
        const PageNumberPlacementOpt    _pageNumberPlacement;
    };
}

//  End of tt3-report/BasicReportTemplate.hpp
