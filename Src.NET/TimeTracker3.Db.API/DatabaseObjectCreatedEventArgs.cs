using System.Diagnostics;

namespace TimeTracker3.Db.API
{
    /// <summary>
    ///     The event raised by a Database when a new
    ///     object is created there.
    /// </summary>
    public sealed class DatabaseObjectCreatedEventArgs : DatabaseEventArgs
    {
        //////////
        //  Construction

        /// <summary>
        ///     Constructs the event arguments.
        /// </summary>
        /// <param name="database">
        ///     The database where a new object has
        ///     been created.
        /// </param>
        /// <param name="objectTypeName">
        ///     The name of the object's type (one of
        ///     the values defined in TypeNames).
        /// </param>
        /// <param name="oid">
        ///     The OID of the newly created object.
        /// </param>
        public DatabaseObjectCreatedEventArgs(
            IDatabase database, string objectTypeName, long oid)
        :   base(database)
        {
            Debug.Assert(objectTypeName != null);

            ObjectTypeName = objectTypeName;
            Oid = oid;
        }

        //////////
        //  Properties

        /// <summary>
        ///     The name of the object's type (one of
        ///     the values defined in TypeNames).
        /// </summary>
        public readonly string ObjectTypeName;

        /// <summary>
        ///     The OID of the newly created object.
        /// </summary>
        public readonly long Oid;
    }
}
