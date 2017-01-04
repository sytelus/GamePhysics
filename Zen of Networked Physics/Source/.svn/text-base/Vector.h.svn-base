namespace Mathematics
{
    /// A vector in 3-space.
    /// Typically used to represent points and vectors in homogenous coordinates for
    /// use with 4x4 matrices. This is less than ideal and in the future we intend
    /// to templatize this class, optimize it with template metaprogramming and
    /// offer a range of pre-fab vector classes Vector<2>, Vector<3>, Vector<4> etc.

    class Vector
    {
    public:

        /// default constructor.
        /// does nothing for speed.

        Vector() {}

        /// construct vector from x,y,z components.

        Vector(float x, float y, float z)
        {
            this->x = x;
            this->y = y;
            this->z = z;
        }

        /// set vector to zero.

        void zero()
        {
            x = 0;
            y = 0;
            z = 0;
        }

        /// negate vector.

	    void negate()
	    {
		    x = -x;
		    y = -y;
		    z = -z;
	    }

        /// add another vector to this vector.

        void add(const Vector &vector)
        {
            x += vector.x;
            y += vector.y;
            z += vector.z;
        }

        /// subtract another vector from this vector.

        void subtract(const Vector &vector)
        {
            x -= vector.x;
            y -= vector.y;
            z -= vector.z;
        }

        /// multiply this vector by a scalar.

        void multiply(float scalar)
        {
            x *= scalar;
            y *= scalar;
            z *= scalar;
        }

        /// divide this vector by a scalar.

	    void divide(float scalar)
	    {
		    assert(scalar!=0);
		    const float inv = 1.0f / scalar;
		    x *= inv;
		    y *= inv;
		    z *= inv;
	    }

        /// calculate dot product of this vector with another vector.

        float dot(const Vector &vector) const
        {
            return x * vector.x + y * vector.y + z * vector.z;
        }

        /// calculate cross product of this vector with another vector.

	    Vector cross(const Vector &vector) const
        {
            return Vector(y * vector.z - z * vector.y, z * vector.x - x * vector.z, x * vector.y - y * vector.x);
        }

        /// calculate cross product of this vector with another vector, store result in parameter.

	    void cross(const Vector &vector, Vector &result) const
        {
            result.x = y * vector.z - z * vector.y;
            result.y = z * vector.x - x * vector.z;
            result.z = x * vector.y - y * vector.x;
        }

        /// calculate length of vector squared

        float lengthSquared() const
        {
            return x*x + y*y + z*z;
        }

        /// calculate length of vector.

        float length() const
        {
		    return sqrt(x*x + y*y + z*z);
        }

        /// normalize vector and return reference to normalized self.

        Vector& normalize()
        {
            const float magnitude = sqrt(x*x + y*y + z*z);
            if (magnitude>epsilon)
            {
                const float scale = 1.0f / magnitude;
                x *= scale;
                y *= scale;
                z *= scale;
            }
		    return *this;
        }

        /// return unit length vector

        Vector unit() const
        {
            Vector vector(*this);
            vector.normalize();
            return vector;
        }

        /// test if vector is normalized.

	    bool normalized() const
	    {
		    return equal(length(),1);
	    }

        /// equals operator

	    bool operator ==(const Vector &other) const
	    {
		    if (equal(x,other.x) && equal(y,other.y) && equal(z,other.z)) 
				return true;
		    else 
				return false;
	    }

        /// not equals operator

	    bool operator !=(const Vector &other) const
	    {
		    return !(*this==other);
	    }

        /// element access

        float& operator [](int i)
        {
            assert(i>=0);
            assert(i<=2);
            return *(&x+i);
        }

        /// element access (const)

        const float& operator [](int i) const
        {
            assert(i>=0);
            assert(i<=2);
            return *(&x+i);
        }

	    friend inline Vector operator-(const Vector &a);
	    friend inline Vector operator+(const Vector &a, const Vector &b);
	    friend inline Vector operator-(const Vector &a, const Vector &b);
	    friend inline Vector operator*(const Vector &a, const Vector &b);
	    friend inline Vector& operator+=(Vector &a, const Vector &b);
	    friend inline Vector& operator-=(Vector &a, const Vector &b);
	    friend inline Vector& operator*=(Vector &a, const Vector &b);

	    friend inline Vector operator*(const Vector &a, float s);
	    friend inline Vector operator/(const Vector &a, float s);
	    friend inline Vector& operator*=(Vector &a, float s);
	    friend inline Vector& operator/=(Vector &a, float s);
	    friend inline Vector operator*(float s, const Vector &a);
	    friend inline Vector& operator*=(float s, Vector &a);

        float x;        ///< x component of vector
        float y;        ///< y component of vector
        float z;        ///< z component of vector
    };


    inline Vector operator-(const Vector &a)
    {
	    return Vector(-a.x, -a.y, -a.z);
    }

    inline Vector operator+(const Vector &a, const Vector &b)
    {
	    return Vector(a.x+b.x, a.y+b.y, a.z+b.z);
    }

    inline Vector operator-(const Vector &a, const Vector &b)
    {
	    return Vector(a.x-b.x, a.y-b.y, a.z-b.z);
    }

    inline Vector operator*(const Vector &a, const Vector &b)
    {
	    return Vector(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
    }

    inline Vector& operator+=(Vector &a, const Vector &b)
    {
	    a.x += b.x;
	    a.y += b.y;
	    a.z += b.z;
	    return a;
    }

    inline Vector& operator-=(Vector &a, const Vector &b)
    {
	    a.x -= b.x;
	    a.y -= b.y;
	    a.z -= b.z;
	    return a;
    }

    inline Vector& operator*=(Vector &a, const Vector &b)
    {
        const float cx = a.y * b.z - a.z * b.y;
        const float cy = a.z * b.x - a.x * b.z;
        const float cz = a.x * b.y - a.y * b.x;
	    a.x = cx;
	    a.y = cy;
	    a.z = cz;
	    return a;
    }

    inline Vector operator*(const Vector &a, float s)
    {
	    return Vector(a.x*s, a.y*s, a.z*s);
    }

    inline Vector operator/(const Vector &a, float s)
    {
	    assert(s!=0);
	    return Vector(a.x/s, a.y/s, a.z/s);
    }

    inline Vector& operator*=(Vector &a, float s)
    {
	    a.x *= s;
	    a.y *= s;
	    a.z *= s;
	    return a;
    }

    inline Vector& operator/=(Vector &a, float s)
    {
	    assert(s!=0);
	    a.x /= s;
	    a.y /= s;
	    a.z /= s;
	    return a;
    }

    inline Vector operator*(float s, const Vector &a)
    {
	    return Vector(a.x*s, a.y*s, a.z*s);
    }

    inline Vector& operator*=(float s, Vector &a)
    {
	    a.x *= s;
	    a.y *= s;
	    a.z *= s;
	    return a;
    }
}
