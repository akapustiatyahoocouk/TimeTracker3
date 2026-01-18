//
//  tt3-db-xml/Preferences.hpp - XML File database preferences
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
    /// \class Preferences tt3-db-xml/API.hpp
    /// \brief The "/Storage/XmlFile" preferences.
    class TT3_DB_XML_PUBLIC Preferences final
        :   public tt3::gui::Preferences
    {
        TT3_DECLARE_SINGLETON(Preferences)

        //////////
        //  Preferences
    public:
        virtual auto    mnemonic() const -> Mnemonic override;
        virtual QString displayName() const override;
        virtual auto    parent() const -> tt3::gui::Preferences* override;
        virtual int     order() const override { return 0; }
        virtual auto    createEditor(
                            ) -> tt3::gui::PreferencesEditor * override;
    };
}

//  End of tt3-db-xml/Preferences.hpp
