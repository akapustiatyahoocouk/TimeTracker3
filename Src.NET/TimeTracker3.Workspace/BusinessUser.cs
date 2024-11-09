using System;
using System.Globalization;
using TimeTracker3.Db.API;
using TimeTracker3.Workspace.Exceptions;

namespace TimeTracker3.Workspace
{
    /// <summary>
    ///     A User in a workspace.
    /// </summary>
    public sealed class BusinessUser : BusinessPrincipal
    {
        //////////
        //  Construction
        internal BusinessUser(Workspace workspace, IUser dataUser)
            : base(workspace, dataUser)
        {
            _DataUser = dataUser;
        }

        //////////
        //  BusinessObject - Operations - life cycle
        public override void Destroy(Credentials credentials)
        {
            throw new NotImplementedException();
        }

        //////////
        //  Properties

        /// <summary>
        ///     Returns the timeout after which, if there is no user activity,
        ///     the current activity is automatically stopped.
        /// </summary>
        /// <param name="credentials">
        ///     The credentials of the service caller.
        /// </param>
        /// <returns>
        ///     The timeout after which, if there is no user activity,
        ///     the current activity is automatically stopped; null == none.
        /// </returns>
        /// <exception cref="WorkspaceException">
        ///     If an error occurs.
        /// </exception>
        public TimeSpan? GetInactivityTimeout(Credentials credentials)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        ///     Sets the timeout after which, if there is no user activity,
        ///     the current activity is automatically stopped.
        /// </summary>
        /// <param name="credentials">
        ///     The credentials of the service caller.
        /// </param>
        /// <param name="inactivityTimeout">
        ///     The new timeout after which, if there is no user activity,
        ///     the current activity is automatically stopped; null == none.
        /// </param>
        /// <exception cref="WorkspaceException">
        ///     If an error occurs.
        /// </exception>
        public void SetInactivityTimeout(Credentials credentials, TimeSpan? inactivityTimeout)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        ///     Returns the real name of this user.
        /// </summary>
        /// <param name="credentials">
        ///     The credentials of the service caller.
        /// </param>
        /// <returns>
        ///     The real name of this user.
        /// </returns>
        /// <exception cref="WorkspaceException">
        ///     If an error occurs.
        /// </exception>
        public string GetRealName(Credentials credentials)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        ///     Sets the real name of this user.
        /// </summary>
        /// <param name="credentials">
        ///     The credentials of the service caller.
        /// </param>
        /// <param name="realName">
        ///     The new real name of this user.
        /// </param>
        /// <exception cref="WorkspaceException">
        ///     If an error occurs.
        /// </exception>
        public void SetRealName(Credentials credentials, string realName)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        ///     Returns the user's preferred UI culture.
        /// </summary>
        /// <param name="credentials">
        ///     The credentials of the service caller.
        /// </param>
        /// <returns>
        ///     The user's preferred UI culture; null == use system.
        /// </returns>
        /// <exception cref="WorkspaceException">
        ///     If an error occurs.
        /// </exception>
        public CultureInfo GetUiCulture(Credentials credentials)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        ///     Sets the user's preferred UI culture.
        /// </summary>
        /// <param name="credentials">
        ///     The credentials of the service caller.
        /// </param>
        /// <returns>
        ///     The user's preferred UI culture; null == use system.
        /// </returns>
        /// <exception cref="WorkspaceException">
        ///     If an error occurs.
        /// </exception>
        public void SetUiCulture(Credentials credentials, CultureInfo uiCulture)
        {
            throw new NotImplementedException();
        }

        //////////
        //  Associations

