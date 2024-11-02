using System;
using System.Drawing;

namespace TimeTracker3.Util
{
    /// <summary>
    ///     A generic parser converting string to a value.
    ///     IMPORTANT: All Parsers are re-entrant.
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
    ///     Attempts to parse a fragment of a string for
    ///     a value of the required type.
    ///     IMPORTANT: All FragmentParsers are re-entrant.
    /// </summary>
    /// <typeparam name="T">
    ///     The type of the parsed value.
    /// </typeparam>
    /// <param name="valueString">
    ///     The string wherein a fragment is to be parsed.
    /// </param>
    /// <param name="scan">
    ///     The position within the string where the parsing
    ///     starts. Upon successful parse is advanced to just
    ///     beyond the parsed portion of the string. Upon parse
    ///     failure remains the same as it was before the call.
    /// </param>
    /// <param name="value">
    ///     The parsed value (on success) or the default value
    ///     of the parsed type (on failure),
    /// </param>
    /// <returns>
    ///     True if the string fragment was successfully parsed,
    ///     else false.
    /// </returns>
    public delegate bool FragmentParser<T>(string valueString, ref int scan, out T value);

    /// <summary>
    ///     Value parsing helpers.
    /// </summary>
    public static class Parsing
    {
        //////////
        //  Operations (fragment parsing) - C# basic types

        /// <summary>
        ///     Converts fragment of a string to a bool value.
        ///     IMPORTANT: Re-entrant.
        /// </summary>
        /// <param name="valueString">
        ///     The string to convert to value.
        /// </param>
        /// <param name="scan">
        ///     The position within the string where the parsing
        ///     starts. Upon successful parse is advanced to just
        ///     beyond the parsed portion of the string. Upon parse
        ///     failure remains the same as it was before the call.
        /// </param>
        /// <param name="value">
        ///     The parsed value (on success) or false (on failure),
        /// </param>
        /// <returns>
        ///     True if the string fragment was successfully parsed,
        ///     else false.
        /// </returns>
        public static bool ParseBoolFragment(string valueString, ref int scan, out bool value)
        {
            if (valueString == null || scan > valueString.Length)
            {
                value = false;
                return false;
            }
            //  true ?
            if (scan + 4 <= valueString.Length &&
                valueString.Substring(scan, 4).Equals("true", StringComparison.InvariantCultureIgnoreCase))
            {
                value = true;
                scan += 4;
                return true;
            }
            if (scan + 3 <= valueString.Length &&
                valueString.Substring(scan, 3).Equals("yes", StringComparison.InvariantCultureIgnoreCase))
            {
                value = true;
                scan += 3;
                return true;
            }
            if (scan + 1 <= valueString.Length &&
                (valueString[scan] == 't' || valueString[scan] == 'T'))
            {
                value = true;
                scan++;
                return true;
            }
            if (scan + 1 <= valueString.Length &&
                (valueString[scan] == 'y' || valueString[scan] == 'Y'))
            {
                value = true;
                scan++;
                return true;
            }
            if (scan + 1 <= valueString.Length &&
                valueString[scan] == '1')
            {
                value = true;
                scan++;
                return true;
            }
            //  false ?
            if (scan + 5 <= valueString.Length &&
                valueString.Substring(scan, 5).Equals("false", StringComparison.InvariantCultureIgnoreCase))
            {
                value = false;
                scan += 5;
                return true;
            }
            if (scan + 2 <= valueString.Length &&
                valueString.Substring(scan, 2).Equals("no", StringComparison.InvariantCultureIgnoreCase))
            {
                value = false;
                scan += 2;
                return true;
            }
            if (scan + 1 <= valueString.Length &&
                (valueString[scan] == 'f' || valueString[scan] == 'F'))
            {
                value = false;
                scan++;
                return true;
            }
            if (scan + 1 <= valueString.Length &&
                (valueString[scan] == 'n' || valueString[scan] == 'N'))
            {
                value = false;
                scan++;
                return true;
            }
            if (scan + 1 <= valueString.Length &&
                valueString[scan] == '0')
            {
                value = false;
                scan++;
                return true;
            }
            //  Give up
            value = false;
            return false;
        }

