//
//  tt3-gui/PreferencesDialog.hpp - The modal "Preferences" dialog
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
#include "tt3-gui/API.hpp"

namespace tt3::gui
{
    //  The modal "Preferences" dialog
    namespace Ui { class PreferencesDialog; }

    class TT3_GUI_PUBLIC PreferencesDialog final : public QDialog
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(PreferencesDialog)

        //////////
        //  Construction/destruction
    public:
        explicit PreferencesDialog(QWidget * parent);
        ~PreferencesDialog();

        //////////
        //  Implementation
    private:
        QMap<QTreeWidgetItem*, PreferencesEditor*>  _editorsForItems;

        //  Helpers
        static bool     _compare(const Preferences * a, const Preferences * b);
        void            _createChildItems(QTreeWidgetItem * parentItem,
                                          QMap<Preferences*,QTreeWidgetItem*> & itemsForPreferences);
        void            _createEditor(QTreeWidgetItem * item);
        void            _loadCurrentPreferences(const QMap<Preferences*,QTreeWidgetItem*> & itemsForPreferences);
        void            _saveCurrentPreferences();

        //////////
        //  Controls
    private:
        Ui::PreferencesDialog * _ui;
        QLabel *            _noPropertiesLabel;
        QStackedLayout *    _editorsFrameLayout;

        //////////
        //  Signal handlers
    private slots:
        void                _preferencesTreeWidgetCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*);
        void                _refresh();
        void                _accept();
        void                _reject();
    };
}

//  End of tt3-gui/PreferencesDialog.hpp

