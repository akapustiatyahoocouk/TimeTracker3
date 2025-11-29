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
        static const TypographicUnit    Centimeter;
        static const TypographicUnit    Millimeter;
        static const TypographicUnit    Inch;
        static const TypographicUnit    Pica;
        static const TypographicUnit    Point;

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
        bool        operator == (const TypographicUnit & op2) const;
        bool        operator != (const TypographicUnit & op2) const;
        bool        operator <  (const TypographicUnit & op2) const;
        bool        operator <= (const TypographicUnit & op2) const;
        bool        operator >  (const TypographicUnit & op2) const;
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
        TypographicSize();
        TypographicSize(const TypographicUnit & unit, float amount);

        //  The default copy constructor, destructor and
        //  assignment operator are all OK

        //////////
        //  Operators
    public:
        bool        operator == (const TypographicSize & op2) const;
        bool        operator != (const TypographicSize & op2) const;
        bool        operator <  (const TypographicSize & op2) const;
        bool        operator <= (const TypographicSize & op2) const;
        bool        operator >  (const TypographicSize & op2) const;
        bool        operator >= (const TypographicSize & op2) const;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the number of typographic units.
        /// \return
        ///     The number of typographic units.
        float           amount() const { return _amount; }

        /// \brief
        ///     Returns the typographic unit in which this size is expressed.
        /// \return
        ///     The typographic unit in which this size is expressed.
        TypographicUnit unit() const { return _unit; }

        /// \brief
        ///     Returns the point size of this size
        /// \return
        ///     The point size of this size
        float           pointSize() const { return _amount * _unit.points(); }

        //////////
        //  Implementation
    private:
        float           _amount;
        TypographicUnit _unit;
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
    auto fromString<tt3::report::TypographicUnit>(
            const QString & s,
            qsizetype & scan
        ) -> tt3::report::TypographicUnit;
    template <> TT3_REPORT_PUBLIC
    auto fromString<tt3::report::TypographicSize>(
            const QString & s,
            qsizetype & scan
        ) -> tt3::report::TypographicSize;
}

//  End of tt3-report/DataTypes.hpp
