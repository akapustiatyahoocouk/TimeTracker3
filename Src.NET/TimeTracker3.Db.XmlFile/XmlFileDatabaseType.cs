using System;
using System.Diagnostics;
using System.Drawing;
using System.Windows.Forms;
using TimeTracker3.Db.API;
using TimeTracker3.Db.API.Exceptions;

namespace TimeTracker3.Db.XmlFile
{
    public sealed class XmlFileDatabaseType : IDatabaseType
    {
        //////////
        //  Constants
        public const string PreferredExtension = ".tt3";

        //////////
        //  Singleton
        private static XmlFileDatabaseType _Instance /*= null*/;

        private XmlFileDatabaseType()
        {
        }

        /// <summary>
        ///     The one and only instance of this class
        /// </summary>
        public static XmlFileDatabaseType Instance => _Instance ?? (_Instance = new XmlFileDatabaseType());

        //////////
        //  IDatabaseType - General properties
        public string Mnemonic => "XmlFile";
        public string DisplayName => "XML file";
        public Image SmallImage => Properties.Resources.XmlDatabaseTypeSmall;
        public Image LargeImage => Properties.Resources.XmlDatabaseTypeLarge;
        public bool IsOperational => true;
        public string ShortStatusReport => "XML file storage is operational";
        public string LongStatusReport => ShortStatusReport;

        //////////
        //  IDatabaseType - DatabaseAddress handling
        public IDatabaseAddress DefaultDatabaseAddress => null;

        public IDatabaseAddress ParseDatabaseAddress(string externalForm)
        {
            Debug.Assert(externalForm != null);

            try
            {
                return new XmlFileDatabaseAddress(externalForm);
            }
            catch (Exception ex)
            {   //  OOPS! Translate & re-throw
                throw new DatabaseException(ex);
            }
        }

        public IDatabaseAddress EnterNewDatabaseAddress(IWin32Window parent)
        {
            using (SaveFileDialog dlg = new SaveFileDialog())
            {
                //  TODO last UI directory dlg.InitialDirectory = "c:\\";
                dlg.Filter = _FileFilter;
                dlg.FilterIndex = 0;
                if (dlg.ShowDialog(parent) == DialogResult.OK)
                {
                    string filePath = dlg.FileName;
                    //  TODO update last UI directory
                    return new XmlFileDatabaseAddress(filePath);
                }
                return null;
            }
        }

        public IDatabaseAddress EnterExistingDatabaseAddress(IWin32Window parent)
        {
            using (OpenFileDialog dlg = new OpenFileDialog())
            {
                //  TODO last UI directory dlg.InitialDirectory = "c:\\";
                dlg.Filter = _FileFilter;
                dlg.FilterIndex = 0;
                if (dlg.ShowDialog(parent) == DialogResult.OK)
                {
                    string filePath = dlg.FileName;
                    //  TODO update last UI directory
                    return new XmlFileDatabaseAddress(filePath);
                }
                return null;
            }
        }

        //////////
        //  IDatabaseType - Database handling
        public IDatabase CreateDatabase(IDatabaseAddress address)
        {
            Debug.Assert(address != null);

            if (!(address is XmlFileDatabaseAddress databaseAddress))
            {   //  OOPS!
                throw new IncompatibleObjectsDatabaseException("Database address", DisplayName + " databases");
            }
            return new XmlFileDatabase(databaseAddress, XmlFileDatabase.Mode.CreateNew);
        }

        public IDatabase OpenDatabase(IDatabaseAddress address)
        {
            Debug.Assert(address != null);

            if (!(address is XmlFileDatabaseAddress databaseAddress))
            {   //  OOPS!
                throw new IncompatibleObjectsDatabaseException("Database address", DisplayName + " databases");
            }
            return new XmlFileDatabase(databaseAddress, XmlFileDatabase.Mode.OpenExisting);
        }

        public void DestroyDatabase(IDatabaseAddress address)
        {
            throw new NotImplementedException();
        }

        //////////
        //  Implementation
        private static readonly string _FileFilter =
            "TimeTracker3 files (*" + PreferredExtension +
            ")|*" + PreferredExtension + "|All files (*.*)|*.*";
    }
}
