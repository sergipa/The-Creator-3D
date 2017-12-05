
namespace TheEngine
{
    class Quaternion
    {
        public Quaternion()
        {
            x = 0.0f;
            y = 0.0f;
            z = 0.0f;
            w = 0.0f;
        }

        public Quaternion(float x, float y, float z, float w)
        {
            this.x = x;
            this.y = y;
            this.z = z;
            this.w = w;
        }

        public float x;
        public float y;
        public float z;
        public float w;

        public static Quaternion identity
        {
            get
            {
                return new Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
            }
        }
    }
}
