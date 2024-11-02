using System.Diagnostics;

namespace TimeTracker3.Db.API
{
    /// <summary>
    ///     The event raised by a Database when an existing
    ///     object is modified there.
    /// </summary>
    public sealed class DatabaseObjectModifiedEventArgs : DatabaseEventArgs
    {
        //////////
        //  Construction

        /// <summary>
        ///     Constructs the event arguments.
        /// </summary>
        /// <param name="database">
        ///     The database where an existing object has
        ///     been modified.
        /// </param>
        /// <param name="objectTypeName">
        ///     The name of the object's type (one of
        ///     the values defined in TypeNames).
        /// </param>
        /// <param name="oid">
        ///     The OID of the modified object.
        /// </param>
        public DatabaseObjectModifiedEventArgs(
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
        ///     The OID of the modified object.
        /// </summary>
        public readonly long Oid;
    }
}
