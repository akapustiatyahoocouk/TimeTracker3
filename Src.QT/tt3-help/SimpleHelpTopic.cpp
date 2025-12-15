//
//  tt3-help/SimpleHelpTopic.cpp - tt3::help::SimpleHelpTopic class implementation
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
//  Construction/destruction
SimpleHelpTopic::SimpleHelpTopic(
        SimpleHelpTopic * parentTopic,
        const QString & name,
        const QString & displayName,
        IContentLoader * contentLoader
    ) : HelpTopic(parentTopic),
        parent(parentTopic),
        children(this),
        _name(name),
        _displayName(displayName),
        _contentLoader(contentLoader)
{
}

SimpleHelpTopic::~SimpleHelpTopic()
{
    for (auto child : std::as_const(_children))
    {
        delete child;
    }
    delete _contentLoader;
}

//////////
//  Operations
void SimpleHelpTopic::setName(const QString name)
{
    _name = name;
}

void SimpleHelpTopic::setDisplayName(const QString & displayName)
{
    _displayName = displayName;
}

void SimpleHelpTopic::setContentUrl(const QUrl & contentUrl)
{
    delete _contentLoader;  //  in case it's not nullptr
    _contentLoader = nullptr;
    _contentLoader =
        ContentLoaderFactory::createContentLoader(contentUrl);  //  may throw
}

//////////
//  SimpleHelpTopic::Children
auto SimpleHelpTopic::Children::createTopic(
        const QString & name,
        const QString & displayName,
        IContentLoader * contentLoader
    ) -> SimpleHelpTopic *
{
    auto topic = new SimpleHelpTopic(_helpTopic, name, displayName, contentLoader);
    _helpTopic->_children.append(topic);
    return topic;
}

//////////
//  Serialization
void SimpleHelpTopic::_serialize(QDomElement topicElement)
{
    topicElement.setAttribute("Name", _name);
    topicElement.setAttribute("DisplayName", _displayName);
    if (_contentLoader != nullptr)
    {
        topicElement.setAttribute("ContentUrl", _contentLoader->contentUrl().toString());
    }
    for (auto child : children)
    {
        QDomElement childElement =
            topicElement.ownerDocument().createElement("Topic");
        topicElement.appendChild(childElement);
        child->_serialize(childElement);
    }
}

//  End of tt3-help/SimpleHelpTopic.cpp
