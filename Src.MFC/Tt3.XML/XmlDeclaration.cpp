//
//  XmlDeclaration.cpp - Implementation of the carl::XmlDeclaration class
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
XmlDeclaration::XmlDeclaration(XmlDocument * document)
    :   _document(document),
        _parentDocument(nullptr),
        _version(XmlVersion::Default),
        _encoding(CharacterSet::getDefault()),
        _standalone(true)
{
    _document->_liveNodes.add(this);
}

XmlDeclaration::~XmlDeclaration() noexcept
{
    if (_parentDocument != nullptr)
    {
        _parentDocument->_xmlDeclaration = nullptr;
        _parentDocument = nullptr;
    }
    _document->_liveNodes.remove(this);
}

//////////
//  XmlNode
XmlDocument * XmlDeclaration::getDocument() const
{
    return _document;
}

XmlNode * XmlDeclaration::getParentNode() const
{
    return _parentDocument;
}

XmlNodeList XmlDeclaration::getChildNodes() const
{
    return XmlNodeList();
}

//////////
//  Operations
XmlVersion XmlDeclaration::getVersion() const
{
    return _version;
}

void XmlDeclaration::setVersion(XmlVersion version)
{
    _version = version;
}

CharacterSet * XmlDeclaration::getEncoding() const
{
    return _encoding;
}

void XmlDeclaration::setEncoding(CharacterSet * encoding)
{
    require(encoding != nullptr);

    _encoding = encoding;
}

bool XmlDeclaration::isStandalone() const
{
    return _standalone;
}

void XmlDeclaration::setStandalone(bool standalone)
{
    _standalone = standalone;
}

//  End of XmlDeclaration.cpp
