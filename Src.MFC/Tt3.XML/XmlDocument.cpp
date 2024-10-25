//
//  XmlDocument.cpp - Implementation of the carl::XmlDocument class
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

namespace
{
    class DomReaderContentHandler final : public XmlContentHandler
    {
        CARL_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(DomReaderContentHandler)

        //////////
        //  Construction/destruction
    public:
        DomReaderContentHandler() : _xmlDocument(nullptr) {}
        virtual ~DomReaderContentHandler() = default;

        //////////
        //  XmlContentHandler
    public:
        virtual void            beginDocument(XmlVersion version, CharacterSet * encoding, bool standalone) override;
        virtual void            endDocument() override;
        virtual void            beginElement(const String & namespaceUri, const String & tagName, const AttributeList & attributes) override;
        virtual void            endElement(const String & namespaceUri, const String & tagName) override;
        virtual void            text(const String & text) override;
        virtual void            comment(const String & text) override;
        virtual void            processingInstruction(const String & target, const AttributeList & attributes) override;

        //////////
        //  Operations
    public:
        XmlDocument *           grabXmlDocument();

        //////////
        //  Implementation
    private:
        XmlDocument *           _xmlDocument;   //  being parsed
        Stack<XmlElement*>      _openElementsStack;
    };

    //////////
    //  XmlContentHandler
    void DomReaderContentHandler::beginDocument(XmlVersion version, CharacterSet * encoding, bool standalone)
    {
        _xmlDocument = new XmlDocument();
        XmlDeclaration * xmlDeclaration = _xmlDocument->createXmlDeclaration(version, encoding, standalone);
        _xmlDocument->setXmlDeclaration(xmlDeclaration);
    }

    void DomReaderContentHandler::endDocument()
    {
    }

    void DomReaderContentHandler::beginElement(const String & namespaceUri, const String & tagName, const AttributeList & attributes)
    {
        XmlElement * xmlElement = _xmlDocument->createElement(namespaceUri, tagName);
        for (Attribute attribute : attributes)
        {
            xmlElement->setAttribute(attribute.namespaceUri, attribute.name, attribute.value);
        }
        if (_openElementsStack.isEmpty())
        {   //  This is the root element of the XML document
            _xmlDocument->setRootElement(xmlElement);
        }
        else
        {   //  This XML element has a parent element
            _openElementsStack.top()->addElement(xmlElement);
        }
        _openElementsStack.push(xmlElement);
    }

    void DomReaderContentHandler::endElement(const String & namespaceUri, const String & tagName)
    {
        require(!_openElementsStack.isEmpty());
        require(_openElementsStack.top()->getNamespaceUri() == namespaceUri);
        require(_openElementsStack.top()->getTagName() == tagName);

        CARL_UNUSED_PARAMETER(namespaceUri);    //  TODO really ?
        CARL_UNUSED_PARAMETER(tagName);

        _openElementsStack.pop();
    }

    void DomReaderContentHandler::text(const String & textParam)
    {
        require(!_openElementsStack.isEmpty());

        XmlText * xmlText = _xmlDocument->createText(textParam);
        _openElementsStack.top()->addText(xmlText);
    }

    void DomReaderContentHandler::comment(const String & textParam)
    {
        require(!_openElementsStack.isEmpty());

        XmlComment * xmlComment = _xmlDocument->createComment(textParam);
        _openElementsStack.top()->addComment(xmlComment);
    }

    void DomReaderContentHandler::processingInstruction(const String & target, const AttributeList & attributes)
    {
        require(!_openElementsStack.isEmpty());

        XmlProcessingInstruction * xmlProcessingInstruction = _xmlDocument->createProcessingInstruction(target);
        for (Attribute attribute : attributes)
        {
            xmlProcessingInstruction->setAttribute(attribute.namespaceUri, attribute.name, attribute.value);
        }
        _openElementsStack.top()->addProcessingInstruction(xmlProcessingInstruction);
    }

    //////////
    //  Operations
    XmlDocument * DomReaderContentHandler::grabXmlDocument()
    {
        XmlDocument * result = _xmlDocument;
        _xmlDocument = nullptr;
        return result;
    }
}

