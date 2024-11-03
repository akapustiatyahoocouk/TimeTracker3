using System;
using System.Diagnostics;

namespace TimeTracker3.Db.API.Exceptions
{
    /// <summary>
    ///     Thrown when an attempt is made to create
    ///     something which already exists.
    /// </summary>
    public class AlreadyExistsDatabaseException : DatabaseException
    {
        //////////
        //  Construction

        /// <summary>
        ///     Constructs the exception.
        /// </summary>
        /// <param name="objectTypeName">
        ///     The user-readable name of the object's type.
        /// </param>
        /// <param name="propertyName">
        ///     The user-readable name of the property where
        ///     duplication would have occurred.
        /// </param>
        /// <param name="propertyValue">
        ///     The value of the would-be duplicate property.
        /// </param>
        public AlreadyExistsDatabaseException(string objectTypeName, string propertyName, object propertyValue)
        : base(Convert.ToString(objectTypeName) + " with " +
               Convert.ToString(propertyName) + " = '" +
               Convert.ToString(propertyValue) + "' already exists")
        {
            Debug.Assert(objectTypeName != null);
            Debug.Assert(propertyName != null);

            ObjectTypeName = objectTypeName;
            PropertyName = propertyName;
            PropertyValue = propertyValue;
        }

        //////////
        //  Properties

        /// <summary>
        ///     The user-readable name of the object's type.
        /// </summary>
        public readonly string ObjectTypeName;

        /// <summary>
        ///     The user-readable name of the property where
        ///     duplication would have occurred.
        /// </summary>
        public readonly string PropertyName;

        /// <summary>
        ///     The value of the would-be duplicate property.
        /// </summary>
        public readonly object PropertyValue;
    }
}
