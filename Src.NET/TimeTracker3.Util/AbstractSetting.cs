using System.Diagnostics;

namespace TimeTracker3.Util
{
    /// <summary>
    ///     The common base class for all setting classes.
    /// </summary>
    public abstract class AbstractSetting
    {
        //////////
        //  Construction
        protected AbstractSetting(string mnemonic)
        {
            Debug.Assert(mnemonic != null);

            _Mnemonic = mnemonic;
        }

        //////////
        //  Properties

        /// <summary>
        ///     The mnemonic identifier of this setting.
        /// </summary>
        public string Mnemonic => _Mnemonic;

        /// <summary>
        ///     The value of this setting, formatted as a string.
        ///     Assigning an invalid valueString to this setting
        ///     is ignored silently.
        /// </summary>
        public abstract string ValueString { get; set; }

        //////////
        //  Implementation
        private readonly string _Mnemonic;
    }
}
