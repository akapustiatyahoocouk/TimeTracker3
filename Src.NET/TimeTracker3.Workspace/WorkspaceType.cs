
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using TimeTracker3.Db.API;

namespace TimeTracker3.Workspace
{
    /// <summary>
    ///     Represents the storage technology used to
    ///     keep the business data persistent.
    /// </summary>
    public sealed class WorkspaceType
    {
        //////////
        //  Construction
        private WorkspaceType(IDatabaseType databaseType)
        {
            Debug.Assert(databaseType != null);

            _DatabaseType = databaseType;
        }

        //////////
        //  General properties

        /// <summary>
        ///     The mnemonic identifier of this workspace type.
        /// </summary>
        public string Mnemonic => _DatabaseType.Mnemonic;

        /// <summary>
        ///     The user/readable display name of this workspace type
        ///     for the current UI culture.
        /// </summary>
        public string DisplayName => _DatabaseType.DisplayName;

        /// <summary>
        ///     The small (16x16) image representing this workspace type.
        /// </summary>
        public Image SmallImage => _DatabaseType.SmallImage;

        /// <summary>
        ///     The large (32x32) image representing this workspace type.
        /// </summary>
        public Image LargeImage => _DatabaseType.LargeImage;

        /// <summary>
        ///     True if this workspace type is "operational" (i.e. can 
        ///     be used), else false.
        /// </summary>
        public bool IsOperational => _DatabaseType.IsOperational;

        /// <summary>
        ///     The short (1 line) status report for this workspace type.
        /// </summary>
        public string ShortStatusReport => _DatabaseType.ShortStatusReport;

        /// <summary>
        ///     The long status report for this workspace type. Can contain
        ///     multiple lines separated bz the newline character '\n'.
        /// </summary>
        public string LongStatusReport => _DatabaseType.LongStatusReport;

        //////////
        //  Registry

        /// <summary>
        ///     An unordered list of all known workspace types.
        /// </summary>
        public static WorkspaceType[] All
        {
            get
            {
                _DetectWorkspaceTypes();
                return _WorkspaceTypes.Values.ToArray();
            }
        }

        //////////
        //  Implementation
        private static readonly Dictionary<string, WorkspaceType> _WorkspaceTypes = new Dictionary<string, WorkspaceType>(); /* = null == not initialized */
        private readonly IDatabaseType _DatabaseType;   //  never null

        //  Helpers
        private static void _DetectWorkspaceTypes()
        {
            foreach (IDatabaseType databaseType in DatabaseTypeManager.AllDatabaseTypes)
            {
                if (!_WorkspaceTypes.ContainsKey(databaseType.Mnemonic))
                {
                    _WorkspaceTypes.Add(databaseType.Mnemonic, new WorkspaceType(databaseType));
                }
            }
        }

        internal static WorkspaceType _MapDatabaseType(IDatabaseType databaseType)
        {
            _DetectWorkspaceTypes();
            return _WorkspaceTypes[databaseType.Mnemonic];
        }
    }
}
