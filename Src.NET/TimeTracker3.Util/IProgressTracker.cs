namespace TimeTracker3.Util
{
    /// <summary>
    ///     An abstract interface to an agent that can be notified
    ///     about progress of a lengthy operation.
    /// </summary>
    public interface IProgressTracker
    {
        //////////
        //  Operations

        /// <summary>
        ///     Called to notify that a specific stage has been reached.
        /// </summary>
        /// <param name="activity">
        ///     The user-readable name of the lengthy activity, null == not known.
        /// </param>
        /// <param name="context">
        ///     The user-readable name of the current context, null == not known.
        /// </param>
        /// <param name="completed">
        ///     The ratio (between 0.0 and 1.0, inclusive) to which the
        ///     lengthy activity has been completed; null == not known.
        /// </param>
        void OnProgressReached(string activity, string context, double? completed);
    }
}
