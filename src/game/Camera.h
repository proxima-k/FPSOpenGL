#pragma once
#include<vector>
#include<glm/glm.hpp>
#include<GLFW/glfw3.h>

#include"Transform.h"

class Camera 
{
public:
	void update();
	void input();

	Transform transform;
};