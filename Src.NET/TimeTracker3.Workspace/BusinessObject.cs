using System.Diagnostics;
using TimeTracker3.Db.API;
using TimeTracker3.Db.API.Exceptions;
using TimeTracker3.Util;
using TimeTracker3.Workspace.Exceptions;

namespace TimeTracker3.Workspace
{
    /// <summary>
    ///     A generic object residing in a workspace.
    /// </summary>
    public abstract class BusinessObject
    {
        //////////
        //  Construction
        internal BusinessObject(Workspace workspace, IDatabaseObject dataObject)
        {
            Debug.Assert(workspace != null);
            Debug.Assert(dataObject != null);
            Debug.Assert(workspace._Guard.IsLockedByCurrentThread);

            _Workspace = workspace;
            _DataObject = dataObject;
            _Oid = new BusinessObjectId(dataObject.Oid);
        }

        //////////
        //  Properties

        /// <summary>
        ///     This object's type.
        ///     Can be safely called on live and dead object alike.
        /// </summary>
        public BusinessObjectType ObjectType => BusinessObjectType._Translate(_DataObject.ObjectType);

        /// <summary>
        ///     True if this business object is live and can
        ///     be used, false if dead.
        /// </summary>
        public bool IsLive => _DataObject.IsLive;

        /// <summary>
        ///     The workspace where this business object resides
        ///     (if live) or used to reside (if dead), never null.
        /// </summary>
        public Workspace Workspace => _Workspace;

        /// <summary>
        ///     The OID of this object - unique per workspace.
        ///     Can be obtained safely for both live and dead objects,
        ///     but can only be set for live objects.
        /// </summary>
        /// <exception cref="WorkspaceException">
        ///     If an error occurs while setting the OID.
        /// </exception>
        public BusinessObjectId Oid { get; set; }

        /// <summary>
        ///     The UI traits of this object, never null.
        ///     Implementations are strongly encouraged to always
        ///     return the same IUiTraits instance for a given
        ///     IDatabaseObject instance.
        /// </summary>
        public IUiTraits UiTraits => _DataObject.UiTraits;

        //////////
        //  Operations - life cycle

        /// <summary>
        ///     Destroys this object, delete-cascading as necessary.
        /// </summary>
        /// <param name="credentials">
        ///     The credentials of the service caller.
        /// </param>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        public abstract void Destroy(Credentials credentials);

        //////////
        //  Implementation
        internal readonly Workspace _Workspace;
        internal readonly IDatabaseObject _DataObject;
        internal readonly BusinessObjectId _Oid;

        //  Helpers
        internal void _CheckLive()
        {
            Debug.Assert(_Workspace._Guard.IsLockedByCurrentThread);

            _Workspace._CheckOpen();
            if (!_DataObject.IsLive)
            {
                throw new ObjectDeadWorkspaceException(BusinessObjectType._Translate(_DataObject.ObjectType), BusinessObjectId._Translate(_DataObject.Oid));
            }
        }
    }
}
