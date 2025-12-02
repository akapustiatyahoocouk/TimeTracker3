//
//  tt3-db-xml/Plugins.hpp - tt3-db-xml Plugins
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

namespace tt3::db::xml
{
    /// \class Plugin tt3-db-xml/API.hpp
    /// \brief The "TT3 XML file database" plugin.
    class TT3_DB_XML_PUBLIC Plugin final
        :   public virtual tt3::util::IPlugin
    {
        DECLARE_SINGLETON(Plugin)

        //////////
        //  tt3::util::IPlugin
    public:
        virtual Mnemonic        mnemonic() const override;
        virtual QString         displayName() const override;
        virtual QString         description() const override;
        virtual QString         copyright() const override;
        virtual QVersionNumber  version() const override;
        virtual QString         buildNumber() const override;
        virtual void            initialize() override;
    };
}

//  End of tt3-db-xml/Plugins.hpp
