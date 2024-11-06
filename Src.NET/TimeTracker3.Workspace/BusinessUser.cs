using System;
using TimeTracker3.Db.API;

namespace TimeTracker3.Workspace
{
    /// <summary>
    ///     A User in a workspace.
    /// </summary>
    public sealed class BusinessUser : BusinessPrincipal
    {
        //////////
        //  Construction
        internal BusinessUser(Workspace workspace, IUser dataUser)
            : base(workspace, dataUser)
        {
            _DataUser = dataUser;
        }

        //////////
        //  BusinessObject - Operations - life cycle
        public override void Destroy()
        {
            throw new NotImplementedException();
        }

        //////////
        //  Implementation
        internal readonly IUser _DataUser;
    }
}
