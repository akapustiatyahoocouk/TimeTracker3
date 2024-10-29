using System.Diagnostics;
using TimeTracker3.Util;

namespace TimeTracker3.Workspace
{
    /// <summary>
    ///     The login credentials provided by the user.
    /// </summary>
    public sealed class Credentials
    {
        //////////
        //  Construction
        internal Credentials(string login, string password)
        {
            Debug.Assert(login != null);
            Debug.Assert(password != null);

            _Login = login;
            _Password = password;
            _PasswordHash = Sha1.HashString(password);
        }

        //////////
        //  Properties

        /// <summary>
        ///     The user-s login identifier.
        /// </summary>
        public string Login => _Login;

        /// <summary>
        ///     The SHA/1 hash of the user-s password.
        /// </summary>
        public string PasswordHash => _PasswordHash;

        //////////
        //  Implementation
        internal readonly string _Login;
        internal readonly string _Password;
        internal readonly string _PasswordHash;  //  SHA-1
    }
}
