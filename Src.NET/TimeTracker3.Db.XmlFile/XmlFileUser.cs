using System.Globalization;
using System;
using System.Diagnostics;
using TimeTracker3.Db.API;
using TimeTracker3.Util;
using System.Collections.Generic;
using System.Linq;
using TimeTracker3.Db.API.Exceptions;
using System.Xml;
using Formatting = TimeTracker3.Util.Formatting;
using System.Net;

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
        //  IUser - Operations - life cycle
        public IAccount CreateAccount(bool enabled, string[] emailAddresses,
            string login, string password, Capabilities capabilities)
        {
            Debug.Assert(emailAddresses != null);
            Debug.Assert(login != null);
            Debug.Assert(password != null);

            using (new Lock(_Database._Guard))
            {
                _CheckLive();

                //  Validate parameters
                if (!_Database._Validator.Account.IsValidAccountEmailAddresses(emailAddresses))
                {
                    throw new InvalidPropertyValueDatabaseException(DatabaseObjectTypes.Account.Name, "emailAddresses", emailAddresses);
                }
                if (!_Database._Validator.Account.IsValidAccountLogin(login))
                {
                    throw new InvalidPropertyValueDatabaseException(DatabaseObjectTypes.Account.Name, "password", "???");
                }
                if (!_Database._Validator.Account.IsValidAccountCapabilities(capabilities))
                {
                    throw new InvalidPropertyValueDatabaseException(DatabaseObjectTypes.Account.Name, "capabilities", capabilities);
                }
                //  Make the change...
                XmlFileAccount account =
                    new XmlFileAccount(this,
                        new XmlFileIDatabaseObjectId(_Database._NextUnusedOid++),
                        emailAddresses, enabled,
                        login, password, capabilities);
                //  ...schedule change notifications...
                _Database._ScheduleNotification(
                    new DatabaseObjectCreatedEventArgs(
                        _Database,
                        DatabaseObjectTypes.Account,
                        account._Oid));
                _Database._ScheduleNotification(
                    new DatabaseObjectModifiedEventArgs(
                        _Database,
                        DatabaseObjectTypes.User,
                        _Oid));
                //  ...and we're done
                return account;
            }
        }

        //////////
        //  Implementation
        private TimeSpan? _InactivityTimeout;
        private string _RealName;
        private CultureInfo _UiCulture; //  null == use system

        internal IList<XmlFileAccount> _Accounts = new List<XmlFileAccount>();

        //////////
        //  Serialization
        internal override void _SerializeProperties(XmlElement element)
        {   //  "element" is the XML element representing this database object
            base._SerializeProperties(element);

            element.SetAttribute("RealName", _RealName);
            if (_InactivityTimeout != null)
            {
                element.SetAttribute("InactivityTimeout", Formatting.Format(_InactivityTimeout));
            }
            if (_UiCulture != null)
            {
                element.SetAttribute("UiCulture", Formatting.Format(_UiCulture));
            }
        }

        internal override void _SerializeAggregations(XmlElement element)
        {   //  "element" is the XML element representing this database object
            base._SerializeAggregations(element);

            Debug.Assert(element.OwnerDocument != null);
            XmlElement accountsElement = element.OwnerDocument.CreateElement("Accounts");
            element.AppendChild(accountsElement);
            foreach (XmlFileAccount account in _Accounts)
            {
                _Database._SerializeObject(account, accountsElement);
            }
        }

        internal override void _SerializeAssociations(XmlElement element)
        {   //  "element" is the XML element representing this database object
            base._SerializeAssociations(element);
            //  TODO  User -> Workload
        }

        //////////
        //  Deserialization
        internal XmlFileUser(XmlFileDatabase database, XmlFileIDatabaseObjectId oid)
            : base(database, oid)
        {
            //  Set up properties
            _InactivityTimeout = null;
            _RealName = "?";
            _UiCulture = null;
        }

        internal override void _DeserializeProperties(XmlElement element)
        {   //  "element" is the XML element representing this database object
            base._DeserializeProperties(element);

            _RealName = element.GetAttribute("RealName");
            if (element.HasAttribute("InactivityTimeout"))
            {
                _InactivityTimeout = Parsing.Parse<TimeSpan>(element.GetAttribute("InactivityTimeout"), TimeSpan.Zero);
            }
            if (element.HasAttribute("UiCulture"))
            {
                _UiCulture = Parsing.Parse<CultureInfo>(element.GetAttribute("UiCulture"), null);
            }
        }

        internal override void _DeserializeAggregations(XmlElement element)
        {   //  "element" is the XML element representing this database object
            base._DeserializeAggregations(element);

            foreach (XmlElement accountsElement in
                     element.ChildNodes
                         .OfType<XmlElement>()
                         .Where(e => e.Name == "Accounts"))
            {
                foreach (XmlElement accountElement in
                         accountsElement.ChildNodes
                             .OfType<XmlElement>()
                             .Where(e => e.Name == DatabaseObjectTypes.Account.Name))
                {
                    long oidValue = long.Parse(accountElement.GetAttribute("OID"));
                    XmlFileIDatabaseObjectId oid = new XmlFileIDatabaseObjectId(oidValue);
                    if (_Database._Objects.ContainsKey(oid))
                    {   //  OOPS! Duplicate OID!
                        throw new DatabaseCorruptDatabaseException(_Database._Address);
                    }
                    //  Create & de-serialize account
                    XmlFileAccount account = new XmlFileAccount(this, oid);
                    account._DeserializeProperties(accountElement);
                    account._DeserializeAggregations(accountElement);
                    _Database._XmlElementsForDatabaseObjects[account] = accountElement;
                }
            }
        }

        internal override void _DeserializeAssociations(XmlElement element)
        {   //  "element" is the XML element representing this database object
            base._DeserializeAssociations(element);
            //  TODO  User -> Workload
        }
    }
}
