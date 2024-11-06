namespace TimeTracker3.Db.API
{
    /// <summary>
    ///     Describes the type of a database object.
    /// </summary>
    public sealed class DatabaseObjectType
    {
        //////////
        //  Construction
        internal DatabaseObjectType(string name)
        {
            _Name = name;
        }

        //////////
        //  Properties

        /// <summary>
        ///     The internal name of this object type
        ///     (i.e. "User", "Account", etc.)
        /// </summary>
        public string Name => _Name;

        //////////
        //  Implementation
        private readonly string _Name;
    }
}
