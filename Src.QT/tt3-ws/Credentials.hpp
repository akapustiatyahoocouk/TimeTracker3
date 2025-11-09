//
//  tt3-ws/Credentials.hpp - Login credentials
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

namespace tt3::ws
{
    /// \class Credentials tt3-ws/API.hpp
    /// \brief The login credentials.
    class TT3_WS_PUBLIC Credentials
    {
        friend class WorkspaceTypeImpl;
        friend class WorkspaceImpl;
        friend class PrincipalImpl;
        friend class UserImpl;
        friend class AccountImpl;
        friend class PrivateActivityImpl;
        friend class PrivateTaskImpl;
        friend class WorkImpl;
        friend class EventImpl;

        friend class BackupCredentials;
        friend class RestoreCredentials;
        friend class ReportCredentials;

        //////////
        //  Constats
    public:
        /// \brief
        ///     The invalid credentials (with empty login).
        static const Credentials Invalid;

        //////////
        //  Construction/destruction/assignment
    public:
        /// \brief
        ///     Constructs invalid credentials (with empty login).
        Credentials() = default;

        /// \brief
        ///     Constructs the credentials.
        /// \param login
        ///     The login identifier.
        /// \param password
        ///     The password.
        Credentials(
                const QString & login,
                const QString & password
            );

        /// \brief
        ///     The class destructor.
        /// \details
        ///     Made virtual to enable RTTI for the class.
        virtual ~Credentials() = default;

        //  Default copy constructor and assignment are both OK

        //////////
        //  Operators
    public:
        /// \brief
        ///     Checks two Credentials for equality.
        /// \details
        ///     Two credentials are equal if all their details are equal.
        /// \param op2
        ///     The Credentials to compare this Credentials yo.
        /// \return
        ///     True if thw two Credentials are equal, else false.
        bool            operator == (const Credentials & op2) const
        {
            return compare(op2) == 0;
        }

        /// \brief
        ///     Checks two Credentials for inequality.
        /// \details
        ///     Two credentials are equal if all their details are equal.
        /// \param op2
        ///     The Credentials to compare this Credentials yo.
        /// \return
        ///     False if thw two Credentials are equal, else true.
        bool            operator != (const Credentials & op2) const
        {
            return compare(op2) != 0;
        }

        /// \brief
        ///     Checks two Credentials for order.
        /// \param op2
        ///     The 2nd Credentials to compare this Credentials to.
        /// \return
        ///     True if this Credentials is lexicographically
        ///     "less than" the 2nd C5redentials, else false.
        bool            operator <  (const Credentials & op2) const
        {
            return compare(op2) < 0;
        }

        /// \brief
        ///     Checks two Credentials for order.
        /// \param op2
        ///     The 2nd Credentials to compare this Credentials to.
        /// \return
        ///     True if this Credentials is lexicographically
        ///     "less than or equal to" the 2nd C5redentials, else false.
        bool            operator <= (const Credentials & op2) const
        {
            return compare(op2) <= 0;
        }

        /// \brief
        ///     Checks two Credentials for order.
        /// \param op2
        ///     The 2nd Credentials to compare this Credentials to.
        /// \return
        ///     True if this Credentials is lexicographically
        ///     "greater than" the 2nd C5redentials, else false.
        bool            operator >  (const Credentials & op2) const
        {
            return compare(op2) > 0;
        }

        /// \brief
        ///     Checks two Credentials for order.
        /// \param op2
        ///     The 2nd Credentials to compare this Credentials to.
        /// \return
        ///     True if this Credentials is lexicographically
        ///     "greater than or equal to" the 2nd C5redentials, else false.
        bool            operator >= (const Credentials & op2) const
        {
            return compare(op2) >= 0;
        }

        //////////
        //  Operations
    public:
        /// \brief
        ///     Checks whether these Credentials are "valid"
        ///     (its the "login" is not empty.
        /// \return
        ///     True if this Credentials are "vaolid" else false.
        virtual bool    isValid() const { return !_login.isEmpty(); }

        /// \brief
        ///     Returns the login identifier of these Credentials.
        /// \return
        ///     The login identifier of these Credentials; empty
        ///     string for "invalid" Credentials.
        QString         login() const { return _login; }

