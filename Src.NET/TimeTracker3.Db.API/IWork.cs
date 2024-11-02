using System;
using TimeTracker3.Db.API.Exceptions;

namespace TimeTracker3.Db.API
{
    /// <summary>
    ///     A "work" is an interval of time that was spend by the 
    ///     user on a given Activity. Note, that such "work" is always 
    ///     linked to the User who has spent the time.
    /// </summary>
    public interface IWork : IDatabaseObject
    {
        //////////
        //  Properties

        /// <summary>
        ///     The UTC date+time when this work has started.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        DateTime StartedAt { get; }

        /// <summary>
        ///     The UTC date+time when this work has finished.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        DateTime FinishedAt { get; }

        //////////
        //  Associations

        /// <summary>
        ///     The activity performed during the work, never null.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        IActivity Activity { get; }

        /// <summary>
        ///     The user who did the work, never null.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        IUser User { get; }
    }
}
