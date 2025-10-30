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
        const QString & name,
        const QString & path,
        const QString & displayName,
        IContentLoader * contentLoader
    ) : IHelpTopic(),
        _name(name),
        _path(path),
        _displayName(displayName),
        _contentLoader(contentLoader)
{
    Q_ASSERT(_contentLoader != nullptr);
}

SimpleHelpTopic::~SimpleHelpTopic()
{
    for (auto child : _children)
    {
        delete child;
    }
    delete _contentLoader;
}

//  End of tt3-help/SimpleHelpTopic.cpp
