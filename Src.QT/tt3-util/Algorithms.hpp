//
//  tt3-util/Algorithms.cpp - Helper algorithms
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
    /// \brief
    ///     Transforms a QSet by applying the specified
    ///     operation to its every element.
    /// \details
    ///     If several elements of the source set map to the
    ///     same destination value, it is only included into
    ///     the result once.
    /// \param src
    ///     The source set.
    /// \param op
    ///     The operation that maps source set elements to destination
    ///     set elements.
    /// \return
    ///     The destination set.
    template <typename S, typename Func>
    decltype(auto) transform(const QSet<S> & src, Func op)
    {
        QSet<decltype(op(S()))> dst;
        dst.reserve(src.size());
        for (auto e : src)
        {
            dst.insert(op(e));
        }
        return dst;
    }

    /// \brief
    ///     Transforms a QList by applying the specified
    ///     operation to its every element.
    /// \param src
    ///     The source list.
    /// \param op
    ///     The operation that maps source list elements to destination
    ///     list elements.
    /// \return
    ///     The destination list.
    template <typename S, typename Func>
    decltype(auto) transform(const QList<S> & src, Func op)
    {
        QList<decltype(op(S()))> dst;
        dst.reserve(src.size());
        for (auto e : src)
        {
            dst.append(op(e));
        }
        return dst;
    }

    /// \brief
    ///     Returns all duplicates from the list, returning the result.
    /// \param
    ///     The original list, possible containing duplicates.
    /// \return
    ///     The result of duplicates removal.
    template <typename T>
    QList<T> unique(const QList<T> & src)
    {
        QList<T> dst;
        dst.reserve(src.size());
        for (auto e : src)
        {
            if (!dst.contains(e))
            {
                dst.append(e);
            }
        }
        return dst;
    }

    /// \class NaturalStringOrder tt3-util/API.hpp
    /// \brief Implements natural string order.
    class TT3_UTIL_PUBLIC NaturalStringOrder final
    {
        TT3_UTILITY_CLASS(NaturalStringOrder)

        //////////
        //  Operations
    public:
        /// \brief
        ///     Compares two strings using natural order.
        /// \param a
        ///     The 1st string to compare.
        /// \param b
        ///     The 2nd string to compare.
        /// \return
        ///     True if the 1st string is "naturally less"
        ///     that the 2nd string, else false.
        static bool     less(const QString & a, const QString & b);
    };
}

//  End of tt3-util/Algorithms.cpp
