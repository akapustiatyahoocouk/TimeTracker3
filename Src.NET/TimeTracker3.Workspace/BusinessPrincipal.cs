using System;
using System.Diagnostics;
using TimeTracker3.Db.API;

namespace TimeTracker3.Workspace
{
    /// <summary>
    ///     A generic access control Principal in a workspace.
    /// </summary>
    public abstract class BusinessPrincipal : BusinessObject
    {
        //////////
        //  Construction
        internal BusinessPrincipal(Workspace workspace, IPrincipal dataPrincipal)
        : base(workspace, dataPrincipal)
        {
            _DataPrincipal = dataPrincipal;
        }

        //////////
        //  Operations - Properties
        //  TODO document

        public string[] GetEmailAddresses(Credentials credentials)
        {
            Debug.Assert(credentials != null);

            throw new NotImplementedException();
        }

        public void SetEmailAddresses(Credentials credentials, string[] emailAddresses)
        {
            Debug.Assert(credentials != null);

            throw new NotImplementedException();
        }

        public bool IsEnabled(Credentials credentials)
        {
            Debug.Assert(credentials != null);

            throw new NotImplementedException();
        }

        public void SetEnabled(Credentials credentials, bool enabled)
        {
            Debug.Assert(credentials != null);

            throw new NotImplementedException();
        }

        //////////
        //  Implementation
        internal readonly IPrincipal _DataPrincipal;
    }
}
