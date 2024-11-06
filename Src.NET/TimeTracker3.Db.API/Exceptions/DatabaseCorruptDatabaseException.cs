using System;
using System.Diagnostics;

namespace TimeTracker3.Db.API.Exceptions
{
    /// <summary>
    ///     Thrown when an attempt is made to open a
    ///     database that has invalid format or
    /// corrupt content.
    /// </summary>
    public class DatabaseCorruptDatabaseException : DatabaseException
    {
        //////////
        //  Construction

        /// <summary>
        ///     Constructs the exception.
        /// </summary>
        /// <param name="databaseAddress">
        ///     The databaseAddress of the offending database.
        /// </param>
        public DatabaseCorruptDatabaseException(IDatabaseAddress databaseAddress)
        : base("Database " + databaseAddress.ExternalForm + " is invalid or corrupt")
        {
            Debug.Assert(databaseAddress != null);

            DatabaseAddress = databaseAddress;
        }

        /// <summary>
        ///     Constructs the exception.
        /// </summary>
        /// <param name="databaseAddress">
        ///     The databaseAddress of the offending database.
        /// </param>
        /// <param name="cause">
        ///     The underlying cause of the exception.
        /// </param>
        public DatabaseCorruptDatabaseException(IDatabaseAddress databaseAddress, Exception cause)
            : base("Database " + databaseAddress.ExternalForm + " is invalid or corrupt", cause)
        {
            Debug.Assert(databaseAddress != null);

            DatabaseAddress = databaseAddress;
        }

        //////////
        //  Properties

        /// <summary>
        ///     The address of the offending database.
        /// </summary>
        public readonly IDatabaseAddress DatabaseAddress;
    }
}
