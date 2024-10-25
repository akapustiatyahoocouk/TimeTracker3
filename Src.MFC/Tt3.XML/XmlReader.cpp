//
//  XmlReader.cpp - Implementation of the carl::XmlReader class
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
XmlReader::XmlReader(XmlContentHandler * contentHandler)
    :   _contentHandler(contentHandler),
        _stream(nullptr)
{
    require(_contentHandler != nullptr);
}

XmlReader::~XmlReader()
{
}

//////////
//  Operations
void XmlReader::parse(ITextInput & stream)
{
    require(_stream == nullptr);

    try
    {
        _stream = &stream;
        _parseXmlStream();
        _stream = nullptr;
    }
    catch (...)
    {   //  OOPS! Cleanup before re-throw
        _stream = nullptr;
    }
}

void XmlReader::parse(const Path & xmlFilePath)
{
    CharacterSet * encoding = _determineEncoding(xmlFilePath);
    TextFileReader reader(xmlFilePath, encoding);
    return parse(reader);
}

//////////
//  Implementation helpers
CharacterSet * XmlReader::_determineEncoding(const Path & xmlFilePath)
{
    //  We must first determine the encoding to use
    BinaryFileReader binaryReader(xmlFilePath);
    ByteArray bytes;
    for (int n = 0; n < 128; n++)
    {
        int b = binaryReader.read();
        if (b == -1)
        {   //  EOF
            break;
        }
        bytes.add(static_cast<UInt8>(b));
    }

    //  UNICODE ? (BOM == U+FEFF)
    if (bytes.size() >= 3 &&
        bytes[0] == 0xEF && bytes[1] == 0xBB && bytes[2] == 0xBF)
    {
        return CharacterSet::getInstance("UTF-8");
    }
    if (bytes.size() >= 4 &&
        bytes[0] == 0x00 && bytes[1] == 0x00 && bytes[2] == 0xFE && bytes[3] == 0xFF)
    {
        return CharacterSet::getInstance("UTF-32BE");
    }
    if (bytes.size() >= 4 &&
        bytes[0] == 0xFF && bytes[1] == 0xFE && bytes[2] == 0x00 && bytes[3] == 0x00)
    {
        return CharacterSet::getInstance("UTF-32LE");
    }
    if (bytes.size() >= 2 &&
        bytes[0] == 0xFE && bytes[1] == 0xFF)
    {
        return CharacterSet::getInstance("UTF-16BE");
    }
    if (bytes.size() >= 2 &&
        bytes[0] == 0xFF && bytes[1] == 0xFE)
    {
        return CharacterSet::getInstance("UTF-16LE");
    }

    //  EBCDIC ?
    if (bytes.size() >= 1 && bytes[0] == 0x4C)
    {
        return CharacterSet::getInstance("IBM037");
    }

    //  ASCII-based ?
    if (bytes.size() >= 1 && bytes[0] == '<')
    {   //  We need to analyze the "encoding" attribute
        String s;
        for (UIntSize i = 0; i < bytes.size(); i++)
        {
            s += static_cast<char>(bytes[i]);
            if (s.endsWith('>'))
            {   //  End of XML declaration
                break;
            }
        }
        if (s.startsWith("<?xml ") && s.endsWith("?>"))
        {   //  Looks like XML declaration
            UIntSize encodingWordIndex, eqIndex, openingQuoteIndex, closingQuoteIndex;
            if (s.findFirst("encoding", encodingWordIndex) &&
                s.findFirst('=', eqIndex, encodingWordIndex + 8) &&
                s.findFirst('"', openingQuoteIndex, eqIndex + 1) &&
                s.findFirst('"', closingQuoteIndex, openingQuoteIndex + 1))
            {
                String encodingName =
                    s.mid(openingQuoteIndex + 1, closingQuoteIndex - openingQuoteIndex - 1).trimmed();
                try
                {
                    return CharacterSet::getInstance(encodingName);
                }
                catch (NoSuchElementError &)
                {   //  OOPS! Encoding not found! TODO report ?
                }
            }
        }
    }

    //  Give up
    return CharacterSet::getDefault();
}

Char XmlReader::_lookahead(UIntSize distance)
{
    while (_prereadCharacters.size() < distance + 1)
    {
        Char c;
        if (!_stream->read(c))
        {
            c = ' ';
        }
        _prereadCharacters.add(c);
    }
    return _prereadCharacters[distance];
}

