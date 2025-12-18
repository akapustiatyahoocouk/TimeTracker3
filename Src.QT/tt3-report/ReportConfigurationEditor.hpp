//
//  tt3-report/ReportConfigurationEditor.hpp - tt3 report configuration editor ADT
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
#include "tt3-report/API.hpp"

namespace tt3::report
{
    /// \class ReportConfigurationEditor tt3-report/API.hpp
    /// \brief A configuration editor for a report generation job.
    class TT3_REPORT_PUBLIC ReportConfigurationEditor
        :   public QWidget
    {
        Q_OBJECT
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ReportConfigurationEditor)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the widget.
        /// \param parent
        ///     The parent for the new widget; nullptr == none.
        explicit ReportConfigurationEditor(
                QWidget * parent
            );

        /// \brief
        ///     The class destructor.
        virtual ~ReportConfigurationEditor();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Populates inner widgets with values taken
        ///     from persistent settings.
        virtual void    loadControlValues() = 0;

        /// \brief
        ///     Saves current values of inner widgets to
        ///     persistent settings IF THESE ARE ALL VALID,
        ///     ELSE DOES NOT SAVE ANY OF THEM.
        virtual void    saveControlValues() = 0;

        /// \brief
        ///     Resets the values of inner widgets to their defaults.
        virtual void    resetControlValues() = 0;

        /// \brief
        ///     Checks whether all inner widgets have values
        ///     that can be saved to persistent settings.
        /// \details
        ///     All text fields that allow only numeric values
        ///     have only valid numbers in them, etc.
        /// \return
        ///     True if all inner widgets have values that can
        ///     be saved to persistent settings, else false.
        virtual bool    isValid() const = 0;

        //////////
        //  Signals
    signals:
        /// \brief
        ///     Emitted when the value of some inner widget changes.
        void            controlValueChanged();
    };
}

//  End of tt3-report/ReportConfigurationEditor.hpp
