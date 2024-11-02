using System;
using System.Diagnostics;

namespace TimeTracker3.Db.API
{
    /// <summary>
    ///     A common base class for all notification
    ///     events raised by a Database.
    /// </summary>
    public abstract class DatabaseEventArgs : EventArgs
    {
        //////////
        //  Construction

        /// <summary>
        ///     Constructs the event arguments.
        /// </summary>
        /// <param name="database">
        ///     The database where a new object has
        ///     been created.
        /// </param>
        protected DatabaseEventArgs(IDatabase database)
        {
            Debug.Assert(database != null);

            Database = database;
        }

        //////////
        //  Properties

        /// <summary>
        ///     The database for which the event was raised.
        /// </summary>
        public readonly IDatabase Database;
    }
}
