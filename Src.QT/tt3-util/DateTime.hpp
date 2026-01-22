//
//  tt3-util/DateTime.hpp - Date/time helpers
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

namespace tt3::util
{
    /// \class TimeSpan tt3-util/API.hpp
    /// \brief A time interval.
    class TT3_UTIL_PUBLIC TimeSpan
    {
        //////////
        //  Constants
    public:
        /// \brief
        ///     The "invalid" time span.
        static const TimeSpan Invalid;

        //////////
        //  Construction/destruction/assignment
    private:
        explicit TimeSpan(int minutes) : _minutes(minutes) {}
    public:
        /// \brief
        ///     Constructs an "invalid" time span.
        TimeSpan() : _minutes(_InvalidMinutes) {}

        /// \brief
        ///     Constructs a time span from the number of hours.
        /// \param h
        ///     The number of hours.
        /// \return
        ///     The time span covering the specified number of
        ///     hours, or an "invalid" time span if overflow occurs.
        static TimeSpan hours(int h);

        /// \brief
        ///     Constructs a time span from the number of minutes.
        /// \param m
        ///     The number of minutes.
        /// \return
        ///     The time span covering the specified number of
        ///     minutes, or an "invalid" time span if overflow occurs.
        static TimeSpan minutes(int m);

        //////////
        //  Operators
    public:
        /// \brief
        ///     Compares two time spans for equality.
        /// \param op2
        ///     The 2nd operand of comparison.
        /// \return
        ///     True if *this and op2 represent the same time
        ///     span, else false. An "invalid" time span compares
        ///     equal to another "invalid" time span and nor equal
        ///     to any valid time span.
        bool        operator == (const TimeSpan & op2) const { return _minutes == op2._minutes; }

        /// \brief
        ///     Compares two time spans for inequality.
        /// \param op2
        ///     The 2nd operand of comparison.
        /// \return
        ///     False if *this and op2 represent the same time
        ///     span, else true. An "invalid" time span compares
        ///     equal to another "invalid" time span and nor equal
        ///     to any valid time span.
        bool        operator != (const TimeSpan & op2) const { return _minutes != op2._minutes; }

        /// \brief
        ///     Compares two time spans for order.
        /// \param op2
        ///     The 2nd operand of comparison.
        /// \return
        ///     True if *this is algebraically "less than" op2,
        ///     else false. An "invalid" time span compares
        ///     equal to another "invalid" time span and less than
        ///     any valid time span.
        bool        operator <  (const TimeSpan & op2) const { return _minutes <  op2._minutes; }

        /// \brief
        ///     Compares two time spans for order.
        /// \param op2
        ///     The 2nd operand of comparison.
        /// \return
        ///     True if *this is algebraically "less than or equal to"
        ///     op2, else false. An "invalid" time span compares
        ///     equal to another "invalid" time span and less than
        ///     any valid time span.
        bool        operator <= (const TimeSpan & op2) const { return _minutes <= op2._minutes; }

        /// \brief
        ///     Compares two time spans for order.
        /// \param op2
        ///     The 2nd operand of comparison.
        /// \return
        ///     True if *this is algebraically "greater than" op2,
        ///     else false. An "invalid" time span compares
        ///     equal to another "invalid" time span and less than
        ///     any valid time span.
        bool        operator >  (const TimeSpan & op2) const { return _minutes >  op2._minutes; }

        /// \brief
        ///     Compares two time spans for order.
        /// \param op2
        ///     The 2nd operand of comparison.
        /// \return
        ///     True if *this is algebraically "greater than or
        ///     equal to" op2, else false. An "invalid" time span
        ///     compares equal to another "invalid" time span and
        ///     less than any valid time span.
        bool        operator >= (const TimeSpan & op2) const { return _minutes >= op2._minutes; }

        /// \brief
        ///     Negates a time span.
        /// \return
        ///     The time span with an opposite sign and same
        ///     magnitude as this one; "invalid" if this time
        ///     span is "invalid".
        TimeSpan    operator -  () const;

        /// \brief
        ///     Adds two time spans.
        /// \param op2
        ///     The 2nd operand of the addition.
        /// \return
        ///     The time span with the magnitude determined by
        ///     addition of magnitudes of two operands; "invalid"
        ///     time span if either opoerand is "invalid" or
        ///     an overfolow occurs.
        TimeSpan    operator +  (const TimeSpan & op2) const;

        /// \brief
        ///     Subtracts two time spans.
        /// \param op2
        ///     The 2nd operand of the subtraction.
        /// \return
        ///     The time span with the magnitude determined by
        ///     subtraction of magnitudes of two operands; "invalid"
        ///     time span if either opoerand is "invalid" or
        ///     an overfolow occurs.
        TimeSpan    operator -  (const TimeSpan & op2) const;

        /// \brief
        ///     Adds a time span to this one.
        /// \param op2
        ///     The 2nd operand of the addition.
        /// \return
        ///     The reference to this operand.
        ///     The magnitude of this operand is determined by
        ///     addition of magnitudes of two operands; "invalid"
        ///     time span if either opoerand is "invalid" or
        ///     an overfolow occurs.
        TimeSpan &  operator += (const TimeSpan & op2);

        /// \brief
        ///     Subtracts a time span from this one.
        /// \param op2
        ///     The 2nd operand of the subtraction.
        /// \return
        ///     The reference to this operand.
        ///     The magnitude of this operand is determined by
        ///     subtraction of magnitudes of two operands; "invalid"
        ///     time span if either opoerand is "invalid" or
        ///     an overfolow occurs.
        TimeSpan &  operator -= (const TimeSpan & op2);

        //////////
        //  Operations
    public:
        /// \brief
        ///     Checks whether this time span is "valid".
        /// \return
        ///     True if this time span is "valid", else false.
        bool        isValid() const { return _minutes != _InvalidMinutes; }

        /// \brief
        ///     Returns the number of full hours in this time span.
        /// \details
        ///     If this time span is "invalid", the call is an error.
        /// \return
        ///     The number of full hours in this time span.
        int         asHours() const { Q_ASSERT(isValid()); return _minutes / 60; }

        /// \brief
        ///     Returns the number of full minutes in this time span.
        /// \details
        ///     If this time span is "invalid", the call is an error.
        /// \return
        ///     The number of full minutes in this time span.
        int         asMinutes() const { Q_ASSERT(isValid()); return _minutes; }

        //////////
        //  Implementation
    private:
        static inline const int _InvalidMinutes = INT_MIN;
        int         _minutes;
    };

    /// \class DateTimeManager tt3-util/API.hpp
    /// \brief The date/time helper services.
    class TT3_UTIL_PUBLIC DateTimeManager final
    {
        TT3_UTILITY_CLASS(DateTimeManager)

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns user-readable display name of the
        ///     specified day-of-week.
        /// \param dayOfWeek
        ///     The day-of-week.
        /// \return
        ///     The user-readable display name of the specified
        ///     day-of-week for the current default locale.
        static QString  displayName(Qt::DayOfWeek dayOfWeek);
    };
}

//  End of tt3-util/DateTime.hpp
