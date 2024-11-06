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
            _Password = password;
            _PasswordHash = Sha1.HashString(password);
        }

        //////////
        //  Operators
        public static bool operator ==(Credentials c1, Credentials c2)
        {
            return c1?.Equals(c2) ?? ReferenceEquals(c2, null);
        }

        public static bool operator !=(Credentials c1, Credentials c2)
        {
            return !(c1 == c2);
        }

        //////////
        //  object
        public override bool Equals(object obj)
        {
            if (ReferenceEquals(this, obj))
            {
                return true;
            }
            if (obj is Credentials op2)
            {
                return _Login == op2._Login &&
                       _PasswordHash == op2._PasswordHash;
            }
            return false;
        }

        public override int GetHashCode()
        {
            return _Login.GetHashCode();
        }

        public override string ToString()
        {
            return _Login;
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
        private readonly string _PasswordHash;  //  SHA-1
    }
}
