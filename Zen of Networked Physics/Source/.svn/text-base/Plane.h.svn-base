/// Plane class.
/// Represents a plane using a normal and a plane constant.

struct Plane
{
	Vector normal;          ///< the plane normal.
	float constant;         ///< the plane constant relative to the plane normal.

    /// Default constructor.
    /// normal is zero, constant is zero.

    Plane()
    {
        normal.zero();
        constant = 0;
    }

    /// Create a plane given a normal and a point on the plane.

    Plane(const Vector &normal, const Vector &point)
    {
        this->normal = normal;
        this->constant = normal.dot(point);
    }

    /// Create a plane given a normal and a plane constant.

    Plane(const Vector &normal, float constant)
    {
        this->normal = normal;
        this->constant = constant;
    }

    /// Normalize the plane normal and adjust the plane constant to match.

	void normalize()
	{
		const float inverseLength = 1.0f / normal.length();
		normal *= inverseLength;
		constant *= inverseLength;
	}

    /// Clip a point against the plane.
    /// @param distance the minimum allowable distance between the point and plane.

    void clip(Vector &point, float distance = 0.0f)
    {
        const float d = (point.dot(normal) - constant) - distance;

        if (d<0)
            point -= normal * d;
    }
};
