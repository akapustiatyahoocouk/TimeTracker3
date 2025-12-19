//
//  tt3-gui/SplashScreen.hpp - The TT3 splash screen
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
    namespace Ui { class SplashScreen; }

    /// \class SplashScreen tt3-gui/API.hpp
    /// \brief The TT3 splash screen.
    class TT3_GUI_PUBLIC SplashScreen final
        :   public QDialog  //  TODO private
    {
        Q_OBJECT
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(SplashScreen)

        //////////
        //  Constants
    public:
        /// \brief
        ///     The preferred time, in milliseconds, for which
        ///     the splash screen is visible during TT3 startup.
        static const int PreferredStartupDurationMs = 5000;

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs, but does not show, the splash screen.
        SplashScreen();

        /// \brief
        ///     The class destructor.
        virtual ~SplashScreen();

        //////////
        //  Operations
    public:
        void        showStartupProgress(
                            const QString & action,
                            const QString & context,
                            float ratioCompleted
                        );

        //////////
        //  Implementation
    private:
        float       _lastRatioCompleted = 0.0;

        //////////
        //  Controls
    private:
        Ui::SplashScreen *  _ui;

        //////////
        //  Signal handlers
    private slots:
        virtual void    accept() override;
        virtual void    reject() override;
    };
}

//  End of tt3-gui/SplashScreen.hpp

