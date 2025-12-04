//
//  tt3-report/DataTypes.hpp - tt3-report helpersdata types
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
    /// \class TypographicUnit tt3-report/API.hpp
    /// \brief The typographic unit.
    class TT3_REPORT_PUBLIC TypographicUnit final
    {
        //////////
        //  Constants
    public:
        static const TypographicUnit    Centimeter; ///< cm
        static const TypographicUnit    Millimeter; ///< mm
        static const TypographicUnit    Inch;       ///< in
        static const TypographicUnit    Pica;       ///< pc
        static const TypographicUnit    Point;      ///< pt

        //////////
        //  Construction/destruction/assignment
    private:
        explicit TypographicUnit(float points);
    public:
        /// \brief
        ///     Constructs a default "point" typographic unit.
        TypographicUnit() : _points(1.0) {}

        //  The default copy constructor, destructor and
        //  assignment operator are all OK

        //////////
        //  Operators
    public:
        /// \brief
        ///     Compares two TypographicUnits for equality.
        /// \param op2
        ///     The 2nd TypographicUnit to compare this TypographicUnit to.
        /// \return
        ///     True if the two TypographicUnits are eequal, else false.
        bool        operator == (const TypographicUnit & op2) const;

        /// \brief
        ///     Compares two TypographicUnits for equality.
        /// \param op2
        ///     The 2nd TypographicUnit to compare this TypographicUnit to.
        /// \return
        ///     False if the two TypographicUnits are eequal, else true.
        bool        operator != (const TypographicUnit & op2) const;

        /// \brief
        ///     Compares two TypographicUnits for order.
        /// \param op2
        ///     The 2nd TypographicUnit to compare this TypographicUnit to.
        /// \return
        ///     True if this TypographicUnit is "less than"
        ///     the 2nd TypographicUnit, else false.
        bool        operator <  (const TypographicUnit & op2) const;

        /// \brief
        ///     Compares two TypographicUnits for order.
        /// \param op2
        ///     The 2nd TypographicUnit to compare this TypographicUnit to.
        /// \return
        ///     True if this TypographicUnit is "less than or equal to"
        ///     the 2nd TypographicUnit, else false.
        bool        operator <= (const TypographicUnit & op2) const;

        /// \brief
        ///     Compares two TypographicUnits for order.
        /// \param op2
        ///     The 2nd TypographicUnit to compare this TypographicUnit to.
        /// \return
        ///     True if this TypographicUnit is "greater than"
        ///     the 2nd TypographicUnit, else false.
        bool        operator >  (const TypographicUnit & op2) const;

        /// \brief
        ///     Compares two TypographicUnits for order.
        /// \param op2
        ///     The 2nd TypographicUnit to compare this TypographicUnit to.
        /// \return
        ///     True if this TypographicUnit is "greater than or equal to"
        ///     the 2nd TypographicUnit, else false.
        bool        operator >= (const TypographicUnit & op2) const;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the number of points in this TypographicUnit.
        /// \return
        ///     The number of points in this TypographicUnit.
        float           points() const { return _points; }

        //////////
        //  Implementation
    private:
        float       _points;
    };
    using TypographicUnitOpt = std::optional<TypographicUnit>;

    /// \class TypographicSize tt3-report/API.hpp
    /// \brief The typographic size (unit + number of units).
    class TT3_REPORT_PUBLIC TypographicSize final
    {
        //////////
        //  Construction/destruction/assignment
    public:
        /// \brief
        ///     Constructs a standard 12pt size.
        TypographicSize();

        /// \brief
        ///     Constructs a typographic size.
        /// \param amount
        ///     The number of typographic units in this typographic size.
        /// \param unit
        ///     The unit in which the typographic size is expressed.
        explicit TypographicSize(
                float amount,
                const TypographicUnit & unit
            );

        //  The default copy constructor, destructor and
        //  assignment operator are all OK

        /// \brief
        ///     Creates a TypographicSize measured in centimeters.
        /// \param amount
        ///     The number of centimeters.
        /// \return
        ///     The TypographicSize with that many centimeters.
        static auto     cm(float amount) -> TypographicSize;

        /// \brief
        ///     Creates a TypographicSize measured in millimeters.
        /// \param amount
        ///     The number of millimeters.
        /// \return
        ///     The TypographicSize with that many millimeters.
        static auto     mm(float amount) -> TypographicSize;

        /// \brief
        ///     Creates a TypographicSize measured in inches.
        /// \param amount
        ///     The number of inches.
        /// \return
        ///     The TypographicSize with that many inches.
        static auto     in(float amount) -> TypographicSize;

        /// \brief
        ///     Creates a TypographicSize measured in c.
        /// \param amount
        ///     The number of inches.
        /// \return
        ///     The TypographicSize with that many inches.
        static auto     pc(float amount) -> TypographicSize;

        /// \brief
        ///     Creates a TypographicSize measured in points.
        /// \param amount
        ///     The number of points.
        /// \return
        ///     The TypographicSize with that many points.
        static auto     pt(float amount) -> TypographicSize;

        //////////
        //  Operators
    public:
        /// \brief
        ///     Compares two TypographicSizes for equality.
        /// \details
        ///     TypographicSizes are compared using their point
        ///     equivalents, so 1in and 72pt will compare "equal".
        /// \param op2
        ///     The 2nd TypographicSize to compare this TypographicSize to.
        /// \return
        ///     True if the two TypographicSizes are eequal, else false.
        bool            operator == (const TypographicSize & op2) const;

        /// \brief
        ///     Compares two TypographicSizes for equality.
        /// \details
        ///     TypographicSizes are compared using their point
        ///     equivalents, so 1in and 72pt will compare "equal".
        /// \param op2
        ///     The 2nd TypographicSize to compare this TypographicSize to.
        /// \return
        ///     False if the two TypographicSizes are eequal, else true.
        bool            operator != (const TypographicSize & op2) const;

        /// \brief
        ///     Compares two TypographicSizes for order.
        /// \details
        ///     TypographicSizes are compared using their point
        ///     equivalents, so 1in and 72pt will compare "equal".
        /// \param op2
        ///     The 2nd TypographicSize to compare this TypographicSize to.
        /// \return
        ///     True if this TypographicSize is "less than"
        ///     the 2nd TypographicSize, else false.
        bool            operator <  (const TypographicSize & op2) const;

        /// \brief
        ///     Compares two TypographicSizes for order.
        /// \details
        ///     TypographicSizes are compared using their point
        ///     equivalents, so 1in and 72pt will compare "equal".
        /// \param op2
        ///     The 2nd TypographicSize to compare this TypographicSize to.
        /// \return
        ///     True if this TypographicSize is "less than or equal to"
        ///     the 2nd TypographicSize, else false.
        bool            operator <= (const TypographicSize & op2) const;

        /// \brief
        ///     Compares two TypographicSizes for order.
        /// \details
        ///     TypographicSizes are compared using their point
        ///     equivalents, so 1in and 72pt will compare "equal".
        /// \param op2
        ///     The 2nd TypographicSize to compare this TypographicSize to.
        /// \return
        ///     True if this TypographicSize is "greater than"
        ///     the 2nd TypographicSize, else false.
        bool            operator >  (const TypographicSize & op2) const;

        /// \brief
        ///     Compares two TypographicSizes for order.
        /// \details
        ///     TypographicSizes are compared using their point
        ///     equivalents, so 1in and 72pt will compare "equal".
        /// \param op2
        ///     The 2nd TypographicSize to compare this TypographicSize to.
        /// \return
        ///     True if this TypographicSize is "greater than or equal to"
        ///     the 2nd TypographicSize, else false.
        bool            operator >= (const TypographicSize & op2) const;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the number of typographic units in this typographic size.
        /// \return
        ///     The number of typographic units in this typographic size.
        float           amount() const { return _amount; }

        /// \brief
        ///     Returns the typographic unit in which this typographic size is expressed.
        /// \return
        ///     The typographic unit in which this typographic size is expressed.
        TypographicUnit unit() const { return _unit; }

        /// \brief
        ///     Returns the point size of this typographic size.
        /// \return
        ///     The point size of this typographic size.
        float           pointSize() const { return _amount * _unit.points(); }

        /// \brief
        ///     Returns the scaled variant of this TypographicSize.
        /// \param scaleFactor
        ///     The scale factor.
        /// \return
        ///     The TypographicSize which uses the same unit as this
        ///     TypographicSize, but whose amount has been multiplied by the
        ///     specified scale factor.
        TypographicSize scaled(float scaleFactor) const;

        //////////
        //  Implementation
    private:
        float           _amount;
        TypographicUnit _unit;
    };
    using TypographicSizeOpt = std::optional<TypographicSize>;

    /// \class FontSpec tt3-report/API.hpp
    /// \brief Specifies the font or font class name.
    class TT3_REPORT_PUBLIC FontSpec final
    {
        //////////
        //  Constants
    public:
        static const FontSpec   Default;    ///< ""
        static const FontSpec   Serif;      ///< serif
        static const FontSpec   SansSerif;  ///< sans-serif
        static const FontSpec   Monospaced; ///< monospace

        //////////
        //  Construction/destruction/assignment
    public:
        /// \brief
        ///     Constructs a default (empty) font name.
        FontSpec();

        /// \brief
        ///     Constructs a custom or standard font name.
        /// \param name
        ///     The font or font class name.
        explicit FontSpec(const QString & name);

        //  The default copy constructor, destructor and
        //  assignment operator are all OK

        //////////
        //  Operators
    public:
        /// \brief
        ///     Compares two FontSpecs for equality.
        /// \details
        ///     FontSpecs are compared case-insensitively.
        /// \param op2
        ///     The 2nd FontSpec to compare this FontSpec to.
        /// \return
        ///     True if the two FontSpecs are eequal, else false.
        bool        operator == (const FontSpec & op2) const;

        /// \brief
        ///     Compares two FontSpecs for equality.
        /// \details
        ///     FontSpecs are compared case-insensitively.
        /// \param op2
        ///     The 2nd FontSpec to compare this FontSpec to.
        /// \return
        ///     False if the two FontSpecs are eequal, else true.
        bool        operator != (const FontSpec & op2) const;

        /// \brief
        ///     Compares two FontSpecs for order.
        /// \details
        ///     FontSpecs are compared case-insensitively.
        /// \param op2
        ///     The 2nd FontSpec to compare this FontSpec to.
        /// \return
        ///     True if this FontSpec is "less than"
        ///     the 2nd FontSpec, else false.
        bool        operator <  (const FontSpec & op2) const;

        /// \brief
        ///     Compares two FontSpecs for order.
        /// \details
        ///     FontSpecs are compared case-insensitively.
        /// \param op2
        ///     The 2nd FontSpec to compare this FontSpec to.
        /// \return
        ///     True if this FontSpec is "less than or equal to"
        ///     the 2nd FontSpec, else false.
        bool        operator <= (const FontSpec & op2) const;

        /// \brief
        ///     Compares two FontSpecs for order.
        /// \details
        ///     FontSpecs are compared case-insensitively.
        /// \param op2
        ///     The 2nd FontSpec to compare this FontSpec to.
        /// \return
        ///     True if this FontSpec is "greater than"
        ///     the 2nd FontSpec, else false.
        bool        operator >  (const FontSpec & op2) const;

        /// \brief
        ///     Compares two FontSpecs for order.
        /// \details
        ///     FontSpecs are compared case-insensitively.
        /// \param op2
        ///     The 2nd FontSpec to compare this FontSpec to.
        /// \return
        ///     True if this FontSpec is "greater than or equal to"
        ///     the 2nd FontSpec, else false.
        bool        operator >= (const FontSpec & op2) const;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Checks if this is a "default" font specification.
        /// \return
        ///     True if this is a "default" font specification, else false.
        bool            isDefault() const;

        /// \brief
        ///     Checks if this is a font class specification.
        /// \return
        ///     True if this is a font class specification, else false.
        bool            isFontClassName() const;

        /// \brief
        ///     Checks if this is a font (not font class!) specification.
        /// \return
        ///     True if this is a font (not font class!) specification, else false.
        bool            isFontName() const;

        /// \brief
        ///     Returns the font or font class name.
        /// \return
        ///     The font or font class name; "" == default font.
        QString         name() const { return _name; }

        //////////
        //  Implementation
    private:
        QString         _name;  //  "" == "default font"
    };
    using FontSpecOpt = std::optional<FontSpec>;

    /// \brief
    ///     The liat of font spect ORDERED BY DECREASING PRIORITY.
    using FontSpecs = QList<FontSpec>;
    using FontSpecsOpt = std::optional<FontSpecs>;

    /// \brief
    ///     Flags that can be applied to a font.
    enum class FontFlag
    {
        Bold = 0x01,        ///\ bold
        Italic = 0x02,      ///\ italic
        Underline = 0x04,   ///\ underline
        StrikeThrough = 0x08///\ strikethrough
    };

    /// \class FontStyle tt3-report/API.hpp
    /// \brief A "set of FontFlag flags" ADT.
    /// \details
    ///     Implemented as a bit set for better performance.
    class TT3_REPORT_PUBLIC FontStyle final
    {
        //////////
        //  Constants
    public:
        static const FontStyle  Plain;          ///< Empty set  - no flags.
        static const FontStyle  Bold;           ///< Bold flag only.
        static const FontStyle  Italic;         ///< Italic flag only.
        static const FontStyle  Underline;      ///< Underline flag only.
        static const FontStyle  StrikeThrough;  ///< StrikeThrough flag only.

        //////////
        //  Construction/destruction/assignment
    private:
        explicit FontStyle(int mask) : _mask(mask) {}
    public:
        /// \brief
        ///     Constructs an empty set of FontFlag flags.
        constexpr FontStyle() : _mask(0) {}

        /// \brief
        ///     Constructs a set with a single FontFlag flag.
        /// \param f
        ///     The FontFlag flag to initialize the set with.
        constexpr FontStyle(FontFlag f) : _mask(int(f)) {}

        //  The default copy constructor, destructor anjd
        //  assignment are all OK

        //////////
        //  Operators
    public:
        /// \brief
        ///     Compares two FontFlag sets for equality.
        /// \param op2
        ///     The 2nd FontFlag set to compare this one with.
        /// \return
        ///     True if the two FontFlag sets are equal, else false.
        bool            operator == (const FontStyle & op2) const { return _mask == op2._mask; }

        /// \brief
        ///     Compares two FontFlag sets for inequality.
        /// \param op2
        ///     The 2nd FontFlag set to compare this one with.
        /// \return
        ///     False if the two FontFlag sets are equal, else true.
        bool            operator != (const FontStyle & op2) const { return _mask != op2._mask; }

        /// \brief
        ///     Calculates a union of two FontFlag sets.
        /// \param op2
        ///     The 2nd FontFlag set.
        /// \return
        ///     The union of two FontFlag sets.
        FontStyle       operator |(const FontStyle & op2) const
        {
            return FontStyle(_mask | op2._mask);
        }

        /// \brief
        ///     Calculates a union of two FontFlag sets.
        /// \param op2
        ///     The 2nd FontFlag set.
        /// \return
        ///     The *this, whose value has been updated to
        ///     be the union of two FontFlag sets.
        FontStyle &     operator |=(const FontStyle & op2)
        {
            _mask |= op2._mask;
            return *this;
        }

        /// \brief
        ///     Calculates an intersection of two FontFlag sets.
        /// \param op2
        ///     The 2nd FontFlag set.
        /// \return
        ///     The intersection of two FontFlag sets.
        FontStyle       operator &(const FontStyle & op2) const
        {
            return FontStyle(_mask & op2._mask);
        }

        /// \brief
        ///     Calculates an intersection of two FontFlag sets.
        /// \param op2
        ///     The 2nd FontFlag set.
        /// \return
        ///     The *this, whose value has been updated to
        ///     be the intersection of two FontFlag sets.
        FontStyle &     operator &=(const FontStyle & op2)
        {
            _mask &= op2._mask;
            return *this;
        }

        //////////
        //  Operations
    public:
        /// \brief
        ///     Checks if this FontFlag set is empty.
        /// \return
        ///     True if this FontFlag set is empty, false if not.
        constexpr bool  isEmpty() const
        {
            return _mask == 0;
        }

        /// \brief
        ///     Checks if this FontFlag set contains the
        ///     specified FontFlag.
        /// \param f
        ///     The FontFlag to check.
        /// \return
        ///     True if this FontFlag set contains the
        ///     specified FontFlag, else false.
        constexpr bool  contains(FontFlag f) const
        {
            return (_mask & int(f)) != 0;
        }

        //////////
        //  Implementation
    private:
        int             _mask;
    };
    using FontStyleOpt = std::optional<FontStyle>;

    /// \brief
    ///     Returns the FontFlag set that contains
    ///     all of the specified FontStyle.
    /// \param a
    ///     The FontFlag to include into the set.
    /// \param b
    ///     The FontFlag to include into the set.
    /// \return
    ///     The FontFlag set that contains all of
    ///     the specified FontStyle.
    TT3_REPORT_PUBLIC
    inline FontStyle operator |(FontFlag a, FontFlag b)
    {
        return FontStyle(a) | FontStyle(b);
    }

    /// \brief
    ///     The specification of a horizontal alignment.
    enum class HorizontalAlignment
    {
        Default,    ///< Use defaule alignment.
        Left,       ///< Align left.
        Center,     ///< Aligh to center.
        Right,      ///< Align right.
        Justify     ///< Justify left and right.
    };
    using HorizontalAlignmentOpt = std::optional<HorizontalAlignment>;

    /// \brief
    ///     The specification of a vertical alignment.
    enum class VerticalAlignment
    {
        Default,    ///< Use defaule alignment.
        Top,        ///< Align to top.
        Middle,     ///< Aligh to middle.
        Bottom      ///< Align to bottom.
    };
    using VerticalAlignmentOpt = std::optional<VerticalAlignment>;

    /// \brief
    ///     The specification of a paage number placement.
    enum class PageNumberPlacement
    {
        Default,        ///< Use default page number placement.
        None,           ///< Mo page numbers.
        TopLeft,        ///< Page numbers at the top-left corner of the page.
        TopCenter,      ///< Page numbers centered at the top of the page.
        TopRight,       ///< Page numbers at the top-right corner of the page.
        BottomLeft,     ///< Page numbers at the bottom-left corner of the page.
        BottomCenter,   ///< Page numbers centered at the bottom of the page.
        BottomRight     ///< Page numbers at the bottom-right corner of the page.
    };
    using PageNumberPlacementOpt = std::optional<PageNumberPlacement>;

    /// \brief
    ///     The specification of a text underline mode.
    enum class UnderlineMode
    {
        Default,    ///< Use default page number placement.
        None,       ///< No underlining.
        Single,     ///< Single-line underline.
        Double      ///< Double-line underline.
    };
    using UnderlineModeOpt = std::optional<UnderlineMode>;

    /// \brief
    ///     The specification of a page orientation.
    enum class PageOrientation
    {
        Default,    ///< Use default page orientation.
        Portrait,   ///< Use portrait page orientation.
        Landscape   ///< Use landscape page orientation.
    };
    using PageOrientationOpt = std::optional<PageOrientation>;

    /// \class PageSetup tt3-report/API.hpp
    /// \brief The page setup.
    class TT3_REPORT_PUBLIC PageSetup final
    {
        //////////
        //  Construction/destruction/assignment
    public:
        /// \brief
        ///     Constructs a default page setup (A4 portrait, 2cm margins).
        PageSetup();

        /// \brief
        ///     Constructs the page setup.
        /// \param pageWidth
        ///     The page width.
        /// \param pageHeight
        ///     The page height.
        /// \param pageOrientation
        ///     The page orientation.
        /// \param leftMargin
        ///     The left margin.
        /// \param rightMargin
        ///     The right margin.
        /// \param topMargin
        ///     The top margin.
        /// \param bottomMargin
        ///     The bottom margin.
        PageSetup(
                const TypographicSize & pageWidth,
                const TypographicSize & pageHeight,
                PageOrientation pageOrientation,
                const TypographicSize & leftMargin,
                const TypographicSize & rightMargin,
                const TypographicSize & topMargin,
                const TypographicSize & bottomMargin
            );

        //  The default copy constructor, destructor and
        //  assignment operator are all OK

        //////////
        //  Operators
    public:
        /// \brief
        ///     Compares two PageSetups for equality.
        /// \param op2
        ///     The 2nd PageSetup to compare this PageSetup to.
        /// \return
        ///     True if the two PageSetups are eequal, else false.
        bool            operator == (const PageSetup & op2) const;

        /// \brief
        ///     Compares two PageSetups for equality.
        /// \param op2
        ///     The 2nd PageSetup to compare this PageSetup to.
        /// \return
        ///     False if the two PageSetups are eequal, else true.
        bool            operator != (const PageSetup & op2) const;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the specified page width.
        /// \details
        ///     This is the smaller of the two page dimensions.
        /// \return
        ///     The specified page width.
        TypographicSize pageWidth() const { return _pageWidth; }

        /// \brief
        ///     Returns the specified page height.
        /// \details
        ///     This is the larger of the two page dimensions.
        /// \return
        ///     The specified page height.
        TypographicSize pageHeight() const { return _pageHeight; }

        /// \brief
        ///     Returns the page orientation.
        /// \return
        ///     The page orientation.
        PageOrientation pageOrientation() const { return _pageOrientation; }

        /// \brief
        ///     Returns the size of the margin at the left edge of the page.
        /// \return
        ///     The size of the margin at the left edge of the page.
        TypographicSize leftMargin() const { return _leftMargin; }

        /// \brief
        ///     Returns the size of the margin at the right edge of the page.
        /// \return
        ///     The size of the margin at the right edge of the page.
        TypographicSize rightMargin() const { return _rightMargin; }

        /// \brief
        ///     Returns the size of the margin at the top edge of the page.
        /// \return
        ///     The size of the margin at the top edge of the page.
        TypographicSize topMargin() const { return _topMargin; }

        /// \brief
        ///     Returns the size of the margin at the bottom edge of the page.
        /// \return
        ///     The size of the margin at the bottom edge of the page.
        TypographicSize bottomMargin() const { return _bottomMargin; }

        /// \brief
        ///     Checks this PageSetup for validity.
        /// \details
        ///     A PageSetup is considered valid if:
        ///     -   its size is >0.
        ///     -   its margins are >= 0.
        ///     -   its margins leave unused area in the middle of the page.
        /// \return
        ///     True if this PageSetup is valid, else false.
        bool           isValid() const;

        //////////
        //  Implementation
    private:
        TypographicSize _pageWidth;
        TypographicSize _pageHeight;
        PageOrientation _pageOrientation;
        TypographicSize _leftMargin;
        TypographicSize _rightMargin;
        TypographicSize _topMargin;
        TypographicSize _bottomMargin;
    };
    using PageSetupOpt = std::optional<PageSetup>;

    /// \class ColorSpec tt3-report/API.hpp
    /// \brief The color specification.
    class TT3_REPORT_PUBLIC ColorSpec final
    {
        //////////
        //  Types
    public:
        /// \brief
        ///     The class of the color specification.
        enum ColorClass
        {
            Default,    ///< Default color.
            Transparent,///< Transparent color.
            Custom      ///< Custom color.
        };

        //////////
        //  Construction/destruction/assignment
    public:
        /// \brief
        ///     Constructs a Default color.
        ColorSpec();

        /// \brief
        ///     Constructs a color from color class.
        /// \param colorClass
        ///     The color class.
        ColorSpec(ColorClass colorClass);

        /// \brief
        ///     Constructs a Custom color.
        /// \param customColor
        ///     The custom color.
        ColorSpec(const QColor & customColor);

        /// \brief
        ///     Constructs a Custom color.
        /// \param r
        ///     The red intensity.
        /// \param g
        ///     The green intensity.
        /// \param b
        ///     The blue intensity.
        /// \param a
        ///     The alpha (opacity) channel.
        ColorSpec(int r, int g, int b, int a = 255);

        //  The default copy constructor, destructor and
        //  assignment operator are all OK

        //////////
        //  Operators
    public:
        /// \brief
        ///     Compares two ColorSpecs for equality.
        /// \param op2
        ///     The 2nd ColorSpec to compare this ColorSpec to.
        /// \return
        ///     True if the two ColorSpecs are eequal, else false.
        bool        operator == (const ColorSpec & op2) const;

        /// \brief
        ///     Compares two ColorSpecs for equality.
        /// \param op2
        ///     The 2nd ColorSpec to compare this ColorSpec to.
        /// \return
        ///     False if the two ColorSpecs are eequal, else true.
        bool        operator != (const ColorSpec & op2) const;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the color class.
        /// \return
        ///     The color class.
        ColorClass      colorClass() const { return _colorClass; }

        /// \brief
        ///     Returns the custom color.
        /// \return
        ///     The custom color or a fully transparent if color
        ///     class is not Custom.
        QColor          customColor() const { return _customColor; }

        //////////
        //  Implementation
    private:
        ColorClass      _colorClass;
        QColor          _customColor;   //  unused unless _colorClass == Custom
    };
    using ColorSpecOpt = std::optional<ColorSpec>;

    /// \brief
    ///     The specification of a border type.
    enum class BorderType
    {
        Default,    ///< Use defaule border.
        None,       ///< No borders.
        Single,     ///< Single border.
        Double      ///< Double border.
    };
    using BorderTypeOpt = std::optional<BorderType>;
}