//////////
//  Construction/destruction/assignment
XmlDocument::XmlDocument()
    :   _xmlDeclaration(nullptr),
        _rootElement(nullptr)
{
}

XmlDocument::XmlDocument(const XmlDocument & src)
    :   XmlNode(),
        _xmlDeclaration(nullptr),
        _rootElement(nullptr)
{
    try
    {
        _xmlDeclaration = _cloneXmlDeclarationForMyself(src._xmlDeclaration);
        _rootElement = _cloneXmlElementForMyself(src._rootElement);
    }
    catch (...)
    {   //  Cleanup before re-throw
        delete _xmlDeclaration;
        delete _rootElement;
        throw;
    }
}

XmlDocument::~XmlDocument() noexcept
{
    //  Delete children
    delete _xmlDeclaration;
    delete _rootElement;

    //  Make sure no nodes are left live in the client code
    require(_liveNodes.size() == 0);
}

XmlDocument & XmlDocument::operator = (const XmlDocument & src)
{
    if (this == &src)
    {
        return *this;
    }

    //  Clone "src"'s nodes for "*this"
    XmlDeclaration * myXmlDeclaration = nullptr;
    XmlElement * myRootElement = nullptr;
    try
    {
        myXmlDeclaration = _cloneXmlDeclarationForMyself(src._xmlDeclaration);
        myRootElement = _cloneXmlElementForMyself(src._rootElement);
    }
    catch (...)
    {   //  Cleanup before re-throw
        delete myXmlDeclaration;
        delete myRootElement;
        throw;
    }

    //  Replace nodes - can't throw
    delete _xmlDeclaration;
    delete _rootElement;
    setXmlDeclaration(myXmlDeclaration);
    setRootElement(myRootElement);

    //  Done
    return *this;
}

//////////
//  XmlNode
XmlDocument * XmlDocument::getDocument() const
{
    return const_cast<XmlDocument*>(this);
}

XmlNode * XmlDocument::getParentNode() const
{
    return nullptr;
}

XmlNodeList XmlDocument::getChildNodes() const
{
    XmlNodeList result;

    if (_xmlDeclaration != nullptr)
    {
        result.add(_xmlDeclaration);
    }
    if (_rootElement != nullptr)
    {
        result.add(_rootElement);
    }
    return result;
}

//////////
//  Operations
XmlDeclaration * XmlDocument::getXmlDeclaration() const
{
    return _xmlDeclaration;
}

void XmlDocument::setXmlDeclaration(XmlDeclaration * xmlDeclaration)
{
    require(xmlDeclaration == nullptr ||
            (xmlDeclaration->_document == this &&
             xmlDeclaration->_parentDocument == nullptr));

    //  The previous XML declaration loses its parent...
    if (_xmlDeclaration != nullptr)
    {
        _xmlDeclaration->_parentDocument = nullptr;
    }
    //  ...and is replaced with new XML declaration...
    _xmlDeclaration = xmlDeclaration;
    //  ...which now has a parent
    if (_xmlDeclaration != nullptr)
    {
        _xmlDeclaration->_parentDocument = this;
    }
}

XmlElement * XmlDocument::getRootElement() const
{
    return _rootElement;
}

void XmlDocument::setRootElement(XmlElement * rootElement)
{
    require(rootElement == nullptr ||
            (rootElement->_document == this &&
             rootElement->_parentNode == nullptr));

    //  The previous root element loses its parent...
    if (_rootElement != nullptr)
    {
        _rootElement->_parentNode = nullptr;
    }
    //  ...and is replaced with new root element...
    _rootElement = rootElement;
    //  ...which now has a parent
    if (_rootElement != nullptr)
    {
        _rootElement->_parentNode = this;
    }
}

XmlDeclaration * XmlDocument::createXmlDeclaration()
{
    return createXmlDeclaration(XmlVersion::Default, CharacterSet::getDefault(), true);
}

XmlDeclaration * XmlDocument::createXmlDeclaration(XmlVersion version, CharacterSet * encoding, bool standalone)
{
    XmlDeclaration * xmlDeclaration = new XmlDeclaration(this);
    xmlDeclaration->setVersion(version);
    xmlDeclaration->setEncoding(encoding);
    xmlDeclaration->setStandalone(standalone);
    return xmlDeclaration;
}

