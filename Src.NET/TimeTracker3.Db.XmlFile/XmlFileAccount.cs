using System;
using System.Diagnostics;
using TimeTracker3.Db.API;
using TimeTracker3.Util;

namespace TimeTracker3.Db.XmlFile
{
    /// <summary>
    ///     A "XML file" database Account implementation.
    /// </summary>
    public sealed class XmlFileAccount : XmlFilePrincipal, IAccount
    {
        //////////
        //  Construction
        internal XmlFileAccount(XmlFileUser user, XmlFileIDatabaseObjectId oid,
            string[] emailAddresses, bool enabled,
            Capabilities capabilities, string login, string password)
        :   base(user._Database, oid, emailAddresses, enabled)
        {
            Debug.Assert(login != null);
            Debug.Assert(password != null);

            //  Set up properties
            _Capabilities = capabilities;
            _Login = login;
            _PasswordHash = Sha1.HashString(password);
            //  Set up associations
            _User = user;
            user._Account.Add(this);
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
        //  IAccount - Properties
        public Capabilities Capabilities
        {
            get => throw new NotImplementedException();
            set => throw new NotImplementedException();
        }

        public string Login
        {
            get => throw new NotImplementedException();
            set => throw new NotImplementedException();
        }

        public string PasswordHash
        {
            get => throw new NotImplementedException();
        }

        public string Password
        {
            set => throw new NotImplementedException();
        }

        //////////
        //  IAccount - Associations
        public IUser User
        {
            get => throw new NotImplementedException();
            set => throw new NotImplementedException();
        }

        public IActivity[] QuickPickList
        {
            get => throw new NotImplementedException();
            set => throw new NotImplementedException();
        }

        //////////
        //  Implementation
        internal Capabilities _Capabilities;
        internal string _Login;
        internal string _PasswordHash;

        internal XmlFileUser _User;    //  never null
    }
}
