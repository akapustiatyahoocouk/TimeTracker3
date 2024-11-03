using System.Diagnostics;

namespace TimeTracker3.Db.API.Exceptions
{
    /// <summary>
    ///     Thrown when an attempt is made to open a
    ///     single-user database or destroy a database
    ///     that are currently open elsewhere.
    /// </summary>
    public class DatabaseInUseDatabaseException : DatabaseException
    {
        //////////
        //  Construction

        /// <summary>
        ///     Constructs the exception.
        /// </summary>
        /// <param name="address">
        ///     The address of the offending database.
        /// </param>
        public DatabaseInUseDatabaseException(IDatabaseAddress address)
        : base("Database " + address.ExternalForm + " is already in use")
        {
            Debug.Assert(address != null);

            Address = address;
        }

        //////////
        //  Properties

        /// <summary>
        ///     The address of the offending database.
        /// </summary>
        public readonly IDatabaseAddress Address;
    }
}
