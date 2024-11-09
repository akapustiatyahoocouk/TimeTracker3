using System;
using TimeTracker3.Db.API;

namespace TimeTracker3.Workspace
{
    /// <summary>
    ///     A WorkStream in a workspace.
    /// </summary>
    public sealed class BusinessWorkStream : BusinessWorkload
    {
        //////////
        //  Construction
        internal BusinessWorkStream(Workspace workspace, IWorkStream dataWorkStream)
            : base(workspace, dataWorkStream)
        {
            _DataWorkStream = dataWorkStream;
        }

        //////////
        //  BusinessObject - Operations - life cycle
        public override void Destroy(Credentials credentials)
        {
            throw new NotImplementedException();
        }

        //////////
        //  Implementation
        internal readonly IWorkStream _DataWorkStream;
    }
}
