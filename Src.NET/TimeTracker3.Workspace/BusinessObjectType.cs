using System;
using System.Collections.Concurrent;
using System.Diagnostics;
using TimeTracker3.Db.API;

namespace TimeTracker3.Workspace
{
    /// <summary>
    ///     A type of a business object
    /// </summary>
    public sealed class BusinessObjectType
    {
        //////////
        //  Constants
        //  TODO document
        public static readonly BusinessObjectType User =
            new BusinessObjectType(DatabaseObjectTypes.User);

        public static readonly BusinessObjectType Account =
            new BusinessObjectType(DatabaseObjectTypes.Account);

        //////////
        //  Construction
        private BusinessObjectType(DatabaseObjectType dataObjectType)
        {
            _DataObjectType = dataObjectType;
        }

        //////////
        //  Properties

        /// <summary>
        ///     The internal name of this object type
        ///     (i.e. "User", "Account", etc.)
        /// </summary>
        public string Name => _DataObjectType.Name;

        //////////
        //  Implementation
        private readonly DatabaseObjectType _DataObjectType;

        private static readonly ConcurrentDictionary<DatabaseObjectType, BusinessObjectType> _ObjectTypeMap =
            new ConcurrentDictionary<DatabaseObjectType, BusinessObjectType>();

        //  Helpers
        public static BusinessObjectType _Translate(DatabaseObjectType dataObjectType)
        {
            Debug.Assert(dataObjectType != null);

            if (!_ObjectTypeMap.TryGetValue(dataObjectType, out BusinessObjectType result))
            {   //  Need to add the mapping
                if (dataObjectType.Equals(DatabaseObjectTypes.User))
                {
                    result = User;
                }
                else if (dataObjectType.Equals(DatabaseObjectTypes.Account))
                {
                    result = Account;
                }
                //  TODO all object types
                else
                {   //  OOPS! Should never happen!
                    throw new ArgumentException("Invalid dataObjectType");
                }
                _ObjectTypeMap[dataObjectType] = result;
            }
            return result;
        }
    }
}