XmlElement * XmlDocument::createElement(const String & tagName)
{
    return createElement(String(), tagName);
}

XmlElement * XmlDocument::createElement(const String & namespaceUri, const String & tagName)
{
    if (!Xml::isValidName(tagName))
    {   //  OOPS!
        throw XmlDomException("Invalid tag name");  //  TODO localize via resources
    }

    XmlElement * xmlElement = new XmlElement(this);
    xmlElement->setNamespaceUri(namespaceUri);
    xmlElement->setTagName(tagName);
    return xmlElement;
}

XmlAttribute * XmlDocument::createAttribute(const String & name, const String & value)
{
    return createAttribute(String(), name, value);
}

XmlAttribute * XmlDocument::createAttribute(const String & namespaceUri, const String & name, const String & value)
{
    if (!Xml::isValidName(name))
    {   //  OOPS!
        throw XmlDomException("Invalid attribute name");    //  TODO localize via resources
    }

    XmlAttribute * xmlAttribute = new XmlAttribute(this);
    xmlAttribute->setNamespaceUri(namespaceUri);
    xmlAttribute->setName(name);
    xmlAttribute->setValue(value);
    return xmlAttribute;
}

XmlText * XmlDocument::createText(const String & text)
{
    XmlText * xmlText = new XmlText(this);
    xmlText->setText(text);
    return xmlText;
}

XmlComment * XmlDocument::createComment(const String & text)
{
    XmlComment * xmlComment = new XmlComment(this);
    xmlComment->setText(text);
    return xmlComment;
}

XmlProcessingInstruction * XmlDocument::createProcessingInstruction(const String & target)
{
    if (!Xml::isValidName(target))
    {   //  OOPS!
        throw XmlDomException("Invalid target");    //  TODO localize via resources
    }

    XmlProcessingInstruction * xmlProcessingInstruction = new XmlProcessingInstruction(this);
    xmlProcessingInstruction->setTarget(target);
    return xmlProcessingInstruction;
}

void XmlDocument::save(ITextOutput & stream)
{
    if (_rootElement == nullptr)
    {   //  OOPS!
        throw XmlDomException("XML document has no root element");  //  TODO localize via resources
    }

    if (_xmlDeclaration != nullptr)
    {
        stream.write("<?xml ");
        switch (_xmlDeclaration->getVersion())
        {
            case XmlVersion::V_1_0:
                stream.write("version=\"1.0\" ");
                break;
            case XmlVersion::V_1_1:
                stream.write("version=\"1.1\" ");
                break;
            case XmlVersion::Unknown:
            default:
                break;
        }
        if (_xmlDeclaration->getEncoding() != nullptr)
        {
            stream.write("encoding=\"" + _xmlDeclaration->getEncoding()->getPreferredName() + "\" ");
        }
        if (_xmlDeclaration->isStandalone())
        {
            stream.write("standalone=\"yes\" ");
        }
        else
        {
            stream.write("standalone=\"no\" ");
        }
        stream.write("?>");
        stream.writeLn();
    }
    _NamespaceMap namespaceMap;
    namespaceMap.put("", "");
    _save(stream, _rootElement, 0, namespaceMap);
}

void XmlDocument::save(const Path & xmlFilePath)
{
    TextFileWriter writer(xmlFilePath);
    if (_xmlDeclaration != nullptr && _xmlDeclaration->getEncoding() != nullptr)
    {
        writer.setCharacterSet(_xmlDeclaration->getEncoding());
    }
    save(writer);
}

void XmlDocument::load(ITextInput & stream)
{
    try
    {
        DomReaderContentHandler contentHandler;
        XmlReader reader(&contentHandler);

        reader.parse(stream);
        XmlDocument * newXmlDocument = contentHandler.grabXmlDocument();
        if (newXmlDocument == nullptr)
        {   //  OOPS! TODO throw properly
            throw XmlDomException("XML document is empty or malformed");    //  TODO localize via resources
        }
        try
        {
            *this = *newXmlDocument;
        }
        catch (...)
        {   //  OOPS! Cleanup before re-throw
            delete newXmlDocument;
            throw;
        }
    }
    catch (XmlSaxException & ex)
    {   //  Convert SAX -> DOM
        throw XmlDomException(ex.toString());
    }
}

