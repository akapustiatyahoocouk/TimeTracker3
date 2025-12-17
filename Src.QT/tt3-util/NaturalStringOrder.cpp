//
//  tt3-util/NaturalStringOrder.cpp - the tt3::til::NaturalStringOrder class implementation
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
#include "tt3-util/API.hpp"
using namespace tt3::util;

namespace
{
    class CiCollator : public QCollator
    {
    public:
        CiCollator()
        {
            setCaseSensitivity(Qt::CaseInsensitive);
            setNumericMode(true);
        }
    };

    class CsCollator : public QCollator
    {
    public:
        CsCollator()
        {
            setCaseSensitivity(Qt::CaseSensitive);
            setNumericMode(true);
        }
    };
}

//////////
//  Operations
bool NaturalStringOrder::less(const QString & a, const QString & b)
{
    static CiCollator ciCollator;
    static CsCollator csCollator;

    if (int ciDiff = ciCollator.compare(a, b))
    {   //  < 0 or > 0, not == 0
        return ciDiff < 0;
    }
    //  "aA" < "aa"!
    return csCollator.compare(a, b) < 0;
}

//  End of tt3-util/NaturalStringOrder.cpp
