namespace TimeTracker3.Db.API
{
    /// <summary>
    ///     A generic access control principal.
    /// </summary>
    public interface IPrincipal : IDatabaseObject
    {
        //////////
        //  Properties

        /// <summary>
        ///     The ordered list of e-mail addresses of this principal,
        ///     can be empty but never null and cannot contain nulls.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        string[] EmailAddresses { get; set; }

        /// <summary>
        ///     True if this principal is "enabled", false if "disabled".
        ///     Disabled principals are as good as nonexistent when
        ///     access control checks are performed.
        /// </summary>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        bool Enabled { get; set; }
    }
}
