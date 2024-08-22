#include "PlayerUI.h"

void PlayerUI::update(GLFWwindow* window, float deltaTime) 
{
	if(textureID == 0)
	{
		loadImage("../res/sprites/placeholder.jpg");
	}

	ImGui::NewFrame();

	// get current draw list
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	// define the position and size for the image
	ImVec2 image_pos = ImVec2(100, 100);
	ImVec2 image_size = ImVec2(200, 200);

	// draw the image
	draw_list->AddImage((void*)textureID, image_pos, ImVec2(image_pos.x + image_size.x, image_pos.y + image_size.y));

	// end ImGui frame and render
	ImGui::Render();
}