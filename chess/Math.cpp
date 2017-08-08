#include "Math.h"

float math::clamp(float value, float min, float max)
{
	if (value < min) {
		return min;
	}
	else if (value > max) {
		return max;
	}
	else {
		return value;
	}
}

float math::to_radians(float degrees)
{
	return degrees * PI / 180.0f;
}

float math::to_degrees(float radians)
{
	return radians * 180.0f / PI;
}

float math::interpolate(const std::vector<std::pair<float, float>>& curve, float x)
{
	if (x > curve.back().first) {
		return curve.back().second;
	}
	if (x < curve[0].first) {
		return curve[0].second;
	}

	std::vector<std::pair<float, float>>::const_iterator left, right;

	left = std::lower_bound(curve.begin(), curve.end(), std::make_pair(x, -INFINITY));

	if (left == curve.begin()) {
		return left->second;
	}

	right = left;
	--right;
	return right->second + (left->second - right->second) * (x - right->first) / (left->first - right->first);
}


float math::length(const vec2& v)
{
	return sqrtf(v.x * v.x + v.y * v.y);
}

float math::dot(const vec2 & a, const vec2 & b)
{
	return a.x * b.x + a.y * b.y;
}

vec2 math::normalize(const vec2 & v)
{
	return v / math::length(v);
}

vec2 math::rotate(const vec2 & v, float angle)
{
	vec2 result;
	result.x = v.x * cosf(angle) - v.y * sinf(angle);
	result.y = v.x * sinf(angle) + v.y * cosf(angle);
	return result;
}

float math::scalar_projection(const vec2 & a, const vec2 & b)
{
	float length = math::length(a);
	if (length == 0.0f) {
		return 0.0f;
	}
	else {
		return dot(a, b) / length;
	}
}

vec2 math::vector_projection(const vec2 & a, const vec2 & b)
{
	float length = math::length(a);
	if (length == 0.0f) {
		return vec2();
	}
	else {
		return dot(a, b) * a / (length * length);
	}
}
