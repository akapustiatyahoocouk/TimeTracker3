namespace TimeTracker3.Db.API
{
    /// <summary>
    ///     An activity that has a life cycle (and, therefore, 
    ///     completion status).
    /// </summary>
    public interface ITask : IActivity
    {
        //////////
        //  Properties

        /// <summary>
        ///     The completion status of this task.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        bool Completed { get; set; }

        /// <summary>
        ///     True if the user shall be asked to make a comment (logged 
        ///     as event) when this task is marked completed, false if not.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        bool RequireCommentOnCompletion { get; set; }
    }
}
