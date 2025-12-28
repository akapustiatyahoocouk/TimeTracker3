//
//  tt3-help/Serializer.cpp - tt3::help::Serializer class implementation
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
#include "tt3-help/API.hpp"
using namespace tt3::help;

//////////
//  Operations
void Serializer::saveToFile(
        HelpCollection * helpCollection,
        const QString & fileName
    )
{
    Q_ASSERT(helpCollection != nullptr);

    //  Create DOM document with a root node
    QDomDocument document;
    QDomProcessingInstruction xmlDeclaration = document.createProcessingInstruction("xml", "version='1.0' encoding='UTF-8' standalone='yes'");
    document.appendChild(xmlDeclaration);

    QDomElement rootElement = document.createElement(helpCollection->collectionElementTag());
    rootElement.setAttribute("FormatVersion", "1");
    document.appendChild(rootElement);

    //  Add content
    helpCollection->serialize(rootElement);

    //  Save to XML file
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {   //  OOPS!
        throw CustomHelpException(fileName + ": " +  file.errorString());
    }
    QTextStream stream(&file);
    document.save(stream, 4);
    file.close();
}

//  End of tt3-help/Serializer.cpp
