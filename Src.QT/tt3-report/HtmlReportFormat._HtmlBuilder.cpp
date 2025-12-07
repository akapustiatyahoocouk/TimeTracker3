//
//  tt3-report/HtmlReportFormat._HtmlBuilder.cpp - tt3::report::HtmlReportFormat::_HtmlBuilder class implementation
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
using HRG = HtmlReportFormat;

//////////
//  Construction/destruction
HRG::_HtmlBuilder::_HtmlBuilder()
{
    reset();
}

HRG::_HtmlBuilder::~_HtmlBuilder()
{
    reset();    //  to chean up
}

//////////
//  Operations
void HRG::_HtmlBuilder::reset()
{
    _htmlAccumulator.clear();
    _spanAccumulator.clear();
    _openTags.clear();
}

void HRG::_HtmlBuilder::openTag(
        const QString & tagName
    )
{
    Q_ASSERT(_isValidTagName(tagName));

    QString tag = "<" + tagName + ">";
    if (_isSpanTag(tagName))
    {   //  Add to "span data"
        _spanAccumulator += tag;
    }
    else
    {   //  Commit "span data" & open a new div tag
        _commitSpanData();
        _htmlAccumulator += _indent(_openTags.size());
        _htmlAccumulator += tag;
        _htmlAccumulator += "\n";
    }

    _openTags.push(tagName);
}

void HRG::_HtmlBuilder::openTag(
        const QString & tagName,
        const QString & attributeName1,
        const QString & attributeValue1
    )
{
    Q_ASSERT(_isValidTagName(tagName));
    Q_ASSERT(_isValidAttributeName(attributeName1));
    Q_ASSERT(_isValidAttributeValue(attributeValue1));

    QString tag =
        "<" + tagName + " " +
        attributeName1 + "=\"" + _escapeAttributeValue(attributeValue1) + "\"" +
        ">";
    if (_isSpanTag(tagName))
    {   //  Add to "span data"
        _spanAccumulator += tag;
    }
    else
    {   //  Commit "span data" & open a new div tag
        _commitSpanData();
        _htmlAccumulator += _indent(_openTags.size());
        _htmlAccumulator += tag;
        _htmlAccumulator += "\n";
    }

    _openTags.push(tagName);
}

void HRG::_HtmlBuilder::openTag(
        const QString & tagName,
        const QString & attributeName1,
        const QString & attributeValue1,
        const QString & attributeName2,
        const QString & attributeValue2
    )
{
    Q_ASSERT(_isValidTagName(tagName));
    Q_ASSERT(_isValidAttributeName(attributeName1));
    Q_ASSERT(_isValidAttributeValue(attributeValue1));
    Q_ASSERT(_isValidAttributeName(attributeName2));
    Q_ASSERT(_isValidAttributeValue(attributeValue2));

    QString tag =
        "<" + tagName + " " +
        attributeName1 + "=\"" + _escapeAttributeValue(attributeValue1) + "\" " +
        attributeName2 + "=\"" + _escapeAttributeValue(attributeValue2) + "\"" +
        ">";
    if (_isSpanTag(tagName))
    {   //  Add to "span data"
        _spanAccumulator += tag;
    }
    else
    {   //  Commit "span data" & open a new div tag
        _commitSpanData();
        _htmlAccumulator += _indent(_openTags.size());
        _htmlAccumulator += tag;
        _htmlAccumulator += "\n";
    }

    _openTags.push(tagName);
}

void HRG::_HtmlBuilder::openTag(
        const QString & tagName,
        const QString & attributeName1,
        const QString & attributeValue1,
        const QString & attributeName2,
        const QString & attributeValue2,
        const QString & attributeName3,
        const QString & attributeValue3
    )
{
    Q_ASSERT(_isValidTagName(tagName));
    Q_ASSERT(_isValidAttributeName(attributeName1));
    Q_ASSERT(_isValidAttributeValue(attributeValue1));
    Q_ASSERT(_isValidAttributeName(attributeName2));
    Q_ASSERT(_isValidAttributeValue(attributeValue2));
    Q_ASSERT(_isValidAttributeName(attributeName3));
    Q_ASSERT(_isValidAttributeValue(attributeValue3));

    QString tag =
        "<" + tagName + " " +
        attributeName1 + "=\"" + _escapeAttributeValue(attributeValue1) + "\" " +
        attributeName2 + "=\"" + _escapeAttributeValue(attributeValue2) + "\"" +
        attributeName3 + "=\"" + _escapeAttributeValue(attributeValue3) + "\"" +
        ">";
    if (_isSpanTag(tagName))
    {   //  Add to "span data"
        _spanAccumulator += tag;
    }
    else
    {   //  Commit "span data" & open a new div tag
        _commitSpanData();
        _htmlAccumulator += _indent(_openTags.size());
        _htmlAccumulator += tag;
        _htmlAccumulator += "\n";
    }

    _openTags.push(tagName);
}

