using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;

namespace TimeTracker3.GUI
{
    /// <summary>
    ///     The registry/manager of known Preferences.
    /// </summary>
    public static class PreferencesManager
    {
        //////////
        //  Properties

        /// <summary>
        ///     An ordered list of all registered
        ///     Preferences.
        /// </summary>
        public static Preferences[] AllPreferences =>
            _AllPreferences.OrderBy(p => p, _Comparer).ToArray();

        /// <summary>
        ///     An ordered list of all registered
        ///     Preferences that have no "Parent".
        /// </summary>
        public static Preferences[] RootPreferences =>
            _AllPreferences.Where(p => p.Parent == null)
                           .OrderBy(p => p, _Comparer)
                           .ToArray();

        //////////
        //  Operations

        /// <summary>
        ///     Registers the specified Preferences.
        /// </summary>
        /// <param name="preferences">
        ///     The preferences to register.
        /// </param>
        /// <returns>
        ///     True if the preferences registration was successful,
        ///     else false.
        /// </returns>
        public static bool RegisterPreferences(Preferences preferences)
        {
            Debug.Assert(preferences != null);

            if (_AllPreferences.Contains(preferences))
            {
                return false;
            }
            _AllPreferences.Add(preferences);
            return true;
        }

        //////////
        //  Implementation
        private static readonly IList<Preferences> _AllPreferences = new List<Preferences>();

        //  Helpers
        internal sealed class _ComparerType : IComparer<Preferences>
        {
            public int Compare(Preferences x, Preferences y)
            {
                Debug.Assert(x != null);
                Debug.Assert(y != null);

                //  "null Order" is "greater than" non-null Order
                if (x.Order != null && y.Order == null)
                {
                    return -1;
                }
                if (x.Order == null && y.Order != null)
                {
                    return 1;
                }
                //  Different orders sort smaller-to-larger
                if (x.Order != null && y.Order != null && x.Order != y.Order)
                {
                    return x.Order.Value - y.Order.Value;
                }
                //  Order is the same - sort by display name
                return string.Compare(x.DisplayName, y.DisplayName, StringComparison.CurrentCultureIgnoreCase);
            }
        }

        internal static readonly _ComparerType _Comparer = new _ComparerType();
    }
}
