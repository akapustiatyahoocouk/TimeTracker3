using System;
using System.Diagnostics;

namespace TimeTracker3.Workspace.Exceptions
{
    /// <summary>
    ///     Thrown when an attempt is made to assign an
    ///     invalid value to a property of an object.
    /// </summary>
    public class InvalidPropertyValueWorkspaceException : WorkspaceException
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
        ///     which the value is invalid.
        /// </param>
        /// <param name="propertyValue">
        ///     The offending property value.
        /// </param>
        public InvalidPropertyValueWorkspaceException(string objectTypeName, string propertyName, object propertyValue)
        : base("Property " + Convert.ToString(propertyName) +
               " of " + Convert.ToString(objectTypeName) +
               " cannot have value '" + Convert.ToString(propertyValue) + "'")
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
