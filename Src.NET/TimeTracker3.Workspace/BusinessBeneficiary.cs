using System;
using TimeTracker3.Db.API;

namespace TimeTracker3.Workspace
{
    /// <summary>
    ///     A Beneficiary in a workspace.
    /// </summary>
    public sealed class BusinessBeneficiary : BusinessObject
    {
        //////////
        //  Construction
        internal BusinessBeneficiary(Workspace workspace, IBeneficiary dataBeneficiary)
            : base(workspace, dataBeneficiary)
        {
            _DataBeneficiary = dataBeneficiary;
        }

        //////////
        //  BusinessObject - Operations - life cycle
        public override void Destroy(Credentials credentials)
        {
            throw new NotImplementedException();
        }

        //////////
        //  Implementation
        internal readonly IBeneficiary _DataBeneficiary;
    }
}
