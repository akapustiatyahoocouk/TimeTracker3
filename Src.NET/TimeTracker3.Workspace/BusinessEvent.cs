using System;
using TimeTracker3.Db.API;

namespace TimeTracker3.Workspace
{
    /// <summary>
    ///     An Event in a workspace.
    /// </summary>
    public sealed class BusinessEvent : BusinessObject
    {
        //////////
        //  Construction
        internal BusinessEvent(Workspace workspace, IEvent dataEvent)
            : base(workspace, dataEvent)
        {
            _DataEvent = dataEvent;
        }

        //////////
        //  BusinessObject - Operations - life cycle
        public override void Destroy(Credentials credentials)
        {
            throw new NotImplementedException();
        }

        //////////
        //  Implementation
        internal readonly IEvent _DataEvent;
    }
}
