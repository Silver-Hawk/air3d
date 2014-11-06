/*
	Basic Vec2 functions wrapped in bvec2 object
*/
#ifndef __BVEC2_H__
#define __BVEC2_H__

#include <cmath>


template <class T>
class bvec2 {
public:
	T x, y;
	
	bvec2() :x(0), y(0) {}
	bvec2(T x, T y) : x(x), y(y) {}
	bvec2(const bvec2& v) : x(v.x), y(v.y) {}
	
	bvec2& operator=(const bvec2& v) {
		x = v.x;
		y = v.y;
		return *this;
	}
	
	bvec2 operator+(bvec2& v) {
		return bvec2(x + v.x, y + v.y);
	}
	bvec2 operator-(bvec2& v) {
		return bvec2(x - v.x, y - v.y);
	}
	
	bvec2& operator+=(bvec2& v) {
		x += v.x;
		y += v.y;
		return *this;
	}
	bvec2& operator-=(bvec2& v) {
		x -= v.x;
		y -= v.y;
		return *this;
	}
	
	bvec2 operator+(double s) {
		return bvec2(x + s, y + s);
	}
	bvec2 operator-(double s) {
		return bvec2(x - s, y - s);
	}
	bvec2 operator*(double s) {
		return bvec2(x * s, y * s);
	}
	bvec2 operator/(double s) {
		return bvec2(x / s, y / s);
	}

	bvec2 operator+(float s) {
		return bvec2(x + s, y + s);
	}
	bvec2 operator-(float s) {
		return bvec2(x - s, y - s);
	}
	bvec2 operator*(float s) {
		return bvec2(x * s, y * s);
	}
	bvec2 operator/(float s) {
		return bvec2(x / s, y / s);
	}
	
	
	bvec2& operator+=(double s) {
		x += s;
		y += s;
		return *this;
	}
	bvec2& operator-=(double s) {
		x -= s;
		y -= s;
		return *this;
	}
	bvec2& operator*=(double s) {
		x *= s;
		y *= s;
		return *this;
	}
	bvec2& operator/=(double s) {
		x /= s;
		y /= s;
		return *this;
	}
	
	void set(T x, T y) {
		this->x = x;
		this->y = y;
	}
	
	void rotate(double deg) {
		double theta = deg / 180.0 * M_PI;
		double c = cos(theta);
		double s = sin(theta);
		double tx = x * c - y * s;
		double ty = x * s + y * c;
		x = tx;
		y = ty;
	}
	
	bvec2& normalize() {
		if (length() == 0) return *this;
		*this *= (1.0 / length());
		return *this;
	}
	
	float dist(bvec2 v) const {
		bvec2 d(v.x - x, v.y - y);
		return d.length();
	}
	float length() const {
		return std::sqrt(x * x + y * y);
	}
	void truncate(double length) {
		double angle = atan2f(y, x);
		x = length * cos(angle);
		y = length * sin(angle);
	}
	
	bvec2 ortho() const {
		return bvec2(y, -x);
	}
	
	static float dot(bvec2 v1, bvec2 v2) {
		return v1.x * v2.x + v1.y * v2.y;
	}
	static float cross(bvec2 v1, bvec2 v2) {
		return (v1.x * v2.y) - (v1.y * v2.x);
	}
	
};

typedef bvec2<float> bvec2f;
typedef bvec2<double> bvec2d;


#endif