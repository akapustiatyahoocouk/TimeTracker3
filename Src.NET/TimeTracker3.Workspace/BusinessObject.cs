using System.Diagnostics;
using TimeTracker3.Db.API;

namespace TimeTracker3.Workspace
{
    /// <summary>
    ///     A generic object residing in a workspace.
    /// </summary>
    public abstract class BusinessObject
    {
        //////////
        //  Construction
        internal BusinessObject(IDatabaseObject dataObject)
        {
            Debug.Assert(dataObject != null);

            _DataObject = dataObject;
        }

        //////////
        //  Implementation
        internal readonly IDatabaseObject _DataObject;
    }
}
