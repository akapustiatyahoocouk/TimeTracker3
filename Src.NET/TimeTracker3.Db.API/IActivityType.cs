using TimeTracker3.Db.API.Exceptions;

namespace TimeTracker3.Db.API
{
    /// <summary>
    ///     The activity category (e.g. "R & R", "engineering", 
    ///     "umbrella activities", etc.)
    /// </summary>
    public interface IActivityType : IDatabaseObject
    {
        //////////
        //  Properties

        /// <summary>
        ///     The user-readable display name of this activity type.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        string DisplayName { get; set; }

        /// <summary>
        ///     The description of this activity type; can contain 
        ///     multiple lines separated by a newline '\n'.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        string Description { get; set; }

        //////////
        //  Associations

        /// <summary>
        ///     An unordered list of all activities of this type,
        ///     never null or contains nulls.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        IActivity[] Activities { get; }
    }
}
