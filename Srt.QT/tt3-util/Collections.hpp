//
//  tt3-util/Collections.cpp - Collections helpers
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
    template <class D, class S>
    QSet<D> transform(const QSet<S> & src,
                      std::function<D(S)> op)
    {
        QSet<D> dst;
        for (auto e : src)
        {
            dst.insert(op(e));
        }
        return dst;
    }
}

//  End of tt3-util/Collections.cpp
