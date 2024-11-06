using System;
using System.Globalization;

namespace TimeTracker3.Db.API
{
    /// <summary>
    ///     Validates properties of a User object
    /// </summary>
    public interface IUserValidator
    {
        //////////
        //  Operations TODO document
        bool IsValidUserEmailAddress(string emailAddress);
        bool IsValidUserEmailAddresses(string[] emailAddresses);
        bool IsValidUserRealName(string realName);
        bool IsValidUserInactivityTimeout(TimeSpan? inactivityTimeout);
        bool IsValidUserUiCulture(CultureInfo uiCulture);
    }

    /// <summary>
    ///     Validates properties of an Account object
    /// </summary>
    public interface IAccountValidator
    {
        //////////
        //  Operations TODO document
        bool IsValidAccountEmailAddress(string emailAddress);
        bool IsValidAccountEmailAddresses(string[] emailAddresses);
        bool IsValidAccountLogin(string login);
        bool IsValidAccountPassword(string password);
        bool IsValidAccountCapabilities(Capabilities capabilities);
    }

    /// <summary>
    ///     A combined validator of all properties of
    ///     all database objects.
    /// </summary>
    public interface IValidator
    {
        //////////
        //  Properties

        /// <summary>
        ///     The validator for User properties.
        /// </summary>
        IUserValidator User { get; }

        /// <summary>
        ///     The validator for Account properties.
        /// </summary>
        IAccountValidator Account { get; }
    }
}