using System;

namespace TimeTracker3.Db.API.Exceptions
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

        //////////
        //  Operations

        /// <summary>
        ///     Translates an arbitrary exception to a
        ///     DatabaseException.
        /// </summary>
        /// <param name="ex">
        ///     The exception to translate to DatabaseException.
        /// </param>
        /// <returns>
        ///     The resulting DatabaseException.
        /// </returns>
        public static DatabaseException Translate(Exception ex)
        {
            return (ex is DatabaseException dex) ? dex : new DatabaseException(ex);
        }
    }
}
