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

    /// \class HelpSiteDoesNotExistException tt3-help/API.hpp
    /// \brief Thrown when a help site is missing or corrupt.
    class TT3_HELP_PUBLIC HelpSiteDoesNotExistException
        :   public HelpException
    {
        //////////
        //  Types
    public:
        /// \brief A type alias to improve code readability.
        using Self = HelpSiteDoesNotExistException;

        //////////
        //  Construction/destruction/assignment
    public:
        /// \brief
        ///     Constructs the exception.
        /// \param location
        ///     The location of the nonexistent help site.
        explicit HelpSiteDoesNotExistException(
                const QString & location
            );
        //  The default destructor, copy constructor and assignment are all OK

        //////////
        //  QException
    public:
        virtual Self *  clone() const override { return new Self(*this); }
        virtual void    raise() const override { throw *this; }

        //////////
        //  tt3::util::Exception
    public:
        virtual QString errorMessage() const override;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the location of the help site.
        /// \return
        ///     The location of the help site.
        QString         location() const { return _location; }

        //////////
        //  Implementation
    private:
        QString         _location;
    };

    /// \class CannotCreateDirectoryException tt3-help/API.hpp
    /// \brief Thrown when a directory needed by help cannot be created.
    class TT3_HELP_PUBLIC CannotCreateDirectoryException
        :   public HelpException
    {
        //////////
        //  Types
    public:
        /// \brief A type alias to improve code readability.
        using Self = CannotCreateDirectoryException;

        //////////
        //  Construction/destruction/assignment
    public:
        /// \brief
        ///     Constructs the exception.
        /// \param path
        ///     The path to the offending directory.
        explicit CannotCreateDirectoryException(
                const QString & path
            );
        //  The default destructor, copy constructor and assignment are all OK

        //////////
        //  QException
    public:
        virtual Self *  clone() const override { return new Self(*this); }
        virtual void    raise() const override { throw *this; }

        //////////
        //  tt3::util::Exception
    public:
        virtual QString errorMessage() const override;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the path to the offending directory.
        /// \return
        ///     The path to the offending directory.
        QString         path() const { return _path; }

        //////////
        //  Implementation
    private:
        QString         _path;
    };

    /// \class FileAlreadyExistsException tt3-help/API.hpp
    /// \brief Thrown when overwriting an exisrting help file with different content.
    class TT3_HELP_PUBLIC FileAlreadyExistsException
        :   public HelpException
    {
        //////////
        //  Types
    public:
        /// \brief A type alias to improve code readability.
        using Self = FileAlreadyExistsException;

        //////////
        //  Construction/destruction/assignment
    public:
        /// \brief
        ///     Constructs the exception.
        /// \param path
        ///     The path to the offending file.
        explicit FileAlreadyExistsException(
                const QString & path
            );
        //  The default destructor, copy constructor and assignment are all OK

        //////////
        //  QException
    public:
        virtual Self *  clone() const override { return new Self(*this); }
        virtual void    raise() const override { throw *this; }

        //////////
        //  tt3::util::Exception
    public:
        virtual QString errorMessage() const override;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the path to the offending directory.
        /// \return
        ///     The path to the offending directory.
        QString         path() const { return _path; }

        //////////
        //  Implementation
    private:
        QString         _path;
    };

    /// \class UnsupportedProtocolException tt3-help/API.hpp
    /// \brief Thrown when attemtng to use URL with an unsupported protocol.
    class TT3_HELP_PUBLIC UnsupportedProtocolException
        :   public HelpException
    {
        //////////
        //  Types
    public:
        /// \brief A type alias to improve code readability.
        using Self = UnsupportedProtocolException;

        //////////
        //  Construction/destruction/assignment
    public:
        /// \brief
        ///     Constructs the exception.
        /// \param url
        ///     The offending URL.
        explicit UnsupportedProtocolException(
                const QString & url
            );
        //  The default destructor, copy constructor and assignment are all OK

        //////////
        //  QException
    public:
        virtual Self *  clone() const override { return new Self(*this); }
        virtual void    raise() const override { throw *this; }

        //////////
        //  tt3::util::Exception
    public:
        virtual QString errorMessage() const override;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the string form of the offending URL.
        /// \return
        ///     The string form of the offending URL.
        QString         url() const { return _url; }

        //////////
        //  Implementation
    private:
        QString         _url;
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
        /// \param errorMessage
        ///     The custom error message.
        explicit CustomHelpException(
                const QString & errorMessage
            ) : _errorMessage(errorMessage) {}
        //  The default destructor, copy constructor and assignment are all OK

        //////////
        //  QException
    public:
        virtual Self *  clone() const override { return new Self(*this); }
        virtual void    raise() const override { throw *this; }

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

