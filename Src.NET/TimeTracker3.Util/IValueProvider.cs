using System;

namespace TimeTracker3.Util
{
    /// <summary>
    ///     Describes the details of the "provided"
    ///     value change.
    /// </summary>
    /// <typeparam name="T">
    ///     The type of the provided value.
    /// </typeparam>
    public sealed class ValueChangedEventArgs<T> : EventArgs
    {
        //////////
        //  Construction

        /// <summary>
        ///     Constructs the value change event arguments.
        /// </summary>
        /// <param name="oldValue">
        ///     The value before the change, can be null.
        /// </param>
        /// <param name="newValue">
        ///     The value after the change, can be null.
        /// </param>
        public ValueChangedEventArgs(T oldValue, T newValue)
        {
            OldValue  = oldValue;
            NewValue = newValue;
        }

        //////////
        //  Properties
        public readonly T OldValue;
        public readonly T NewValue;
    }

    /// <summary>
    ///     The type of the "value change" event.
    /// </summary>
    /// <typeparam name="T">
    ///     The type of the provided value.
    /// </typeparam>
    /// <param name="sender">
    ///     The sender of the event.
    /// </param>
    /// <param name="e">
    ///     The descriptor of the value change details.
    /// </param>
    public delegate void ValueChangedHandler<T>(object sender, ValueChangedEventArgs<T> e);

    /// <summary>
    ///     An abstract interface to an agent that can
    ///     "provide" value and notify registered
    ///     listeners when the value it "provides"
    ///     changes.
    /// </summary>
    /// <typeparam name="T">
    ///     The type of the provided value.
    /// </typeparam>
    public interface IValueProvider<T>
    {
        //////////
        //  Properties

        /// <summary>
        ///     The value currently "provided" by
        ///     this provider.
        /// </summary>
        T Credentials { get; }

        /// <summary>
        ///     Raised by this value provider when the
        ///     value it provides change.
        /// </summary>
        event ValueChangedHandler<T> ValueChanged;
    }
}
