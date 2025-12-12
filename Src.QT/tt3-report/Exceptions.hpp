//
//  tt3-report/Exceptions.hpp - reporting exceptions
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

namespace tt3::report
{
    /// \class ReportException tt3-report/API.hpp
    /// \brief The common base class for all reporting exceptions.
    class TT3_REPORT_PUBLIC ReportException
        :   public tt3::util::Exception
    {
        //////////
        //  Construction/destruction/assignment
    public:
        /// \brief
        ///     Constructs the exception.
        ReportException();
    };


    /// \class InvalidReportTemplateException tt3-db-api/API.hpp
    /// \brief Thrown when a report template is malformed.
    class TT3_REPORT_PUBLIC InvalidReportTemplateException
        :   public ReportException
    {
        //////////
        //  Types
    public:
        /// \brief A type alias to improve code readability.
        using Self = InvalidReportTemplateException;

        //////////
        //  Construction/destruction/assignment
    public:
        /// \brief
        ///     Constructs the exception.
        InvalidReportTemplateException();

        //////////
        //  QException
    public:
        virtual Self *  clone() const override { return new Self(*this); }
        virtual void    raise() const override { throw *this; }

        //////////
        //  tt3::util::Exception
    public:
        virtual QString errorMessage() const override;
    };

    /// \class ReportTemplateDoesNotExistException tt3-db-api/API.hpp
    /// \brief Thrown when a report template conflict occurs.
    class TT3_REPORT_PUBLIC ReportTemplateAlreadyExistsException
        :   public ReportException
    {
        //////////
        //  Types
    public:
        /// \brief A type alias to improve code readability.
        using Self = ReportTemplateAlreadyExistsException;

        //////////
        //  Construction/destruction/assignment
    public:
        /// \brief
        ///     Constructs the exception.
        /// \param mnemonic
        ///     The mnemonic identifier of the conflicting report template.
        /// \param displayName
        ///     The user-readable display name of the conflicting report template.
        ReportTemplateAlreadyExistsException(
                const tt3::util::Mnemonic & mnemonic,
                const QString & displayName
            );

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
        ///     Returns the mnemonic identifier of the conflicting report template.
        /// \return
        ///     The mnemonic identifier of the conflicting report template.
        auto            mnemonic() const -> tt3::util::Mnemonic { return _mnemonic; }

        /// \brief
        ///     Returns the user-readable display name of the conflicting report template.
        /// \return
        ///     The user-readable display name of the conflicting report template.
        QString         displayName() const { return _displayName; }

        //////////
        //  Implementation
    private:
        tt3::util::Mnemonic _mnemonic;
        QString         _displayName;
    };

    /// \class ReportTemplateDoesNotExistException tt3-db-api/API.hpp
    /// \brief Thrown when a report template does not exist.
    class TT3_REPORT_PUBLIC ReportTemplateDoesNotExistException
        :   public ReportException
    {
        //////////
        //  Types
    public:
        /// \brief A type alias to improve code readability.
        using Self = ReportTemplateDoesNotExistException;

        //////////
        //  Construction/destruction/assignment
    public:
        /// \brief
        ///     Constructs the exception.
        /// \param mnemonic
        ///     The mnemonic identifier of the conflicting report template.
        explicit ReportTemplateDoesNotExistException(
                const tt3::util::Mnemonic & mnemonic
            );

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
        ///     Returns the mnemonic identifier of the conflicting report template.
        /// \return
        ///     The mnemonic identifier of the conflicting report template.
        auto            mnemonic() const -> tt3::util::Mnemonic { return _mnemonic; }

        //////////
        //  Implementation
    private:
        tt3::util::Mnemonic _mnemonic;
    };

    /// \class StyleDoesNotExistException tt3-db-api/API.hpp
    /// \brief Thrown when a style does not exist in a report template.
    class TT3_REPORT_PUBLIC StyleDoesNotExistException
        :   public ReportException
    {
        //////////
        //  Types
    public:
        /// \brief A type alias to improve code readability.
        using Self = StyleDoesNotExistException;

        //////////
        //  Construction/destruction/assignment
    public:
        /// \brief
        ///     Constructs the exception.
        /// \param styleName
        ///     The name of the nonexistent style.
        /// \param reportTemplatenemonic
        ///     The mnemonic identifier of the report template
        ///     where a required style does not exist.
        /// \param reportTemplateDisplayName;
        ///     The user-readable display name of the report template
        ///     where a required style does not exist.
        StyleDoesNotExistException(
                const tt3::util::Mnemonic & styleName,
                const tt3::util::Mnemonic & reportTemplatenemonic,
                const QString & reportTemplateDisplayName
            );

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
        ///     Returns the name of the nonexistent style.
        /// \return
        ///     The name of the nonexistent style.
        auto            styleName() const -> tt3::util::Mnemonic { return _styleName; }

        /// \brief
        ///     Returns the mnemonic identifier of the report template
        ///     where a required style does not exist.
        /// \return
        ///     The mnemonic identifier of the report template
        ///     where a required style does not exist.
        auto            reportTemplateMnemonic() const -> tt3::util::Mnemonic { return _reportTemplatenemonic; }

        /// \brief
        ///     Returns the user-readable display name of the report template
        ///     where a required style does not exist.
        /// \return
        ///     The user-readable display name of the report template
        ///     where a required style does not exist.
        QString         reportTemplateDisplayName() const { return _reportTemplateDisplayName; }

        //////////
        //  Implementation
    private:
        tt3::util::Mnemonic _styleName;
        tt3::util::Mnemonic _reportTemplatenemonic;
        QString         _reportTemplateDisplayName;
    };

    /// \class InvalidReportException tt3-db-api/API.hpp
    /// \brief Thrown when a report is malformed.
    class TT3_REPORT_PUBLIC InvalidReportException
        :   public ReportException
    {
        //////////
        //  Types
    public:
        /// \brief A type alias to improve code readability.
        using Self = InvalidReportException;

        //////////
        //  Construction/destruction/assignment
    public:
        /// \brief
        ///     Constructs the exception.
        InvalidReportException();

        //////////
        //  QException
    public:
        virtual Self *  clone() const override { return new Self(*this); }
        virtual void    raise() const override { throw *this; }

        //////////
        //  tt3::util::Exception
    public:
        virtual QString errorMessage() const override;
    };

    /// \class CustomReportException tt3-db-api/API.hpp
    /// \brief Thrown when must carry a custom error message (from OS, etc.)
    class TT3_REPORT_PUBLIC CustomReportException
        :   public ReportException
    {
        //////////
        //  Types
    public:
        /// \brief A type alias to improve code readability.
        using Self = CustomReportException;

        //////////
        //  Construction/destruction/assignment
    public:
        /// \brief
        ///     Constructs the exception.
        /// \param errorMessage
        ///     The custom error message.
        explicit CustomReportException(
                const QString & errorMessage
            );

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
        //  Implementation
    private:
        QString         _errorMessage;
    };
}

//  End of tt3-report/Exceptions.hpp
