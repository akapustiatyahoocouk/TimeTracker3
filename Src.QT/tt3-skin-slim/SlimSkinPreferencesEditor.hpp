//
//  tt3-skin-slim/SlimSkinPreferencesEditor.hpp - The "/Interface/SlimSkin" preferences editor
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
#include "tt3-skin-slim/API.hpp"

namespace tt3::skin::slim
{
    namespace Ui { class SlimSkinPreferencesEditor; }

    /// \class SlimSkinPreferencesEditor tt3-skin-slim/API.hpp
    /// \brief The editor widget for Slim skin preferences.
    class TT3_SKIN_SLIM_PUBLIC SlimSkinPreferencesEditor final
        :   public tt3::gui::PreferencesEditor
    {
        Q_OBJECT
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(SlimSkinPreferencesEditor)

    public:
        /// \brief
        ///     Constructs the widget.
        /// \param parent
        ///     The parent for the widget; nullptr == none.
        explicit SlimSkinPreferencesEditor(QWidget *parent = nullptr);

        /// \brief
        ///     The class destructor.
        virtual ~SlimSkinPreferencesEditor();

        //////////
        //  tt3::gui::PreferencesEditor
    public:
        virtual auto    preferences() const -> tt3::gui::Preferences * override;
        virtual void    loadControlValues() override;
        virtual void    saveControlValues() override;
        virtual void    resetControlValues() override;
        virtual bool    isValid() const override;

        //////////
        //  Controls
    private:
        Ui::SlimSkinPreferencesEditor *const    _ui;
    };
}

//  End of tt3-skin-slim/SlimSkinPreferencesEditor.hpp