void HRG::_HtmlBuilder::openTag(
        const QString & tagName,
        const QString & attributeName1,
        const QString & attributeValue1,
        const QString & attributeName2,
        const QString & attributeValue2,
        const QString & attributeName3,
        const QString & attributeValue3,
        const QString & attributeName4,
        const QString & attributeValue4
    )
{
    Q_ASSERT(_isValidTagName(tagName));
    Q_ASSERT(_isValidAttributeName(attributeName1));
    Q_ASSERT(_isValidAttributeValue(attributeValue1));
    Q_ASSERT(_isValidAttributeName(attributeName2));
    Q_ASSERT(_isValidAttributeValue(attributeValue2));
    Q_ASSERT(_isValidAttributeName(attributeName3));
    Q_ASSERT(_isValidAttributeValue(attributeValue3));
    Q_ASSERT(_isValidAttributeName(attributeName4));
    Q_ASSERT(_isValidAttributeValue(attributeValue4));

    QString tag =
        "<" + tagName + " " +
        attributeName1 + "=\"" + _escapeAttributeValue(attributeValue1) + "\" " +
        attributeName2 + "=\"" + _escapeAttributeValue(attributeValue2) + "\"" +
        attributeName3 + "=\"" + _escapeAttributeValue(attributeValue3) + "\"" +
        attributeName4 + "=\"" + _escapeAttributeValue(attributeValue4) + "\"" +
        ">";
    if (_isSpanTag(tagName))
    {   //  Add to "span data"
        _spanAccumulator += tag;
    }
    else
    {   //  Commit "span data" & open a new div tag
        _commitSpanData();
        _htmlAccumulator += _indent(_openTags.size());
        _htmlAccumulator += tag;
        _htmlAccumulator += "\n";
    }

    _openTags.push(tagName);
}

void HRG::_HtmlBuilder::closeTag(
        const QString & tagName
    )
{
    Q_ASSERT(_isValidTagName(tagName));
    Q_ASSERT(!_openTags.isEmpty() && _openTags.top() == tagName);

    _openTags.pop();

    QString tag = "</" + tagName + ">";
    if (_isSpanTag(tagName))
    {   //  Add to "span data"
        _spanAccumulator += tag;
    }
    else
    {   //  Commit "span data" & open a new div tag
        _commitSpanData();
        _htmlAccumulator += _indent(_openTags.size());
        _htmlAccumulator += tag;
        _htmlAccumulator += "\n";
    }
}

void HRG::_HtmlBuilder::writeTag(
        const QString & tagName
    )
{
    Q_ASSERT(_isValidTagName(tagName));

    QString tag = "<" + tagName + "/>";
    if (_isSpanTag(tagName))
    {   //  Add to "span data"
        _spanAccumulator += tag;
    }
    else
    {   //  Commit "span data" & open a new div tag
        _commitSpanData();
        _htmlAccumulator += _indent(_openTags.size());
        _htmlAccumulator += tag;
        _htmlAccumulator += "\n";
    }
}

void HRG::_HtmlBuilder::writeTag(
        const QString & tagName,
        const QString & attributeName1,
        const QString & attributeValue1
    )
{
    Q_ASSERT(_isValidTagName(tagName));
    Q_ASSERT(_isValidAttributeName(attributeName1));
    Q_ASSERT(_isValidAttributeValue(attributeValue1));

    QString tag =
        "<" + tagName + " " +
        attributeName1 + "=\"" + _escapeAttributeValue(attributeValue1) + "\"" +
        "/>";
    if (_isSpanTag(tagName))
    {   //  Add to "span data"
        _spanAccumulator += tag;
    }
    else
    {   //  Commit "span data" & open a new div tag
        _commitSpanData();
        _htmlAccumulator += _indent(_openTags.size());
        _htmlAccumulator += tag;
        _htmlAccumulator += "\n";
    }
}

