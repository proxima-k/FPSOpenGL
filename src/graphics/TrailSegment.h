#pragma once

class TrailSegment
{
public:
	float x;
	float y;
	float z;
	float yOrigin;

	TrailSegment(float xVal, float yVal, float zVal, float yOrigin)
		: x(xVal), y(yVal), z(zVal), yOrigin(yOrigin) {}

	bool operator==(const TrailSegment& other) const 
	{
		return (x == other.x && 
				y == other.y && 
				z == other.z && 
				yOrigin == other.yOrigin);
	}
};