//
//  tt3-util/OnceExecutor.cpp - OnceExecutor class implementation
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
#include "tt3-util/API.hpp"
using namespace util;

OnceExecutor * OnceExecutor::_onceExecutors = nullptr;

//////////
//  Construction/destruction
OnceExecutor::OnceExecutor(const char * file, int line, void (*f)())
    :   _file(file), _line(line)
{
    for (OnceExecutor * oi = _onceExecutors; oi != nullptr; oi = oi->_nextInChain)
    {
        if (line == oi->_line && strcmp(file, oi->_file) == 0)
        {   //  Already recorded
            oi->_usageCount++;
            return;
        }
    }
    //  Register a new record
    Q_ASSERT(_usageCount == 1);
    _nextInChain = _onceExecutors;
    _onceExecutors = this;
    qDebug() << _file;
    qDebug() << _line;
    f();
}

OnceExecutor::~OnceExecutor()
{
    for (OnceExecutor ** oi = &_onceExecutors; (*oi) != nullptr; oi = &((*oi)->_nextInChain))
    {
        if (_line == (*oi)->_line && strcmp(_file, (*oi)->_file) == 0)
        {   //  This one!
            if (--(*oi)->_usageCount == 0)
            {   //  Unregister this record
                *oi = _nextInChain;
                _nextInChain = nullptr;
            }
            return;
        }
    }
    Q_ASSERT(false);
}


//  End of tt3-util/OnceExecutor.cpp
