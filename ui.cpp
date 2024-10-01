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
            ppm1Changed(ppm1);
            opChanged(operation);
        }
    }
    if (!ppm1.empty()) {
        ImGui::Text(ppm1.c_str());
    }

    if (ImGui::Button("Browse PPM B")) {
        if (const auto path = os::browse_dialog()) {
            ppm2 = *path;
            ppm2Changed(ppm2);
            opChanged(operation);
        }
    }
    if (!ppm2.empty()) {
        ImGui::Text(ppm2.c_str());
    }

    ImGui::Checkbox("Bilinear", &bilinear);

    if (ImGui::RadioButton("Split", operation==operation::split)) {
        operation = split;
        opChanged(operation);
    }
    if (ImGui::RadioButton("Add", operation==operation::add)) {
        operation = add;
        opChanged(operation);
    }
    if (ImGui::RadioButton("Subtract", operation==operation::sub)) {
        operation = sub;
        opChanged(operation);
    }
    if (ImGui::RadioButton("Multiply", operation==operation::mul)) {
        operation = mul;
        opChanged(operation);
    }
    if (ImGui::RadioButton("Negate", operation==operation::negate)) {
        operation = negate;
        opChanged(operation);
    }
    if (ImGui::RadioButton("Log", operation==operation::log)) {
        operation = log;
        opChanged(operation);
    }
    if (ImGui::RadioButton("Gamma", operation==operation::gamma)) {
        operation = gamma;
        opChanged(operation);
    }

    if (operation==operation::gamma) {
        if (ImGui::DragFloat("gamma", &gamma_val, 0.01, 0, 1)) {
            opChanged(operation);
        }

        if (ImGui::DragFloat("gamma_c", &gamma_c, 0.1, -100, 100)) {
            opChanged(operation);
        }
    } else {
        if (ImGui::DragFloat("log_c", &log_c, 0.1, -100, 100)) {
            opChanged(operation);
        }
    }
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
