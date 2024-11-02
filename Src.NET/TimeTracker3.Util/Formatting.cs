using System;
using System.Drawing;

namespace TimeTracker3.Util
{
    /// <summary>
    ///     A generic formatter converting value to string.
    ///     IMPORTANT: All Formatters are re-entrant.
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
        //  Operations - C# basic types

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

        //////////
        //  Operations - System.* types

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

        //////////
        //  Operations - System.Drawing.* types

        /// <summary>
        ///     Converts a Point value to string.
        /// </summary>
        /// <param name="p">
        ///     The value to convert.
        /// </param>
        /// <returns>
        ///     The string representation of the value.
        /// </returns>
        public static string FormatPoint(Point p)
        {
            return '(' + p.X.ToString() + ',' + p.Y.ToString() + ')';
        }

        /// <summary>
        ///     Converts a Size value to string.
        /// </summary>
        /// <param name="sz">
        ///     The value to convert.
        /// </param>
        /// <returns>
        ///     The string representation of the value.
        /// </returns>
        public static string FormatSize(Size sz)
        {
            return '(' + sz.Width.ToString() + ',' + sz.Height.ToString() + ')';
        }

        /// <summary>
        ///     Converts a Rectangle value to string.
        /// </summary>
        /// <param name="rc">
        ///     The value to convert.
        /// </param>
        /// <returns>
        ///     The string representation of the value.
        /// </returns>
        public static string FormatRectangle(Rectangle rc)
        {
            return FormatPoint(rc.Location) + '-' + FormatSize(rc.Size);
        }

        /// <summary>
        ///     Converts an arbitrary value to string.
        /// </summary>
        /// <param name="value">
        ///     The value to convert.
        /// </param>
        /// <returns>
        ///     The string representation of the value.
        /// </returns>
        /// <exception cref="NotImplementedException">
        ///     If default formatting for the specified type is
        ///     not supported.
        /// </exception>
        public static string Format<T>(T value)
        {
            return GetDefaultFormatter<T>()(value);
        }

        //////////
        //  Operations (formatter factories)

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
        {    //  TODO implement caching type -> Formatter
            //  C# basic types
            if (typeof(bool) == typeof(T))
            {
                return (Formatter<T>)Delegate.CreateDelegate(typeof(Formatter<bool>), typeof(Formatting), "FormatBool", false);
            }
            if (typeof(int) == typeof(T))
            {
                return (Formatter<T>)Delegate.CreateDelegate(typeof(Formatter<int>), typeof(Formatting), "FormatInt", false);
            }
            if (typeof(string) == typeof(T))
            {
                return (Formatter<T>)Delegate.CreateDelegate(typeof(Formatter<string>), typeof(Formatting), "FormatString", false);
            }
            //  System.* types
            if (typeof(DateTime) == typeof(T))
            {
                return (Formatter<T>)Delegate.CreateDelegate(typeof(Formatter<DateTime>), typeof(Formatting), "FormatDateTime", false);
            }
            //  System.Drawing.* types
            if (typeof(Point) == typeof(T))
            {
                return (Formatter<T>)Delegate.CreateDelegate(typeof(Formatter<Point>), typeof(Formatting), "FormatPoint", false);
            }
            if (typeof(Size) == typeof(T))
            {
                return (Formatter<T>)Delegate.CreateDelegate(typeof(Formatter<Size>), typeof(Formatting), "FormatSize", false);
            }
            if (typeof(Rectangle) == typeof(T))
            {
                return (Formatter<T>)Delegate.CreateDelegate(typeof(Formatter<Rectangle>), typeof(Formatting), "FormatRectangle", false);
            }
            //  TODO more types
            throw new NotImplementedException();
        }
    }
}
