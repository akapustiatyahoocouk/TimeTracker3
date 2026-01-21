//
//  tt3-db-xml/PreferencesEditor.hpp - XML File database preferences editor
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
#pragma once
#include "tt3-db-xml/API.hpp"

namespace tt3::db::xml
{
    namespace Ui { class PreferencesEditor; }

    /// \class PreferencesEditor tt3-db-xml/API.hpp
    /// \brief The "/Storage/XmlFile" preferences editor.
    class TT3_DB_XML_PUBLIC PreferencesEditor final
        :   public tt3::gui::PreferencesEditor
    {
        Q_OBJECT
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(PreferencesEditor)

        //////////
        //  Construction/destruction
    public:
        explicit PreferencesEditor(QWidget *parent = nullptr);
        virtual ~PreferencesEditor();

        //////////
        //  PreferencesEditor
    public:
        virtual auto    preferences() const -> Preferences * override;
        virtual void    loadControlValues() override;
        virtual void    saveControlValues() override;
        virtual void    resetControlValues() override;
        virtual bool    isValid() const override;

        //////////
        //  Implementation
    private:
        //  Helpers
        tt3::util::TimeSpan _selectedSaveInterval() const;
        void                _setSelectedSaveInterval(const tt3::util::TimeSpan & saveInterval);

        //////////
        //  Controls
    private:
        Ui::PreferencesEditor * _ui;

        //////////
        //  Signal handlers
    private slots:
        void            _saveIntervalConboBoxCurrentIndexChanged(int);
    };
}

//  End of tt3-db-xml/PreferencesEditor.hpp

