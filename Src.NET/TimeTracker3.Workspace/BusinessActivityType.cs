using System;
using TimeTracker3.Db.API;

namespace TimeTracker3.Workspace
{
    /// <summary>
    ///     An ActivityType in a workspace.
    /// </summary>
    public sealed class BusinessActivityType : BusinessObject
    {
        //////////
        //  Construction
        internal BusinessActivityType(Workspace workspace, IActivityType dataActivityType)
            : base(workspace, dataActivityType)
        {
            _DataActivityType = dataActivityType;
        }

        //////////
        //  BusinessObject - Operations - life cycle
        public override void Destroy(Credentials credentials)
        {
            throw new NotImplementedException();
        }

        //////////
        //  Implementation
        internal readonly IActivityType _DataActivityType;
    }
}
