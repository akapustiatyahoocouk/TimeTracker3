namespace TimeTracker3.Db.API
{
    /// <summary>
    ///     An activity available to only a specific user.
    /// </summary>
    public interface IPrivateActivity
    {
        //////////
        //  Associations

        /// <summary>
        ///     The user to whom this private activity belongs, never null.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        IUser Owner { get; }
    }
}
