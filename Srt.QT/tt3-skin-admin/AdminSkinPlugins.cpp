//
//  tt3-skin-admin/AdminSkinPlugins.cpp - admin skin plugins
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
#include "tt3-skin-admin/API.hpp"
using namespace skin::admin;

namespace
{
    class AdminSkinPlugin final : public virtual util::IPlugin
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(AdminSkinPlugin)

        //////////
        //  Construction/destruction
    public:
        AdminSkinPlugin() = default;
        virtual ~AdminSkinPlugin() = default;

        //////////
        //  util::IPlugin
    public:
        virtual QString         mnemonic() const override
        {
            return "tt3-skin-admin";
        }

        virtual QString         displayName() const override
        {
            return "Administrator skin";
        }

        virtual QString         desciption() const override
        {
            return "Enables administrator skin for TimeTracker3";
        }

        virtual QString         copyright() const override
        {
            return "Copyright (C) 2026, Andrey Kapustin";
        }

        virtual QVersionNumber  version() const override
        {
            return QVersionNumber(1, 0, 0);
        }

        virtual void            initialize() throws(QException) override
        {
            util::ComponentRegistry::registerComponent(AdminSkinComponent::instance());
            gui::SkinRegistry::registerSkin(AdminSkin::instance());
        }
    };
}

BEGIN_PLUGIN_TABLE()
    EXPORT_PLUGIN(AdminSkinPlugin)
END_PLUGIN_TABLE()

//  End of tt3-skin-admin/AdminSkinPlugins.cpp
