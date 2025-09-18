//
//  tt3-db-xml/Plugins.cpp - XML file DB plugins
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
#include "tt3-db-xml/API.hpp"
using namespace tt3::db::xml;

namespace
{
    class XmlDbPlugin final
        :   public virtual tt3::util::IPlugin
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(XmlDbPlugin)

        //////////
        //  Construction/destruction
    public:
        XmlDbPlugin() = default;
        virtual ~XmlDbPlugin() = default;

        //////////
        //  util::IPlugin
    public:
        using Mnemonic = tt3::util::Mnemonic;

        virtual Mnemonic        mnemonic() const override
        {
            return M(tt3-db-xml);
        }

        virtual QString         displayName() const override
        {
            return "XML file databases";
        }

        virtual QString         description() const override
        {
            return "Enables storing TimeTracker3 databases as XML files (single-user)";
        }

        virtual QString         copyright() const override
        {
            return tt3::util::ProductInformation::applicationCopyright();
        }

        virtual QVersionNumber  version() const override
        {
            return tt3::util::ProductInformation::applicationVersion();
        }

        virtual QString         buildNumber() const override
        {
            return tt3::util::ProductInformation::applicationBuildNumber();
        }

        virtual void            initialize() override
        {
            tt3::util::ComponentManager::registerComponent(Component::instance());
            tt3::db::api::DatabaseTypeManager::registerDatabaseType(DatabaseType::instance());
        }
    };
}

BEGIN_PLUGIN_TABLE()
    EXPORT_PLUGIN(XmlDbPlugin)
END_PLUGIN_TABLE()

//  End of tt3-db-xml/Plugins.cpp