        /// <summary>
        ///     Converts fragment of a string to an int value.
        ///     IMPORTANT: Re-entrant.
        /// </summary>
        /// <param name="valueString">
        ///     The string to convert to value.
        /// </param>
        /// <param name="scan">
        ///     The position within the string where the parsing
        ///     starts. Upon successful parse is advanced to just
        ///     beyond the parsed portion of the string. Upon parse
        ///     failure remains the same as it was before the call.
        /// </param>
        /// <param name="value">
        ///     The parsed value (on success) or 0 (on failure),
        /// </param>
        /// <returns>
        ///     True if the string fragment was successfully parsed,
        ///     else false.
        /// </returns>
        public static bool ParseIntFragment(string valueString, ref int scan, out int value)
        {
            if (valueString == null || scan > valueString.Length)
            {
                value = 0;
                return false;
            }

            //  Skip sign...
            int prescan = scan;
            bool isNegative = false;
            if (_SkipCharacter(valueString, '+', ref prescan))
            {   //  Nothing to do
            }
            else if (_SkipCharacter(valueString, '-', ref prescan))
            {
                isNegative = true;
            }

            //  Parse magnitude
            int numDigits = 0, temp = 0;
            while (prescan < valueString.Length &&
                   valueString[prescan] >= '0' && valueString[prescan] <= '9')
            {
                int newTemp = temp * 10 + (valueString[prescan] - '0');
                if (newTemp / 10 != temp)
                {   //  OOPS! Overflow!
                    value = 0;
                    return false;
                }

                temp = newTemp;
                prescan++;
                numDigits++;
            }

            //  Assemble the value, advance and we're done
            if (numDigits == 0)
            {   //  OOPS! No digits!
                value = 0;
                return false;
            }
            value = isNegative ? -temp : temp;
            scan = prescan;
            return true;
        }

        /// <summary>
        ///     Converts fragment of a string to a string value.
        ///     IMPORTANT: Re-entrant.
        /// </summary>
        /// <param name="valueString">
        ///     The string to convert to value.
        /// </param>
        /// <param name="scan">
        ///     The position within the string where the parsing
        ///     starts. Upon successful parse is advanced to just
        ///     beyond the parsed portion of the string. Upon parse
        ///     failure remains the same as it was before the call.
        /// </param>
        /// <param name="value">
        ///     The parsed value (on success) or null (on failure),
        /// </param>
        /// <returns>
        ///     True if the string fragment was successfully parsed,
        ///     else false.
        /// </returns>
        public static bool ParseStringFragment(string valueString, ref int scan, out string value)
        {
            if (valueString == null || scan > valueString.Length)
            {
                value = null;
                return false;
            }

            value = valueString.Substring(scan);
            scan = valueString.Length;
            return true;
        }

        //////////
        //  Operations (fragment parsing) - System.* types

        /// <summary>
        ///     Converts fragment of a string to a DateTime value.
        ///     IMPORTANT: Re-entrant.
        /// </summary>
        /// <param name="valueString">
        ///     The string to convert to value.
        /// </param>
        /// <param name="scan">
        ///     The position within the string where the parsing
        ///     starts. Upon successful parse is advanced to just
        ///     beyond the parsed portion of the string. Upon parse
        ///     failure remains the same as it was before the call.
        /// </param>
        /// <param name="value">
        ///     The parsed value (on success) or DateTime.MinValue
        ///     (on failure),
        /// </param>
        /// <returns>
        ///     True if the string fragment was successfully parsed,
        ///     else false.
        /// </returns>
        public static bool ParseDateTimeFragment(string valueString, ref int scan, out DateTime value)
        {
            if (valueString == null || scan > valueString.Length)
            {
                value = DateTime.MinValue;
                return false;
            }

            //  dd/MM/yyyy HH:mm:ss.FFF
            int prescan = scan;
            if (ParseIntFragment(valueString, ref prescan, out int day) &&
                _SkipCharacter(valueString, '/', ref prescan) &&
                ParseIntFragment(valueString, ref prescan, out int month) &&
                _SkipCharacter(valueString, '/', ref prescan) &&
                ParseIntFragment(valueString, ref prescan, out int year) &&
                _SkipCharacter(valueString, ' ', ref prescan) &&
                ParseIntFragment(valueString, ref prescan, out int hour) &&
                _SkipCharacter(valueString, ':', ref prescan) &&
                ParseIntFragment(valueString, ref prescan, out int minute) &&
                _SkipCharacter(valueString, ':', ref prescan) &&
                ParseIntFragment(valueString, ref prescan, out int second) &&
                _SkipCharacter(valueString, '.', ref prescan) &&
                ParseIntFragment(valueString, ref prescan, out int millisecond))
            {
                try
                {
                    value = new DateTime(year, month, day, hour, minute, second, millisecond, DateTimeKind.Utc);
                    scan = prescan;
                    return true;
                }
                catch (Exception)
                {   //  OOPS! some date/time component is out f range
                    value = DateTime.MinValue;
                    return false;
                }
            }
            value = DateTime.MinValue;
            return false;
        }

