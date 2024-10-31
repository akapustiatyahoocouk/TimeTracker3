using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TimeTracker3.GUI
{
    /// <summary>
    ///     An abstract interface to a "skin" - a way to
    ///     present TT3 UI to the user.
    /// </summary>
    public interface ISkin
    {
        //////////
        //  Properties

        //////////
        //  General properties

        /// <summary>
        ///     The mnemonic identifier of this skin.
        /// </summary>
        string Mnemonic { get; }

        /// <summary>
        ///     The user/readable display name of this skin
        ///     for the current UI culture.
        /// </summary>
        string DisplayName { get; }

        /// <summary>
        ///     The small (16x16) image representing this skin.
        /// </summary>
        Image SmallImage { get; }

        /// <summary>
        ///     The large (32x32) image representing this skin.
        /// </summary>
        Image LargeImage { get; }

        /// <summary>
        ///     True if this skin is currently "active"
        ///     (i.e. visible), else false.
        /// </summary>
        bool IsActive { get; }

        //////////
        //  Operations

        /// <summary>
        ///     Activates (shows) this skin, has no effect
        ///     if this skin is already active.
        /// </summary>
        void Activate();

        /// <summary>
        ///     Deactivates (hides) this skin, has no effect
        ///     if this skin is already inactive.
        /// </summary>
        void Deactivate();
    }
}
