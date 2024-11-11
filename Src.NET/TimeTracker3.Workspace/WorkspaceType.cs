using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;
using TimeTracker3.Db.API;
using TimeTracker3.Workspace.Exceptions;

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
        //  DatabaseAddress handling

        /// <summary>
        ///     If workspaces of this type has a concept of a "default"
        ///     workspace, returns its address, otherwise returns null.
        /// </summary>
        public WorkspaceAddress DefaultWorkspaceAddress
        {
            get
            {
                IDatabaseAddress databaseAddress = _DatabaseType.DefaultDatabaseAddress;
                return databaseAddress == null ? null : _MapDatabaseAddress(databaseAddress);
            }
        }

        /// <summary>
        ///     Parses an external (re-parsable) form of a workspace
        ///     address of this type.
        /// </summary>
        /// <param name="externalForm">
        ///     The external (re-parsable) form of a workspace address.
        /// </param>
        /// <returns>
        ///     The parsed workspace address.
        /// </returns>
        /// <exception cref="WorkspaceException">
        ///     Thrown if the address parsing fails for any reason.
        /// </exception>
        public WorkspaceAddress ParseWorkspaceAddress(string externalForm)
        {
            Debug.Assert(externalForm != null);

            try
            {
                return _MapDatabaseAddress(_DatabaseType.ParseDatabaseAddress(externalForm));
            }
            catch (Exception ex)
            {   //  OOPS! Translate & re-throw
                throw WorkspaceException.Translate(ex);
            }
        }

        /// <summary>
        ///     Prompts the user to enter the address for a new workspace
        ///     of this type.
        /// </summary>
        /// <param name="parent">
        ///     The window to use as a parent for any modal dialogs used
        ///     during address entry.
        /// </param>
        /// <returns>
        ///     The workspace address entered by the user or null if the user
        ///     has chosen to cancel the address entry process.
        /// </returns>
        public WorkspaceAddress EnterNewWorkspaceAddress(IWin32Window parent)
        {
            try
            {
                IDatabaseAddress databaseAddress = _DatabaseType.EnterNewDatabaseAddress(parent);
                return (databaseAddress == null) ? null : _MapDatabaseAddress(databaseAddress);
            }
            catch (Exception ex)
            {   //  OOPS! Translate & re-throw
                throw WorkspaceException.Translate(ex);
            }
        }

        /// <summary>
        ///     Prompts the user to enter the address of an existing
        ///     workspace of this type.
        /// </summary>
        /// <param name="parent">
        ///     The window to use as a parent for any modal dialogs used
        ///     during address entry.
        /// </param>
        /// <returns>
        ///     The workspace address entered by the user or null if the user
        ///     has chosen to cancel the address entry process.
        /// </returns>
        public WorkspaceAddress EnterExistingWorkspaceAddress(IWin32Window parent)
        {
            try
            {
                IDatabaseAddress databaseAddress = _DatabaseType.EnterExistingDatabaseAddress(parent);
                return (databaseAddress == null) ? null : _MapDatabaseAddress(databaseAddress);
            }
            catch (Exception ex)
            {   //  OOPS! Translate & re-throw
                throw WorkspaceException.Translate(ex);
            }
        }

        //////////
        //  Workspace handling

        /// <summary>
        ///     Creates a new empty workspace of this type at the specified
        ///     address.
        /// </summary>
        /// <param name="address">
        ///     The address to create a new workspace at.
        /// </param>
        /// <param name="adminCredentials">
        ///     The credentials for the administrator
        ///     for the new workspace.
        /// </param>
        /// <returns>
        ///     The newly created workspace.
        /// </returns>
        /// <exception cref="WorkspaceException">
        ///     If an error occurs.
        /// </exception>
        public Workspace CreateWorkspace(WorkspaceAddress address, Credentials adminCredentials)
        {
            Debug.Assert(address != null);

            if (address.WorkspaceType != this)
            {   //  OOPS! DatabaseAddress does not belong to this workspace type
                throw new IncompatibleObjectsWorkspaceException("WorkspaceWorkspaceAddress", "Workspace");
            }
            //  Do the work
            IDatabase database = null;
            try
            {
                database = _DatabaseType.CreateDatabase(address._DatabaseAddress);
                //  Create Admin user...
                IUser dataUser = database.CreateUser(true, Array.Empty<string>(), adminCredentials._Login, null, null);
                //  ...with an Admin account...
                dataUser.CreateAccount(true, Array.Empty<string>(), adminCredentials._Login, adminCredentials._Password, Capabilities.Administrator);
                //  ...and we're done
                return new Workspace(database);
            }
            catch (Exception ex)
            {   //  OOPS! Cleanup, translate & re-throw
                try
                {
                    database?.Close();
                    _DatabaseType.DestroyDatabase(address._DatabaseAddress);
                }
                catch (Exception)
                {   //  Suppress - we already have "ex"
                }
                throw WorkspaceException.Translate(ex);
            }
        }

        /// <summary>
        ///     Opens an existing workspace of this type at the specified
        ///     address.
        /// </summary>
        /// <param name="address">
        ///     The address to open a workspace at.
        /// </param>
        /// <returns>
        ///     The newly opened workspace.
        /// </returns>
        /// <exception cref="WorkspaceException">
        ///     If an error occurs.
        /// </exception>
        public Workspace OpenWorkspace(WorkspaceAddress address)
        {
            Debug.Assert(address != null);

            if (address.WorkspaceType != this)
            {   //  OOPS! DatabaseAddress does not belong to this workspace type
                throw new IncompatibleObjectsWorkspaceException("WorkspaceWorkspaceAddress", "Workspace");
            }
            //  Do the work
            try
            {
                IDatabase database = _DatabaseType.OpenDatabase(address._DatabaseAddress);
                return new Workspace(database);
            }
            catch (Exception ex)
            {   //  OOPS! Translate & re-throw
                throw WorkspaceException.Translate(ex);
            }
        }

        /// <summary>
        ///     Destroys an existing workspace of this type at the specified
        ///     address. The workspace must not be in use by any process.
        /// </summary>
        /// <param name="address">
        ///     The address to destroy a workspace at.
        /// </param>
        /// <exception cref="WorkspaceException">
        ///     If an error occurs.
        /// </exception>
        public void DestroyWorkspace(WorkspaceAddress address)
        {
            Debug.Assert(address != null);

            if (address.WorkspaceType != this)
            {   //  OOPS! DatabaseAddress does not belong to this workspace type
                throw new IncompatibleObjectsWorkspaceException("WorkspaceWorkspaceAddress", "Workspace");
            }
            //  Do the work
            try
            {
                _DatabaseType.DestroyDatabase(address._DatabaseAddress);
            }
            catch (Exception ex)
            {   //  OOPS! Translate & re-throw
                throw WorkspaceException.Translate(ex);
            }
        }

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

        private readonly Dictionary<IDatabaseAddress, WorkspaceAddress> _DatabaseAddressMapping = new Dictionary<IDatabaseAddress, WorkspaceAddress>();

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

        internal WorkspaceAddress _MapDatabaseAddress(IDatabaseAddress databaseAddress)
        {
            if (!_DatabaseAddressMapping.ContainsKey(databaseAddress))
            {
                _DatabaseAddressMapping.Add(databaseAddress, new WorkspaceAddress(databaseAddress));
            }
            return _DatabaseAddressMapping[databaseAddress];
        }
    }
}
