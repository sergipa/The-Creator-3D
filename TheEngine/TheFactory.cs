using System.Runtime.CompilerServices;

namespace TheEngine
{
    class TheFactory : TheComponent
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern void StartFactory();

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern TheGameObject Spawn();

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern void SetSpawnPosition(TheVector3 position);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern void SetRotationPosition(TheVector3 rotation);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern void SetSpawnScale(TheVector3 scale);
    }
}
