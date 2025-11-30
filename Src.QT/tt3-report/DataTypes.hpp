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
        ///     False if the two TypographicUnitssare eequal, else true.
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
        const float     _points;
    };

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
                const TypographicUnit & unit = TypographicUnit::Point
            );

        //  The default copy constructor, destructor and
        //  assignment operator are all OK

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
        bool        operator == (const TypographicSize & op2) const;

        /// \brief
        ///     Compares two TypographicSizes for equality.
        /// \details
        ///     TypographicSizes are compared using their point
        ///     equivalents, so 1in and 72pt will compare "equal".
        /// \param op2
        ///     The 2nd TypographicSize to compare this TypographicSize to.
        /// \return
        ///     False if the two TypographicSizes are eequal, else true.
        bool        operator != (const TypographicSize & op2) const;

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
        bool        operator <  (const TypographicSize & op2) const;

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
        bool        operator <= (const TypographicSize & op2) const;

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
        bool        operator >  (const TypographicSize & op2) const;

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
        bool        operator >= (const TypographicSize & op2) const;

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

        //////////
        //  Implementation
    private:
        float           _amount;
        TypographicUnit _unit;
    };

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

    /// \brief
    ///     The liat of font spect ORDERED BY DECREASING PRIORITY.
    using FontSpecs = QList<FontSpec>;

    /// \brief
    ///     Flags that can be applied to a font.
    enum class FontFlag
    {
        Bold = 0x01,        ///\ bold
        Italic = 0x02,      ///\ italic
        Underline = 0x04,   ///\ underline
        StrikeThrough = 0x08///\ strikethrough
    };

    /// \class FontFlags tt3-report/API.hpp
    /// \brief A "set of FontFlag flags" ADT.
    /// \details
    ///     Implemented as a bit set for better performance.
    class TT3_REPORT_PUBLIC FontFlags final
    {
        //////////
        //  Constants
    public:
        static const FontFlags  Plain;          ///< Empty set  - no flags.
        static const FontFlags  Bold;           ///< Bold flag only.
        static const FontFlags  Italic;         ///< Italic flag only.
        static const FontFlags  Underline;      ///< Underline flag only.
        static const FontFlags  StrikeThrough;  ///< StrikeThrough flag only.

        //////////
        //  Construction/destruction/assignment
    private:
        explicit FontFlags(int mask) : _mask(mask) {}
    public:
        /// \brief
        ///     Constructs an empty set of FontFlag flags.
        constexpr FontFlags() : _mask(0) {}

        /// \brief
        ///     Constructs a set with a single FontFlag flag.
        /// \param f
        ///     The FontFlag flag to initialize the set with.
        constexpr FontFlags(FontFlag f) : _mask(int(f)) {}

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
        bool            operator == (const FontFlags & op2) const { return _mask == op2._mask; }

        /// \brief
        ///     Compares two FontFlag sets for inequality.
        /// \param op2
        ///     The 2nd FontFlag set to compare this one with.
        /// \return
        ///     False if the two FontFlag sets are equal, else true.
        bool            operator != (const FontFlags & op2) const { return _mask != op2._mask; }

        /// \brief
        ///     Calculates a union of two FontFlag sets.
        /// \param op2
        ///     The 2nd FontFlag set.
        /// \return
        ///     The union of two FontFlag sets.
        FontFlags       operator |(const FontFlags & op2) const
        {
            return FontFlags(_mask | op2._mask);
        }

        /// \brief
        ///     Calculates a union of two FontFlag sets.
        /// \param op2
        ///     The 2nd FontFlag set.
        /// \return
        ///     The *this, whose value has been updated to
        ///     be the union of two FontFlag sets.
        FontFlags &     operator |=(const FontFlags & op2)
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
        FontFlags       operator &(const FontFlags & op2) const
        {
            return FontFlags(_mask & op2._mask);
        }

        /// \brief
        ///     Calculates an intersection of two FontFlag sets.
        /// \param op2
        ///     The 2nd FontFlag set.
        /// \return
        ///     The *this, whose value has been updated to
        ///     be the intersection of two FontFlag sets.
        FontFlags &     operator &=(const FontFlags & op2)
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

    /// \brief
    ///     Returns the FontFlag set that contains
    ///     all of the specified FontFlags.
    /// \param a
    ///     The FontFlag to include into the set.
    /// \param b
    ///     The FontFlag to include into the set.
    /// \return
    ///     The FontFlag set that contains all of
    ///     the specified FontFlags.
    TT3_REPORT_PUBLIC
    inline FontFlags operator |(FontFlag a, FontFlag b)
    {
        return FontFlags(a) | FontFlags(b);
    }

    /// \brief
    ///     The specification of a horizontal alignment.
    enum class HorizontalAlignment
    {
        Default,    ///< Use defaule alignment.
        Left,       ///< Align left.
        Center,     ///< Aligh to center.
        Right       ///< Align right.
    };

    /// \brief
    ///     The specification of a vertical alignment.
    enum class VerticalAlignment
    {
        Default,    ///< Use defaule alignment.
        Top,        ///< Align to top.
        Middle,     ///< Aligh to middle.
        Bottom      ///< Align to bottom.
    };

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

    /// \brief
    ///     The specification of a text underline mode.
    enum class UnderlineMode
    {
        Default,    ///< Use default page number placement.
        None,       ///< Mo underlining.
        Single,     ///< Single-line underline.
        Double      ///< Double-line underline.
    };
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
    QString toString<tt3::report::FontFlag>(
            const tt3::report::FontFlag & value
        );
    template <> TT3_REPORT_PUBLIC
    QString toString<tt3::report::FontFlags>(
            const tt3::report::FontFlags & value
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
    auto fromString<tt3::report::FontFlags>(
            const QString & s,
            qsizetype & scan
        ) -> tt3::report::FontFlags;
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
}

//  End of tt3-report/DataTypes.hpp