//  Formatting/parsing
namespace tt3::util
{
    template <> TT3_REPORT_PUBLIC
    QString toString<tt3::report::TypographicUnit>(
            const tt3::report::TypographicUnit & value
        );
    template <> TT3_REPORT_PUBLIC
    QString toString<tt3::report::TypographicSize>(
            const tt3::report::TypographicSize & value
        );
    template <> TT3_REPORT_PUBLIC
    QString toString<tt3::report::FontSpec>(
            const tt3::report::FontSpec & value
        );
    template <> TT3_REPORT_PUBLIC
    QString toString<tt3::report::FontSpecs>(
            const tt3::report::FontSpecs & value
        );
    template <> TT3_REPORT_PUBLIC
    QString toString<tt3::report::FontFlag>(
            const tt3::report::FontFlag & value
        );
    template <> TT3_REPORT_PUBLIC
    QString toString<tt3::report::FontStyle>(
            const tt3::report::FontStyle & value
        );
    template <> TT3_REPORT_PUBLIC
    QString toString<tt3::report::HorizontalAlignment>(
            const tt3::report::HorizontalAlignment & value
        );
    template <> TT3_REPORT_PUBLIC
    QString toString<tt3::report::VerticalAlignment>(
            const tt3::report::VerticalAlignment & value
        );
    template <> TT3_REPORT_PUBLIC
    QString toString<tt3::report::PageNumberPlacement>(
            const tt3::report::PageNumberPlacement & value
        );
    template <> TT3_REPORT_PUBLIC
    QString toString<tt3::report::UnderlineMode>(
            const tt3::report::UnderlineMode & value
        );
    template <> TT3_REPORT_PUBLIC
    QString toString<tt3::report::PageOrientation>(
            const tt3::report::PageOrientation & value
        );
    template <> TT3_REPORT_PUBLIC
    QString toString<tt3::report::PageSetup>(
            const tt3::report::PageSetup & value
        );
    template <> TT3_REPORT_PUBLIC
    QString toString<tt3::report::ColorSpec>(
            const tt3::report::ColorSpec & value
        );
    template <> TT3_REPORT_PUBLIC
    QString toString<tt3::report::BorderType>(
            const tt3::report::BorderType & value
        );

