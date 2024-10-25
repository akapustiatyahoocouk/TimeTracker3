//
//  XmlProcessingInstruction.cpp - Implementation of the carl::XmlProcessingInstruction class
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
//  Construction (internal only)/destruction
XmlProcessingInstruction::XmlProcessingInstruction(XmlDocument * document)
    :   _document(document),
        _parentElement(nullptr)
{
    _document->_liveNodes.add(this);
}

XmlProcessingInstruction::~XmlProcessingInstruction() noexcept
{
    while (!_attributes.isEmpty())
    {   //  TODO optimize for speed
        delete _attributes[0];
    }

    if (_parentElement != nullptr)
    {
        _parentElement->_innerNodes.remove(this);
        _parentElement = nullptr;
    }
    _document->_liveNodes.remove(this);
}

//////////
//  XmlNode
XmlDocument * XmlProcessingInstruction::getDocument() const
{
    return _document;
}

XmlNode * XmlProcessingInstruction::getParentNode() const
{
    return _parentElement;
}

XmlNodeList XmlProcessingInstruction::getChildNodes() const
{
    XmlNodeList result;
    result.addAll(_attributes);
    return result;
}

//////////
//  Operations
XmlElement * XmlProcessingInstruction::getParentElement() const
{
    return _parentElement;
}

XmlAttributeList XmlProcessingInstruction::getAttributes() const
{
    return _attributes;
}

String XmlProcessingInstruction::getTarget() const
{
    return _target;
}

void XmlProcessingInstruction::setTarget(const String & target)
{
    if (!Xml::isValidName(target))
    {   //  OOPS!
        throw XmlDomException("Invalid target");    //  TODO localize via resources
    }

    _target = target;
}

XmlAttribute * XmlProcessingInstruction::findAttribute(const String & name) const
{
    for (XmlAttribute * attribute : _attributes)
    {
        if (attribute->getName() == name)
        {
            return attribute;
        }
    }
    return nullptr;
}

XmlAttribute * XmlProcessingInstruction::findAttribute(const String & namespaceUri, const String & name) const
{
    for (XmlAttribute * attribute : _attributes)
    {
        if (attribute->getName() == name && attribute->getNamespaceUri() == namespaceUri)
        {
            return attribute;
        }
    }
    return nullptr;
}

void XmlProcessingInstruction::addAttribute(XmlAttribute * xmlAttribute)
{
    require(xmlAttribute != nullptr);
    require(xmlAttribute->_document == _document);
    require(xmlAttribute->_parentNode == nullptr);

    if (findAttribute(xmlAttribute->getNamespaceUri(), xmlAttribute->getName()) != nullptr)
    {   //  OOPS!
        throw XmlDomException("Attribute already exists");  //  TODO localize via resources
    }

    _attributes.add(xmlAttribute);
    xmlAttribute->_parentNode = this;
}

void XmlProcessingInstruction::setAttribute(const String & name, const String & value)
{
    setAttribute(String(), name, value);
}

void XmlProcessingInstruction::setAttribute(const String & namespaceUri, const String & name, const String & value)
{
    XmlAttribute * xmlAttribute = findAttribute(namespaceUri, name);
    if (xmlAttribute != nullptr)
    {   //  Change value
        xmlAttribute->setValue(value);
    }
    else
    {   //  Create attribute
        xmlAttribute = _document->createAttribute(namespaceUri, name, value);
        addAttribute(xmlAttribute);
    }
}

//  End of XmlProcessingInstruction.cpp
