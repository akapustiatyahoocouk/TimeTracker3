using TimeTracker3.Util;

namespace TimeTracker3.Workspace
{
    /// <summary>
    ///     The "provider" of the "current" workspace.
    /// </summary>
    public sealed class CurrentWorkspaceProvider : IValueProvider<Workspace>
    {
        //////////
        //  Singleton
        private static CurrentWorkspaceProvider _Instance /*= null*/;

        private CurrentWorkspaceProvider()
        {
        }

        /// <summary>
        ///     The one and only instance of this class
        /// </summary>
        public static CurrentWorkspaceProvider Instance => _Instance ?? (_Instance = new CurrentWorkspaceProvider());

        //////////
        //  IValueProvider<Workspace> - Properties
        public Workspace Value
        {
            get => _CurrentWorkspace;
            set
            {
                if (value != _CurrentWorkspace)
                {   //  The provided value actually changes
                    ValueChangedEventArgs<Workspace> e =
                        new ValueChangedEventArgs<Workspace>(_CurrentWorkspace, value);
                    _CurrentWorkspace = value;
                    //  Notify registered event listeners
                    ValueChanged?.Invoke(this, e);
                }
            }
        }

        //////////
        //  IValueProvider<Workspace> - Events
        public event ValueChangedHandler<Workspace> ValueChanged;

        //////////
        //  Implementation
        private Workspace _CurrentWorkspace; //  can be null, else always "open"! TODO do we need to synchronize ?
    }
}
