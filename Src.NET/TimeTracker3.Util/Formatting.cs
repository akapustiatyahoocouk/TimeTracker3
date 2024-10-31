using System;

namespace TimeTracker3.Util
{
    /// <summary>
    ///     A generic formatter converting value to string.
    /// </summary>
    /// <typeparam name="T">
    ///     The value's type.
    /// </typeparam>
    /// <param name="value">
    ///     The value to convert to string.
    /// </param>
    /// <returns>
    ///     The string representation of the value.
    /// </returns>
    public delegate string Formatter<T>(T value);

    /// <summary>
    ///     Value formatting helpers.
    /// </summary>
    public static class Formatting
    {
        //////////
        //  Operations

        /// <summary>
        ///     Converts a bool value to string.
        /// </summary>
        /// <param name="b">
        ///     The value to convert.
        /// </param>
        /// <returns>
        ///     The string representation of the value.
        /// </returns>
        public static string FormatBool(bool b)
        {
            return b ? "true" : "false";
        }

        /// <summary>
        ///     Converts an int value to string.
        /// </summary>
        /// <param name="n">
        ///     The value to convert.
        /// </param>
        /// <returns>
        ///     The string representation of the value.
        /// </returns>
        public static string FormatInt(int n)
        {
            return n.ToString();
        }

        /// <summary>
        ///     Converts a string value to string.
        /// </summary>
        /// <param name="s">
        ///     The value to convert.
        /// </param>
        /// <returns>
        ///     The string representation of the value.
        /// </returns>
        public static string FormatString(string s)
        {
            return s;
        }

        /// <summary>
        ///     Converts a DateTime value to string.
        /// </summary>
        /// <param name="dt">
        ///     The value to convert.
        /// </param>
        /// <returns>
        ///     The string representation of the value.
        /// </returns>
        public static string FormatDateTime(DateTime dt)
        {
            if (dt.Kind == DateTimeKind.Local)
            {   //  Must convert to UTC
                dt = TimeZoneInfo.ConvertTimeToUtc(dt, TimeZoneInfo.Local);
            }
            return dt.ToString("dd/MM/yyyy HH:mm:ss.FFF");
        }

        /// <summary>
        ///     Selects the default (as provided by this class) 
        ///     formatter for the specified type.
        /// </summary>
        /// <typeparam name="T">
        ///     The type to create a formatter for.
        /// </typeparam>
        /// <returns>
        ///     The formatter.
        /// </returns>
        /// <exception cref="NotImplementedException">
        ///     If default formatting for the specified type is
        ///     not supported.
        /// </exception>
        public static Formatter<T> GetDefaultFormatter<T>()
        {
            if (typeof(bool) == typeof(T))
            {
                return (Formatter<T>)Delegate.CreateDelegate(typeof(Formatter<bool>), typeof(Formatting), "FormatBool", false);
            }
            else if (typeof(int) == typeof(T))
            {
                return (Formatter<T>)Delegate.CreateDelegate(typeof(Formatter<int>), typeof(Formatting), "FormatInt", false);
            }
            else if (typeof(string) == typeof(T))
            {
                return (Formatter<T>)Delegate.CreateDelegate(typeof(Formatter<string>), typeof(Formatting), "FormatString", false);
            }
            else if (typeof(DateTime) == typeof(T))
            {
                return (Formatter<T>)Delegate.CreateDelegate(typeof(Formatter<DateTime>), typeof(Formatting), "FormatDateTime", false);
            }
            else
            {
                throw new NotImplementedException();
            }
        }
    }
}