void XmlDocument::load(const Path & xmlFilePath)
{
    DomReaderContentHandler contentHandler;
    XmlReader reader(&contentHandler);

    reader.parse(xmlFilePath);
    XmlDocument * newXmlDocument = contentHandler.grabXmlDocument();
    if (newXmlDocument == nullptr)
    {   //  OOPS! TODO throw properly
        throw XmlDomException("XML document is empty or malformed");    //  TODO localize via resources
    }
    try
    {
        *this = *newXmlDocument;
    }
    catch (...)
    {   //  OOPS! Cleanup before re-throw
        delete newXmlDocument;
        throw;
    }
}

//////////
//  Implementation helpers
void XmlDocument::_save(ITextOutput & stream, XmlElement * xmlElement, UIntSize depth, _NamespaceMap & namespaceMap)
{
    //  Save the namespace map state as it was before the element.
    //  If the element does not define any "xmlns:" attributes, the "namespaceMap"
    //  will never be modified, so the actual "shared impl" of "namespaceMap" and
    //  "savedNamespaceMap" will remain the same
    _NamespaceMap savedNamespaceMap = namespaceMap;

    //  Prepare attribute string for the element
    String attributesString = _createAttributesString(xmlElement->getAttributes(), namespaceMap);

    //  Emit the element
    XmlNodeList innerNodes = xmlElement->getInnerNodes();
    if (innerNodes.size() == 0)
    {   //  No inner nodes
        stream.write(_getMarginForDepth(depth));
        stream.write("<");
        if (!xmlElement->getNamespaceUri().isEmpty())
        {   //  Make sure the namespace prefix for the URI in question is defined...
            if (!namespaceMap.containsKey(xmlElement->getNamespaceUri()))
            {
                for (int i = 1; ; i++)
                {
                    String candidateNamespacePrefix = "ns" + toString(i);
                    if (!namespaceMap.containsValue(candidateNamespacePrefix))
                    {
                        namespaceMap.put(xmlElement->getNamespaceUri(), candidateNamespacePrefix);
                        break;
                    }
                }
            }
            //  ...and use it
            String namespacePrefix = namespaceMap.get(xmlElement->getNamespaceUri());
            if (!namespacePrefix.isEmpty())
            {
                stream.write(namespacePrefix);
                stream.write(':');
            }
        }
        stream.write(xmlElement->getTagName());
        if (attributesString.length() != 0)
        {
            stream.write(" ");
            stream.write(attributesString);
        }
        stream.write("/>");
        stream.writeLn();
    }
    else if (innerNodes.size() == 0 &&
             dynamic_cast<XmlText*>(innerNodes[0]) != nullptr)
    {   //  A single inner node that is text - special handling
        stream.write(_getMarginForDepth(depth));
        stream.write("<");
        //  TODO namespace
        stream.write(xmlElement->getTagName());
        if (attributesString.length() != 0)
        {
            stream.write(" ");
            stream.write(attributesString);
        }
        stream.write(">");

        XmlText * xmlText = dynamic_cast<XmlText*>(innerNodes[0]);
        stream.write(Xml::encodeXmlEntities(_normalizeText(xmlText->getText())));

        stream.write("</");
        //  TODO namespace
        stream.write(xmlElement->getTagName());
        stream.write(">");
        stream.writeLn();
    }
    else
    {   //  Inner nodes exist - general case
        stream.write(_getMarginForDepth(depth));
        stream.write("<");
        //  TODO namespace
        stream.write(xmlElement->getTagName());
        if (attributesString.length() != 0)
        {
            stream.write(" ");
            stream.write(attributesString);
        }
        stream.write(">");
        stream.writeLn();

        for (XmlNode * innerNode : xmlElement->getInnerNodes())
        {
            if (XmlElement * innerElement = dynamic_cast<XmlElement*>(innerNode))
            {
                _save(stream, innerElement, depth + 1, namespaceMap);
            }
            else if (XmlText * innerText = dynamic_cast<XmlText*>(innerNode))
            {
                _save(stream, innerText, depth + 1);
            }
            else if (XmlComment * innerComment = dynamic_cast<XmlComment*>(innerNode))
            {
                _save(stream, innerComment, depth + 1);
            }
            else if (XmlProcessingInstruction * innerProcessingInstruction = dynamic_cast<XmlProcessingInstruction*>(innerNode))
            {
                _save(stream, innerProcessingInstruction, depth + 1, namespaceMap);
            }
            else
            {   //  OOPS! This should never have been allowed!
                failure();
            }
        }

        stream.write(_getMarginForDepth(depth));
        stream.write("</");
        //  TODO namespace
        stream.write(xmlElement->getTagName());
        stream.write(">");
        stream.writeLn();
    }

    //  Restore namespace map to what it was before the element
    namespaceMap = savedNamespaceMap;
}

