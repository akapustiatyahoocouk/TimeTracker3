//
//  tt3-help/Exceptions.hpp - Help exceptions
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

namespace tt3::help
{
    /// \class HelpException tt3-help/API.hpp
    /// \brief The common base class for all Help exceptions.
    class TT3_HELP_PUBLIC HelpException
        :   public tt3::util::Exception
    {
        //////////
        //  Construction/destruction/assignment
    public:
        /// \brief
        ///     Constructs the exception.
        HelpException() = default;
    };

    /// \class CustomHelpException tt3-help/API.hpp
    /// \brief Thrown when must carry a custom error message (from OS, etc.)
    class TT3_HELP_PUBLIC CustomHelpException
        :   public HelpException
    {
        //////////
        //  Types
    public:
        /// \brief A type alias to improve code readability.
        using Self = CustomHelpException;

        //////////
        //  Construction/destruction/assignment
    public:
        /// \brief
        ///     Constructs the exception.
        /// \param cerrorMessage
        ///     The custom error message.
        explicit CustomHelpException(
                const QString & errorMessage
            ) : _errorMessage(errorMessage) {}

        //////////
        //  QException
    public:
        virtual Self *  clone() const { return new Self(*this); }
        virtual void    raise() const { throw *this; }

        //////////
        //  tt3::util::Exception
    public:
        virtual QString errorMessage() const override { return _errorMessage; }

        //////////
        //  Implementation
    private:
        QString         _errorMessage;
    };
}

//  End of tt3-Exceptions/Exceptions.hpp

