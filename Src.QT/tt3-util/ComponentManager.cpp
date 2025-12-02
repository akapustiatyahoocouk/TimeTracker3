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

struct ComponentManager::_Impl
{
    Mutex                       guard;
    QMap<Mnemonic, IComponent*> registry;
    Components                  initializedComponents;
};

namespace
{
    QString iniFileName()
    {
        QDir home = QDir::home();
        return home.filePath(".tt3");
    }
}

//////////
//  Operations
Components ComponentManager::allComponents()
{
    _Impl * impl = _impl();
    Lock _(impl->guard);

    QList<IComponent*> values = impl->registry.values();
    return Components(values.cbegin(), values.cend());
}

bool ComponentManager::registerComponent(IComponent * component)
{
    Q_ASSERT(component != nullptr);

    _Impl * impl = _impl();
    Lock _(impl->guard);

    if (IComponent * registeredComponent = findComponent(component->mnemonic(), component->version()))
    {
        return component == registeredComponent;
    }
    if (!SubsystemManager::registerSubsystem(component->subsystem()))
    {   //  OOPS! Can't proceed!
        return false;
    }
    Mnemonic key = component->mnemonic() + "|" + util::toString(component->version());
    impl->registry[key] = component;
    return true;
}

IComponent * ComponentManager::findComponent(const tt3::util::Mnemonic & mnemonic, const QVersionNumber & version)
{
    _Impl * impl = _impl();
    Lock _(impl->guard);

    Mnemonic key = mnemonic + "|" + toString(version);
    return impl->registry.contains(key) ? impl->registry[key] : nullptr;
}

IComponent * ComponentManager::findComponent(const Mnemonic & mnemonic)
{
    _Impl * impl = _impl();
    Lock _(impl->guard);

    IComponent * result = nullptr;
    for (IComponent * component : impl->registry.values())
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

void ComponentManager::initializeComponents()
{
    _Impl * impl = _impl();
    Lock _(impl->guard);

    for (bool keepGoing = true; keepGoing; )
    {
        keepGoing = false;
        for (auto component : impl->registry.values())
        {
            if (!impl->initializedComponents.contains(component))
            {   //  Try this one!
                try
                {
                    component->iniialize(); //  may throw
                    impl->initializedComponents.insert(component);
                    keepGoing = true;
                }
                catch (const Exception & ex)
                {   //  OOPS! Log, but suppress
                    qCritical() << ex;
                }
                catch (const Error & ex)
                {   //  OOPS! Log, but suppress
                    qCritical() << ex;
                }
                catch (...)
                {   //  OOPS! Suppress, though
                }
            }
        }

    }
}

void ComponentManager::deinitializeComponents()
{
    _Impl * impl = _impl();
    Lock _(impl->guard);

    for (auto component : impl->initializedComponents)
    {
        try
        {   //  Be defensive - cleanup as many as possible
            component->deiniialize();
        }
        catch (const Exception & ex)
        {   //  OOPS! Log, but suppress
            qCritical() << ex;
        }
        catch (const Error & ex)
        {   //  OOPS! Log, but suppress
            qCritical() << ex;
        }
        catch (...)
        {   //  OOPS! Suppress, though
        }
    }
    impl->initializedComponents.clear();
}

void ComponentManager::loadComponentSettings()
{
    _Impl * impl = _impl();
    Lock _(impl->guard);

    QFile iniFile(iniFileName());
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
                    currentComponent = findComponent(componentMnemonic, componentVersion);
                    continue;
                }
                catch (const ParseException & ex)
                {
                    qCritical() << ex;
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
    else
    {   //  OOPS! Log, but suppress
        qCritical() << iniFile.errorString();
    }
}

void ComponentManager::saveComponentSettings()
{
    _Impl * impl = _impl();
    Lock _(impl->guard);

    QFile iniFile(iniFileName());
    if (iniFile.open(QIODevice::WriteOnly))
    {
        QTextStream iniStream(&iniFile);

        QList<IComponent*> allComponents =
            ComponentManager::allComponents().values();
        std::sort(
            allComponents.begin(),
            allComponents.end(),
            [](auto a, auto b)
            {
                return a->mnemonic() < b->mnemonic();
            });
        for (IComponent * component : std::as_const(allComponents))
        {   //  Sorted by component mnemonic to simplify lookin text editor
            iniStream << "["
                      << component->mnemonic().toString()
                      << ":"
                      << toString(component->version())
                      << "]"
                      << Qt::endl;

            QList<AbstractSetting*> componentSettings =
                component->settings()->settings().values();
            std::sort(
                componentSettings.begin(),
                componentSettings.end(),
                [](auto a, auto b)
                {
                    return a->mnemonic() < b->mnemonic();
                });
            for (AbstractSetting * setting : std::as_const(componentSettings))
            {   //  Sorted by setting mnemonic to simplify lookin text editor
                iniStream << setting->mnemonic().toString()
                          << "="
                          << setting->valueString()
                          << Qt::endl;
            }
            iniStream << Qt::endl;
        }
    }
    else
    {   //  OOPS! Log, but suppress
        qCritical() << iniFile.errorString();
    }
}

Locales ComponentManager::supportedLocales()
{
    _Impl * impl = _impl();
    Lock _(impl->guard);

    Locales result;
    for (IComponent * component : impl->registry.values())
    {
        result.unite(component->resources()->supportedLocales());
    }
    return result;
}

Locales ComponentManager::fullySupportedLocales()
{
    _Impl * impl = _impl();
    Lock _(impl->guard);

    Locales result;
    bool firstTime = true;
    for (IComponent * component : impl->registry.values())
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

//////////
//  Implementation helpers
ComponentManager::_Impl * ComponentManager::_impl()
{
    static _Impl impl;

    return &impl;
}

//  End of tt3-util/ComponentManager.cpp
