using System;
using TimeTracker3.Db.API.Exceptions;

namespace TimeTracker3.Db.API
{
    /// <summary>
    ///     A particular activity that a user can spend time doing.
    /// </summary>
    public interface IActivity : IDatabaseObject
    {
        //////////
        //  Properties

        /// <summary>
        ///     The user-readable display name of this activity.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        string DisplayName { get; set; }

        /// <summary>
        ///     The description of this activity; can contain 
        ///     multiple lines separated by a newline '\n'.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        string Description { get; set; }

        /// <summary>
        ///     The timeout after which, if there is no user activity,
        ///     this activity is automatically stopped; null == none.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        TimeSpan? Timeout { get; set; }

        /// <summary>
        ///     True if the user shall be asked to make a comment (logged 
        ///     as event) when this activity is started, false if not.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        bool RequireCommentOnStart { get; set; }

        /// <summary>
        ///     True if the user shall be asked to make a comment (logged 
        ///     as event) when this activity is finished, false if not.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        bool RequireCommentOnFinish { get; set; }

        /// <summary>
        ///     True if a full-screen reminder shall be displayed while 
        ///     this activity. is underway. This is to ensure that when 
        ///     user starts an activity such as "lunch break" or "meeting"
        ///     and leaves the room he then does not forget to end this
        ///     activity when back at his workplace.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        bool FullScreenReminder { get; set; }

        //////////
        //  Associations

        /// <summary>
        ///     The type of this activity, null^== none.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        IActivityType ActivityType { get; set; }

        /// <summary>
        ///     An unordered list of all works booked for this activity,
        ///     never null or contains nulls, but can be empty.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        IWork[] Works { get; }

        /// <summary>
        ///     An unordered list of all events booked for this activity,
        ///     never null or contains nulls, but can be empty.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        IEvent[] Events { get; }
    }
}
