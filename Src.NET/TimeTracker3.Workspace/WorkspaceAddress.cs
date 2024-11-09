using System.Diagnostics;
using TimeTracker3.Db.API;

namespace TimeTracker3.Workspace
{
    /// <summary>
    ///     Represents a location of a workspace of a
    ///     specific type.
    /// </summary>
    public sealed class WorkspaceAddress
    {
        //////////
        //  Construction
        internal WorkspaceAddress(IDatabaseAddress databaseAddress)
        {
            Debug.Assert(databaseAddress != null);

            _DatabaseAddress = databaseAddress;
        }

        //////////
        //  object
        public override bool Equals(object obj)
        {
            if (ReferenceEquals(this, obj))
            {
                return true;
            }
            if (obj is WorkspaceAddress op2)
            {
                return _DatabaseAddress.Equals(op2._DatabaseAddress);
            }
            return false;
        }

        public override int GetHashCode()
        {
            return _DatabaseAddress.GetHashCode();
        }

        //////////
        //  Properties

        /// <summary>
        ///     The workspace type to which this workspace
        ///     address belongs, never null.
        /// </summary>
        public WorkspaceType WorkspaceType => WorkspaceType._MapDatabaseType(_DatabaseAddress.DatabaseType);

        /// <summary>
        ///     The user/readable display form of this
        ///     workspace address the current UI culture.
        /// </summary>
        public string DisplayForm => _DatabaseAddress.DisplayForm;

        /// <summary>
        ///     The external (re-parsable) form of this
        ///     workspace address.
        /// </summary>
        public string ExternalForm => _DatabaseAddress.ExternalForm;

        //////////
        //  Implementation
        internal readonly IDatabaseAddress _DatabaseAddress;
    }
}
