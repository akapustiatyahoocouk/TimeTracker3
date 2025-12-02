//
//  tt3-gui/FullScreenReminderWindow.hpp - The full-screen activity  reminder window
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
    namespace Ui { class FullScreenReminderWindow; }

    /// \class FullScreenReminderWindow tt3-gui/API.hpp
    /// \brief The full-screen activity  reminder window.
    class TT3_GUI_PUBLIC FullScreenReminderWindow
        :   public QDialog
    {
        Q_OBJECT
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(FullScreenReminderWindow)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the full-screen reminder window.
        FullScreenReminderWindow();

        /// \brief
        ///     The class destructor.
        virtual ~FullScreenReminderWindow();

        //////////
        //  QWidget
    protected:
        /// \brief
        ///     Called to handle a "dialog window shown" event.
        /// \param event
        ///     The event detils.
        void            showEvent(QShowEvent * event) override;

        /// \brief
        ///     Called to handle a "dialog window hidden" event.
        /// \param event
        ///     The event detils.
        void            hideEvent(QHideEvent * event) override;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Refreshes this window to reflect the "current" activity.
        void            refresh();

        //////////
        //  Controls
    private:
        Ui::FullScreenReminderWindow *const _ui;
        QTimer                  _refreshTimer;
;

        //  Drawing resources
        LabelDecorations        _labelDecorations;
        PushButtonDecorations   _pushButtonDecorations;

        //////////
        //  Signal handlers
    private slots:
        void            _currentThemeChanged(ITheme *, ITheme *);
        void            _stopPushButtonClicked();
        void            _refreshTimerTimeout();
        virtual void    accept() override;
        virtual void    reject() override;
    };
}

//  End of tt3-gui/FullScreenReminderWindow.hpp

