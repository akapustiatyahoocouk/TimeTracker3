using System;

namespace TimeTracker3.Db.API
{
    /// <summary>
    ///     An "event" is a one-off occasion that, for the purposes 
    ///     of timekeeping, occurs at a given date+time but does 
    ///     not have a "duration".
    /// </summary>
    public interface IEvent : IDatabaseObject
    {
        //////////
        //  Properties

        /// <summary>
        ///     The UTC date+time when this event has occurred.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        DateTime OccurredAt { get; }

        /// <summary>
        ///     The 1-line event summary.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        string Summary { get; }

        //////////
        //  Associations

        /// <summary>
        ///     The activity related to the event, null == none.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        IActivity Activity { get; }

        /// <summary>
        ///     The user who logged the event, never null.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        IUser User { get; }
    }
}
