using System;
using System.Globalization;

namespace TimeTracker3.Util
{
    /// <summary>
    ///     A generic parser converting string to a value.
    /// </summary>
    /// <typeparam name="T">
    ///     The value's type.
    /// </typeparam>
    /// <param name="valueString">
    ///     The string to convert to value.
    /// </param>
    /// <param name="defaultValue">
    ///     The value to return if the parsing fails.
    /// </param>
    /// <returns>
    ///     The parsed value.
    /// </returns>
    public delegate T Parser<T>(string valueString, T defaultValue);

    /// <summary>
    ///     Value parsing helpers.
    /// </summary>
    public static class Parsing
    {
        //////////
        //  Operations

        /// <summary>
        ///     Parses a string representation of a bool value.
        /// </summary>
        /// <param name="valueString">
        ///     The string representation of a bool value.
        /// </param>
        /// <param name="defaultValue">
        ///     The value to return when the parsing fails.
        /// </param>
        /// <returns>
        ///     The parsed value (on success) or default 
        ///     value (on parse failure).
        /// </returns>
        public static bool ParseBool(string valueString, bool defaultValue)
        {
            if (valueString == null)
            {
                return defaultValue;
            }
            return valueString.Equals("true", StringComparison.InvariantCultureIgnoreCase) ||
                   valueString.Equals("t", StringComparison.InvariantCultureIgnoreCase) ||
                   valueString.Equals("yes", StringComparison.InvariantCultureIgnoreCase) ||
                   valueString.Equals("y", StringComparison.InvariantCultureIgnoreCase) ||
                   valueString.Equals("1", StringComparison.InvariantCultureIgnoreCase);
        }

        /// <summary>
        ///     Parses a string representation of an int value.
        /// </summary>
        /// <param name="valueString">
        ///     The string representation of an int value.
        /// </param>
        /// <param name="defaultValue">
        ///     The value to return when the parsing fails.
        /// </param>
        /// <returns>
        ///     The parsed value (on success) or default 
        ///     value (on parse failure).
        /// </returns>
        public static int ParseInt(string valueString, int defaultValue)
        {
            if (valueString == null)
            {
                return defaultValue;
            }
            return int.TryParse(valueString, out var n) ? n : defaultValue;
        }

        /// <summary>
        ///     Parses a string representation of a string value.
        /// </summary>
        /// <param name="valueString">
        ///     The string representation of a string value.
        /// </param>
        /// <param name="defaultValue">
        ///     The value to return when the parsing fails.
        /// </param>
        /// <returns>
        ///     The parsed value (on success) or default 
        ///     value (on parse failure).
        /// </returns>
        public static string ParseString(string valueString, string defaultValue)
        {
            return valueString ?? defaultValue;
        }

        /// <summary>
        ///     Parses a string representation of a DateTime value.
        /// </summary>
        /// <param name="valueString">
        ///     The string representation of a DateTime value.
        /// </param>
        /// <param name="defaultValue">
        ///     The value to return when the parsing fails.
        /// </param>
        /// <returns>
        ///     The parsed value (on success) or default 
        ///     value (on parse failure).
        /// </returns>
        public static DateTime ParseDateTime(string valueString, DateTime defaultValue)
        {
            if (valueString == null)
            {
                return defaultValue;
            }

            return DateTime.TryParseExact(valueString,
                                          "dd/MM/yyyy HH:mm:ss.FFF", 
                                          new DateTimeFormatInfo(), 
                                          DateTimeStyles.AssumeUniversal,
                                          out var dt) ? dt.ToUniversalTime() : defaultValue;
        }

        /// <summary>
        ///     Selects the default (as provided by this class) 
        ///     parser for the specified type.
        /// </summary>
        /// <typeparam name="T">
        ///     The type to create a parser for.
        /// </typeparam>
        /// <returns>
        ///     The parser.
        /// </returns>
        /// <exception cref="NotImplementedException">
        ///     If default parsing for the specified type is
        ///     not supported.
        /// </exception>
        public static Parser<T> GetDefaultParser<T>()
        {
            if (typeof(bool) == typeof(T))
            {
                return (Parser<T>)Delegate.CreateDelegate(typeof(Parser<bool>), typeof(Parsing), "ParseBool", false);
            }
            else if (typeof(int) == typeof(T))
            {
                return (Parser<T>)Delegate.CreateDelegate(typeof(Parser<int>), typeof(Parsing), "ParseInt", false);
            }
            else if (typeof(string) == typeof(T))
            {
                return (Parser<T>)Delegate.CreateDelegate(typeof(Parser<string>), typeof(Parsing), "ParseString", false);
            }
            else if (typeof(DateTime) == typeof(T))
            {
                return (Parser<T>)Delegate.CreateDelegate(typeof(Parser<DateTime>), typeof(Parsing), "ParseDateTime", false);
            }
            else
            {
                throw new NotImplementedException();
            }
        }
    }
}
