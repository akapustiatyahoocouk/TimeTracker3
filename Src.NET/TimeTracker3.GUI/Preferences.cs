using System.Linq;

namespace TimeTracker3.GUI
{
    /// <summary>
    ///     The "Preferences" is a way to organize persistent
    ///     settings into a hierarchy that can be edited via GUI.
    ///     Concrete derived classes will typically be
    ///     singletons registered by plugins.
    /// </summary>
    public abstract class Preferences
    {
        //////////
        //  Properties

        /// <summary>
        ///     The immediate parent Preferences node of
        ///     this Preferences node, null if none.
        /// </summary>
        public abstract Preferences Parent { get; }

        /// <summary>
        ///     The ordered list of immediate child
        ///     Preferences nodes of this Preferences node.
        /// </summary>
        public Preferences[] Children
        {
            get
            {
                return PreferencesManager.AllPreferences
                            .Where(p => p.Parent == this)
                            .OrderBy(p => p, PreferencesManager._Comparer)
                            .ToArray();
            }
        }

        /// <summary>
        ///     The user-readable display name of this
        ///     Preferences node.
        /// </summary>
        public abstract string DisplayName { get; }

        /// <summary>
        ///     The "order" of this Preferences node within
        ///     its parent Preferences node. Preferences nodes
        ///     with the same "order" are sorted lexicographically
        ///     by DisplayName. The null "order" is considered
        ///     "greater than" any non-null "order".
        /// </summary>
        public abstract int? Order { get; }
    }
}
