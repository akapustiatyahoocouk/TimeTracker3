//
//  XmlElement.cpp - Implementation of the carl::XmlElement class
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
XmlElement::XmlElement(XmlDocument * document)
    :   _document(document),
        _parentNode(nullptr)
{
    _document->_liveNodes.add(this);
}

XmlElement::~XmlElement() noexcept
{
    while (!_attributes.isEmpty())
    {   //  TODO optimize for speed
        delete _attributes[0];
    }
    while (!_innerNodes.isEmpty())
    {   //  TODO optimize for speed
        delete _innerNodes[0];
    }

    if (XmlDocument * document = dynamic_cast<XmlDocument*>(_parentNode))
    {
        document->_rootElement = nullptr;
        _parentNode = nullptr;
    }
    else if (XmlElement * element = dynamic_cast<XmlElement*>(_parentNode))
    {
        element->_innerNodes.remove(this);
        _parentNode = nullptr;
    }
    require(_parentNode == nullptr);
    _document->_liveNodes.remove(this);
}

//////////
//  XmlNode
XmlDocument * XmlElement::getDocument() const
{
    return _document;
}

XmlNode * XmlElement::getParentNode() const
{
    return _parentNode;
}

XmlNodeList XmlElement::getChildNodes() const
{
    XmlNodeList result;
    result.addAll(_attributes);
    result.addAll(_innerNodes);
    return result;
}

//////////
//  Operations
XmlElement * XmlElement::getParentElement() const
{
    return dynamic_cast<XmlElement*>(_parentNode);
}

XmlAttributeList XmlElement::getAttributes() const
{
    return _attributes;
}

XmlNodeList XmlElement::getInnerNodes() const
{
    return _innerNodes;
}

XmlElementList XmlElement::getChildElements() const
{
    XmlElementList result;
    for (XmlNode * innerNode : _innerNodes)
    {
        if (XmlElement * innerElement = dynamic_cast<XmlElement*>(innerNode))
        {
            result.add(innerElement);
        }
    }
    return result;
}

XmlElementList XmlElement::getChildElements(const String & tagName) const
{
    XmlElementList result;
    for (XmlNode * innerNode : _innerNodes)
    {
        if (XmlElement * innerElement = dynamic_cast<XmlElement*>(innerNode))
        {
            if (innerElement->getTagName() == tagName)
            {
                result.add(innerElement);
            }
        }
    }
    return result;
}

String XmlElement::getNamespaceUri() const
{
    return _namespaceUri;
}

void XmlElement::setNamespaceUri(const String & namespaceUri)
{
    _namespaceUri = namespaceUri;
}

String XmlElement::getTagName() const
{
    return _tagName;
}

void XmlElement::setTagName(const String & tagName)
{
    if (!Xml::isValidName(tagName))
    {   //  OOPS!
        throw XmlDomException("Invalid tag name");  //  TODO localize via resources
    }

    _tagName = tagName;
}

XmlAttribute * XmlElement::findAttribute(const String & name) const
{
    for (XmlAttribute * attribute : _attributes)
    {
        if (attribute->getName() == name && attribute->getNamespaceUri().isEmpty())
        {
            return attribute;
        }
    }
    return nullptr;
}

XmlAttribute * XmlElement::findAttribute(const String & namespaceUri, const String & name) const
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

String XmlElement::getAttributeValue(const String & name, const String & defaultValue) const
{
    XmlAttribute * xmlAttribute = findAttribute(name);
    return (xmlAttribute != nullptr) ? xmlAttribute->getValue() : defaultValue;
}

String XmlElement::getAttributeValue(const String & namespaceUri, const String & name, const String & defaultValue) const
{
    XmlAttribute * xmlAttribute = findAttribute(namespaceUri, name);
    return (xmlAttribute != nullptr) ? xmlAttribute->getValue() : defaultValue;
}

void XmlElement::addAttribute(XmlAttribute * xmlAttribute)
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

void XmlElement::setAttribute(const String & name, const String & value)
{
    setAttribute(String(), name, value);
}

void XmlElement::setAttribute(const String & namespaceUri, const String & name, const String & value)
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

void XmlElement::addElement(XmlElement * xmlElement)
{
    require(xmlElement != nullptr && xmlElement != this);
    require(xmlElement->_document == _document);
    require(xmlElement->_parentNode == nullptr);

    _innerNodes.add(xmlElement);
    xmlElement->_parentNode = this;
}

void XmlElement::addText(XmlText * xmlText)
{
    require(xmlText != nullptr);
    require(xmlText->_document == _document);
    require(xmlText->_parentElement == nullptr);

    _innerNodes.add(xmlText);
    xmlText->_parentElement = this;
}

void XmlElement::addComment(XmlComment * xmlComment)
{
    require(xmlComment != nullptr);
    require(xmlComment->_document == _document);
    require(xmlComment->_parentElement == nullptr);

    _innerNodes.add(xmlComment);
    xmlComment->_parentElement = this;
}

void XmlElement::addProcessingInstruction(XmlProcessingInstruction * xmlProcessingInstruction)
{
    require(xmlProcessingInstruction != nullptr);
    require(xmlProcessingInstruction->_document == _document);
    require(xmlProcessingInstruction->_parentElement == nullptr);

    _innerNodes.add(xmlProcessingInstruction);
    xmlProcessingInstruction->_parentElement = this;
}

//  End of XmlElement.cpp
