namespace TimeTracker3.Db.API
{
    /// <summary>
    ///     Standard (predefined) database object types,
    ///     to use in DB API implementations.
    /// </summary>
    public static class DatabaseObjectTypes
    {
        //////////
        //  Properties TODO document
        public static readonly DatabaseObjectType User =
            new DatabaseObjectType("User");

        public static readonly DatabaseObjectType Account =
            new DatabaseObjectType("Account");

        public static readonly DatabaseObjectType ActivityType =
            new DatabaseObjectType("ActivityType");

        public static readonly DatabaseObjectType PublicActivity =
            new DatabaseObjectType("PublicActivity");

        public static readonly DatabaseObjectType PublicTask =
            new DatabaseObjectType("PublicTask");

        public static readonly DatabaseObjectType PrivateActivity =
            new DatabaseObjectType("PrivateActivity");

        public static readonly DatabaseObjectType PrivateTask =
            new DatabaseObjectType("PrivateTask");

        public static readonly DatabaseObjectType Work =
            new DatabaseObjectType("Work");

        public static readonly DatabaseObjectType Event =
            new DatabaseObjectType("Event");

        public static readonly DatabaseObjectType Beneficiary =
            new DatabaseObjectType("Beneficiary");

        public static readonly DatabaseObjectType Project =
            new DatabaseObjectType("Project");

        public static readonly DatabaseObjectType WorkStream =
            new DatabaseObjectType("WorkStream");
    }
}
