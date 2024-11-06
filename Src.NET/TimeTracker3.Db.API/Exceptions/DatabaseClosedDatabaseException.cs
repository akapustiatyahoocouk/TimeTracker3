using System.Diagnostics;

namespace TimeTracker3.Db.API.Exceptions
{
    /// <summary>
    ///     Thrown when an attempt is made to use a closed database.
    /// </summary>
    public class DatabaseClosedDatabaseException : DatabaseException
    {
        //////////
        //  Construction

        /// <summary>
        ///     Constructs the exception.
        /// </summary>
        /// <param name="databaseAddress">
        ///     The address of the "closed" database.
        /// </param>
        public DatabaseClosedDatabaseException(IDatabaseAddress databaseAddress)
        : base("The database " + databaseAddress.ExternalForm + " is closed")
        {
            Debug.Assert(databaseAddress != null);
            DatabaseAddress = databaseAddress;
        }

        //////////
        //  Properties

        /// <summary>
        ///     The address of the "closed" database.
        /// </summary>
        public readonly IDatabaseAddress DatabaseAddress;
    }
}
