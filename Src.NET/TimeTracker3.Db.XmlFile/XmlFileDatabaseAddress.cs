using System.Diagnostics;
using System.IO;
using TimeTracker3.Db.API;

namespace TimeTracker3.Db.XmlFile
{
    /// <summary>
    ///     An "address" of an XML file database.
    /// </summary>
    public sealed class XmlFileDatabaseAddress : IDatabaseAddress
    {
        //////////
        //  Construction
        internal XmlFileDatabaseAddress(string fileName)
        {
            Debug.Assert(fileName != null);

            _FileName = Path.GetFullPath(fileName);
        }

        //////////
        //  IDatabaseAddress - Properties
        public IDatabaseType DatabaseType => XmlFileDatabaseType.Instance;
        public string DisplayForm => _FileName;
        public string ExternalForm => _FileName;

        //////////
        //  Implementation
        internal readonly string _FileName; //  always full path
    }
}
