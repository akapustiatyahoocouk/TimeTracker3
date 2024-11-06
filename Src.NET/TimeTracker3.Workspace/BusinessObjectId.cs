using System.Diagnostics;
using TimeTracker3.Db.API;

namespace TimeTracker3.Workspace
{
    /// <summary>
    ///     An OID of a workspace object.
    /// </summary>
    public sealed class BusinessObjectId
    {
        //////////
        //  Construction
        internal BusinessObjectId(IDatabaseObjectId dataOid)
        {
            Debug.Assert(dataOid != null);

            _DataOid = dataOid;
        }

        //////////
        //  Operators
        public static bool operator ==(BusinessObjectId c1, BusinessObjectId c2)
        {
            return c1?.Equals(c2) ?? ReferenceEquals(c2, null);
        }

        public static bool operator !=(BusinessObjectId c1, BusinessObjectId c2)
        {
            return !(c1 == c2);
        }

        //////////
        //  object
        public override bool Equals(object obj)
        {
            if (obj is BusinessObjectId op2)
            {
                return _DataOid.Equals(op2._DataOid);
            }
            return false;
        }

        public override int GetHashCode()
        {
            return _DataOid.GetHashCode();
        }

        public override string ToString()
        {
            return _DataOid.ToString();
        }

        //////////
        //  Implementation
        private readonly IDatabaseObjectId _DataOid;

        //  Helpers
        internal static BusinessObjectId _Translate(IDatabaseObjectId dataOid)
        {
            Debug.Assert(dataOid != null);

            return new BusinessObjectId(dataOid);   //  TODO implement weak caching
        }
    }
}
