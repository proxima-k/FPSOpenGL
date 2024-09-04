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
};