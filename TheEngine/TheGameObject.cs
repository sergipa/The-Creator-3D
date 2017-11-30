using System.Runtime.CompilerServices;

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
        public extern void SetName(string value);

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
    }
}
