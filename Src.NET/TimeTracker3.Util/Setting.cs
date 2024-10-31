using System.Diagnostics;

namespace TimeTracker3.Util
{
    /// <summary>
    ///     A generic persistent setting of type T.
    /// </summary>
    /// <typeparam name="T">
    ///     The data type of the setting's value.
    /// </typeparam>
    public sealed class Setting<T> : AbstractSetting
    {
        //////////
        //  Construction

        /// <summary>
        ///     Constructs the setting.
        /// </summary>
        /// <param name="mnemonic">
        ///     The mnemonic identifier of this setting.
        /// </param>
        /// <param name="formatter">
        ///     The formatter to use for formatting this Setting's value.
        /// </param>
        /// <param name="parser">
        ///     The parser to use for parsing this Setting's value.
        /// </param>
        /// <param name="value">
        ///     The initial value of this setting.
        /// </param>
        public Setting(string mnemonic, Formatter<T> formatter, Parser<T> parser, T value)
            : base(mnemonic)

        {
            Debug.Assert(formatter != null);
            Debug.Assert(parser != null);

            _Formatter = formatter;
            _Parser = parser;
            _Value = value;
        }

        /// <summary>
        ///     Constructs the setting; uses default formatter and parser.
        /// </summary>
        /// <param name="mnemonic">
        ///     The mnemonic identifier of this setting.
        /// </param>
        /// <param name="value">
        ///     The initial value of this setting.
        /// </param>
        public Setting(string mnemonic, T value)
            : base(mnemonic)
        {
            _Formatter = Formatting.GetDefaultFormatter<T>(); 
            _Parser = Parsing.GetDefaultParser<T>();
            _Value = value;
        }

        //////////
        //  AbstractSetting
        public override string ValueString
        {
            get => _Formatter(_Value);
            set => _Value = _Parser(value, _Value);
        }

        //////////
        //  Properties

        /// <summary>
        ///     The value of the setting, can be null for
        ///     object types.
        /// </summary>
        public T Value
        {
            get => _Value;
            set => _Value = value;
        }

        //////////
        //  Implementation
        private readonly Formatter<T> _Formatter;
        private readonly Parser<T> _Parser;
        private T _Value;
    }
}
