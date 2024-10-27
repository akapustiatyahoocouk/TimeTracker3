namespace TimeTracker3.Db.API
{
    /// <summary>
    ///     A task available only to a specific user.
    /// </summary>
    public interface IPrivateTask : IPrivateActivity, ITask
    {
        //////////
        //  Associations

        /// <summary>
        ///     The immediate parent of this private task, null == none.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        IPrivateTask Parent { get; set; }

        /// <summary>
        ///     An unordered list of all immediate children of this 
        ///     private task, never null or contains nulls, but can be empty.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        IPrivateTask[] Childern { get; }
    }
}
