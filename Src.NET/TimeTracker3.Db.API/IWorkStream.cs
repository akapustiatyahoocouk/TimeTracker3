namespace TimeTracker3.Db.API
{
    /// <summary>
    ///     A "work stream" is a workload that does not have a 
    ///     life cycle (e.g. "coaching and training", etc.)
    /// </summary>
    public interface IWorkStream : IWorkload
    {
    }
}
