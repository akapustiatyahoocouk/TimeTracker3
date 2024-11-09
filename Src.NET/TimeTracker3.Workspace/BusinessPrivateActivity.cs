using System;
using TimeTracker3.Db.API;

namespace TimeTracker3.Workspace
{
    /// <summary>
    ///     A PrivateActivity in a workspace.
    /// </summary>
    public class BusinessPrivateActivity : BusinessActivity
    {
        //////////
        //  Construction
        internal BusinessPrivateActivity(Workspace workspace, IPrivateActivity dataPrivateActivity)
            : base(workspace, dataPrivateActivity)
        {
            _DataPrivateActivity = dataPrivateActivity;
        }

        //////////
        //  BusinessObject - Operations - life cycle
        public override void Destroy(Credentials credentials)
        {
            throw new NotImplementedException();
        }

        //////////
        //  Implementation
        internal readonly IPrivateActivity _DataPrivateActivity;
    }
}
