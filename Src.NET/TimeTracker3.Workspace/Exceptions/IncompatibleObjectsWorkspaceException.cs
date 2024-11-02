using System;

namespace TimeTracker3.Workspace.Exceptions
{
    public class IncompatibleObjectsWorkspaceException : WorkspaceException
    {
        //////////
        //  Construction
        //  TODO document all

        public IncompatibleObjectsWorkspaceException(string obj1, string obj2)
            : base(obj1 + " is not compatible with " + obj2)
        {
            Object1 = obj1;
            Object2 = obj2;
        }

        public IncompatibleObjectsWorkspaceException(string obj1, string obj2, Exception cause)
            : base(obj1 + " is not compatible with " + obj2, cause)
        {
            Object1 = obj1;
            Object2 = obj2;
        }

        //////////
        //  Properties
        //  TODO document all

        public readonly string Object1;
        public readonly string Object2;
    }
}
