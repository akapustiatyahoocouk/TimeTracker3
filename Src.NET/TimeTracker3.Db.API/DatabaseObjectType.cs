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

        public string Name => _Name;

        //////////
        //  Implementation
        private readonly string _Name;
    }
}
