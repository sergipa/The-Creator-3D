#pragma once

template <typename T>
class Vector2
{
public:
	Vector2();
	Vector2(T X, T Y);

	T x;
	T y;
};

typedef Vector2<int>          Vector2i;
typedef Vector2<unsigned int> Vector2u;
typedef Vector2<float>        Vector2f;

template<typename T>
inline Vector2<T>::Vector2()
{
	x = 0;
	y = 0;
}

template<typename T>
inline Vector2<T>::Vector2(T X, T Y)
{
	x = X;
	y = Y;
}

template<typename T>
inline Vector2<T> operator-(const Vector2<T>& right)
{
	return Vector2<T>(-right.x, -right.y);
}

template<typename T>
inline Vector2<T>& operator+=(Vector2<T>& left, const Vector2<T>& right)
{
	left.x += right.x;
	left.y += right.y;

	return left;
}

template<typename T>
inline Vector2<T>& operator-=(Vector2<T>& left, const Vector2<T>& right)
{
	left.x -= right.x;
	left.y -= right.y;

	return left;
}

template<typename T>
inline Vector2<T> operator+(const Vector2<T>& left, const Vector2<T>& right)
{
	return Vector2<T>(left.x + right.x, left.y + right.y);
}

template<typename T>
inline Vector2<T> operator-(const Vector2<T>& left, const Vector2<T>& right)
{
	return Vector2<T>(left.x - right.x, left.y - right.y);
}

template<typename T>
inline Vector2<T> operator*(const Vector2<T>& left, T right)
{
	return Vector2<T>(left.x * right, left.y * right);
}

template<typename T>
inline Vector2<T> operator*(T left, const Vector2<T>& right)
{
	return Vector2<T>(right.x * left, right.y * left);
}

template<typename T>
inline Vector2<T>& operator*=(Vector2<T>& left, T right)
{
	left.x *= right;
	left.y *= right;

	return left;
}

template<typename T>
inline Vector2<T> operator/(const Vector2<T>& left, T right)
{
	return Vector2<T>(left.x / right, left.y / right);
}

template<typename T>
inline Vector2<T>& operator/=(Vector2<T>& left, T right)
{
	left.x /= right;
	left.y /= right;

	return left;
}

template<typename T>
inline bool operator==(const Vector2<T>& left, const Vector2<T>& right)
{
	return (left.x == right.x) && (left.y == right.y);
}

template<typename T>
inline bool operator!=(const Vector2<T>& left, const Vector2<T>& right)
{
	return (left.x != right.x) || (left.y != right.y);
}