        /// <summary>
        ///     Returns an unordered list of all accounts of this user.
        /// </summary>
        /// <param name="credentials">
        ///     The credentials of the service caller.
        /// </param>
        /// <returns>
        ///     An unordered list of all accounts of this user,
        ///     never null or contains nulls, but can be empty.
        /// </returns>
        /// <exception cref="WorkspaceException">
        ///     If an error occurs.
        /// </exception>
        public BusinessAccount[] GetAccounts(Credentials credentials)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        ///     Returns an unordered list of all private activities and
        ///     tasks of this user.
        /// </summary>
        /// <param name="credentials">
        ///     The credentials of the service caller.
        /// </param>
        /// <returns>
        ///     An unordered list of all private activities and tasks
        ///     of this user, never null or contains nulls, but can be empty.
        /// </returns>
        /// <exception cref="WorkspaceException">
        ///     If an error occurs.
        /// </exception>
        public BusinessPrivateActivity[] GetPrivateActivitiesAndTasks(Credentials credentials)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        ///     Returns an unordered list of all private activities
        ///     (but not tasks) of this user.
        /// </summary>
        /// <param name="credentials">
        ///     The credentials of the service caller.
        /// </param>
        /// <returns>
        ///     An unordered list of all private activities (but not
        ///     tasks) of this user, never null or contains nulls, but can be empty.
        /// </returns>
        /// <exception cref="WorkspaceException">
        ///     If an error occurs.
        /// </exception>
        public BusinessPrivateActivity[] GetPrivateActivities(Credentials credentials)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        ///     Returns an unordered list of all private
        ///     tasks of this user.
        /// </summary>
        /// <param name="credentials">
        ///     The credentials of the service caller.
        /// </param>
        /// <returns>
        ///     An unordered list of all private tasks of this user,
        ///     never null or contains nulls, but can be empty.
        /// </returns>
        /// <exception cref="WorkspaceException">
        ///     If an error occurs.
        /// </exception>
        public BusinessPrivateTask[] GetPrivateTasks(Credentials credentials)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        ///     Returns an unordered list of all root (i.e. without
        ///     a parent) private tasks of this user.
        /// </summary>
        /// <param name="credentials">
        ///     The credentials of the service caller.
        /// </param>
        /// <returns>
        ///     An unordered list of all root (i.e. without a parent)
        ///     private tasks of this user, never null or contains
        ///     nulls, but can be empty.
        /// </returns>
        /// <exception cref="WorkspaceException">
        ///     If an error occurs.
        /// </exception>
        BusinessPrivateTask[] GetRootPrivateTasks(Credentials credentials)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        ///     Returns an unordered list of all works
        ///     booked by this user.
        /// </summary>
        /// <param name="credentials">
        ///     The credentials of the service caller.
        /// </param>
        /// <returns>
        ///     An unordered list of all works booked by this user,
        ///     never null or contains nulls, but can be empty.
        /// </returns>
        /// <exception cref="WorkspaceException">
        ///     If an error occurs.
        /// </exception>
        public BusinessWork[] GetWorks(Credentials credentials)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        ///     Returns an unordered list of all events
        ///     booked by this user.
        /// </summary>
        /// <param name="credentials">
        ///     The credentials of the service caller.
        /// </param>
        /// <returns>
        ///     An unordered list of all events booked by this user,
        ///     never null or contains nulls, but can be empty.
        /// </returns>
        /// <exception cref="WorkspaceException">
        ///     If an error occurs.
        /// </exception>
        public BusinessEvent[] GetEvents(Credentials credentials)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        ///     Returns an unordered list of all workloads
        ///     where this user is permitted to contribute.
        /// </summary>
        /// <param name="credentials">
        ///     The credentials of the service caller.
        /// </param>
        /// <returns>
        ///     An unordered list of all workloads where this user is
        ///     permitted to contribute, never null or contains nulls, but
        ///     can be empty.
        /// </returns>
        /// <exception cref="WorkspaceException">
        ///     If an error occurs.
        /// </exception>
        public BusinessWorkload[] GetPermittedWorkloads(Credentials credentials)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        ///     Adds the specified workload to the list
        ///     of workloads permitted for this user.
        /// </summary>
        /// <param name="credentials">
        ///     The credentials of the service caller.
        /// </param>
        /// <param name="workload">
        ///     The workload to assign this user to.
        /// </param>
        /// <exception cref="WorkspaceException">
        ///     If an error occurs.
        /// </exception>
        public void AddPermittedWorkload(Credentials credentials, BusinessWorkload workload)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        ///     Removes the specified workload from the
        ///     list of workloads permitted for this user.
        /// </summary>
        /// <param name="credentials">
        ///     The credentials of the service caller.
        /// </param>
        /// <param name="workload">
        ///     The workload to un-assign this user to.
        /// </param>
        /// <exception cref="WorkspaceException">
        ///     If an error occurs.
        /// </exception>
        public void RemovePermittedWorkload(Credentials credentials, BusinessWorkload workload)
        {
            throw new NotImplementedException();
        }

        //////////
        //  Implementation
        internal readonly IUser _DataUser;
    }
}
