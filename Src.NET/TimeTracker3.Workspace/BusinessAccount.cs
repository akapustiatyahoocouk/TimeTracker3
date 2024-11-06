using System;
using TimeTracker3.Db.API;

namespace TimeTracker3.Workspace
{
    /// <summary>
    ///     An Account in a workspace.
    /// </summary>
    public sealed class BusinessAccount : BusinessPrincipal
    {
        //////////
        //  Construction
        internal BusinessAccount(Workspace workspace, IAccount dataAccount)
            : base(workspace, dataAccount)
        {
            _DataAccount = dataAccount;
        }

        //////////
        //  BusinessObject - Operations - life cycle
        public override void Destroy()
        {
            throw new NotImplementedException();
        }

        //////////
        //  Implementation
        internal readonly IAccount _DataAccount;
    }
}
