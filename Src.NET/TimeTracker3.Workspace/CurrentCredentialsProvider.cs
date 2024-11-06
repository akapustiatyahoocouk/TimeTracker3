using TimeTracker3.Util;

namespace TimeTracker3.Workspace
{
    /// <summary>
    ///     The "provider" of the "current" credentials.
    /// </summary>
    public sealed class CurrentCredentialsProvider : IValueProvider<Credentials>
    {
        //////////
        //  Singleton
        private static CurrentCredentialsProvider _Instance /*= null*/;

        private CurrentCredentialsProvider()
        {
        }

        /// <summary>
        ///     The one and only instance of this class
        /// </summary>
        public static CurrentCredentialsProvider Instance => _Instance ?? (_Instance = new CurrentCredentialsProvider());

        //////////
        //  IValueProvider<Workspace> - Properties
        public Credentials Value
        {
            get => _CurrentCredentials;
            set
            {
                if (!Equals(value, _CurrentCredentials))
                {   //  The provided value actually changes
                    ValueChangedEventArgs<Credentials> e =
                        new ValueChangedEventArgs<Credentials>(_CurrentCredentials, value);
                    _CurrentCredentials = value;
                    //  Notify registered event listeners
                    ValueChanged?.Invoke(this, e);
                }
            }
        }

        //////////
        //  IValueProvider<Workspace> - Events
        public event ValueChangedHandler<Credentials> ValueChanged;

        //////////
        //  Implementation
        private Credentials _CurrentCredentials; //  can be null TODO do we need to synchronize ?
    }
}