void XmlReader::_skipCharacter()
{
    _lookahead(0);
    _prereadCharacters.removeAt(0);
}

void XmlReader::_skipCharacter(Char c)
{
    if (_lookahead(0) != c)
    {   //  OOPS!
        throw XmlSaxException(String("'") + _lookahead(0) + "' encountered where '" + c + "' was expected");    //  TODO localize via resources
    }
    _prereadCharacters.removeAt(0);
}

void XmlReader::_skipSpaces()
{
    while (_lookahead(0).isSpace() && _stream->ready())
    {
        _skipCharacter();
    }
}

void XmlReader::_parseXmlStream()
{
    //  Re-initialize parser state
    _prereadCharacters.clear();
    _openXmlElementTags.clear();

    //  An XML stream will normally start from an XML declaration.
    //  However, we also allow an XML document without the XML declaration.
    XmlVersion version = XmlVersion::Default;
    CharacterSet * encoding = CharacterSet::getDefault();
    bool standalone = true;

    _skipSpaces();
    if (_lookahead(0) == '<' &&
        _lookahead(1) == '?' &&
        _lookahead(2) == 'x' &&
        _lookahead(3) == 'm' &&
        _lookahead(4) == 'l' &&
        _lookahead(5).isSpace())
    {   //  XML declaration exists
        _parseXmlDeclaration(version, encoding, standalone);
    }
    _contentHandler->beginDocument(version, encoding, standalone);

    //  The root element MUST start here!
    _skipSpaces();
    if (_lookahead(0) != '<' || _lookahead(1) == '!')
    {   //  OOPS!
        throw XmlSaxException("No root element");   //  TODO localize via resources
    }
    _NamespaceMap namespaceMap;
    namespaceMap.put("", "");
    _parseXmlElement(namespaceMap);

    //  Done parsing the XML document
    _contentHandler->endDocument();
}

void XmlReader::_parseXmlDeclaration(XmlVersion & version, CharacterSet *& encoding, bool & standalone)
{
    //  Parse the XML declaration
    _skipCharacter('<');
    _skipCharacter('?');
    _skipCharacter('x');
    _skipCharacter('m');
    _skipCharacter('l');

    _NamespaceMap namespaceMap;
    namespaceMap.put("", "");
    _AttributeList attributes = _parseAttributes(namespaceMap);  //  may update "namespaceMap"

    _skipSpaces();
    _skipCharacter('?');
    _skipCharacter('>');

    //  Analyse the XML declaration
    version = XmlVersion::Default;
    encoding = CharacterSet::getDefault();
    standalone = true;
    for (_Attribute attribute : attributes)
    {
        if (attribute.namespaceUri.isEmpty() && attribute.name == "version")
        {
            if (attribute.value == "1.0")
            {
                version = XmlVersion::V_1_0;
            }
            else if (attribute.value == "1.1")
            {
                version = XmlVersion::V_1_1;
            }
            else
            {
                throw XmlSaxException("Invalid XML version '" + attribute.value + "' in XML declaration");  //  TODO localize via resources
            }
        }
        else if (attribute.namespaceUri.isEmpty() && attribute.name == "encoding")
        {
            try
            {
                encoding = CharacterSet::getInstance(attribute.value);
            }
            catch (NoSuchElementError &)
            {
                throw XmlSaxException("Invalid encoding '" + attribute.value + "' in XML declaration"); //  TODO localize via resources
            }
        }
        else if (attribute.namespaceUri.isEmpty() && attribute.name == "standalone")
        {
            if (attribute.value == "yes")
            {
                standalone = true;
            }
            else if (attribute.value == "no")
            {
                standalone = false;
            }
            else
            {
                throw XmlSaxException("Invalid standalone attribute '" + attribute.value + "' in XML declaration"); //  TODO localize via resources
            }
        }
    }

    //  Done
}

