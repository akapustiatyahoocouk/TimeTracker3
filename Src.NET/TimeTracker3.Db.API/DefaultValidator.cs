using System.Globalization;
using System;
using System.Linq;
using System.Net.Mail;

namespace TimeTracker3.Db.API
{
    /// <summary>
    ///     The "default" validator - implements
    ///     standard validation rules.
    /// </summary>
    public class DefaultValidator : IValidator,
        IUserValidator, IAccountValidator
    {
        //////////
        //  IValidator - Properties
        public IUserValidator User => this;
        public IAccountValidator Account => this;

        //////////
        //  IUserValidator - Operations
        public bool IsValidUserEmailAddress(string emailAddress)
        {
            return _IsValidEmailAddress(emailAddress);
        }

        public bool IsValidUserEmailAddresses(string[] emailAddresses)
        {
            return _IsValidEmailAddresses(emailAddresses);
        }

        public bool IsValidUserRealName(string realName)
        {
            return _IsValidName(realName, 128);
        }

        public bool IsValidUserInactivityTimeout(TimeSpan? inactivityTimeout)
        {
            return !inactivityTimeout.HasValue ||
                   inactivityTimeout.Value.TotalSeconds > 0;
        }

        public bool IsValidUserUiCulture(CultureInfo uiCulture)
        {
            return true;   //  TODO really ?
        }

        //////////
        //  IAccountValidator - Operations
        public bool IsValidAccountEmailAddress(string emailAddress)
        {
            return _IsValidEmailAddress(emailAddress);
        }

        public bool IsValidAccountEmailAddresses(string[] emailAddresses)
        {
            return _IsValidEmailAddresses(emailAddresses);
        }

        public bool IsValidAccountLogin(string login)
        {
            return _IsValidName(login, 128);
        }

        public bool IsValidAccountPassword(string password)
        {
            return password != null;
        }

        public bool IsValidAccountCapabilities(Capabilities capabilities)
        {
            return (capabilities & Capabilities.All) == capabilities;
        }

        //////////
        //  Implementation helpers
        private static bool _IsValidName(string s, int maxLength)
        {
            return !string.IsNullOrEmpty(s) &&
                   (s.Length <= maxLength) &&
                   (s.Trim().Length == s.Length);
        }

        private static bool _IsValidEmailAddress(string emailAddress)
        {
            try
            {
                MailAddress _ = new MailAddress(emailAddress);
                return true;
            }
            catch (FormatException)
            {
                return false;
            }
        }

        private static bool _IsValidEmailAddresses(string[] emailAddresses)
        {
            return (emailAddresses != null) &&
                   !emailAddresses.Contains(null) &&
                   emailAddresses.All(_IsValidEmailAddress);
        }
    }
}
