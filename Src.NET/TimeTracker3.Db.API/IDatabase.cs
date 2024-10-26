using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TimeTracker3.Db.API
{
    /// <summary>
    ///     A connection to a persistent data storage.
    /// </summary>
    internal interface IDatabase
    {
        //////////
        //  Properties

        /// <summary>
        ///     The type of this database; can be obtained safely for
        ///     both open and closed databases.
        /// </summary>
        IDatabaseType Type { get; }

        /// <summary>
        ///     The address of this database; can be obtained safely for
        ///     both open and closed databases.
        /// </summary>
        IDatabaseType Address { get; }

        /// <summary>
        ///     True if this database is currently open, false if closed.
        /// </summary>
        bool IsOpen { get; }

        //////////
        //  Operations - general

        /// <summary>
        ///     Closes this database, has no effect if already closed.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     Throws if an error occurs (the database is still marked 
        ///     as "closed" and all resources it uses are released before 
        ///     the exception is thrown).
        /// </exception>
        void Close();
    }
}