    template <> TT3_REPORT_PUBLIC
    auto fromString<tt3::report::TypographicUnit>(
            const QString & s,
            qsizetype & scan
        ) -> tt3::report::TypographicUnit;
    template <> TT3_REPORT_PUBLIC
    auto fromString<tt3::report::TypographicSize>(
            const QString & s,
            qsizetype & scan
        ) -> tt3::report::TypographicSize;
    template <> TT3_REPORT_PUBLIC
    auto fromString<tt3::report::FontSpec>(
            const QString & s,
            qsizetype & scan
        ) -> tt3::report::FontSpec;
    template <> TT3_REPORT_PUBLIC
    auto fromString<tt3::report::FontFlag>(
            const QString & s,
            qsizetype & scan
        ) -> tt3::report::FontFlag;
    template <> TT3_REPORT_PUBLIC
    auto fromString<tt3::report::FontStyle>(
            const QString & s,
            qsizetype & scan
        ) -> tt3::report::FontStyle;
    template <> TT3_REPORT_PUBLIC
    auto fromString<tt3::report::HorizontalAlignment>(
            const QString & s,
            qsizetype & scan
        ) -> tt3::report::HorizontalAlignment;
    template <> TT3_REPORT_PUBLIC
    auto fromString<tt3::report::VerticalAlignment>(
            const QString & s,
            qsizetype & scan
        ) -> tt3::report::VerticalAlignment;
    template <> TT3_REPORT_PUBLIC
    auto fromString<tt3::report::PageNumberPlacement>(
            const QString & s,
            qsizetype & scan
        ) -> tt3::report::PageNumberPlacement;
    template <> TT3_REPORT_PUBLIC
    auto fromString<tt3::report::UnderlineMode>(
            const QString & s,
            qsizetype & scan
        ) -> tt3::report::UnderlineMode;
    template <> TT3_REPORT_PUBLIC
    auto fromString<tt3::report::PageOrientation>(
            const QString & s,
            qsizetype & scan
        ) -> tt3::report::PageOrientation;
    template <> TT3_REPORT_PUBLIC
    auto fromString<tt3::report::PageSetup>(
            const QString & s,
            qsizetype & scan
        ) -> tt3::report::PageSetup;
    template <> TT3_REPORT_PUBLIC
    auto fromString<tt3::report::ColorSpec>(
            const QString & s,
            qsizetype & scan
        ) -> tt3::report::ColorSpec;
    template <> TT3_REPORT_PUBLIC
    auto fromString<tt3::report::BorderType>(
            const QString & s,
            qsizetype & scan
        ) -> tt3::report::BorderType;
}

//  End of tt3-report/DataTypes.hpp
