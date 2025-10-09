//
//  tt3-gui/Color.hpp - Color management helpers
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
    /// \class ColorManager tt3-gui/API.hpp
    /// \brief Provides color management helper services.
    class TT3_GUI_PUBLIC ColorManager final
    {
        UTILITY_CLASS(ColorManager)

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns a color that is mid-way between the two colors.
        /// \param a
        ///     The 1st color.
        /// \param b
        ///     The 2nd color.
        /// \return
        ///     The color that is mid-way between the two colors.
        static QColor   mid(const QColor & a, const QColor & b);

        /// \brief
        ///     Returns a visually "redder" variant of the original color.
        /// \param c
        ///     The original color
        /// \return
        ///     The visually "redder" variant of the original color.
        static QColor   redder(const QColor & c);

        /// \brief
        ///     Returns a visually "slightly redder" variant of the original color.
        /// \param c
        ///     The original color
        /// \return
        ///     The visually "slightly redder" variant of the original color.
        static QColor   slightlyRedder(const QColor & c);

        /// \brief
        ///     Returns a visually "greener" variant of the original color.
        /// \param c
        ///     The original color
        /// \return
        ///     The visually "greener" variant of the original color.
        static QColor   greener(const QColor & c);

        /// \brief
        ///     Returns a visually "slightly greener" variant of the original color.
        /// \param c
        ///     The original color
        /// \return
        ///     The visually "slightly greener" variant of the original color.
        static QColor   slightlyGreener(const QColor & c);
    };
}

//  End of tt3-gui/Color.hpp
