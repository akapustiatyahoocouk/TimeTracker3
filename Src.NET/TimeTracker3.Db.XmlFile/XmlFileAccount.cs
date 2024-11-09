using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Xml;
using TimeTracker3.Db.API;
using TimeTracker3.Util;
using Formatting = TimeTracker3.Util.Formatting;

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
            string login, string password, Capabilities capabilities)
        :   base(user._Database, oid, emailAddresses, enabled)
        {
            Debug.Assert(login != null);
            Debug.Assert(password != null);

            //  Set up properties
            _Login = login;
            _PasswordHash = Sha1.HashString(password);
            _Capabilities = capabilities;
            //  Set up associations
            _User = user;
            user._Accounts.Add(this);
        }

        //////////
        //  IDatabaseObject - Properties
        public override DatabaseObjectType ObjectType => DatabaseObjectTypes.Account;

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
        internal string _Login;
        internal string _PasswordHash;
        internal Capabilities _Capabilities;

        internal XmlFileUser _User;    //  never null

        //////////
        //  Serialization
        internal override void _SerializeProperties(XmlElement element)
        {   //  "element" is the XML element representing this database object
            base._SerializeProperties(element);

            element.SetAttribute("Login", _Login);
            element.SetAttribute("PasswordHash", _PasswordHash);
            element.SetAttribute("Capabilities", Formatting.Format(_Capabilities));
        }

        internal override void _SerializeAggregations(XmlElement element)
        {   //  "element" is the XML element representing this database object
            base._SerializeAggregations(element);
            //  Account has no aggregations of its own
        }

        internal override void _SerializeAssociations(XmlElement element)
        {   //  "element" is the XML element representing this database object
            base._SerializeAssociations(element);
            //  TODO Account -> quickPickList
        }


        //////////
        //  Deserialization
        internal XmlFileAccount(XmlFileUser user, XmlFileIDatabaseObjectId oid)
            : base(user._Database, oid)
        {
            //  Set up properties
            _Login = "?";
            _PasswordHash = Sha1.HashString("");
            _Capabilities = Capabilities.None;
            //  Set up associations
            _User = user;
            user._Accounts.Add(this);
        }

        internal override void _DeserializeProperties(XmlElement element)
        {   //  "element" is the XML element representing this database object
            base._DeserializeProperties(element);

            _Login = element.GetAttribute("Login");
            _PasswordHash = element.GetAttribute("PasswordHash");
            _Capabilities = Parsing.Parse<Capabilities>(element.GetAttribute("Capabilities"), Capabilities.None);
        }

        internal override void _DeserializeAggregations(XmlElement element)
        {   //  "element" is the XML element representing this database object
            base._DeserializeAggregations(element);
            //  Account has no aggregations of its own
        }

        internal override void _DeserializeAssociations(XmlElement element)
        {   //  "element" is the XML element representing this database object
            base._DeserializeAssociations(element);
            //  TODO Account -> quickPickList
        }
    }
}
