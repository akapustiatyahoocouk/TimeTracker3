using TimeTracker3.Db.API;

namespace TimeTracker3.Db.XmlFile
{
    /// <summary>
    ///     The OID of an object in an "XML File" database.
    /// </summary>
    public struct XmlFileIDatabaseObjectId : IDatabaseObjectId
    {
        //////////
        //  Construction
        internal XmlFileIDatabaseObjectId(long oid)
        {
            _Oid = oid;
        }

        //////////
        //  Object
        public override bool Equals(object obj)
        {
            if (obj is XmlFileIDatabaseObjectId op2)
            {
                return _Oid == op2._Oid;
            }
            return false;
        }

        public override int GetHashCode()
        {
            return _Oid.GetHashCode();
        }

        //////////
        //  Implementation
        private readonly long _Oid;
    }
}
