using System;
using System.Diagnostics;
using TimeTracker3.Db.API;
using TimeTracker3.Util;
using TimeTracker3.Workspace.Exceptions;

namespace TimeTracker3.Workspace
{
    public sealed class Workspace
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
                lock (_Guard)
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
            lock (_Guard)
            {
                try
                {
                    _Database.Close();
                    //  TODO mark all business objects dead
                    //  TODO drop business object caches
                }
                catch (Exception ex)
                {   //  OOPS! Translate & re-throw.
                    throw WorkspaceException.Translate(ex);
                }
            }
        }

        //////////
        //  Implementation
        private readonly IDatabase _Database;
        private readonly WorkspaceType _Type;
        private readonly WorkspaceAddress _Address;

        internal readonly object _Guard = new object(); //  for synchronizing all calls

        //  Object caches
        private readonly WeakValueDictionary<IDatabaseObject, BusinessObject> _ObjectCache =
            new WeakValueDictionary<IDatabaseObject, BusinessObject>();
    }
}