void XmlReader::_parseXmlElement(_NamespaceMap & namespaceMap)
{
    //  Save the namespace map state as it was before the element.
    //  If the element does not define any "xmlns:" attributes, the "namespaceMap"
    //  will never be modified, so the actual "shared impl" of "namespaceMap" and
    //  "savedNamespaceMap" will remain the same
    _NamespaceMap savedNamespaceMap = namespaceMap;

    //  "<tag attributes..."
    _skipCharacter('<');
    _skipSpaces();
    String tagNamespaceUri;
    String tagName =  _parseIdentifier();
    _AttributeList attributes = _parseAttributes(namespaceMap);  //  may update "namespaceMap"
    _skipSpaces();

    _resolveNamespaceUri(namespaceMap, tagName, tagNamespaceUri);

    if (_lookahead(0) == '/')
    {   //  "<tag attributes />"
        _skipCharacter('/');
        _skipCharacter('>');
        _contentHandler->beginElement(tagNamespaceUri, tagName, attributes);
        _contentHandler->endElement(tagNamespaceUri, tagName);
    }
    else
    {   //  "<tag attributes><inner></tag>"
        _skipCharacter('>');
        _contentHandler->beginElement(tagNamespaceUri, tagName, attributes);
        _openXmlElementTags.push(tagName);

        //  Parse innards
        while (_stream->ready())
        {
            //  Closing tag ?
            if (_lookahead(0) == '<' && _lookahead(1) == '/')
            {   //  Yes - no more innards
                break;
            }
            //  Comment ?
            if (_lookahead(0) == '<' && _lookahead(1) == '!' &&
                _lookahead(2) == '-' && _lookahead(3) == '-')
            {   //  Yes
                _parseComment();
                continue;
            }
            //  PI ?
            if (_lookahead(0) == '<' && _lookahead(1) == '?')
            {   //  Yes
                _parseProcessingInstruction(namespaceMap);
                continue;
            }
            //  Opening tag ?
            if (_lookahead(0) == '<')
            {   //  Yes - parse the XML element
                _parseXmlElement(namespaceMap);
                continue;
            }
            //  Just some text - fetch textual data until EOF or '<'...
            String text;
            while (_stream->ready() && _lookahead(0) != '<')
            {
                text += _lookahead(0);
                _skipCharacter();
            }
            //  ...and process it
            text.trim();
            if (!text.isEmpty())
            {
                _contentHandler->text(Xml::decodeXmlEntities(text));
            }
        }

        //  Parse closing tag
        _skipCharacter('<');
        _skipCharacter('/');
        _skipSpaces();
        String closingTagName =  _parseIdentifier();
        _skipSpaces();
        _skipCharacter('>');
        //  Match closing tag with opening tag
        if (closingTagName == tagName)
        {   //  Match
            _contentHandler->endElement(tagNamespaceUri, tagName);
            _openXmlElementTags.pop();
        }
        else
        {   //  OOPS!
            throw XmlSaxException("Closing tag '" + closingTagName + "' does not match opening tag '" + tagName + "'"); //  TODO localize via resources
        }
    }

    //  Restore namespace map to what it was before the element
    namespaceMap = savedNamespaceMap;
}

void XmlReader::_parseComment()
{
    //  Skip comment opener
    _skipCharacter('<');
    _skipCharacter('!');
    _skipCharacter('-');
    _skipCharacter('-');

    //  Fetch comment text
    String text;
    while (_stream->ready())
    {
        if (_lookahead(0) == '-' && _lookahead(1) == '-' && _lookahead(2) == '>')
        {   //  End-of-comment
            break;
        }
        text += _lookahead(0);
        _skipCharacter();
    }

    //  Skip comment closer
    _skipCharacter('-');
    _skipCharacter('-');
    _skipCharacter('>');

    //  Process & done
    _contentHandler->comment(Xml::decodeXmlEntities(text));
}

void XmlReader::_parseProcessingInstruction(_NamespaceMap & namespaceMap)
{
    //  Save the namespace map state as it was before the PI.
    //  If the element does not define any "xmlns:" attributes, the "namespaceMap"
    //  will never be modified, so the actual "shared impl" of "namespaceMap" and
    //  "savedNamespaceMap" will remain the same
    _NamespaceMap savedNamespaceMap = namespaceMap;

    //  PI opener
    _skipCharacter('<');
    _skipCharacter('?');
    _skipSpaces();
    String target =  _parseIdentifier();

    //  PI attributes
    _AttributeList attributes = _parseAttributes(namespaceMap);  //  may update "namespaceMap"

    //  PI closer
    _skipSpaces();
    _skipCharacter('?');
    _skipCharacter('>');

    //  Handle & we're done
    _contentHandler->processingInstruction(target, attributes);

    //  Restore namespace map to what it was before the PI
    namespaceMap = savedNamespaceMap;
}

