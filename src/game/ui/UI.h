#include <glew/glew.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

class UI
{
public:
    static void Init(GLFWwindow* window);
    static void Begin();
    static void End();
    static void Render();
    static void Shutdown();

private:
    static GLuint LoadTextureFromFile(const char* filename);
};
