using TimeTracker3.Db.API.Exceptions;

namespace TimeTracker3.Db.API
{
    /// <summary>
    ///     An agent (person or organization, whether internal 
    ///     or external) who benefits from a given workload; this 
    ///     will typically be the one billed for it.
    /// </summary>
    public interface IBeneficiary
    {
        //////////
        //  Properties

        /// <summary>
        ///     The user-readable display name of this beneficiary.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        string DisplayName { get; set; }

        /// <summary>
        ///     The description of this beneficiary; can contain 
        ///     multiple lines separated by a newline '\n'.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        string Description { get; set; }

        //////////
        //  Associations
        /// <summary>
        ///     An unordered list of all workloads associated with 
        ///     this beneficiary, never null or contains nulls, but can be empty.
        /// </summary>
        IWorkload [] Workloads { get; }

        /// <summary>
        ///     Associates the specified workload with this beneficiary,
        ///     has no effect if already associated.
        /// </summary>
        /// <param name="workload">
        ///     The workload to associate with this beneficiary.
        /// </param>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        void AddWorkload(IWorkload workload);

        /// <summary>
        ///     Dis-associates the specified workload from this beneficiary,
        ///     has no effect if already not associated.
        /// </summary>
        /// <param name="workload">
        ///     The workload to dis-associate from this beneficiary.
        /// </param>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        void RemoveWorkload(IWorkload workload);
    }
}
