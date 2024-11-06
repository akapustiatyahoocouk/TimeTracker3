using System.Globalization;
using System;
using System.Collections;
using System.Diagnostics;
using TimeTracker3.Db.API;
using TimeTracker3.Util;
using System.Collections.Generic;

namespace TimeTracker3.Db.XmlFile
{
    /// <summary>
    ///     A "XML file" database User implementation.
    /// </summary>
    public sealed class XmlFileUser : XmlFilePrincipal, IUser
    {
        //////////
        //  Construction
        internal XmlFileUser(XmlFileDatabase database, XmlFileIDatabaseObjectId oid,
            string[] emailAddresses, bool enabled,
            TimeSpan? inactivityTimeout, string realName, CultureInfo uiCulture)
            : base(database, oid, emailAddresses, enabled)
        {
            Debug.Assert(!inactivityTimeout.HasValue || inactivityTimeout.Value.TotalSeconds > 0);
            Debug.Assert(realName != null);

            //  Set up properties
            _InactivityTimeout = inactivityTimeout;
            _RealName = realName;
            _UiCulture = uiCulture;
        }

        //////////
        //  IDatabaseObject - Properties
        public override DatabaseObjectType ObjectType => DatabaseObjectTypes.User;

        //////////
        //  IDatabaseObject - Operations - life cycle
        public override void Destroy()
        {
            throw new NotImplementedException();
        }

        //////////
        //  IUser - Properties
        public TimeSpan? InactivityTimeout
        {
            get
            {
                using (new Lock(_Database._Guard))
                {
                    _CheckLive();
                    return _InactivityTimeout;
                }
            }
            set
            {
                throw new NotImplementedException();
            }
        }

        public string RealName
        {
            get
            {
                using (new Lock(_Database._Guard))
                {
                    _CheckLive();
                    return _RealName;
                }
            }
            set
            {
                throw new NotImplementedException();
            }
        }

        public CultureInfo UiCulture
        {
            get
            {
                using (new Lock(_Database._Guard))
                {
                    _CheckLive();
                    return _UiCulture;
                }
            }
            set
            {
                throw new NotImplementedException();
            }
        }

        //////////
        //  IUser - Associations
        public IAccount[] Accounts => throw new NotImplementedException();
        public IPrivateActivity[] PrivateActivitiesAndTasks => throw new NotImplementedException();
        public IPrivateActivity[] PrivateActivities => throw new NotImplementedException();
        public IPrivateTask[] PrivateTasks => throw new NotImplementedException();
        public IPrivateTask[] RootPrivateTasks => throw new NotImplementedException();
        public IWork[] Works => throw new NotImplementedException();
        public IEvent[] Events => throw new NotImplementedException();
        public IWorkload[] PermittedWorkloads => throw new NotImplementedException();

        public void AddPermittedWorkload(IWorkload workload)
        {
            throw new NotImplementedException();
        }

        public void RemovePermittedWorkload(IWorkload workload)
        {
            throw new NotImplementedException();
        }

        //////////
        //  Implementation
        private TimeSpan? _InactivityTimeout;
        private string _RealName;
        private CultureInfo _UiCulture; //  null == use system

        internal IList<XmlFileAccount> _Account = new List<XmlFileAccount>();
    }
}