        //////////
        //  Operations (fragment parsing) - System.Drawing.* types

        /// <summary>
        ///     Converts fragment of a string to a Point value.
        ///     IMPORTANT: Re-entrant.
        /// </summary>
        /// <param name="valueString">
        ///     The string to convert to value.
        /// </param>
        /// <param name="scan">
        ///     The position within the string where the parsing
        ///     starts. Upon successful parse is advanced to just
        ///     beyond the parsed portion of the string. Upon parse
        ///     failure remains the same as it was before the call.
        /// </param>
        /// <param name="value">
        ///     The parsed value (on success) or (0,0) (on failure),
        /// </param>
        /// <returns>
        ///     True if the string fragment was successfully parsed,
        ///     else false.
        /// </returns>
        public static bool ParsePointFragment(string valueString, ref int scan, out Point value)
        {
            if (valueString == null || scan > valueString.Length)
            {
                value = new Point();
                return false;
            }

            //  (x,y))
            int prescan = scan;
            if (_SkipCharacter(valueString, '(', ref prescan) && 
                ParseIntFragment(valueString, ref prescan, out int x) &&
                _SkipCharacter(valueString, ',', ref prescan) &&
                ParseIntFragment(valueString, ref prescan, out int y) &&
                _SkipCharacter(valueString, ')', ref prescan))
            {
                value = new Point(x, y);
                scan = prescan;
                return true;
            }
            value = new Point();
            return false;
        }

        /// <summary>
        ///     Converts fragment of a string to a Size value.
        ///     IMPORTANT: Re-entrant.
        /// </summary>
        /// <param name="valueString">
        ///     The string to convert to value.
        /// </param>
        /// <param name="scan">
        ///     The position within the string where the parsing
        ///     starts. Upon successful parse is advanced to just
        ///     beyond the parsed portion of the string. Upon parse
        ///     failure remains the same as it was before the call.
        /// </param>
        /// <param name="value">
        ///     The parsed value (on success) or 0x0 (on failure),
        /// </param>
        /// <returns>
        ///     True if the string fragment was successfully parsed,
        ///     else false.
        /// </returns>
        public static bool ParseSizeFragment(string valueString, ref int scan, out Size value)
        {
            if (valueString == null || scan > valueString.Length)
            {
                value = new Size();
                return false;
            }

            //  (w,h))
            int prescan = scan;
            if (_SkipCharacter(valueString, '(', ref prescan) &&
                ParseIntFragment(valueString, ref prescan, out int w) &&
                _SkipCharacter(valueString, ',', ref prescan) &&
                ParseIntFragment(valueString, ref prescan, out int h) &&
                _SkipCharacter(valueString, ')', ref prescan))
            {
                value = new Size(w, h);
                scan = prescan;
                return true;
            }
            value = new Size();
            return false;
        }