void XmlDocument::_save(ITextOutput & stream, XmlText * xmlText, UIntSize depth)
{
    stream.write(_getMarginForDepth(depth));
    stream.write(Xml::encodeXmlEntities(_normalizeText(xmlText->getText())));
    stream.writeLn();
}

void XmlDocument::_save(ITextOutput & stream, XmlComment * xmlComment, UIntSize depth)
{
    stream.write(_getMarginForDepth(depth));
    stream.write("<!-- ");
    stream.write(Xml::encodeXmlEntities(_normalizeText(xmlComment->getText())));
    stream.write("-->");
    stream.writeLn();
}

void XmlDocument::_save(ITextOutput & stream, XmlProcessingInstruction * xmlProcessingInstruction, UIntSize depth, _NamespaceMap & namespaceMap)
{
    //  Save the namespace map state as it was before the element.
    //  If the element does not define any "xmlns:" attributes, the "namespaceMap"
    //  will never be modified, so the actual "shared impl" of "namespaceMap" and
    //  "savedNamespaceMap" will remain the same
    _NamespaceMap savedNamespaceMap = namespaceMap;

    //  Prepare attribute string for the element
    String attributesString = _createAttributesString(xmlProcessingInstruction->getAttributes(), namespaceMap);

    //  Emit the processing instruction
    stream.write(_getMarginForDepth(depth));
    stream.write("<?");
    stream.write(xmlProcessingInstruction->getTarget());
    stream.write(" ");
    stream.write(attributesString);
    stream.write("?>");
    stream.writeLn();

    //  Restore namespace map to what it was before the element
    namespaceMap = savedNamespaceMap;
}

String XmlDocument::_createAttributesString(const XmlAttributeList & attributes, _NamespaceMap & namespaceMap)
{
    for (XmlAttribute * xmlAttribute : attributes)
    {
        if (xmlAttribute->getNamespaceUri().isEmpty() && xmlAttribute->getName() == "nsuri")
        {   //  This attribute declares the default namespace
            namespaceMap.put(xmlAttribute->getValue(), "");
        }
        else if (xmlAttribute->getNamespaceUri().isEmpty() && xmlAttribute->getName().startsWith("nsuri:"))
        {
            namespaceMap.put(xmlAttribute->getValue(), xmlAttribute->getName().mid(6));
        }
    }

    String result;
    for (XmlAttribute * xmlAttribute : attributes)
    {
        if (result.length() != 0)
        {
            result += ' ';
        }
        if (!xmlAttribute->getNamespaceUri().isEmpty())
        {   //  Make sure the namespace prefix for the URI in question is defined...
            if (!namespaceMap.containsKey(xmlAttribute->getNamespaceUri()))
            {
                for (int i = 1; ; i++)
                {
                    String candidateNamespacePrefix = "ns" + toString(i);
                    if (!namespaceMap.containsValue(candidateNamespacePrefix))
                    {
                        namespaceMap.put(xmlAttribute->getNamespaceUri(), candidateNamespacePrefix);
                        break;
                    }
                }
            }
            //  ...and use it
            String namespacePrefix = namespaceMap.get(xmlAttribute->getNamespaceUri());
            if (!namespacePrefix.isEmpty())
            {
                result += namespacePrefix;
                result += ':';
            }
        }
        result += xmlAttribute->getName();
        result += '=';
        result += '"';
        result += Xml::encodeXmlEntities(xmlAttribute->getValue());
        result += '"';
    }
    return result;
}

