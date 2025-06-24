// GLFW + OpenGL3 + ImGui template application with a custom title bar and drag functionality.

#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <windows.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"

static bool is_dragging = false;
static POINT drag_offset;

static void glfw_error_callback(int error, const char *description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

int main(int, char **)
{
    // Hide console window
    HWND consoleWindow = GetConsoleWindow();
    if (consoleWindow)
    {
        ShowWindow(consoleWindow, SW_HIDE);
    }

    // Set the console title
    SetConsoleTitleA("WINDOW TITLE");

    glfwSetErrorCallback([](int error, const char *desc)
                         { fprintf(stderr, "GLFW Error: %s\n", desc); });
    if (!glfwInit())
        return 1;

    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);

    // Center the window on the screen
    int xpos = (mode->width - 800) / 2;
    int ypos = (mode->height - 600) / 2;

    // Create a window
    GLFWwindow *window = glfwCreateWindow(500, 300, "App", nullptr, nullptr);
    if (!window)
        return 1;

    glfwSetWindowPos(window, xpos, ypos);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        return 1;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    ImGui::StyleColorsDark();
    io.IniFilename = NULL;
    io.LogFilename = NULL;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    int refresh_rate = 0; // Default refresh rate
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        static double last_mx = 0, last_my = 0;

        // Dragging functionality
        if (!is_dragging && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        {
            // check if we’re in the title region…
            double mx, my;
            glfwGetCursorPos(window, &mx, &my);
            if (my < 50.0) // Height from top
            {
                // get window pos in screen coords
                int wx, wy;
                glfwGetWindowPos(window, &wx, &wy);

                // get cursor pos in screen coords
                POINT p;
                GetCursorPos(&p);

                // store offset between cursor and window origin
                drag_offset.x = p.x - wx;
                drag_offset.y = p.y - wy;
                is_dragging = true;
            }
        }
        if (is_dragging && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        {
            POINT p;
            GetCursorPos(&p); // screen coords
            int new_x = p.x - drag_offset.x;
            int new_y = p.y - drag_offset.y;
            glfwSetWindowPos(window, new_x, new_y);
        }
        else
        {
            is_dragging = false;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        io.DisplaySize = ImVec2((float)display_w, (float)display_h);
        ImVec2 win_size = io.DisplaySize;

        ImGui::Begin("UI", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground);

        ImGui::SetWindowPos(ImVec2(0, 0));
        ImGui::SetWindowSize(win_size);
        //----------------------------------
        // Main Drawing Area
        //----------------------------------

        ImGui::Text("Your App");
        ImGui::Text("This is a custom window with a title bar and drag functionality.");

        ImGui::SetCursorPosX(win_size.x - 30);
        ImGui::SetCursorPosY(10);
        if (ImGui::Button("X", ImVec2(20, 20)))
        {
            glfwSetWindowShouldClose(window, true);
        }

        //----------------------------------
        //----------------------------------
        //----------------------------------
        ImGui::End();

        ImGui::Render();
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