void HRG::_HtmlBuilder::writeText(
        const QString & text
    )
{
    QString escapedText = _escapeText(text);
    if (!escapedText.isEmpty())
    {
        if (_spanAccumulator.length() != 0)
        {
            _spanAccumulator += ' ';
        }
        _spanAccumulator += escapedText;
    }
}

QString HRG::_HtmlBuilder::html()
{
    _commitSpanData();
    return _htmlAccumulator;
}

//////////
//  Implementation helpers
bool HRG::_HtmlBuilder::_isSpanTag(const QString & tagName)
{
    return tagName.compare("span", Qt::CaseInsensitive) == 0 ||
           tagName.compare("a", Qt::CaseInsensitive) == 0;
}

void HRG::_HtmlBuilder::_commitSpanData()
{
    QString spanData = _spanAccumulator.trimmed();
    _spanAccumulator.clear();
    if (!spanData.isEmpty())
    {   //  Indent + data TODO break "spanData" into multiple lines if too long
        _htmlAccumulator += _indent(_openTags.size() + 1);
        _htmlAccumulator += spanData;
        _htmlAccumulator += "\n";
    }
}

QString HRG::_HtmlBuilder::_indent(qsizetype level)
{
    return QString(level * 2, ' ');
}

bool HRG::_HtmlBuilder::_isTagNameStart(QChar c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool HRG::_HtmlBuilder::_isTagNameChar(QChar c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');    //  HTML does not use digits, underscors, etc. in tag names
}

bool HRG::_HtmlBuilder::_isValidTagName(const QString & tagName)
{
    if (tagName.length() == 0)
    {
        return false;
    }
    else if (!_isTagNameStart(tagName[0]))
    {
        return false;
    }
    else
    {
        for (int i = 1; i < tagName.length(); i++)
        {
            if (!_isTagNameChar(tagName[i]))
            {
                return false;
            }
        }
        return true;
    }
}

bool HRG::_HtmlBuilder::_isAttributeNameStart(QChar c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool HRG::_HtmlBuilder::_isAttributeNameChar(QChar c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
           (c >= '0' && c <= '9') || (c == '-' || c == '_' || c == '.');
}

bool HRG::_HtmlBuilder::_isValidAttributeName(const QString & attributeName)
{
    if (attributeName.length() == 0)
    {
        return false;
    }
    else if (!_isAttributeNameStart(attributeName[0]))
    {
        return false;
    }
    else
    {
        for (int i = 1; i < attributeName.length(); i++)
        {
            if (!_isAttributeNameChar(attributeName[i]))
            {
                return false;
            }
        }
        return true;
    }
}

bool HRG::_HtmlBuilder::_isValidAttributeValue(const QString & /*attributeValue*/)
{
    return true;
}

QString HRG::_HtmlBuilder::_escapeAttributeValue(
        const QString & value
    )
{
    QString result;
    for (int i = 0; i < value.length(); i++)
    {
        QChar c = value[i];
        switch (c.unicode())
        {
            case '<':
                result.append("&lt;");
                break;
            case '>':
                result += "&gt;";
                break;
            case '&':
                result += "&amp;";
                break;
            case '\'':
                result += "&apos;";
                break;
            case '"':
                result += "&quot;";
                break;
            case L'\u00A0':
                result += "&nbsp;";
                break;
            case L'\u2022':
                result += "&bull;";
                break;
            default:
                if (c < '\x20' || c == '\x7F')
                {   //  ASCII control
                    char s[8];
                    sprintf(s, "&#%d", c.unicode());
                    result += s;
                }
                else
                {   //  Just a character
                    result += c;
                }
                break;
        }
    }
    return result;
}

QString HRG::_HtmlBuilder::_escapeText(
        const QString & text
    )
{
    return _escapeAttributeValue(text); //  TODO really ?
}

//  End of tt3-report/HtmlReportFormat._HtmlBuilder.cpp
