namespace TimeTracker3.Db.API
{
    /// <summary>
    ///     A "project" is a workload that has a life cycle (milestones, 
    ///     completion state, etc.).
    /// </summary>
    public interface IProject : IWorkload
    {
        //////////
        //  Properties

        /// <summary>
        ///     The completion status of this project.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        bool Completed { get; set; }

        //////////
        //  Associations

        /// <summary>
        ///     The immediate parent of this project, null == none.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        IProject Parent { get; set; }

        /// <summary>
        ///     An unordered list of all immediate children of this 
        ///     project, never null or contains nulls, but can be empty.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        IProject[] Childern { get; }
    }
}
