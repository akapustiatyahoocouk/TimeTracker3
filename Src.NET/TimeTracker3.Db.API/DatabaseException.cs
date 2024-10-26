using System;

namespace TimeTracker3.Db.API
{
    /// <summary>
    ///     The common base class for all exceptions throws 
    ///     by Database services.
    /// </summary>
    public class DatabaseException : Exception
    {
        //////////
        //  Construction

        public DatabaseException(string message)
            : base(message)
        {
        }

        public DatabaseException(string message, Exception cause)
            : base(message, cause)
        {
        }

        public DatabaseException(Exception cause)
            : base(cause.Message, cause)
        {
        }
    }
}
