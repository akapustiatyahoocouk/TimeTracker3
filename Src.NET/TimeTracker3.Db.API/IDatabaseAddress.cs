namespace TimeTracker3.Db.API
{
    /// <summary>
    ///     Represents a location of a specific database.
    ///     IMPORTANT: Concrete classes implementing this
    ///     interface MUST override object.Equals() and
    ///     object.GetHashCode() as necessary.
    /// </summary>
    public interface IDatabaseAddress
    {
        //////////
        //  Properties

        /// <summary>
        ///     The database type to which this database address belongs,
        ///     never null.
        /// </summary>
        IDatabaseType DatabaseType { get; }

        /// <summary>
        ///     The user/readable display form of this database address
        ///     for the current UI culture.
        /// </summary>
        string DisplayForm { get; }

        /// <summary>
        ///     The external (re-parsable) form of this database address.
        /// </summary>
        string ExternalForm { get; }
    }
}
