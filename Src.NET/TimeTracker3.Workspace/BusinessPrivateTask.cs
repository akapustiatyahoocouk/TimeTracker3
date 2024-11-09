using System;
using TimeTracker3.Db.API;

namespace TimeTracker3.Workspace
{
    /// <summary>
    ///     A PrivateTask in a workspace.
    /// </summary>
    public sealed class BusinessPrivateTask : BusinessPrivateActivity, BusinessTask
    {
        //////////
        //  Construction
        internal BusinessPrivateTask(Workspace workspace, IPrivateTask dataPrivateTask)
            : base(workspace, dataPrivateTask)
        {
            _DataPrivateTask = dataPrivateTask;
        }

        //////////
        //  BusinessObject - Operations - life cycle
        public override void Destroy(Credentials credentials)
        {
            throw new NotImplementedException();
        }

        //////////
        //  Implementation
        internal readonly IPrivateTask _DataPrivateTask;
    }
}
