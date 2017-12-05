
namespace TheEngine
{
    class Vector3
    {
        public Vector3()
        {
            x = 0.0f;
            y = 0.0f;
            z = 0.0f;
        }

        public Vector3(float x, float y, float z)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }

        public float x;
        public float y;
        public float z;

        public static Vector3 zero
        {
            get
            {
                return new Vector3(); ;
            }
        }

        public static Vector3 operator +(Vector3 a, Vector3 b)
        {
            return new Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
        }

        public static Vector3 operator -(Vector3 a, Vector3 b)
        {
            return new Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
        }

        public static Vector3 operator -(Vector3 a)
        {
            return new Vector3(-a.x, -a.y, -a.z);
        }

        public static Vector3 operator *(Vector3 a, float value)
        {
            return new Vector3(a.x * value, a.y * value, a.z * value);
        }

        public static Vector3 operator *(float value, Vector3 a)
        {
            return new Vector3(a.x * value, a.y * value, a.z * value);
        }

        public static Vector3 operator /(Vector3 a, float value)
        {
            return new Vector3(a.x / value, a.y / value, a.z / value);
        }

        public static bool operator ==(Vector3 a, Vector3 b)
        {
            return a.Equals(b);
        }

        public static bool operator !=(Vector3 a, Vector3 b)
        {
            return !(a == b);
        }

        public override string ToString()
        {
            return string.Format("({0:F1}, {1:F1}, {2:F1})", new object[] { x, y, z });
        }

    }
}
