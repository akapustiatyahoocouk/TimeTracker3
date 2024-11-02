using TimeTracker3.Db.API.Exceptions;

namespace TimeTracker3.Db.API
{
    /// <summary>
    ///     A role that a person can play in TimeTracker.
    /// </summary>
    public interface IAccount : IPrincipal
    {
        //////////
        //  Properties

        /// <summary>
        ///     The capabilities assigned to this account.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        Capabilities Capabilities { get; set; }

        /// <summary>
        ///     The login identifier of this account.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        string Login { get; set; }

        /// <summary>
        ///     The SHA-1 hash of password of this account
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        string PasswordHash { get; }

        /// <summary>
        ///     The new password to set for this account.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        string Password { set; }

        //////////
        //  Associations

        /// <summary>
        ///     The user to whom this account belongs, never null.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        IUser User { get; set; }

        /// <summary>
        ///     The ordered list of all "quick pick" activities for 
        ///     this account, never null or contains nulls, but 
        ///     can be empty.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        IActivity[] QuickPickList { get; set; }
    }
}
