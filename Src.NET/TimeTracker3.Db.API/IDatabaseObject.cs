using TimeTracker3.Db.API.Exceptions;

namespace TimeTracker3.Db.API
{
    /// <summary>
    ///     A generic object residing in a database.
    /// </summary>
    public interface IDatabaseObject
    {
        //////////
        //  Properties

        /// <summary>
        ///     This object's type. Implementations should use
        ///     constants defined in DatabaseObjectTypes class.
        ///     Can be safely called on live and dead object alike.
        /// </summary>
        DatabaseObjectType ObjectType { get; }

        /// <summary>
        ///     True if this object is live and can be used, false 
        ///     if dead.
        /// </summary>
        bool IsLive { get; }

        /// <summary>
        ///     The database where this object resides (if live) or
        ///     used to reside (if dead), never null.
        /// </summary>
        IDatabase Database { get; }

        /// <summary>
        ///     The OID of this object - unique per database.
        ///     Can be obtained safely for both live and dead objects,
        ///     but can only be set for live objects.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs while setting the OID.
        /// </exception>
        IDatabaseObjectId Oid { get; set; }

        /// <summary>
        ///     The UI traits of this object, never null.
        ///     Implementations are strongly encouraged to always
        ///     return the same IUiTraits instance for a given 
        ///     IDatabaseObject instance.
        /// </summary>
        IUiTraits UiTraits { get; }

        //////////
        //  Operations - life cycle

        /// <summary>
        ///     Destroys this object, delete-cascading as necessary.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        void Destroy();
    }
}
