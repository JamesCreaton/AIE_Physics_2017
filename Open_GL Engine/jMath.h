//
//
//#ifndef J_MATH_H
//#define J_MATH_H
//
//#include <math.h>
//
///// This function is used to ensure that a floating point number is not a NaN or infinity.
//inline bool jMathIsValid(float x)
//{
//	int ix = *reinterpret_cast<int*>(&x);
//	return (ix & 0x7f800000) != 0x7f800000;
//}
//
///// This is a approximate yet fast inverse square-root.
//inline float jMathInvSqrt(float x)
//{
//	union
//	{
//		float x;
//		int i;
//	} convert;
//
//	convert.x = x;
//	float xhalf = 0.5f * x;
//	convert.i = 0x5f3759df - (convert.i >> 1);
//	x = convert.x;
//	x = x * (1.5f - xhalf * x * x);
//	return x;
//}
//
///// Perform the dot product on two vectors.
//inline float Vector2Dot(const Vector2& a, const Vector2& b)
//{
//	return a.x * b.x + a.y * b.y;
//}
//
///// Perform the cross product on a vector and a scalar. In 2D this produces
///// a vector.
//inline Vector2 Vector2Cross(const Vector2& a, float s)
//{
//	return Vector2(s * a.y, -s * a.x);
//}
//
///// Add two vectors component-wise.
//inline Vector2 operator + (const Vector2& a, const Vector2& b)
//{
//	return Vector2(a.x + b.x, a.y + b.y);
//}
//
///// Subtract two vectors component-wise.
//inline Vector2 operator - (const Vector2& a, const Vector2& b)
//{
//	return Vector2(a.x - b.x, a.y - b.y);
//}
//
//inline Vector2 operator * (float s, const Vector2& a)
//{
//	return Vector2(s * a.x, s * a.y);
//}
//
//inline bool operator == (const Vector2& a, const Vector2& b)
//{
//	return a.x == b.x && a.y == b.y;
//}
//
//inline bool operator != (const Vector2& a, const Vector2& b)
//{
//	return a.x != b.x || a.y != b.y;
//}
//
//inline float Vector2Distance(const Vector2& a, const Vector2& b)
//{
//	Vector2 c = a - b;
//	return c.Length();
//}
//
//inline float  Vector2DistanceSquared(const Vector2& a, const Vector2& b)
//{
//	Vector2 c = a - b;
//	return Vector2Dot(c, c);
//}
//
//#define	jMathSqrt(x)	sqrtf(x)
//#define	jMathAtan2(y, x)	atan2f(y, x)
//#define jMath_pi			3.14159265359f
//#define FLT_EPSILON			1.192092896e-07F      // smallest such that 1.0+FLT_EPSILON != 1.0
//
//struct Vector2
//{
//	/// Default constructor does nothing (for performance).
//	Vector2() {}
//
//	/// Construct using coordinates.
//	Vector2(float xIn, float yIn) : x(xIn), y(yIn) {}
//
//	/// Set this vector to all zeros.
//	void SetZero() { x = 0.0f; y = 0.0f; }
//
//	/// Set this vector to some specified coordinates.
//	void Set(float x_, float y_) { x = x_; y = y_; }
//
//	/// Negate this vector.
//	Vector2 operator -() const { Vector2 v; v.Set(-x, -y); return v; }
//
//	/// Read from and indexed element.
//	float operator () (int i) const
//	{
//		return (&x)[i];
//	}
//
//	/// Write to an indexed element.
//	float& operator () (int i)
//	{
//		return (&x)[i];
//	}
//
//	/// Add a vector to this vector.
//	void operator += (const Vector2& v)
//	{
//		x += v.x; y += v.y;
//	}
//
//	/// Subtract a vector from this vector.
//	void operator -= (const Vector2& v)
//	{
//		x -= v.x; y -= v.y;
//	}
//
//	/// Multiply this vector by a scalar.
//	void operator *= (float a)
//	{
//		x *= a; y *= a;
//	}
//
//	/// Get the length of this vector (the norm).
//	float Length() const
//	{
//		return jMathSqrt(x * x + y * y);
//	}
//
//	/// Get the length squared. For performance, use this instead of
//	/// b2Vec2::Length (if possible).
//	float LengthSquared() const
//	{
//		return x * x + y * y;
//	}
//
//	/// Convert this vector into a unit vector. Returns the length.
//	float Normalize()
//	{
//		float length = Length();
//		if (length < FLT_EPSILON)
//		{
//			return 0.0f;
//		}
//		float invLength = 1.0f / length;
//		x *= invLength;
//		y *= invLength;
//
//		return length;
//	}
//
//	/// Does this vector contain finite coordinates?
//	bool IsValid() const
//	{
//		return jMathIsValid(x) && jMathIsValid(y);
//	}
//
//	/// Get the skew vector such that dot(skew_vec, other) == cross(vec, other)
//	Vector2 Skew() const
//	{
//		return Vector2(-y, x);
//	}
//
//	float x, y;
//};
//
//
//#endif