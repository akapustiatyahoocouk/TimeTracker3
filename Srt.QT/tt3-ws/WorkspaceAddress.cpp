//
//  tt3-ws/WorkspaceAddress.cpp - tt3::ws::WorkspaceAddress class implementation
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
#include "tt3-ws/API.hpp"
using namespace tt3::ws;

//////////
//  Construction/destruction/assignment
WorkspaceAddress::WorkspaceAddress(tt3::db::api::IDatabaseAddress * databaseAddress)
    :   _databaseAddress(nullptr)
{   //  TODO synchronize ?
    _databaseAddress = databaseAddress;
    if (_databaseAddress != nullptr)
    {
        _databaseAddress->addReference();
    }
}

WorkspaceAddress::WorkspaceAddress()
    :   _databaseAddress(nullptr)
{
}

WorkspaceAddress::WorkspaceAddress(const WorkspaceAddress & src)
    :   _databaseAddress(nullptr)
{   //  TODO synchronize ?
    _databaseAddress = src._databaseAddress;
    if (_databaseAddress != nullptr)
    {
        _databaseAddress->addReference();
    }
}

WorkspaceAddress::WorkspaceAddress(WorkspaceAddress && src)
    :   _databaseAddress(nullptr)
{   //  TODO synchronize ?
    _databaseAddress = src._databaseAddress;
    src._databaseAddress = nullptr;
}

WorkspaceAddress::~WorkspaceAddress()
{   //  TODO synchronize ?
    if (_databaseAddress != nullptr)
    {
        _databaseAddress->removeReference();
    }
}

WorkspaceAddress & WorkspaceAddress::operator = (const WorkspaceAddress & src)
{
    if (this != &src)
    {   //  TODO synchronize ?
        if (_databaseAddress != nullptr)
        {
            _databaseAddress->removeReference();
        }
        _databaseAddress = src._databaseAddress;
        if (_databaseAddress != nullptr)
        {
            _databaseAddress->addReference();
        }
    }
    return *this;
}

WorkspaceAddress & WorkspaceAddress::operator = (WorkspaceAddress && src)
{   //  TODO synchronize ?
    std::swap(_databaseAddress, src._databaseAddress);
    return *this;
}

//////////
//  Operators
bool WorkspaceAddress::operator == (const WorkspaceAddress & op2) const
{   //  TODO synchronize ?
    return _databaseAddress == op2._databaseAddress;
}

bool WorkspaceAddress::operator != (const WorkspaceAddress & op2) const
{   //  TODO synchronize ?
    return _databaseAddress != op2._databaseAddress;
}

bool WorkspaceAddress::operator <  (const WorkspaceAddress & op2) const
{   //  TODO synchronize ?
    return _databaseAddress < op2._databaseAddress;
}

bool WorkspaceAddress::operator <= (const WorkspaceAddress & op2) const
{   //  TODO synchronize ?
    return _databaseAddress == op2._databaseAddress;
}

bool WorkspaceAddress::operator >  (const WorkspaceAddress & op2) const
{   //  TODO synchronize ?
    return _databaseAddress > op2._databaseAddress;
}

bool WorkspaceAddress::operator >= (const WorkspaceAddress & op2) const
{   //  TODO synchronize ?
    return _databaseAddress >= op2._databaseAddress;
}

//////////
//  Operations (general)
bool WorkspaceAddress::isValid() const
{   //  TODO synchronize ?
    return _databaseAddress != nullptr;
}

WorkspaceType * WorkspaceAddress::workspaceType() const
{   //  TODO synchronize ?
    return (_databaseAddress == nullptr) ?
                nullptr :
                WorkspaceTypeRegistry::_findWorkspaceType(_databaseAddress->databaseType());
}

QString WorkspaceAddress::displayForm() const
{   //  TODO synchronize ?
    return (_databaseAddress == nullptr) ?
                "" :
                _databaseAddress->displayForm();
}

QString WorkspaceAddress::externalForm() const
{   //  TODO synchronize ?
    return (_databaseAddress == nullptr) ?
                "" :
                _databaseAddress->externalForm();
}

//  End of tt3-ws/WorkspaceAddress.cpp
