//
//  tt3-util/ComponentRegistry.cpp - ComponentRegistry class implementation
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

//////////
//  Operationds
QSet<Component*> ComponentRegistry::allComponents()
{
    return toSet(_registry().values());
}

bool ComponentRegistry::registerComponent(Component * component)
{   //  TODO synchronize ?
    Q_ASSERT(component != nullptr);

    Component * registeredComponent = findComponent(component->mnemonic(), component->version());
    if (registeredComponent != nullptr)
    {
        return component == registeredComponent;
    }
    else
    {
        qDebug() << "Registering " << component->displayName();
        QString key = component->mnemonic() + "|" + util::toString(component->version());
        _registry()[key] = component;
        return true;
    }
}

Component * ComponentRegistry::findComponent(const QString & mnemonic, const QVersionNumber & version)
{   //  TODO synchronize ?
    QString key = mnemonic + "|" + util::toString(version);
    return _registry().contains(key) ? _registry()[key] : nullptr;
}

Component * ComponentRegistry::findComponent(const QString & mnemonic)
{   //  TODO synchronize ?
    Component * result = nullptr;
    for (Component * component : _registry().values())
    {
        if (component->mnemonic() == mnemonic)
        {
            if (result == nullptr ||
                component->version() > result->version())
            {
                result = component;
            }
        }
    }
    return result;
}

//////////
//  Implementation
QMap<QString, Component*> & ComponentRegistry::_registry()
{
    static QMap<QString, Component*> registry;
    return registry;
}

//  End of tt3-util/ComponentRegistry.cpp
