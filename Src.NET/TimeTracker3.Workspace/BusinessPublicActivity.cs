using System;
using TimeTracker3.Db.API;

namespace TimeTracker3.Workspace
{
    /// <summary>
    ///     A PublicActivity in a workspace.
    /// </summary>
    public class BusinessPublicActivity : BusinessActivity
    {
        //////////
        //  Construction
        internal BusinessPublicActivity(Workspace workspace, IPublicActivity dataPublicActivity)
            : base(workspace, dataPublicActivity)
        {
            _DataPublicActivity = dataPublicActivity;
        }

        //////////
        //  BusinessObject - Operations - life cycle
        public override void Destroy(Credentials credentials)
        {
            throw new NotImplementedException();
        }

        //////////
        //  Implementation
        internal readonly IPublicActivity _DataPublicActivity;
    }
}
