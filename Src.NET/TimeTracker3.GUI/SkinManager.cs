using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;

namespace TimeTracker3.GUI
{
    /// <summary>
    ///     The registry/manager of known skins.
    /// </summary>
    public static class SkinManager
    {
        //////////
        //  Properties

        /// <summary>
        ///     An unordered list of all registered Skins.
        /// </summary>
        public static ISkin[] AllSkins => _AllSkins.ToArray();

        /// <summary>
        ///     The "current" skin; always active.
        /// </summary>
        public static ISkin CurrentSkin
        {
            get => _CurrentSkin;
            set
            {
                if (value != _CurrentSkin)
                {   //  Change
                    _CurrentSkin?.Deactivate();
                    _CurrentSkin = value;
                }
                //  Just make sure it's [still] active
                _CurrentSkin?.Activate();
            }
        }

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
        public static bool RegisterSkin(ISkin skin)
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
        private static ISkin _CurrentSkin; /* = null; */
    }
}
