using System;
using System.Diagnostics;

namespace TimeTracker3.Db.API.Exceptions
{
    /// <summary>
    ///     Thrown when an attempt is made to access an
    ///     object which does not exist.
    /// </summary>
    public class DoesNotExistDatabaseException : DatabaseException
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
        ///     The user-readable name of the property for
        ///     which an object does not exist.
        /// </param>
        /// <param name="propertyValue">
        ///     The value of the property that does not exist.
        /// </param>
        public DoesNotExistDatabaseException(string objectTypeName, string propertyName, object propertyValue)
        : base(Convert.ToString(objectTypeName) + " with " +
               Convert.ToString(propertyName) + " = '" +
               Convert.ToString(propertyValue) + "' does not exist")
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
        ///     The user-readable name of the property for
        ///     which an object does not exist.
        /// </summary>
        public readonly string PropertyName;

        /// <summary>
        ///     The value of the property that does not exist.
        /// </summary>
        public readonly object PropertyValue;
    }
}
