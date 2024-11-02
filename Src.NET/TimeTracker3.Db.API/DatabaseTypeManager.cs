using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;

namespace TimeTracker3.Db.API
{
    /// <summary>
    ///     The registry/manager of known database types.
    /// </summary>
    public static class DatabaseTypeManager
    {
        //////////
        //  Properties

        /// <summary>
        ///     An unordered list of all registered database types.
        /// </summary>
        public static IDatabaseType[] AllDatabaseTypes
        {
            get => _DatabaseTypes.ToArray();
        }

        //////////
        //  Operations

        /// <summary>
        ///     Registers the specified database type.
        /// </summary>
        /// <param name="databaseType">
        ///     The database type to register.
        /// </param>
        /// <returns>
        ///     True if the database type registration was 
        ///     successful, else false.
        /// </returns>
        public static bool RegisterDatabaseType(IDatabaseType databaseType)
        {
            Debug.Assert(databaseType != null);

            IDatabaseType registeredDatabaseType = FindDatabaseType(databaseType.Mnemonic);
            if (registeredDatabaseType == null)
            {
                _DatabaseTypes.Add(databaseType);
                return true;
            }
            return registeredDatabaseType == databaseType;
        }

        /// <summary>
        ///     Finds a registered database type by mnemonic.
        /// </summary>
        /// <param name="mnemonic">
        ///     The mnemonic to look for.
        /// </param>
        /// <returns>
        ///     The database type with the required mnemonic, null if not found.
        /// </returns>
        public static IDatabaseType FindDatabaseType(string mnemonic)
        {
            Debug.Assert(mnemonic != null);

            foreach (IDatabaseType databaseType in _DatabaseTypes)
            {
                if (databaseType.Mnemonic == mnemonic)
                {
                    return databaseType;
                }
            }
            return null;
        }

        //////////
        //  Implementation
        private static readonly IList<IDatabaseType> _DatabaseTypes = new List<IDatabaseType>();
    }
}