        //////////
        //  Comparison and order
    public:
        /// \brief
        ///     Compares these Credentials with another Credentials.
        /// \param op2
        ///     The 2nd Credentials to compare these Credentials to.
        /// \return
        ///     A negative, zero or positive value, depending
        ///     on whether these Credentials are "less then",
        ///     "equal to" or "greater then" the 2ns Credentials.
        virtual int     compare(const Credentials & op2) const;

    protected:
        /// \brief
        ///     Compares these Credentials with another Credentials.
        /// \details
        ///     This is an implementation of a compare-via-double-dispatch.
        /// \param op2
        ///     The 2nd Credentials to compare these Credentials to.
        /// \return
        ///     A negative, zero or positive value, depending
        ///     on whether these Credentials are "less then",
        ///     "equal to" or "greater then" the 2ns Credentials.
        virtual int     compare2(const Credentials & op2) const;

        /// \brief
        ///     Compares these Credentials with another Credentials.
        /// \details
        ///     This is an implementation of a compare-via-double-dispatch.
        /// \param op2
        ///     The 2nd Credentials to compare these Credentials to.
        /// \return
        ///     A negative, zero or positive value, depending
        ///     on whether these Credentials are "less then",
        ///     "equal to" or "greater then" the 2ns Credentials.
        virtual int     compare2(const BackupCredentials & op2) const;

        /// \brief
        ///     Compares these Credentials with another Credentials.
        /// \details
        ///     This is an implementation of a compare-via-double-dispatch.
        /// \param op2
        ///     The 2nd Credentials to compare these Credentials to.
        /// \return
        ///     A negative, zero or positive value, depending
        ///     on whether these Credentials are "less then",
        ///     "equal to" or "greater then" the 2ns Credentials.
        virtual int     compare2(const RestoreCredentials & op2) const;

        /// \brief
        ///     Compares these Credentials with another Credentials.
        /// \details
        ///     This is an implementation of a compare-via-double-dispatch.
        /// \param op2
        ///     The 2nd Credentials to compare these Credentials to.
        /// \return
        ///     A negative, zero or positive value, depending
        ///     on whether these Credentials are "less then",
        ///     "equal to" or "greater then" the 2ns Credentials.
        virtual int     compare2(const ReportCredentials & op2) const;

        //////////
        //  Implementation
    private:
        QString         _login;
        QString         _password;  //  always "" if _login == ""
    };

    TT3_WS_PUBLIC inline size_t qHash(const tt3::ws::Credentials & key, size_t seed)
    {
        return qHash(key.login(), seed);
    }

