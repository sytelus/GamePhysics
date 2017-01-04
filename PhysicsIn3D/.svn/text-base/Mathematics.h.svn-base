/// General mathematics routines

#include <math.h>
#include <float.h>
#include <assert.h>

namespace Mathematics
{
	const float epsilon = 0.00001f;                         ///< floating point epsilon for single precision. todo: verify epsilon value and usage
	const float epsilonSquared = epsilon * epsilon;         ///< epsilon value squared

	const float pi = 3.1415926f;                            ///< pi stored at a reasonable precision for single precision floating point.

	/// test for floating point equality within [-epsilon,+epsilon]

	inline bool equal(float a, float b)
	{
		const float d = a - b;
		if (d<epsilon && d>-epsilon) 
			return true;
		else 
			return false;
	}

	/// determine the minimum floating point value

	inline float minimum(float a, float b)
	{
		if (a<b) 
			return a;
		else 
			return b;
	}

	/// determine the maximum floating point value

	inline float maximum(float a, float b)
	{
		if (a>b) 
			return a;
		else 
			return b;
	}

	/// calculate the square root of a floating point number.

	inline float sqrt(float value)
	{
		assert(value>=0);
		return (float) pow(value, 0.5f);
	}

	/// calculate the sine of a floating point angle in radians.

	inline float sin(float radians)
	{
		return (float) ::sin(radians);
	}

	/// calculate the cosine of a floating point angle in radians.

	inline float cos(float radians)
	{
		return (float) ::cos(radians);
	}

	/// calculate the tangent of a floating point angle in radians.

	inline float tan(float radians)
	{
		return (float) ::tan(radians);
	}

	/// calculate the arcsine of a floating point value. result is in radians.

	inline float asin(float value)
	{
		return (float) ::asin(value);
	}

	/// calculate the arccosine of a floating point value. result is in radians.

	inline float acos(float value)
	{
		return (float) ::acos(value);
	}

	/// calculate the arctangent of a floating point value y/x. result is in radians.

	inline float atan2(float y, float x)
	{
		return (float) ::atan2(y,x);
	}

	/// calculate the floor of a floating point value.
	/// the floor is the nearest integer strictly less than or equal to the floating point number.

	inline float floor(float value)
	{
		return (float) ::floor(value);
	}

	/// calculate the ceiling of a floating point value.
	/// the ceil is the nearest integer strictly greater than or equal to the floating point number.

	inline float ceiling(float value)
	{                     
		return (float) ::ceil(value);
	}

	/// quickly determine the sign of a floating point number.

	inline unsigned int sign(const float& v)
	{	
		return (((unsigned int&)v) & 0x80000000);
	}

	/// fast floating point absolute value.

	inline float abs(float v)
	{
		*(int *)&v &= 0x7fffffff;
		return v;
	}

	/// interpolate between interval [a,b] with t in [0,1].

	inline float lerp(float a, float b, float t)
	{
		return a + (b - a) * t;
	}

	/// snap floating point number to grid.

	inline float snap(float p, float grid)
	{
		return grid ? float( floor((p + grid*0.5f)/grid) * grid) : p;
	}
}
