using TimeTracker3.Db.API.Exceptions;

namespace TimeTracker3.Db.API
{
    public interface IWorkload : IDatabaseObject
    {
        //////////
        //  Properties

        /// <summary>
        ///     The user-readable display name of this workload.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        string DisplayName { get; set; }

        /// <summary>
        ///     The description of this workload; can contain 
        ///     multiple lines separated by a newline '\n'.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        string Description { get; set; }

        //////////
        //  Associations

        /// <summary>
        ///     An unordered list of all users that are assigned to 
        ///     this workload, never null or contains nulls, but can be empty.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        IUser[] AssignedUsers { get; }

        /// <summary>
        ///     Assigns the specified user to this workload, has no
        ///     effect if already assigned.
        /// </summary>
        /// <param name="user">
        ///     The user to assign to this workload.
        /// </param>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        void AssignUser(IUser user);

        /// <summary>
        ///     Un-assigns the specified user from this workload, has no
        ///     effect if already not assigned.
        /// </summary>
        /// <param name="user">
        ///     The user to un-assign to this workload.
        /// </param>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        void UnassignUser(IUser user);

        /// <summary>
        ///     An unordered list of all beneficiaries associated with 
        ///     this workload, never null or contains nulls, but can be empty.
        /// </summary>
        IBeneficiary[] Beneficiaries { get; }

        /// <summary>
        ///     Associates the specified beneficiary with this workload,
        ///     has no effect if already associated.
        /// </summary>
        /// <param name="beneficiary">
        ///     The beneficiary to associate with this workload.
        /// </param>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        void AddBeneficiary(IBeneficiary beneficiary);

        /// <summary>
        ///     Dis-associates the specified beneficiary from this workload,
        ///     has no effect if already not associated.
        /// </summary>
        /// <param name="beneficiary">
        ///     The beneficiary to dis-associate from this workload.
        /// </param>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        void RemoveBeneficiary(IBeneficiary beneficiary);
    }
}
