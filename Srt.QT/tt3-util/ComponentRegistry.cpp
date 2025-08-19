//
//  tt3-util/ComponentRegistry.cpp - tt3::util::ComponentRegistry class implementation
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
using namespace tt3::util;

QMap<QString, IComponent*> ComponentRegistry::_registry;

//////////
//  Operationds
QSet<IComponent*> ComponentRegistry::allComponents()
{
    QList<IComponent*> values = _registry.values();
    return QSet<IComponent*>(values.begin(), values.end());
}

bool ComponentRegistry::registerComponent(IComponent * component)
{   //  TODO synchronize ?
    Q_ASSERT(component != nullptr);

    IComponent * registeredComponent = findComponent(component->mnemonic(), component->version());
    if (registeredComponent != nullptr)
    {
        return component == registeredComponent;
    }
    else
    {
        qDebug() << "Registering " << component->displayName();
        QString key = component->mnemonic() + "|" + util::toString(component->version());
        _registry[key] = component;
        return true;
    }
}

IComponent * ComponentRegistry::findComponent(const QString & mnemonic, const QVersionNumber & version)
{   //  TODO synchronize ?
    QString key = mnemonic + "|" + util::toString(version);
    return _registry.contains(key) ? _registry[key] : nullptr;
}

IComponent * ComponentRegistry::findComponent(const QString & mnemonic)
{   //  TODO synchronize ?
    IComponent * result = nullptr;
    for (IComponent * component : _registry.values())
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

//  End of tt3-util/ComponentRegistry.cpp
