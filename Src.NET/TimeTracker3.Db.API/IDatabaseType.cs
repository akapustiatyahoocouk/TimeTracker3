using System.Drawing;
using System.Windows.Forms;
using TimeTracker3.Db.API.Exceptions;

namespace TimeTracker3.Db.API
{
    /// <summary>
    ///     Represents the storage technology used to
    ///     keep persistent data..
    /// </summary>
    public interface IDatabaseType
    {
        //////////
        //  General properties

        /// <summary>
        ///     The mnemonic identifier of this database type.
        /// </summary>
        string Mnemonic { get; }

        /// <summary>
        ///     The user/readable display name of this database type
        ///     for the current UI culture.
        /// </summary>
        string DisplayName { get; }

        /// <summary>
        ///     The small (16x16) image representing this database type.
        /// </summary>
        Image SmallImage { get; }

        /// <summary>
        ///     The large (32x32) image representing this database type.
        /// </summary>
        Image LargeImage { get; }

        /// <summary>
        ///     True if this database type is "operational" (i.e. can 
        ///     be used), else false.
        /// </summary>
        bool IsOperational { get; }

        /// <summary>
        ///     The short (1 line) status report for this database type.
        /// </summary>
        string ShortStatusReport { get; }

        /// <summary>
        ///     The long status report for this database type. Can contain
        ///     multiple lines separated bz the newline character '\n'.
        /// </summary>
        string LongStatusReport { get; }

        //////////
        //  DatabaseAddress handling

        /// <summary>
        ///     If databases of this type has a concept of a "default"
        ///     database, returns its address, otherwise returns null.
        /// </summary>
        IDatabaseAddress DefaultDatabaseAddress { get; }

        /// <summary>
        ///     Parses an external (re-parsable) form of a database 
        ///     address of this type.
        /// </summary>
        /// <param name="externalForm">
        ///     The external (re-parsable) form of a database address.
        /// </param>
        /// <returns>
        ///     The parsed database address.
        /// </returns>
        /// <exception cref="DatabaseException">
        ///     Thrown if the address parsing fails for any reason.
        /// </exception>
        IDatabaseAddress ParseDatabaseAddress(string externalForm);

        /// <summary>
        ///     Prompts the user to enter the address for a new database 
        ///     of this type.
        /// </summary>
        /// <param name="parent">
        ///     The window to use as a parent for any modal dialogs used
        ///     during address entry.
        /// </param>
        /// <returns>
        ///     The database address entered by the user or null if the user
        ///     has chosen to cancel the address entry process.
        /// </returns>
        IDatabaseAddress EnterNewDatabaseAddress(IWin32Window parent);

        /// <summary>
        ///     Prompts the user to enter the address of an existing
        ///     database of this type.
        /// </summary>
        /// <param name="parent">
        ///     The window to use as a parent for any modal dialogs used
        ///     during address entry.
        /// </param>
        /// <returns>
        ///     The database address entered by the user or null if the user
        ///     has chosen to cancel the address entry process.
        /// </returns>
        IDatabaseAddress EnterExistingDatabaseAddress(IWin32Window parent);

        //////////
        //  Database handling

        /// <summary>
        ///     Creates a new empty database of this type at the specified 
        ///     address.
        /// </summary>
        /// <param name="address">
        ///     The address to create a new database at.
        /// </param>
        /// <returns>
        ///     The newly created database.
        /// </returns>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        IDatabase CreateDatabase(IDatabaseAddress address);

        /// <summary>
        ///     Opens an existing database of this type at the specified 
        ///     address.
        /// </summary>
        /// <param name="address">
        ///     The address to open a database at.
        /// </param>
        /// <returns>
        ///     The newly opened database.
        /// </returns>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        IDatabase OpenDatabase(IDatabaseAddress address);

        /// <summary>
        ///     Destroys an existing database of this type at the specified 
        ///     address. The database must not be in use by any process.
        /// </summary>
        /// <param name="address">
        ///     The address to destroy a database at.
        /// </param>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        void DestroyDatabase(IDatabaseAddress address);
    }
}
