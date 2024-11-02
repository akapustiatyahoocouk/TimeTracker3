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
        public Credentials(string login, string password)
        {
            Debug.Assert(login != null);
            Debug.Assert(password != null);

            _Login = login;
            //  TODO keep? kill? _Password = password;
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

        /// <summary>
        ///     The "current" credentials; can be null.
        /// </summary>
        public static Credentials Current
        {
            get => _CurrentCredentials;
            set => _CurrentCredentials = value;
        }

        //////////
        //  Implementation
        private readonly string _Login;
        //  TODO keep? kill? private readonly string _Password;
        private readonly string _PasswordHash;  //  SHA-1

        private static Credentials _CurrentCredentials; //  can be null!
    }
}
