using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;

namespace TimeTracker3.Util
{
    /// <summary>
    ///     A collection of related "Setting"s.
    /// </summary>
    public abstract class Settings
    {
        //////////
        //  Construction
        protected Settings(string mnemonic)
        {
            Debug.Assert(mnemonic != null);

            _Mnemonic = mnemonic;
        }

        //////////
        //  Properties

        /// <summary>
        ///     The mnemonic identifier of this settings bunch.
        /// </summary>
        public string Mnemonic => _Mnemonic;

        /// <summary>
        ///     An unordered list of all "Setting"s added
        ///     to this Settings.
        /// </summary>
        public AbstractSetting[] AllSettings => _Settings.Values.ToArray();

        //////////
        //  Operations

        /// <summary>
        ///     Adds the specified Setting to this Settings.
        /// </summary>
        /// <param name="setting"></param>
        public void Add(AbstractSetting setting)
        {
            Debug.Assert(setting != null);

            if (!_Settings.ContainsKey(setting.Mnemonic))
            {
                _Settings.Add(setting.Mnemonic, setting);
            }
        }

        //////////
        //  Implementation
        private readonly string _Mnemonic;
        private readonly Dictionary<string, AbstractSetting> _Settings = new Dictionary<string, AbstractSetting>();
    }
}
