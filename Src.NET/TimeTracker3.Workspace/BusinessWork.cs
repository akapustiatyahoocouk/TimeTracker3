using System;
using TimeTracker3.Db.API;

namespace TimeTracker3.Workspace
{
    /// <summary>
    ///     A Work in a workspace.
    /// </summary>
    public sealed class BusinessWork : BusinessObject
    {
        //////////
        //  Construction
        internal BusinessWork(Workspace workspace, IWork dataWork)
            : base(workspace, dataWork)
        {
            _DataWork = dataWork;
        }

        //////////
        //  BusinessObject - Operations - life cycle
        public override void Destroy(Credentials credentials)
        {
            throw new NotImplementedException();
        }

        //////////
        //  Implementation
        internal readonly IWork _DataWork;
    }
}
