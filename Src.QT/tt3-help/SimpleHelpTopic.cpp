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
        SimpleHelpTopic * parent_,
        const QString & name,
        const QString & displayName,
        IContentLoader * contentLoader
    ) : HelpTopic(parent_),
        parent(parent_),
        _name(name),
        _displayName(displayName),
        _contentLoader(contentLoader)
{
}

SimpleHelpTopic::~SimpleHelpTopic()
{
    for (auto child : _children)
    {
        delete child;
    }
    delete _contentLoader;
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

//  End of tt3-help/SimpleHelpTopic.cpp
