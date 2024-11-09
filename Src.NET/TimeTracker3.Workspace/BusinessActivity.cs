using System;
using TimeTracker3.Db.API;

namespace TimeTracker3.Workspace
{
    /// <summary>
    ///     An Activity in a workspace.
    /// </summary>
    public abstract class BusinessActivity : BusinessObject
    {
        //////////
        //  Construction
        internal BusinessActivity(Workspace workspace, IActivity dataActivity)
            : base(workspace, dataActivity)
        {
            _DataActivity = dataActivity;
        }

        //////////
        //  BusinessObject - Operations - life cycle
        public override void Destroy(Credentials credentials)
        {
            throw new NotImplementedException();
        }

        //////////
        //  Implementation
        internal readonly IActivity _DataActivity;
    }
}
