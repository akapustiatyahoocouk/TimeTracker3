//
//  tt3-tgui/UiHelpers.hpp - various UI helpers
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

namespace tt3::gui
{
    /// \class RefreshGuard tt3-gui/API.hpp
    /// \brief A helper class to avoid recursive refresh of UI widgets.
    class TT3_GUI_PUBLIC RefreshGuard final
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(RefreshGuard);

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     The class constructor.
        /// \details
        ///     Captures the previous value of a "refresh guard" -
        ///     a variable that known whether a refresh()) is
        ///     currently underway - and sets it to true.
        /// \param refreshUnderway
        ///     The STATIC refresh guard.
        explicit RefreshGuard(bool & refreshUnderway)
            :   _refreshUnderway(refreshUnderway),
                _savedRefreshUnderway(refreshUnderway)
        {
            _refreshUnderway = true;
        }

        /// \brief
        ///     The class destructor.
        /// \details
        ///     Restores the saved "refresh guard" value.
        ~RefreshGuard()
        {
            _refreshUnderway = _savedRefreshUnderway;
        }

        //////////
        //  Operators
    public:
        /// \brief
        ///     Checks whether a refresh() should proceed.
        /// \details
        ///     A refresh() should proceed when a "refresh guard"
        ///     value captured diring instance construction is false.
        /// \return
        ///     True if a refresh() should proceed, false if not.
                    operator bool() const { return !_savedRefreshUnderway; }

        //////////
        //  Implementation
    private:
        bool &      _refreshUnderway;
        const bool  _savedRefreshUnderway;
    };
}

//  End of tt3-gui/UiHelpers.hpp
