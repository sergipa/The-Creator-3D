using System;
using System.Runtime.CompilerServices;

namespace TheEngine
{
    public class TheComponent
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern TheComponent GetComponent(Type type);

        public T GetComponent<T>() where T : TheComponent
        {
            return GetComponent(typeof(T)) as T;
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern TheComponent AddComponent(Type type);

        public T AddComponent<T>() where T : TheComponent
        {
            return AddComponent(typeof(T)) as T;
        }
    }
}
