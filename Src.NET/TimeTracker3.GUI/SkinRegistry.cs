using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;

namespace TimeTracker3.GUI
{
    /// <summary>
    ///     The registry of known skins.
    /// </summary>
    public static class SkinRegistry
    {
        //////////
        //  Properties

        /// <summary>
        ///     An unordered list of all registered Skins.
        /// </summary>
        public static ISkin[] AllSkins => _AllSkins.ToArray();

        //////////
        //  Operations

        /// <summary>
        ///     Registers the specified Skins.
        /// </summary>
        /// <param name="skin">
        ///     The skin to register.
        /// </param>
        /// <returns>
        ///     True if the skin registration was successful,
        /// else false.
        /// </returns>
        public static bool RegisterSkins(ISkin skin)
        {
            Debug.Assert(skin != null);

            ISkin registeredSkins = FindSkin(skin.Mnemonic);
            if (registeredSkins == null)
            {
                _AllSkins.Add(skin);
                return true;
            }
            return registeredSkins == skin;
        }

        /// <summary>
        ///     Finds a registered Skins by mnemonic.
        /// </summary>
        /// <param name="mnemonic">
        ///     The mnemonic to look for.
        /// </param>
        /// <returns>
        ///     The Skin with the required mnemonic, null if not found.
        /// </returns>
        public static ISkin FindSkin(string mnemonic)
        {
            Debug.Assert(mnemonic != null);

            foreach (ISkin skin in _AllSkins)
            {
                if (skin.Mnemonic == mnemonic)
                {
                    return skin;
                }
            }
            return null;
        }

        //////////
        //  Implementation
        private static readonly IList<ISkin> _AllSkins = new List<ISkin>();
    }
}