        /// <summary>
        ///     Converts fragment of a string to a Rectangle value.
        ///     IMPORTANT: Re-entrant.
        /// </summary>
        /// <param name="valueString">
        ///     The string to convert to value.
        /// </param>
        /// <param name="scan">
        ///     The position within the string where the parsing
        ///     starts. Upon successful parse is advanced to just
        ///     beyond the parsed portion of the string. Upon parse
        ///     failure remains the same as it was before the call.
        /// </param>
        /// <param name="value">
        ///     The parsed value (on success) or (0,0)-0x0 (on failure),
        /// </param>
        /// <returns>
        ///     True if the string fragment was successfully parsed,
        ///     else false.
        /// </returns>
        public static bool ParseRectangleFragment(string valueString, ref int scan, out Rectangle value)
        {
            if (valueString == null || scan > valueString.Length)
            {
                value = new Rectangle();
                return false;
            }

            //  (w,h))
            int prescan = scan;
            if (ParsePointFragment(valueString, ref prescan, out Point location) &&
                _SkipCharacter(valueString, '-', ref prescan) &&
                ParseSizeFragment(valueString, ref prescan, out Size size))
            {
                value = new Rectangle(location, size);
                scan = prescan;
                return true;
            }
            value = new Rectangle();
            return false;
        }

        //////////
        //  Operations - complete string parsing

        /// <summary>
        ///     Converts string to a value of a required type.
        ///     IMPORTANT: All Parsers are re-entrant.
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
        /// <exception cref="NotImplementedException">
        ///     If default parsing for the specified type is
        ///     not supported.
        /// </exception>
        public static T Parse<T>(string valueString, T defaultValue)
        {
            FragmentParser<T> fp = GetDefaultFragmentParser<T>();   //  TODO cache!
            int scan = 0;
            if (fp(valueString, ref scan, out T value) && scan == valueString.Length)
            {
                return value;
            }
            return defaultValue;
        }

        //////////
        //  Operations (parser factories)

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
            return Parse<T>;
        }

        /// <summary>
        ///     Selects the default (as provided by this class) 
        ///     fragment parser for the specified type.
        /// </summary>
        /// <typeparam name="T">
        ///     The type to create a fragment parser for.
        /// </typeparam>
        /// <returns>
        ///     The fragment parser.
        /// </returns>
        /// <exception cref="NotImplementedException">
        ///     If default fragment parsing for the specified
        ///     type is not supported.
        /// </exception>
        public static FragmentParser<T> GetDefaultFragmentParser<T>()
        {   //  TODO implement caching type -> FragmentParser
            //  C# basic types
            if (typeof(bool) == typeof(T))
            {
                return (FragmentParser<T>)Delegate.CreateDelegate(typeof(FragmentParser<bool>), typeof(Parsing), "ParseBoolFragment", false);
            }
            if (typeof(int) == typeof(T))
            {
                return (FragmentParser<T>)Delegate.CreateDelegate(typeof(FragmentParser<int>), typeof(Parsing), "ParseIntFragment", false);
            }
            if (typeof(string) == typeof(T))
            {
                return (FragmentParser<T>)Delegate.CreateDelegate(typeof(FragmentParser<string>), typeof(Parsing), "ParseStringFragment", false);
            }
            //  System.* types
            if (typeof(DateTime) == typeof(T))
            {
                return (FragmentParser<T>)Delegate.CreateDelegate(typeof(FragmentParser<DateTime>), typeof(Parsing), "ParseDateTimeFragment", false);
            }
            //  System.Drawing.* types
            if (typeof(Point) == typeof(T))
            {
                return (FragmentParser<T>)Delegate.CreateDelegate(typeof(FragmentParser<Point>), typeof(Parsing), "ParsePointFragment", false);
            }
            if (typeof(Size) == typeof(T))
            {
                return (FragmentParser<T>)Delegate.CreateDelegate(typeof(FragmentParser<Size>), typeof(Parsing), "ParseSizeFragment", false);
            }
            if (typeof(Rectangle) == typeof(T))
            {
                return (FragmentParser<T>)Delegate.CreateDelegate(typeof(FragmentParser<Rectangle>), typeof(Parsing), "ParseRectangleFragment", false);
            }
            //  System.Drawing.* types
            //  TODO more types
            throw new NotImplementedException();
        }

        //////////
        //  Helpers
        private static bool _SkipCharacter(string s, char c, ref int scan)
        {
            if (scan < s.Length && s[scan] == c)
            {
                scan++;
                return true;
            }
            return false;
        }
    }
}