    /// \class BackupCredentials tt3-ws/API.hpp
    /// \brief Special credentials that allow backing up a database.
    class TT3_WS_PUBLIC BackupCredentials final
        :   public Credentials
    {
        friend class WorkspaceImpl;

        //////////
        //  Construction/destruction/assignment
    public:
        /// \brief
        ///     Constructs an invalid BackupCredentials.
        BackupCredentials();

        /// \brief
        ///     Constructs the credentials.
        /// \param login
        ///     The login identifier.
        /// \param password
        ///     The password.
        /// \param issuedAt
        ///     The UTC date+time when these BackupCredentials were issued.
        /// \param expireAt
        ///     The UTC date+time when these BackupCredentials will expire.
        BackupCredentials(
                const QString & login,
                const QString & password,
                const QDateTime & issuedAt,
                const QDateTime & expireAt
            );

        //  The default destructor, copy constructor and
        //  assignment operators are all OK

        //////////
        //  Operators
    public:
        /// \brief
        ///     Checks two BackupCredentials for equality.
        /// \details
        ///     Two BackupCredentials are equal if all their details are equal.
        /// \param op2
        ///     The BackupCredentials to compare this BackupCredentials yo.
        /// \return
        ///     True if thw two BackupCredentials are equal, else false.
        bool            operator == (const BackupCredentials & op2) const
        {
            return compare(op2) == 0;
        }

        /// \brief
        ///     Checks two BackupCredentials for inequality.
        /// \details
        ///     Two BackupCredentials are equal if all their details are equal.
        /// \param op2
        ///     The BackupCredentials to compare this BackupCredentials yo.
        /// \return
        ///     False if thw two BackupCredentials are equal, else true.
        bool            operator != (const BackupCredentials & op2) const
        {
            return compare(op2) != 0;
        }

        /// \brief
        ///     Checks two BackupCredentials for order.
        /// \param op2
        ///     The 2nd BackupCredentials to compare this BackupCredentials to.
        /// \return
        ///     True if this BackupCredentials is lexicographically
        ///     "less than" the 2nd C5redentials, else false.
        bool            operator <  (const BackupCredentials & op2) const
        {
            return compare(op2) < 0;
        }

        /// \brief
        ///     Checks two BackupCredentials for order.
        /// \param op2
        ///     The 2nd BackupCredentials to compare this BackupCredentials to.
        /// \return
        ///     True if this BackupCredentials is lexicographically
        ///     "less than or equal to" the 2nd C5redentials, else false.
        bool            operator <= (const BackupCredentials & op2) const
        {
            return compare(op2) <= 0;
        }

        /// \brief
        ///     Checks two BackupCredentials for order.
        /// \param op2
        ///     The 2nd BackupCredentials to compare this BackupCredentials to.
        /// \return
        ///     True if this BackupCredentials is lexicographically
        ///     "greater than" the 2nd C5redentials, else false.
        bool            operator >  (const BackupCredentials & op2) const
        {
            return compare(op2) > 0;
        }

        /// \brief
        ///     Checks two BackupCredentials for order.
        /// \param op2
        ///     The 2nd BackupCredentials to compare this BackupCredentials to.
        /// \return
        ///     True if this BackupCredentials is lexicographically
        ///     "greater than or equal to" the 2nd C5redentials, else false.
        bool            operator >= (const BackupCredentials & op2) const
        {
            return compare(op2) >= 0;
        }

        //////////
        //  Credentials
    public:
        /// \brief
        ///     Checks whether these Credentials are "valid"
        ///     (its the "login" is not empty.
        /// \return
        ///     True if this Credentials are "vaolid" else false.
        virtual bool    isValid() const override;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the UTC date+time when these BackupCredentials were issued.
        /// \return
        ///     The UTC date+time when these BackupCredentials were issued.
        QDateTime       issuedAt() const { return _issuedAt; }

        /// \brief
        ///     Returns the UTC date+time when these BackupCredentials will expire.
        /// \return
        ///     The UTC date+time when these BackupCredentials will expire.
        QDateTime       expireAt() const { return _expireAt; }

        //////////
        //  Comparison and order
    public:
        virtual int     compare(const Credentials & op2) const override;
    protected:
        virtual int     compare2(const Credentials & op2) const override;
        virtual int     compare2(const BackupCredentials & op2) const override;
        virtual int     compare2(const RestoreCredentials & op2) const override;
        virtual int     compare2(const ReportCredentials & op2) const override;

        //////////
        //  Implementation
    private:
        QDateTime       _issuedAt;  //  UTC
        QDateTime       _expireAt;  //  UTC
    };

