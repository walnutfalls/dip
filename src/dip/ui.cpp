#include  "ui.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <os/browse_dialog.hpp>

dip::ui::ui(GLFWwindow *window): _window(window) {
    memset(command, 0, 1024 * sizeof(char));

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    //io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
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

    drawControls();
    drawConsole();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void dip::ui::drawControls() {
    int width, height;
    glfwGetWindowSize(_window, &width, &height);

    ImGui::SetNextWindowPos({ 0, 0 });
    ImGui::SetNextWindowSize({ width/4.f, height/2.f });

    ImGui::Begin("Controls", &my_tool_active);

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

    if (ImGui::RadioButton("Split", operation == operation::split)) {
        operation = split;
        opChanged(operation);
    }
    if (ImGui::RadioButton("Add", operation == operation::add)) {
        operation = add;
        opChanged(operation);
    }
    if (ImGui::RadioButton("Subtract", operation == operation::sub)) {
        operation = sub;
        opChanged(operation);
    }
    if (ImGui::RadioButton("Multiply", operation == operation::mul)) {
        operation = mul;
        opChanged(operation);
    }
    if (ImGui::RadioButton("Negate", operation == operation::negate)) {
        operation = negate;
        opChanged(operation);
    }
    if (ImGui::RadioButton("Log", operation == operation::log)) {
        operation = log;
        opChanged(operation);
    }
    if (ImGui::RadioButton("Gamma", operation == operation::gamma)) {
        operation = gamma;
        opChanged(operation);
    }
    if (ImGui::RadioButton("Closest Component Labels", operation == operation::label)) {
        operation = label;
        opChanged(operation);
    }

    if (operation == operation::gamma) {
        if (ImGui::DragFloat("gamma", &gamma_val, 0.01, 0, 1)) {
            opChanged(operation);
        }

        if (ImGui::DragFloat("gamma_c", &gamma_c, 0.1, -100, 100)) {
            opChanged(operation);
        }
    } else if (operation == operation::label) {
        if (ImGui::DragFloat("CCL sensitivity", &ccl_sensitivity, 0.01, 0, 1)) {
            opChanged(operation);
        }

        if (ImGui::RadioButton("4-neighbor", connectivity == connectivity::four)) {
            connectivity = four;
            opChanged(operation);
        }
        if (ImGui::RadioButton("8-neighbor", connectivity == connectivity::eight)) {
            connectivity = eight;
            opChanged(operation);
        }
        if (ImGui::RadioButton("m-neighbor", connectivity == connectivity::m)) {
            connectivity = m;
            opChanged(operation);
        }
    } else if (operation == operation::log) {
        if (ImGui::DragFloat("log_c", &log_c, 0.1, -100, 100)) {
            opChanged(operation);
        }
        if (ImGui::DragFloat("log_base", &log_base, 0.1, 1.1, 20)) {
            opChanged(operation);
        }
    }

    if (ImGui::Button("Save Image")) {
        if (const auto path = os::save_dialog()) {
            saveClicked(*path);
        }
    }

    ImGui::End();
}

void dip::ui::drawConsole() {
    int width, height;
    glfwGetWindowSize(_window, &width, &height);

    ImGui::SetNextWindowPos({ 0, height * 0.75f});
    ImGui::SetNextWindowSize({ static_cast<float>(width), height * 0.75f});


    ImGui::Begin("Console", &my_tool_active);

    if(ImGui::InputText("Input", command, 1023, ImGuiInputTextFlags_EnterReturnsTrue)) {
        const std::string cmd(command);
        commandIssued(cmd);
        command[0] = '\0';
        history[2] = history[1];
        history[1] = history[0];
        history[0] = cmd;
    }
    ImGui::SameLine();
    if (ImGui::Button("Enter")) {
        const std::string cmd(command);
        commandIssued(cmd);
        command[0] = '\0';
        history[2] = history[1];
        history[1] = history[0];
        history[0] = cmd;
    }
    ImGui::NewLine();

    fullhist = history[0] + "\n" + history[1] + "\n" + history[2];
    ImGui::Text(fullhist.c_str());

    ImGui::End();
}