XmlReader::_AttributeList XmlReader::_parseAttributes(_NamespaceMap & namespaceMap)
{
    _AttributeList result;

    for (; ; )
    {
        _skipSpaces();
        if (Xml::isValidNameStart(_lookahead(0)))
        {   //  An attribute name starts here
            String name = _parseIdentifier();
            //  Skip '='
            _skipSpaces();
            _skipCharacter('=');
            _skipSpaces();
            //  Parse attribute value
            _skipCharacter('"');
            String value;
            while (_lookahead(0) != '"' && _stream->ready())
            {
                value += _lookahead(0);
                _skipCharacter();
            }
            _skipCharacter('"');
            //  Record the attribute
            _Attribute attribute;
            if (name.startsWith("xmlns:"))
            {   //  Namespace prefix declaration amends the namespace map
                String namespacePrefix = name.mid(6);
                if (namespacePrefix == "xmlns")
                {   //  OOPS! Not allowed!
                    throw XmlSaxException("Attribute '" + name + "' is not permitted"); //  TODO localize via resources
                }
                namespaceMap.put(namespacePrefix, value);
            }
            else if (name == "xmlns")
            {   //  Default namespace declaration amends the namespace map
                namespaceMap.put("", value);
            }
            //  Just an attribute
            attribute.name = name;
            attribute.value = Xml::decodeXmlEntities(value);
            result.add(attribute);
        }
        else
        {   //  No more attributes
            break;
        }
    }
    //  Apply namespace map
    for (UIntSize i = 0; i < result.size(); i++)
    {
        _resolveNamespaceUri(namespaceMap, result[i].name, result[i].namespaceUri);
    }
    //  Guard against duplicate attributes
    for (UIntSize i = 0; i < result.size(); i++)
    {
        for (UIntSize j = i + 1; j < result.size(); j++)
        {
            if (result[i].namespaceUri == result[j].namespaceUri &&
                result[i].name == result[j].name)
            {
                if (result[i].namespaceUri.isEmpty())
                {
                    throw XmlSaxException("Attribute '" + result[i].name + "' specified more than once"); //  TODO localize via resources
                }
                else
                {
                    throw XmlSaxException("Attribute '" + result[i].namespaceUri + ":" + result[i].name + "' specified more than once"); //  TODO localize via resources
                }
            }
        }
    }
    //  Done
    return result;
}

String XmlReader::_parseIdentifier()
{
    String result;
    if (!Xml::isValidNameStart(_lookahead(0)))
    {   //  OOPS! An identifier does not start here!
        throw XmlSaxException(String("'") + _lookahead(0) + "' encountered where <IDENTIFIER> was expected");   //  TODO localize via resources
    }
    result += _lookahead(0);
    _skipCharacter();
    while (Xml::isValidNameChar(_lookahead(0)))
    {
        result += _lookahead(0);
        _skipCharacter();
    }
    return result;
}

void XmlReader::_resolveNamespaceUri(const _NamespaceMap & namespaceMap, String & name, String & namespaceUri)
{
    if (name != "xmlns" && !name.startsWith("xmlns:"))
    {   //  Keep the "xmlns[:...]" unaffected
        UIntSize namespacePrefixSeparator;
        if (name.findFirst(':', namespacePrefixSeparator))
        {   //  There looks to be a namespace prefix...
            String namespacePrefix = name.left(namespacePrefixSeparator);
            if (namespaceMap.containsKey(namespacePrefix))
            {   //  ...which is mapped - apply it
                namespaceUri = namespaceMap[namespacePrefix];
                name = name.mid(namespacePrefixSeparator + 1);
            }
            else
            {   //  ...which is not mapped - we have an error
                throw XmlSaxException("Namespace prefix '" + namespacePrefix + "' not defined");   //  TODO localize via resources
            }
        }
        else if (namespaceMap.containsKey(""))
        {   //  No ':' in tag name - but must to apply the default namespace
            namespaceUri = namespaceMap[""];
        }
    }
}

//  End of XmlReader.cpp
