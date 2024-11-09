using System;
using System.Collections.Generic;
using System.Diagnostics;
using TimeTracker3.Db.API;
using TimeTracker3.Util;
using TimeTracker3.Workspace.Exceptions;

namespace TimeTracker3.Workspace
{
    public sealed class Workspace : IDisposable
    {
        //////////
        //  Construction
        internal Workspace(IDatabase database)
        {
            Debug.Assert(database != null);

            _Database = database;
            _Type = WorkspaceType._MapDatabaseType(database.Type);
            _Address = _Type._MapDatabaseAddress(database.Address);
        }

        //////////
        //  IDisposable
        public void Dispose()
        {
            try
            {
                Close();
            }
            catch (Exception)
            {   //  TODO log ?
            }
        }

        //////////
        //  Properties

        /// <summary>
        ///     The type of this workspace; can be obtained safely for
        ///     both open and closed workspaces, never null.
        /// </summary>
        public WorkspaceType Type => _Type;

        /// <summary>
        ///     The address of this workspace; can be obtained safely for
        ///     both open and closed workspaces, never null.
        /// </summary>
        public WorkspaceAddress Address => _Address;

        /// <summary>
        ///     True if this workspace is currently open, false if closed.
        /// </summary>
        public bool IsOpen
        {
            get
            {
                using (new Lock(_Guard))
                {
                    return _Database.IsOpen;
                }
            }
        }


        //////////
        //  Operations - general

        /// <summary>
        ///     Closes this workspace, has no effect if already closed.
        /// </summary>
        /// <exception cref="WorkspaceException">
        ///     Throws if an error occurs (the workspace is still marked
        ///     as "closed" and all resources it uses are released before
        ///     the exception is thrown).
        /// </exception>
        public void Close()
        {
            using (new Lock(_Guard))
            {
                try
                {
                    _Database.Close();
                }
                catch (Exception ex)
                {   //  OOPS! Translate & re-throw.
                    throw WorkspaceException.Translate(ex);
                }
                finally
                {
                    //  TODO mark all business objects dead
                    //  TODO drop business object caches
                    //  A "closed" workspace cannot be "current"
                    if (this == CurrentWorkspaceProvider.Instance.Value)
                    {
                        CurrentWorkspaceProvider.Instance.Value = null;
                    }
                }
            }
        }

        //////////
        //  Access control

        /// <summary>
        ///     If there exists an "enabled" account in this
        ///     database with the specified login credentials
        ///     and it belongs to an "enabled" user, returns that
        ///     account, otherwise returns null.
        /// </summary>
        /// <param name="credentials">
        ///     The login credentials.
        /// </param>
        /// <returns>
        ///     The account with the specified login credentials,
        ///     null if none.
        /// </returns>
        /// <exception cref="WorkspaceException">
        ///     If an error occurs.
        /// </exception>
        public BusinessAccount TryLogin(Credentials credentials)
        {
            Debug.Assert(credentials != null);

            using (new Lock(_Guard))
            {
                _CheckOpen();
                IAccount dataAccount =
                    _Database.TryLogin(credentials._Login, credentials._Password);
                if (dataAccount == null)
                {
                    return null;
                }
                //  Translate to business proxy & return
                return _GetBusinessProxy<BusinessAccount>(dataAccount);
            }
        }

        /// <summary>
        ///     Retrieves the access capabilities granted to the
        ///     client with the specified credentials.
        /// </summary>
        /// <param name="credentials">
        ///     The client's credentials.
        /// </param>
        /// <returns>
        ///     The client's access capabilities or null if the
        ///     specified Credentials do not grant access to this
        ///     workspace at all.
        /// </returns>
        /// <exception cref="WorkspaceException">
        ///     If an error occurs.
        /// </exception>
        public BusinessCapabilities? GetCapabilities(Credentials credentials)
        {
            Debug.Assert(credentials != null);

            using (new Lock(_Guard))
            {
                _CheckOpen();
                //  Already cached ?
                if (_CapabilitiesCache.TryGetValue(credentials, out BusinessCapabilities? capabilities))
                {   //  Found in cache!
                    return capabilities;
                }
                //  Determine...
                if (_CapabilitiesCache.Count >= _MaxCapabilitiesCacheSize)
                {
                    _CapabilitiesCache.Clear();
                }
                IAccount dataAccount = _Database.TryLogin(credentials._Login, credentials._Password);
                if (dataAccount == null)
                {   //  Login failed - no capabilities
                    _CapabilitiesCache.Add(credentials, null);
                    return null;
                }
                //  Login succeeded - translate, cache and return
                //  the account's capabilities.
                Capabilities dataCapabilities = dataAccount.Capabilities;
                BusinessCapabilities result = (BusinessCapabilities)dataCapabilities;
                _CapabilitiesCache.Add(credentials, result);
                return result;
            }
        }

        //////////
        //  Implementation
        private readonly IDatabase _Database;
        private readonly WorkspaceType _Type;
        private readonly WorkspaceAddress _Address;

        internal readonly Guard _Guard = new Guard(); //  for synchronizing all calls

        //  Object caches
        private readonly WeakValueDictionary<IDatabaseObject, BusinessObject> _ObjectCache =
            new WeakValueDictionary<IDatabaseObject, BusinessObject>();

        private readonly Dictionary<Credentials, BusinessCapabilities?> _CapabilitiesCache =
            new Dictionary<Credentials, BusinessCapabilities?>();
        private const int _MaxCapabilitiesCacheSize = 256;

        //  Helpers
        internal void _CheckOpen()
        {
            Debug.Assert(_Guard.IsLockedByCurrentThread);

            if (!_Database.IsOpen)
            {
                throw new WorkspaceClosedWorkspaceException(_Address);
            }
        }

        internal T _GetBusinessProxy<T>(IDatabaseObject dataObject)
        where T : BusinessObject
        {
            Debug.Assert(_Guard.IsLockedByCurrentThread);
            Debug.Assert(dataObject != null);

            //  Is the business proxy already cached for dataObject?
            BusinessObject proxy;
            switch (dataObject)
            {
                case IUser dataUser:
                    proxy = _ObjectCache[dataUser];
                    if (proxy is BusinessUser)
                    {
                        return (T)proxy;
                    }
                    proxy = new BusinessUser(this, dataUser);
                    _ObjectCache[dataUser] = proxy;
                    return (T)proxy;
                case IAccount dataAccount:
                    proxy = _ObjectCache[dataAccount];
                    if (proxy is BusinessAccount)
                    {
                        return (T)proxy;
                    }
                    proxy = new BusinessAccount(this, dataAccount);
                    _ObjectCache[dataAccount] = proxy;
                    return (T)proxy;
                //  TODO other object types
                default: //  OOPS! Should never happen!
                    throw new ArgumentException("Invalid dataObject");
            }
            //  No - add entry to the cache
        }
    }
}