    /// \class RestoreCredentials tt3-ws/API.hpp
    /// \brief Special credentials that allow restoring a database.
    class TT3_WS_PUBLIC RestoreCredentials final
        :   public Credentials
    {
        friend class WorkspaceImpl;

        //////////
        //  Construction/destruction/assignment
    public:
        /// \brief
        ///     Constructs an invalid restoreCredentials.
        RestoreCredentials();

        /// \brief
        ///     Constructs the credentials.
        /// \param login
        ///     The login identifier.
        /// \param password
        ///     The password.
        /// \param issuedAt
        ///     The UTC date+time when these RestoreCredentials were issued.
        /// \param expireAt
        ///     The UTC date+time when these RestoreCredentials will expire.
        RestoreCredentials(
                const QString & login,
                const QString & password,
                const QDateTime & issuedAt,
                const QDateTime & expireAt
            );

        //  The default destructor, copy constructor and
        //  assignment operators are all OK

        //////////
        //  Operators
    public:
        /// \brief
        ///     Checks two RestoreCredentials for equality.
        /// \details
        ///     Two RestoreCredentials are equal if all their details are equal.
        /// \param op2
        ///     The RestoreCredentials to compare this RestoreCredentials yo.
        /// \return
        ///     True if thw two RestoreCredentials are equal, else false.
        bool            operator == (const RestoreCredentials & op2) const
        {
            return compare(op2) == 0;
        }

        /// \brief
        ///     Checks two RestoreCredentials for inequality.
        /// \details
        ///     Two RestoreCredentials are equal if all their details are equal.
        /// \param op2
        ///     The RestoreCredentials to compare this RestoreCredentials yo.
        /// \return
        ///     False if thw two RestoreCredentials are equal, else true.
        bool            operator != (const RestoreCredentials & op2) const
        {
            return compare(op2) != 0;
        }

        /// \brief
        ///     Checks two RestoreCredentials for order.
        /// \param op2
        ///     The 2nd RestoreCredentials to compare this RestoreCredentials to.
        /// \return
        ///     True if this RestoreCredentials is lexicographically
        ///     "less than" the 2nd C5redentials, else false.
        bool            operator <  (const RestoreCredentials & op2) const
        {
            return compare(op2) < 0;
        }

        /// \brief
        ///     Checks two RestoreCredentials for order.
        /// \param op2
        ///     The 2nd RestoreCredentials to compare this RestoreCredentials to.
        /// \return
        ///     True if this RestoreCredentials is lexicographically
        ///     "less than or equal to" the 2nd C5redentials, else false.
        bool            operator <= (const RestoreCredentials & op2) const
        {
            return compare(op2) <= 0;
        }

        /// \brief
        ///     Checks two RestoreCredentials for order.
        /// \param op2
        ///     The 2nd RestoreCredentials to compare this RestoreCredentials to.
        /// \return
        ///     True if this RestoreCredentials is lexicographically
        ///     "greater than" the 2nd C5redentials, else false.
        bool            operator >  (const RestoreCredentials & op2) const
        {
            return compare(op2) > 0;
        }

        /// \brief
        ///     Checks two RestoreCredentials for order.
        /// \param op2
        ///     The 2nd RestoreCredentials to compare this RestoreCredentials to.
        /// \return
        ///     True if this RestoreCredentials is lexicographically
        ///     "greater than or equal to" the 2nd C5redentials, else false.
        bool            operator >= (const RestoreCredentials & op2) const
        {
            return compare(op2) >= 0;
        }

        //////////
        //  Credentials
    public:
        /// \brief
        ///     Checks whether these Credentials are "valid"
        ///     (its the "login" is not empty.
        /// \return
        ///     True if this Credentials are "vaolid" else false.
        virtual bool    isValid() const override;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the UTC date+time when these RestoreCredentials were issued.
        /// \return
        ///     The UTC date+time when these RestoreCredentials were issued.
        QDateTime       issuedAt() const { return _issuedAt; }

        /// \brief
        ///     Returns the UTC date+time when these RestoreCredentials will expire.
        /// \return
        ///     The UTC date+time when these RestoreCredentials will expire.
        QDateTime       expireAt() const { return _expireAt; }

        //////////
        //  Comparison and order
    public:
        virtual int     compare(const Credentials & op2) const override;
    protected:
        virtual int     compare2(const Credentials & op2) const override;
        virtual int     compare2(const BackupCredentials & op2) const override;
        virtual int     compare2(const RestoreCredentials & op2) const override;
        virtual int     compare2(const ReportCredentials & op2) const override;

        //////////
        //  Implementation
    private:
        QDateTime       _issuedAt;  //  UTC
        QDateTime       _expireAt;  //  UTC
    };

