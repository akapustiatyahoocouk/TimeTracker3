//
//  tt3-util/ToolManager.cpp - tt3::util::ToolManager class implementation
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

struct ToolManager::_Impl
{
    Mutex                   guard;
    QMap<Mnemonic, ITool*>  registry;
};

//////////
//  Operations
Tools ToolManager::allTools()
{
    _Impl * impl = _impl();
    Lock _(impl->guard);

    QList<ITool*> values = impl->registry.values();
    return Tools(values.cbegin(), values.cend());
}

bool ToolManager::registerTool(ITool * tool)
{
    Q_ASSERT(tool != nullptr);

    _Impl * impl = _impl();
    Lock _(impl->guard);

    if (impl->registry.contains(tool->mnemonic()))
    {
        return tool == impl->registry[tool->mnemonic()];
    }
    impl->registry[tool->mnemonic()] = tool;
    return true;
}

ITool * ToolManager::findTool(const Mnemonic & mnemonic)
{
    _Impl * impl = _impl();
    Lock _(impl->guard);

    return impl->registry.contains(mnemonic) ? impl->registry[mnemonic] : nullptr;
}

//////////
//  Implementation helpers
ToolManager::_Impl * ToolManager::_impl()
{
    static _Impl impl;
    return &impl;
}

//  End of tt3-util/ToolManager.cpp
