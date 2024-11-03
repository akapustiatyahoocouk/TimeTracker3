using System;

namespace TimeTracker3.Db.API.Exceptions
{
    public class IncompatibleObjectsDatabaseException : DatabaseException
    {
        //////////
        //  Construction

        /// <summary>
        ///     Constructs the exception.
        /// </summary>
        /// <param name="obj1">
        ///     The object that is incompatible with obj2.
        /// </param>
        /// <param name="obj2">
        ///     The object with which obj1 is incompatible.
        /// </param>
        public IncompatibleObjectsDatabaseException(object obj1, object obj2)
            : base(Convert.ToString(obj1)  + " is not compatible with " + Convert.ToString(obj2))
        {
            Object1 = obj1;
            Object2 = obj2;
        }

        public IncompatibleObjectsDatabaseException(object obj1, object obj2, Exception cause)
            : base(Convert.ToString(obj1) + " is not compatible with " + Convert.ToString(obj2), cause)
        {
            Object1 = obj1;
            Object2 = obj2;
        }

        //////////
        //  Properties

        /// <summary>
        ///     The object that is incompatible with Object2.
        /// </summary>
        public readonly object Object1;

        /// <summary>
        ///     The object with which Object1 is incompatible.
        /// </summary>
        public readonly object Object2;
    }
}
