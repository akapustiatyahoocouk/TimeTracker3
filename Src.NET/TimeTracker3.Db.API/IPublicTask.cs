namespace TimeTracker3.Db.API
{
    /// <summary>
    ///     A task available to all users.
    /// </summary>
    public interface IPublicTask : IPublicActivity, ITask
    {
        //////////
        //  Associations

        /// <summary>
        ///     The immediate parent of this public task, null == none.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        IPublicTask Parent { get; set; }

        /// <summary>
        ///     An unordered list of all immediate children of this 
        ///     public task, never null or contains nulls, but can be empty.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        IPublicTask[] Childern { get; }
    }
}
