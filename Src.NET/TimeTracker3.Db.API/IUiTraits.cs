using System.Drawing;

namespace TimeTracker3.Db.API
{
    /// <summary>
    ///     The UI traits of a database object.
    /// </summary>
    public interface IUiTraits
    {
        //////////
        //  Properties

        /// <summary>
        ///     The internal name of this object's type.
        /// </summary>
        string TypeName { get; }

        /// <summary>
        ///     The user-readable display name of the object's 
        ///     type for the current UI culture.
        /// </summary>
        string TypeDisplayName { get; }

        /// <summary>
        ///     The user-readable display name of the object 
        ///     for the current UI culture.
        /// </summary>
        string DisplayName { get; }

        /// <summary>
        ///     The small (16x16) image representing the object.
        /// </summary>
        Image SmallImage { get; }

        /// <summary>
        ///     The large (32x32) image representing the object.
        /// </summary>
        Image LargeImage { get; }
    }
}
