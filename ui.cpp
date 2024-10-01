#include "ui.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <os/browse_dialog.hpp>

dip::ui::ui(GLFWwindow *window) {


    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    //io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init("#version 330");
}

dip::ui::~ui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void dip::ui::draw() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();


    ImGui::Begin("Target, Source Image", &my_tool_active);

    if (ImGui::Button("Browse PPM A")) {
        if (const auto path = os::browse_dialog()) {
            ppm1 = *path;
        }
    }
    if (!ppm1.empty()) {
        ImGui::Text(ppm1.c_str());
    }

    if (ImGui::Button("Browse PPM B")) {
        if (const auto path = os::browse_dialog()) {
            ppm2 = *path;
        }
    }
    if (!ppm2.empty()) {
        ImGui::Text(ppm2.c_str());
    }

    ImGui::Checkbox("Bilinear", &bilinear);

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
