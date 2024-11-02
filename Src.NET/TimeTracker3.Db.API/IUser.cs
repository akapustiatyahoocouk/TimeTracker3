using System;
using System.Globalization;
using TimeTracker3.Db.API.Exceptions;

namespace TimeTracker3.Db.API
{
    /// <summary>
    ///     An actual person who can log in to the TimeTracker 
    ///     and do some work there.
    /// </summary>
    public interface IUser : IPrincipal
    {
        //////////
        //  Properties

        /// <summary>
        ///     The timeout after which, if there is no user activity,
        ///     the current activity is automatically stopped; null == none.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        TimeSpan? InactivityTimeout { get; set; }

        /// <summary>
        ///     The real name of this user.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        string RealName { get; set; }

        /// <summary>
        ///     The user's preferred UI locale; null == use system.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        CultureInfo UiCulture { get; set; }

        //////////
        //  Associations

        /// <summary>
        ///     An unordered list of all accounts of this user,
        ///     never null or contains nulls, but can be empty.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        IAccount[] Accounts { get; }

        /// <summary>
        ///     An unordered list of all private activities and tasks 
        ///     of this user, never null or contains nulls, but can be empty.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        IPrivateActivity[] PrivateActivitiesAndTasks { get; }

        /// <summary>
        ///     An unordered list of all private activities (but not
        ///     tasks) of this user, never null or contains nulls, but can be empty.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        IPrivateActivity[] PrivateActivities { get; }

        /// <summary>
        ///     An unordered list of all private tasks of this user,
        ///     never null or contains nulls, but can be empty.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        IPrivateTask[] PrivateTasks { get; }

        /// <summary>
        ///     An unordered list of all root (i.e. without a parent)
        ///     private tasks of this user.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        IPrivateTask[] RootPrivateTasks { get; }

        /// <summary>
        ///     An unordered list of all works booked by this user,
        ///     never null or contains nulls, but can be empty.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        IWork[] Works { get; }

        /// <summary>
        ///     An unordered list of all events booked by this user,
        ///     never null or contains nulls, but can be empty.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        IEvent[] Events { get; }

        /// <summary>
        ///     An unordered list of all workloads where this user is
        ///     permitted to contribute, never null or contains nulls, but 
        ///     can be empty.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        IWorkload[] PermittedWorkloads { get; }

        /// <summary>
        ///     Adds the specified workload to the list of workloads 
        ///     permitted for this user.
        /// </summary>
        /// <param name="workload">
        ///     The workload to assign this user to.
        /// </param>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        void AddPermittedWorkload(IWorkload workload);

        /// <summary>
        ///     Removes the specified workload from the list of workloads 
        ///     permitted for this user.
        /// </summary>
        /// <param name="workload">
        ///     The workload to un-assign this user to.
        /// </param>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        void RemovePermittedWorkload(IWorkload workload);
    }
}
