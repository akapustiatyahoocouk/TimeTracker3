using System;
using TimeTracker3.Db.API;

namespace TimeTracker3.Workspace
{
    /// <summary>
    ///     A Workload in a workspace.
    /// </summary>
    public abstract class BusinessWorkload : BusinessObject
    {
        //////////
        //  Construction
        internal BusinessWorkload(Workspace workspace, IWorkload dataWorkload)
            : base(workspace, dataWorkload)
        {
            _DataWorkload = dataWorkload;
        }

        //////////
        //  BusinessObject - Operations - life cycle
        public override void Destroy(Credentials credentials)
        {
            throw new NotImplementedException();
        }

        //////////
        //  Implementation
        internal readonly IWorkload _DataWorkload;
    }
}
