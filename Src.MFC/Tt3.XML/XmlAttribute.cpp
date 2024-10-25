//
//  XmlAttribute.cpp - Implementation of the carl::XmlAttribute class
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
XmlAttribute::XmlAttribute(XmlDocument * document)
    :   _document(document),
        _parentNode(nullptr)
{
    _document->_liveNodes.add(this);
}

XmlAttribute::~XmlAttribute() noexcept
{
    if (XmlElement * parentElement = dynamic_cast<XmlElement*>(_parentNode))
    {
        parentElement->_attributes.remove(this);
        _parentNode = nullptr;
    }
    else if (XmlProcessingInstruction * parentProcessingInstruction = dynamic_cast<XmlProcessingInstruction*>(_parentNode))
    {
        parentProcessingInstruction->_attributes.remove(this);
        _parentNode = nullptr;
    }
    require(_parentNode == nullptr);
    _document->_liveNodes.remove(this);
}

//////////
//  XmlNode
XmlDocument * XmlAttribute::getDocument() const
{
    return _document;
}

XmlNode * XmlAttribute::getParentNode() const
{
    return _parentNode;
}

XmlNodeList XmlAttribute::getChildNodes() const
{
    return XmlNodeList();
}

//////////
//  Operations
XmlElement * XmlAttribute::getParentElement() const
{
    return dynamic_cast<XmlElement*>(_parentNode);
}

XmlProcessingInstruction * XmlAttribute::getParentProcessingInstruction() const
{
    return dynamic_cast<XmlProcessingInstruction*>(_parentNode);
}

String XmlAttribute::getNamespaceUri() const
{
    return _namespaceUri;
}

void XmlAttribute::setNamespaceUri(const String & namespaceUri)
{
    //  No change is a special case
    if (namespaceUri == _namespaceUri)
    {
        return;
    }

    //  After assignment there must be no attribute duplication in parent element
    if (XmlElement * parentElement = dynamic_cast<XmlElement*>(_parentNode))
    {
        if (parentElement->findAttribute(namespaceUri, _name) != nullptr)
        {   //  OOPS! Already exists!
            throw XmlDomException("Attribute already exists");  //  TODO localize via resources
        }
    }
    else if (XmlProcessingInstruction * parentProcessingInstruction = dynamic_cast<XmlProcessingInstruction*>(_parentNode))
    {
        if (parentProcessingInstruction->findAttribute(namespaceUri, _name) != nullptr)
        {   //  OOPS! Already exists!
            throw XmlDomException("Attribute already exists");  //  TODO localize via resources
        }
    }

    //  Make the change
    _namespaceUri = namespaceUri;
}

String XmlAttribute::getName() const
{
    return _name;
}

void XmlAttribute::setName(const String & name)
{
    if (!Xml::isValidName(name))
    {   //  OOPS!
        throw XmlDomException("Invalid attribute name");    //  TODO localize via resources
    }

    //  No change is a special case
    if (name == _name)
    {
        return;
    }

    //  After assignment there must be no attribute duplication in parent element
    if (XmlElement * parentElement = dynamic_cast<XmlElement*>(_parentNode))
    {
        if (parentElement->findAttribute(_namespaceUri, name) != nullptr)
        {   //  OOPS! Already exists!
            throw XmlDomException("Attribute already exists");  //  TODO localize via resources
        }
    }
    else if (XmlProcessingInstruction * parentProcessingInstruction = dynamic_cast<XmlProcessingInstruction*>(_parentNode))
    {
        if (parentProcessingInstruction->findAttribute(_namespaceUri, name) != nullptr)
        {   //  OOPS! Already exists!
            throw XmlDomException("Attribute already exists");  //  TODO localize via resources
        }
    }

    //  Make the change
    _name = name;
}

String XmlAttribute::getValue() const
{
    return _value;
}

void XmlAttribute::setValue(const String & value)
{
    _value = value;
}

//  End of XmlAttribute.cpp
