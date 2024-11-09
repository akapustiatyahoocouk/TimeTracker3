using System;
using TimeTracker3.Db.API;

namespace TimeTracker3.Workspace
{
    /// <summary>
    ///     A Project in a workspace.
    /// </summary>
    public sealed class BusinessProject : BusinessWorkload
    {
        //////////
        //  Construction
        internal BusinessProject(Workspace workspace, IProject dataProject)
            : base(workspace, dataProject)
        {
            _DataProject = dataProject;
        }

        //////////
        //  BusinessObject - Operations - life cycle
        public override void Destroy(Credentials credentials)
        {
            throw new NotImplementedException();
        }

        //////////
        //  Implementation
        internal readonly IProject _DataProject;
    }
}
