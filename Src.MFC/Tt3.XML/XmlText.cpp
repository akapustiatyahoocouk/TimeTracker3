//
//  XmlText.cpp - Implementation of the carl::XmlText class
//
//  Copyright (C) 2010-2021, Cybernetic Intelligence GmbH
//
//  $Date: 2021-02-27 00:29:32 +0000 (Sat, 27 Feb 2021) $
//  $Revision: 861 $
//  $Author: akapusti $
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3.0 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//////////
#include "CARL.hpp"
using namespace carl;

//////////
//  Construction/destruction
XmlText::XmlText(XmlDocument * document)
    :   _document(document),
        _parentElement(nullptr)
{
    _document->_liveNodes.add(this);
}

XmlText::~XmlText() noexcept
{
    if (_parentElement != nullptr)
    {
        _parentElement->_innerNodes.remove(this);
        _parentElement = nullptr;
    }
    _document->_liveNodes.remove(this);
}

//////////
//  XmlNode
XmlDocument * XmlText::getDocument() const
{
    return _document;
}

XmlNode * XmlText::getParentNode() const
{
    return _parentElement;
}

XmlNodeList XmlText::getChildNodes() const
{
    return XmlNodeList();
}

//////////
//  Operations
XmlElement * XmlText::getParentElement() const
{
    return _parentElement;
}

String XmlText::getText() const
{
    return _text;
}

void XmlText::setText(const String & text)
{
    _text = text;
}

//  End of XmlText.cpp
