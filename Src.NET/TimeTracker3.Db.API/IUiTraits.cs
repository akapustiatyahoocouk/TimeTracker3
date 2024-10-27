namespace TimeTracker3.Db.API
{
    /// <summary>
    ///     The UI traits of a database object.
    /// </summary>
    public interface IUiTraits
    {
        //////////
        //  Properties

        /// <summary>
        ///     The user-readable display name of this object's 
        ///     type for the current UI culture.
        /// </summary>
        string TypeDisplayName { get; }

        /// <summary>
        ///     The user-readable display name of this object 
        ///     for the current UI culture.
        /// </summary>
        string DisplayName { get; }
    }
}
