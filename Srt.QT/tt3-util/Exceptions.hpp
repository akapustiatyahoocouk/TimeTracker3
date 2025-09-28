//
//  tt3-util/Exceptions.cpp - Exceptions framework
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

namespace tt3::util
{
    /// The common base class for all tt3-specific throwable objects.
    using Throwable = QException;

    /// \class Exception tt3-util/API.hpp
    /// \brief The common base class for all tt3-specific exceptions.
    /// \details
    ///     These are throwables intended to be caught; they will
    ///     never be throws if the TT3 operated in an ideal world.
    class TT3_UTIL_PUBLIC Exception
        :   public Throwable
    {
        using Self = Exception;

        //////////
        //  Construction/destruction/assignment
    public:
        /// \brief
        ///     Constructs the exception.
        Exception() = default;

        //////////
        //  QException
    public:
        /// \brief
        ///     Throws this exception.
        /// \exception
        ///     *this always.
        Q_NORETURN
        void            raise() const override = 0;

        /// \brief
        ///     Returns a deep clone of this exception.
        /// \return
        ///     The deep clone of this exception.
        Self *          clone() const override = 0;

            //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the error message describing the exception.
        /// \return
        ///     The error message describing the exception for
        ///     the current default locale
        virtual QString errorMessage() const = 0;
    };

    /// \class MissingResourceException tt3-util/API.hpp
    /// \brief Thrown when a requested resource is not available.
    class TT3_UTIL_PUBLIC MissingResourceException
        :   public Exception
    {
        using Self = MissingResourceException;

        //////////
        //  Construction/destruction/assignment
    public:
        /// \brief
        ///     Constructs the exception.
        /// \param resourceFactoryName
        ///     The name of the resource factory where a
        ///     resource is missing.
        /// \param sectionId
        ///     The ID of the section whetr a resource is missing.
        /// \param resourceId
        ///     The ID of the missing resource.
        MissingResourceException(
                const QString & resourceFactoryName,
                const ResourceSectionId & sectionId,
                const ResourceId & resourceId
            );
        //  The default copy constructor, assignment operator
        //  and destructor are all OK.

        //////////
        //  QException
    public:
        Q_NORETURN
        void            raise() const override { throw *this; }
        Self *          clone() const override { return new Self(*this); }

        //////////
        //  Exception
    public:
        virtual QString errorMessage() const override;

        //////////
        //  Property accessors
    public:
        /// \brief
        ///     Returns the name of the resource factory where a resource is missing.
        /// \return
        ///     The name of the resource factory where a resource is missing.
        QString         resourceFactoryName(
                            ) const { return _resourceFactoryName; }

        /// \brief
        ///     Returns the ID of the resource section where the resource is missing.
        /// \return
        ///     The ID of the resource section where the resource is missing.
        auto            sectionId(
                            ) const -> ResourceSectionId { return _sectionId; }

        /// \brief
        ///     Returnshe ID of the resource that is missing.
        /// \return
        ///     The ID of the resource that is missing.
        auto            resourceId(
                            ) const -> ResourceId { return _resourceId; }

        //////////
        //  Implementation
    private:
        QString             _resourceFactoryName;
        ResourceSectionId   _sectionId;
        ResourceId          _resourceId;
    };

    /// \brief
    ///     Thrown when an attempt to parse a string
    ///     (or a portion thereof) fails.
    class TT3_UTIL_PUBLIC ParseException
        :   public Exception
    {
        using Self = ParseException;

        //////////
        //  Construction/destruction/assignment
    public:
        /// \brief
        ///     Constructs the exception.
        /// \param string
        ///     The string which was being parsed.
        /// \param position
        ///     The position within the string at which
        ///     a parse error has occurred.
        ParseException(
                const QString & string,
                qsizetype position
            );
        //  The default copy constructor, assignment operator
        //  and destructor are all OK.

        //////////
        //  QException
    public:
        void            raise() const override { throw *this; }
        Self *          clone() const override { return new Self(*this); }

        //////////
        //  Exception
    public:
        virtual QString errorMessage(
                            ) const override;

        //////////
        //  Property accessors
    public:
        /// \brief
        ///     Returns the string which has failed to parse.
        /// \return
        ///     The string which has failed to parse.
        QString         string(
                            ) const { return _string; }

        /// \brief
        ///     Returns the position within the parsed string where a
        ///     parse error occurred
        /// \return
        ///     The position within the parsed string where a parse
        ///     error occurred
        auto            position(
                            ) const -> qsizetype { return _position; }

        //////////
        //  Implementation
    private:
        QString         _string;
        qsizetype       _position;
    };

    /// \class Error tt3-util/API.hpp
    /// \brief The common base class for all tt3-specific errors.
    /// \details
    ///     These are throwables NOT intended to be caught; they will
    ///     never be throws if the TT3 operated in a sane world.
    class TT3_UTIL_PUBLIC Error
        :   public Throwable
    {
        using Self = Error;

        //////////
        //  Construction/destruction/assignment
    public:
        /// \brief
        ///     Constructs the error.
        Error() = default;

        //////////
        //  QException
    public:
        /// \brief
        ///     Throws this error.
        /// \exception
        ///     *this always.
        Q_NORETURN
            void            raise() const override = 0;

        /// \brief
        ///     Returns a deep clone of this error.
        /// \return
        ///     The deep clone of this error.
        Self *          clone() const override = 0;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the error message describing the error.
        /// \return
        ///     The error message describing the error for
        ///     the current default locale
        virtual QString errorMessage(
                            ) const = 0;
    };

    /// \class ProgramError tt3-util/API.hpp
    /// \brief The common base class for all program errors.
    /// \details
    ///     These are errors that indicate the need to fix an
    ///     issue in the program.
    class TT3_UTIL_PUBLIC ProgramError
        :   public Error
    {
        using Self = ProgramError;

        //////////
        //  Construction/destruction/assignment
    public:
        /// \brief
        ///     Constructs the error.
        ProgramError() = default;
    };

    /// \class SystemError tt3-util/API.hpp
    /// \brief The common base class for all program errors.
    /// \details
    ///     These are errors that indicate an unusual condition
    ///     in the system which prevents TT3 from running normally.
    class TT3_UTIL_PUBLIC SystemError
        :   public Error
    {
        using Self = SystemError;

        //////////
        //  Construction/destruction/assignment
    public:
        /// \brief
        ///     Constructs the error.
        SystemError() = default;
    };

    /// \class NotImplementedError tt3-util/API.hpp
    /// \brief Thrown when a feature is not implemented.
    class TT3_UTIL_PUBLIC NotImplementedError
        :   public ProgramError
    {
        using Self = NotImplementedError;

        //////////
        //  Construction/destruction/assignment
    public:
        /// \brief
        ///     Constructs the error.
        NotImplementedError();
        //  The default copy constructor, assignment operator
        //  and destructor are all OK.

        //////////
        //  QException
    public:
        Q_NORETURN
        void            raise() const override { throw *this; }
        Self *          clone() const override { return new Self(*this); }

        //////////
        //  Error
    public:
        virtual QString errorMessage(
                            ) const override;
    };
}

//  End of tt3-util/Exceptions.cpp