    /// \class ReportCredentials tt3-ws/API.hpp
    /// \brief Special credentials that allow reporting from a database.
    class TT3_WS_PUBLIC ReportCredentials final
        :   public Credentials
    {
        friend class WorkspaceImpl;

        //////////
        //  Construction/destruction/assignment
    public:
        /// \brief
        ///     Constructs an invalid ReportCredentials.
        ReportCredentials();

        /// \brief
        ///     Constructs the credentials.
        /// \param login
        ///     The login identifier.
        /// \param password
        ///     The password.
        /// \param issuedAt
        ///     The UTC date+time when these ReportCredentials were issued.
        /// \param expireAt
        ///     The UTC date+time when these ReportCredentials will expire.
        ReportCredentials(
                const QString & login,
                const QString & password,
                const QDateTime & issuedAt,
                const QDateTime & expireAt
            );

        //  The default destructor, copy constructor and
        //  assignment operators are all OK

        //////////
        //  Operators
    public:
        /// \brief
        ///     Checks two ReportCredentials for equality.
        /// \details
        ///     Two ReportCredentials are equal if all their details are equal.
        /// \param op2
        ///     The ReportCredentials to compare this ReportCredentials yo.
        /// \return
        ///     True if thw two ReportCredentials are equal, else false.
        bool            operator == (const ReportCredentials & op2) const
        {
            return compare(op2) == 0;
        }

        /// \brief
        ///     Checks two ReportCredentials for inequality.
        /// \details
        ///     Two ReportCredentials are equal if all their details are equal.
        /// \param op2
        ///     The ReportCredentials to compare this ReportCredentials yo.
        /// \return
        ///     False if thw two ReportCredentials are equal, else true.
        bool            operator != (const ReportCredentials & op2) const
        {
            return compare(op2) != 0;
        }

        /// \brief
        ///     Checks two ReportCredentials for order.
        /// \param op2
        ///     The 2nd ReportCredentials to compare this ReportCredentials to.
        /// \return
        ///     True if this ReportCredentials is lexicographically
        ///     "less than" the 2nd C5redentials, else false.
        bool            operator <  (const ReportCredentials & op2) const
        {
            return compare(op2) < 0;
        }

        /// \brief
        ///     Checks two ReportCredentials for order.
        /// \param op2
        ///     The 2nd ReportCredentials to compare this ReportCredentials to.
        /// \return
        ///     True if this ReportCredentials is lexicographically
        ///     "less than or equal to" the 2nd C5redentials, else false.
        bool            operator <= (const ReportCredentials & op2) const
        {
            return compare(op2) <= 0;
        }

        /// \brief
        ///     Checks two ReportCredentials for order.
        /// \param op2
        ///     The 2nd ReportCredentials to compare this ReportCredentials to.
        /// \return
        ///     True if this ReportCredentials is lexicographically
        ///     "greater than" the 2nd C5redentials, else false.
        bool            operator >  (const ReportCredentials & op2) const
        {
            return compare(op2) > 0;
        }

        /// \brief
        ///     Checks two ReportCredentials for order.
        /// \param op2
        ///     The 2nd ReportCredentials to compare this ReportCredentials to.
        /// \return
        ///     True if this ReportCredentials is lexicographically
        ///     "greater than or equal to" the 2nd C5redentials, else false.
        bool            operator >= (const ReportCredentials & op2) const
        {
            return compare(op2) >= 0;
        }

        //////////
        //  Credentials
    public:
        /// \brief
        ///     Checks whether these Credentials are "valid"
        ///     (its the "login" is not empty.
        /// \return
        ///     True if this Credentials are "vaolid" else false.
        virtual bool    isValid() const override;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the UTC date+time when these ReportCredentials were issued.
        /// \return
        ///     The UTC date+time when these ReportCredentials were issued.
        QDateTime       issuedAt() const { return _issuedAt; }

        /// \brief
        ///     Returns the UTC date+time when these ReportCredentials will expire.
        /// \return
        ///     The UTC date+time when these ReportCredentials will expire.
        QDateTime       expireAt() const { return _expireAt; }

        //////////
        //  Comparison and order
    public:
        virtual int     compare(const Credentials & op2) const override;
    protected:
        virtual int     compare2(const Credentials & op2) const override;
        virtual int     compare2(const BackupCredentials & op2) const override;
        virtual int     compare2(const RestoreCredentials & op2) const override;
        virtual int     compare2(const ReportCredentials & op2) const override;

        //////////
        //  Implementation
    private:
        QDateTime       _issuedAt;  //  UTC
        QDateTime       _expireAt;  //  UTC
    };
}

//  End of tt3-ws/Credentials.hpp
