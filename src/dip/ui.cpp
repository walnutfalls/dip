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

    ImGui::Begin("Controls");

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

    if (ImGui::RadioButton("Split", operation == split)) {
        operation = split;
        opChanged(operation);
    }
    if (ImGui::RadioButton("Add", operation == add)) {
        operation = add;
        opChanged(operation);
    }
    if (ImGui::RadioButton("Subtract", operation == sub)) {
        operation = sub;
        opChanged(operation);
    }
    if (ImGui::RadioButton("Multiply", operation == mul)) {
        operation = mul;
        opChanged(operation);
    }
    if (ImGui::RadioButton("Negate", operation == negate)) {
        operation = negate;
        opChanged(operation);
    }
    if (ImGui::RadioButton("Log", operation == log)) {
        operation = log;
        opChanged(operation);
    }
    if (ImGui::RadioButton("Gamma", operation == gamma)) {
        operation = gamma;
        opChanged(operation);
    }
    if (ImGui::RadioButton("Closest Component Labels", operation == label)) {
        operation = label;
        opChanged(operation);
    }

    if (operation == gamma) {
        if (ImGui::DragFloat("gamma", &gamma_val, 0.01f, 0.f, 1.f)) {
            opChanged(operation);
        }

        if (ImGui::DragFloat("gamma_c", &gamma_c, 0.1f, -100.f, 100.f)) {
            opChanged(operation);
        }
    } else if (operation == label) {
        if (ImGui::DragFloat("CCL sensitivity", &ccl_sensitivity, 0.01f, 0.f, 1.f)) {
            opChanged(operation);
        }

        if (ImGui::RadioButton("4-neighbor", connectivity == four)) {
            connectivity = four;
            opChanged(operation);
        }
        if (ImGui::RadioButton("8-neighbor", connectivity == eight)) {
            connectivity = eight;
            opChanged(operation);
        }
        if (ImGui::RadioButton("m-neighbor", connectivity == m)) {
            connectivity = m;
            opChanged(operation);
        }
    } else if (operation == log) {
        if (ImGui::DragFloat("log_c", &log_c, 0.1f, -100.f, 100.f)) {
            opChanged(operation);
        }
        if (ImGui::DragFloat("log_base", &log_base, 0.1f, 1.1f, 20.f)) {
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


    static bool on = false;

    ImGui::SetNextWindowPos({ 0, on ? height * 0.75f : height - ImGui::GetFrameHeight()});
    ImGui::SetNextWindowSize({ static_cast<float>(width), height * 0.25f});

    ImGui::SetNextWindowCollapsed(!console_visible, ImGuiCond_Once);
    on = ImGui::Begin("Console");

    const float fullWidth = ImGui::GetContentRegionAvail().x;
    ImGui::SetNextItemWidth(fullWidth);

    if(ImGui::InputText("##", command, 1023, ImGuiInputTextFlags_EnterReturnsTrue)) {
        const std::string cmd(command);
        commandIssued(cmd);
        command[0] = '\0';
        write_output(cmd);
    }

    if (ImGui::IsItemHovered() || (!ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0)))
        ImGui::SetKeyboardFocusHere(-1);

    _full_history = history[0];
    for (int i = 1; i < HistoryLength; ++i) {
        _full_history += '\n';
        _full_history += history[i];
    }

    ImGui::SetNextItemWidth(fullWidth);

    ImGui::BeginChild("ScrollableText", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
    ImGui::Text(_full_history.c_str());
    ImGui::EndChild();

    ImGui::End();
}

void dip::ui::write_output(const std::string &output) {
    for (int i = HistoryLength-1; i > 1; --i) {
        history[i] = history[i-1];
    }
    history[1] = output;
}