String XmlDocument::_normalizeText(const String & text)
{
    String result = text;

    //  1.  Replace any whitespace characters with space
    for (UIntSize i = 0; i < result.length(); i++)
    {
        if (result[i].isSpace())
        {
            result.setAt(i, ' ');
        }
    }

    //  2.  Replace every consecutive run of spaces with a single space
    for (UIntSize i = 0; i < result.length(); )
    {
        if (result[i] != ' ')
        {   //  Just a character - skip
            i++;
        }
        else if (i + 1 >= result.length() || result[i] != ' ')
        {   //  A space not followed by another space - skip
            i++;
        }
        else
        {   //  A space followed by another space - remove
            result.removeAt(i);
        }
    }

    //  3.  Remove leading/tailing spaces
    result.trim();

    //  Done
    return result;
}

String XmlDocument::_getMarginForDepth(UIntSize depth)
{
    if (!_mapDepthsToMarginStrings.containsKey(depth))
    {
        String level = "  ";
        String marginString;
        for (UIntSize i = 0; i < depth; i++)
        {
            marginString += level;
        }
        _mapDepthsToMarginStrings.put(depth, marginString);
    }
    return _mapDepthsToMarginStrings.get(depth);
}

XmlDeclaration * XmlDocument::_cloneXmlDeclarationForMyself(XmlDeclaration * xmlDeclaration)
{
    if (xmlDeclaration == nullptr)
    {
        return nullptr;
    }
    return createXmlDeclaration(xmlDeclaration->getVersion(),
                                xmlDeclaration->getEncoding(),
                                xmlDeclaration->isStandalone());
}

XmlElement * XmlDocument::_cloneXmlElementForMyself(XmlElement * xmlElement)
{
    if (xmlElement == nullptr)
    {
        return xmlElement;
    }
    XmlElement * result = nullptr;
    try
    {
        result = createElement(xmlElement->getNamespaceUri(),
                               xmlElement->getTagName());
        //  Copy attributes
        for (XmlAttribute * xmlAttribute : xmlElement->getAttributes())
        {
            result->setAttribute(xmlAttribute->getNamespaceUri(),
                                 xmlAttribute->getName(),
                                 xmlAttribute->getValue());
        }
        //  Copy innards
        for (XmlNode * innerNode : xmlElement->getInnerNodes())
        {
            if (XmlElement * innerElement = dynamic_cast<XmlElement*>(innerNode))
            {
                result->addElement(_cloneXmlElementForMyself(innerElement));
            }
            else if (XmlText * innerText = dynamic_cast<XmlText*>(innerNode))
            {
                result->addText(_cloneXmlTextForMyself(innerText));
            }
            else if (XmlComment * innerComment = dynamic_cast<XmlComment*>(innerNode))
            {
                result->addComment(_cloneXmlCommentForMyself(innerComment));
            }
            else if (XmlProcessingInstruction * innerProcessingInstruction = dynamic_cast<XmlProcessingInstruction*>(innerNode))
            {
                result->addProcessingInstruction(_cloneXmlProcessingInstructionForMyself(innerProcessingInstruction));
            }
            else
            {   //  OOPS! This should never have been!
                failure();
            }
        }
        return result;
    }
    catch (...)
    {   //  Cleanup before re-throw
        delete result;
        throw;
    }
}

XmlText * XmlDocument::_cloneXmlTextForMyself(XmlText * xmlText)
{
    return createText(xmlText->getText());
}

XmlComment * XmlDocument::_cloneXmlCommentForMyself(XmlComment * xmlComment)
{
    return createComment(xmlComment->getText());
}

XmlProcessingInstruction * XmlDocument::_cloneXmlProcessingInstructionForMyself(XmlProcessingInstruction * xmlProcessingInstruction)
{
    XmlProcessingInstruction * result = nullptr;
    try
    {
        result = createProcessingInstruction(xmlProcessingInstruction->getTarget());
        //  Copy attributes
        for (XmlAttribute * xmlAttribute : xmlProcessingInstruction->getAttributes())
        {
            result->setAttribute(xmlAttribute->getNamespaceUri(),
                                 xmlAttribute->getName(),
                                 xmlAttribute->getValue());
        }
        //  Done
        return result;
    }
    catch (...)
    {   //  Cleanup before re-throw
        delete result;
        throw;
    }
}

//  End of XmlDocument.cpp
