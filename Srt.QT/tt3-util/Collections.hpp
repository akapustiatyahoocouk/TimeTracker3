//
//  tt3-util/Collections.hpp - Collections and collection helpers
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
    template <class T> QSet<T> toSet(const QList<T> & list)
    {
        return QSet(list.begin(), list.end());
    }

    template <class T> QList<T> toList(const QSet<T> & set)
    {
        return QList(set.begin(), set.end());
    }
}

//  End of tt3-util/Collections.hpp
