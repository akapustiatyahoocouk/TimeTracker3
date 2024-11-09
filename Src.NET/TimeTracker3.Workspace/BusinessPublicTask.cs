using System;
using TimeTracker3.Db.API;

namespace TimeTracker3.Workspace
{
    /// <summary>
    ///     A PublicTask in a workspace.
    /// </summary>
    public sealed class BusinessPublicTask : BusinessPublicActivity, BusinessTask
    {
        //////////
        //  Construction
        internal BusinessPublicTask(Workspace workspace, IPublicTask dataPublicTask)
            : base(workspace, dataPublicTask)
        {
            _DataPublicTask = dataPublicTask;
        }

        //////////
        //  BusinessObject - Operations - life cycle
        public override void Destroy(Credentials credentials)
        {
            throw new NotImplementedException();
        }

        //////////
        //  Implementation
        internal readonly IPublicTask _DataPublicTask;
    }
}
