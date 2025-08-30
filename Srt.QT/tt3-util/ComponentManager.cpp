//
//  tt3-util/ComponentManager.cpp - tt3::util::ComponentManager class implementation
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

Mutex ComponentManager::_guard;
QMap<Mnemonic, IComponent*> ComponentManager::_registry;

//////////
//  Operations
Components ComponentManager::allComponents()
{
    Lock lock(_guard);

    QList<IComponent*> values = _registry.values();
    return Components(values.begin(), values.end());
}

bool ComponentManager::registerComponent(IComponent * component)
{
    Q_ASSERT(component != nullptr);

    Lock lock(_guard);

    if (IComponent * registeredComponent = findComponent(component->mnemonic(), component->version()))
    {
        return component == registeredComponent;
    }
    qDebug() << "Registering " << component->displayName();
    /*  TODO kill off? Change ...Managers to singletons?
    if (!SubsystemManager::registerSubsystem(component->subsystem()))
    {   //  OOPS! Can't proceed!
        return false;
    }
    */
    Mnemonic key = component->mnemonic() + "|" + util::toString(component->version());
    _registry[key] = component;
    return true;
}

IComponent * ComponentManager::findComponent(const tt3::util::Mnemonic & mnemonic, const QVersionNumber & version)
{
    Lock lock(_guard);

    Mnemonic key = mnemonic + "|" + util::toString(version);
    return _registry.contains(key) ? _registry[key] : nullptr;
}

IComponent * ComponentManager::findComponent(const Mnemonic & mnemonic)
{
    Lock lock(_guard);

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

void ComponentManager::loadComponentSettings()
{
    Lock lock(_guard);

    QDir home = QDir::home();
    QString iniFileName = home.filePath(".tt3");

    QFile iniFile(iniFileName);
    if (iniFile.open(QIODevice::ReadOnly))
    {
        IComponent * currentComponent = nullptr;
        QTextStream iniStream(&iniFile);
        while (!iniStream.atEnd())
        {
            QString line = iniStream.readLine();
            //  Section header ?
            line  = line .trimmed();
            if (line.startsWith("[") && line.endsWith("]"))
            {
                int separatorIndex = static_cast<int>(line.indexOf(':'));
                if (separatorIndex == -1)
                {
                    currentComponent = nullptr;
                    continue;
                }
                try
                {
                    Mnemonic componentMnemonic(line.mid(1, separatorIndex - 1));
                    QVersionNumber componentVersion(fromString<QVersionNumber>(line.mid(separatorIndex + 1, line.length() - separatorIndex - 2)));
                    currentComponent = ComponentManager::findComponent(componentMnemonic, componentVersion);
                    continue;
                }
                catch (const ParseException &)
                {
                    currentComponent = nullptr;
                    continue;
                }
            }
            //  <name> = <value>?
            int eqIndex = static_cast<int>(line.indexOf('='));
            if (eqIndex == -1 || currentComponent == nullptr)
            {
                continue;
            }
            Mnemonic settingMnemonic(line.mid(0, eqIndex).trimmed());
            QString settingValueString(line.mid(eqIndex + 1).trimmed());
            if (AbstractSetting * setting = currentComponent->settings()->findSetting(settingMnemonic))
            {
                setting->setValueString(settingValueString);
            }
        }
    }
}

void ComponentManager::saveComponentSettings()
{
    Lock lock(_guard);

    QDir home = QDir::home();
    QString iniFileName = home.filePath(".tt3");

    QFile iniFile(iniFileName);
    if (iniFile.open(QIODevice::WriteOnly))
    {
        QTextStream iniStream(&iniFile);
        for (IComponent * component : ComponentManager::allComponents())
        {
            iniStream << "["
                      << component->mnemonic().toString()
                      << ":"
                      << toString(component->version())
                      << "]"
                      << Qt::endl;
            for (AbstractSetting * setting : component->settings()->settings())
            {
                iniStream
                    << setting->mnemonic().toString()
                    << "="
                    << setting->valueString()
                    << Qt::endl;
            }
            iniStream << Qt::endl;
        }
    }
}

Locales ComponentManager::supportedLocales()
{
    tt3::util::Lock lock(_guard);

    Locales result;
    for (IComponent * component : _registry.values())
    {
        result.unite(component->resources()->supportedLocales());
    }
    return result;
}

Locales ComponentManager::fullySupportedLocales()
{
    tt3::util::Lock lock(_guard);

    Locales result;
    bool firstTime = true;
    for (IComponent * component : _registry.values())
    {
        if (firstTime)
        {
            result = component->resources()->supportedLocales();
            firstTime = false;
        }
        else
        {
            result.intersect(component->resources()->supportedLocales());
        }
    }
    return result;
}

//  End of tt3-util/ComponentManager.cpp
