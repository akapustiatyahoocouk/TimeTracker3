using System;
using TimeTracker3.Db.API.Exceptions;

namespace TimeTracker3.Db.API
{
    /// <summary>
    ///     A connection to a persistent data storage.
    /// </summary>
    public interface IDatabase
    {
        //////////
        //  Properties

        /// <summary>
        ///     The type of this database; can be obtained safely for
        ///     both open and closed databases, never null.
        /// </summary>
        IDatabaseType Type { get; }

        /// <summary>
        ///     The address of this database; can be obtained safely for
        ///     both open and closed databases, never null.
        /// </summary>
        IDatabaseAddress Address { get; }

        /// <summary>
        ///     True if this database is currently open, false if closed.
        /// </summary>
        bool IsOpen { get; }

        /// <summary>
        ///     The validator used by this Database; can be obtained
        ///     safely for both open and closed databases, never null.
        /// </summary>
        IValidator Validator { get; }

        //////////
        //  Operations - general

        /// <summary>
        ///     Closes this database, has no effect if already closed.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     Throws if an error occurs (the database is still marked
        ///     as "closed" and all resources it uses are released before
        ///     the exception is thrown).
        /// </exception>
        void Close();

        //////////
        //  Associations

        /// <summary>
        ///     An unordered list of all users in the database,
        ///     never null or contains nulls, but can be empty.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        IUser[] Users { get; }

        /// <summary>
        ///     An unordered list of all activity types in the database,
        ///     never null or contains nulls, but can be empty.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        IActivityType[] ActivityTypes { get; }

        /// <summary>
        ///     An unordered list of all public activities and tasks
        ///     in the database, never null or contains nulls, but can be empty.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        IPublicActivity[] PublicActivitiesAndTasks { get; }

        /// <summary>
        ///     An unordered list of all public activities (but not
        ///     tasks) in the database, never null or contains nulls,
        ///     but can be empty.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        IPublicActivity[] PublicActivities { get; }

        /// <summary>
        ///     An unordered list of all public tasks in the database.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        IPublicTask[] PublicTasks { get; }

        /// <summary>
        ///     An unordered list of all root (i.e. with no parent)
        ///     public tasks in the database, never null or contains nulls,
        ///     but can be empty.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        IPublicTask[] RootPublicTasks { get; }

        /// <summary>
        ///     An unordered list of all works (regardless of the user
        ///     which booked them), never null or contains nulls, but can be empty.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        IWork[] Works { get; }

        /// <summary>
        ///     An unordered list of all events (regardless of the user
        ///     which booked them), never null or contains nulls, but can be empty.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        IEvent[] Events { get; }

        /// <summary>
        ///     An unordered list of all beneficiaries, never null
        ///     or contains nulls, but can be empty.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        IBeneficiary[] Beneficiaries { get; }

        /// <summary>
        ///     An unordered list of all workloads (whether projects
        ///     or work streams), never null or contains nulls, but can be empty.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        IWorkload[] Workloads { get; }

        /// <summary>
        ///     An unordered list of all projects (whether root,
        ///     intermediate or leaf), never null or contains nulls,
        ///     but can be empty.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        IProject[] Projects { get; }

        /// <summary>
        ///     An unordered list of all root projects, never null
        ///     or contains nulls, but can be empty.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        IProject[] RootProjects { get; }

        /// <summary>
        ///     An unordered list of all work streams, never null
        ///     or contains nulls, but can be empty.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        IWorkStream[] WorkStreams { get; }

        //////////
        //  Access control

        /// <summary>
        ///     If there exists an "enabled" account in this
        ///     database with the specified login and password
        ///     and it belongs to an "enabled" user, returns that
        ///     account, otherwise returns null.
        /// </summary>
        /// <param name="login">
        ///     The required account login.
        /// </param>
        /// <param name="password">
        ///     The required account password.
        /// </param>
        /// <returns>
        ///     The account with the specified credentials, null
        ///     if none.
        /// </returns>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        IAccount TryLogin(string login, string password);

        //////////
        //  Events

        /// <summary>
        ///     Raised ON AN ARBITRARY WORKER THREAD when a
        ///     new object is created in this database.
        /// </summary>
        event EventHandler<DatabaseObjectCreatedEventArgs> ObjectCreated;

        /// <summary>
        ///     Raised ON AN ARBITRARY WORKER THREAD when an
        ///     existing object is modified in this database.
        /// </summary>
        event EventHandler<DatabaseObjectModifiedEventArgs> ObjectModified;

        /// <summary>
        ///     Raised ON AN ARBITRARY WORKER THREAD when an
        ///     existing object is destroyed in this database.
        /// </summary>
        event EventHandler<DatabaseObjectDestroyedEventArgs> ObjectDestroyed;
    }
}
