//
//  tt3-report/PageSetup.cpp - tt3::report::PageSetup enum implementation
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
#include "tt3-report/API.hpp"
using namespace tt3::report;

//////////
//  Construction/destruction/assignment
PageSetup::PageSetup(
    ) : _pageWidth(TypographicSize(210, TypographicUnit::Millimeter)),
        _pageHeight(TypographicSize(297, TypographicUnit::Millimeter)),
        _pageOrientation(PageOrientation::Portrait),
        _leftMargin(TypographicSize(20, TypographicUnit::Millimeter)),
        _rightMargin(TypographicSize(20, TypographicUnit::Millimeter)),
        _topMargin(TypographicSize(20, TypographicUnit::Millimeter)),
        _bottomMargin(TypographicSize(20, TypographicUnit::Millimeter))
{
}

PageSetup::PageSetup(
        const TypographicSize & pageWidth,
        const TypographicSize & pageHeight,
        PageOrientation pageOrientation,
        const TypographicSize & leftMargin,
        const TypographicSize & rightMargin,
        const TypographicSize & topMargin,
        const TypographicSize & bottomMargin
    ) : _pageWidth(pageWidth),
        _pageHeight(pageHeight),
        _pageOrientation(pageOrientation),
        _leftMargin(leftMargin),
        _rightMargin(rightMargin),
        _topMargin(topMargin),
        _bottomMargin(bottomMargin)
{
}

//////////
//  Operators
bool PageSetup::operator == (const PageSetup & op2) const
{
    return _pageWidth == op2._pageWidth &&
           _pageHeight == op2._pageHeight &&
           _pageOrientation == op2._pageOrientation &&
           _leftMargin == op2._leftMargin &&
           _rightMargin == op2._rightMargin &&
           _topMargin == op2._topMargin &&
           _bottomMargin == op2._bottomMargin;
}

bool PageSetup::operator != (const PageSetup & op2) const
{
    return !(*this == op2);
}

//////////
//  Operations
bool PageSetup::isValid() const
{
    TypographicSize pageWidth, pageHeight;
    switch (_pageOrientation)
    {
        case PageOrientation::Landscape:
            pageWidth = std::max(_pageWidth, _pageHeight);
            pageHeight = std::min(_pageWidth, _pageHeight);
            break;
        case PageOrientation::Portrait:
        case PageOrientation::Default:
        default:    //  be defensive in release mode
            pageWidth = std::min(_pageWidth, _pageHeight);
            pageHeight = std::max(_pageWidth, _pageHeight);
            break;
    }
        return pageWidth.pointSize() > 0.0 &&
               pageHeight.pointSize() > 0.0 &&
               _leftMargin.pointSize() >= 0.0 &&
               _rightMargin.pointSize() >= 0.0 &&
               _topMargin.pointSize() >= 0.0 &&
               _bottomMargin.pointSize() >= 0.0 &&
               pageWidth.pointSize() > _leftMargin.pointSize() + _rightMargin.pointSize() &&
               pageHeight.pointSize() > _topMargin.pointSize() + _bottomMargin.pointSize();
}

//  Formatting/parsing
namespace
{
    void skipCharacter(const QString & s, qsizetype & scan, QChar c)
    {
        if (scan < s.length() && s[scan] == c)
        {
            scan++;
        }
        else
        {
            throw tt3::util::ParseException(s, scan);
        }
    }
}

namespace tt3::util
{
    template <> TT3_REPORT_PUBLIC
    QString toString<PageSetup>(
            const PageSetup & value
        )
    {   //  "pw*ph,po,lm/rm/tm/bm"
        return toString(value.pageWidth()) +
               "*" +
               toString(value.pageHeight()) +
               "," +
               toString(value.pageOrientation()) +
               "," +
               toString(value.leftMargin()) +
               "/" +
               toString(value.rightMargin()) +
               "/" +
               toString(value.topMargin()) +
               "/" +
               toString(value.bottomMargin());
    }

    template <> TT3_REPORT_PUBLIC
    auto fromString<PageSetup>(
            const QString & s,
            qsizetype & scan
        ) -> PageSetup
    {
        if (scan < 0 || scan > s.length())
        {   //  OOPS!
            throw ParseException(s, scan);
        }
        //  "pw*ph,po,lm/rm/tm/bm"
        qsizetype prescan = scan;
        auto pageWidth = fromString<TypographicSize>(s, prescan);
        skipCharacter(s, prescan, '*');
        auto pageHeight = fromString<TypographicSize>(s, prescan);
        skipCharacter(s, prescan, ',');
        auto pageOrientation = fromString<PageOrientation>(s, prescan);
        skipCharacter(s, prescan, ',');
        auto leftMargin = fromString<TypographicSize>(s, prescan);
        skipCharacter(s, prescan, '/');
        auto rightMargin = fromString<TypographicSize>(s, prescan);
        skipCharacter(s, prescan, '/');
        auto topMargin = fromString<TypographicSize>(s, prescan);
        skipCharacter(s, prescan, '/');
        auto bottomMargin = fromString<TypographicSize>(s, prescan);
        scan = prescan;
        return PageSetup(
            pageWidth, pageHeight,
            pageOrientation,
            leftMargin, rightMargin, topMargin, bottomMargin);
    }
}

//  End of tt3-report/PageSetup.cpp
