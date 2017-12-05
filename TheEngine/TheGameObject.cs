using System.Runtime.CompilerServices;
using System;

namespace TheEngine
{
    public class TheGameObject
    {
        public TheGameObject()
        {
            CreateNewGameObject(this);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern void CreateNewGameObject(TheGameObject gameObject);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public void SetName(string value)
        {
            if (this == null) throw new NullReferenceException();
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern string GetName();

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern void SetActive(bool value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool IsActive();

        public static extern TheGameObject Self
        {
            [MethodImpl(MethodImplOptions.InternalCall)]
            get;
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern Component GetComponent(Type type);
    }
}
