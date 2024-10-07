#include  "ui.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>


#include <os/browse_dialog.hpp>

dip::ui::ui(GLFWwindow *window): _window(window) {
    //memset(command, 0, 1024 * sizeof(char));

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
    int width, height;
    glfwGetWindowSize(_window, &width, &height);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    drawControls(width, height);
    drawControls2(width, height);

    drawConsole(width, height);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void dip::ui::drawControls(int width, int height) {
    ImGui::SetNextWindowPos({0, 0});
    ImGui::SetNextWindowSize({width / 4.f, height / 2.f});

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
    ImGui::Checkbox("Operands/Output toggle", &split_view);


    if (ImGui::RadioButton("Add", operation == add)) {
        operation = add;
        split_view = false;
        opChanged(operation);
    }
    if (ImGui::RadioButton("Subtract", operation == sub)) {
        operation = sub;
        split_view = false;
        opChanged(operation);
    }
    if (ImGui::RadioButton("Multiply", operation == mul)) {
        operation = mul;
        split_view = false;
        opChanged(operation);
    }
    if (ImGui::RadioButton("Negate", operation == negate)) {
        operation = negate;
        split_view = false;
        opChanged(operation);
    }
    if (ImGui::RadioButton("Log", operation == log)) {
        operation = log;
        split_view = false;
        opChanged(operation);
    }
    if (ImGui::RadioButton("Gamma", operation == gamma)) {
        operation = gamma;
        split_view = false;
        opChanged(operation);
    }
    if (ImGui::RadioButton("Closest Component Labels", operation == label)) {
        operation = label;
        split_view = false;
        opChanged(operation);
    }

    if (operation == gamma) {
        if (ImGui::DragFloat("gamma", &gamma_val, 0.01f, 0.f, 1.f)) {
            split_view = false;
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

void dip::ui::drawControls2(const int width, const int height) {
    ImGui::SetNextWindowPos({(0.67f)* static_cast<float>(width), 0});
    ImGui::SetNextWindowSize({ 0.33f * static_cast<float>(width), static_cast<float>(height)*0.75f });

    ImGui::Begin("PP2");
    ImGui::BeginTabBar("tabs");

    if (ImGui::BeginTabItem("Histograms")) {
        drawHistograms();
        ImGui::EndTabItem();
    }

    if (ImGui::BeginTabItem("Filter")) {
        drawFilter();
        ImGui::EndTabItem();
    }

    ImGui::EndTabBar();
    ImGui::End();
}

void dip::ui::drawConsole(int width, int height) {
    static bool on = false;

    ImGui::SetNextWindowPos({0, on ? height * 0.75f : height - ImGui::GetFrameHeight()});
    ImGui::SetNextWindowSize({static_cast<float>(width), height * 0.25f});

    ImGui::SetNextWindowCollapsed(!console_visible, ImGuiCond_Once);
    on = ImGui::Begin("Console");

    const float fullWidth = ImGui::GetContentRegionAvail().x;
    ImGui::SetNextItemWidth(fullWidth);

    if (ImGui::InputText("##", command, 1024, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackAlways,
                         [](ImGuiInputTextCallbackData *data) -> int {
                             const auto self = static_cast<dip::ui *>(data->UserData);

                             if (self->_dirty) {
                                 strcpy(data->Buf, self->command);
                                 data->BufDirty = true;
                                 data->BufTextLen = strlen(data->Buf);
                                 self->_dirty = false;
                                 data->CursorPos = data->BufTextLen;
                             }

                             return 0;
                         }, this)) {
        commandIssued(command);
        command[0] = '\0';
        write_output(command);
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

void dip::ui::drawHistograms() {
    if (ImGui::Button("Equalize A")) {
        histogram_equalize(a);
    }
    ImGui::SameLine();
    if (ImGui::Button("Equalize B")) {
        histogram_equalize(b);
    }
    ImGui::SameLine();
    if (ImGui::Button("Equalize Output")) {
        histogram_equalize(output);
    }

    ImGui::SameLine();
    if (ImGui::Button("Histogram Match A to B")) {
        split_view = false;
        histogram_equalize(match);
    }

    ImGui::Text("A");
    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(1.0f, 0.f, 0.0f, 1.0f));
    ImGui::PlotHistogram("R", histogramA[0], 256, 0, nullptr, FLT_MAX, FLT_MAX, ImVec2(0, 64));
    ImGui::PopStyleColor();

    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.0f, 1.f, 0.0f, 1.0f));
    ImGui::PlotHistogram("G", histogramA[1], 256, 0, nullptr, FLT_MAX, FLT_MAX, ImVec2(0, 64));
    ImGui::PopStyleColor();

    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.f, 0.f, 1.f, 1.0f));
    ImGui::PlotHistogram("B", histogramA[2], 256, 0, nullptr, FLT_MAX, FLT_MAX, ImVec2(0, 64));
    ImGui::PopStyleColor();

    ImGui::Text("B");
    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(1.0f, 0.f, 0.0f, 1.0f));
    ImGui::PlotHistogram("R", histogramB[0], 256, 0, nullptr, FLT_MAX, FLT_MAX, ImVec2(0, 64));
    ImGui::PopStyleColor();
    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.0f, 1.f, 0.0f, 1.0f));
    ImGui::PlotHistogram("G", histogramB[1], 256, 0, nullptr, FLT_MAX, FLT_MAX, ImVec2(0, 64));
    ImGui::PopStyleColor();
    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.f, 0.f, 1.f, 1.0f));
    ImGui::PlotHistogram("B", histogramB[2], 256, 0, nullptr, FLT_MAX, FLT_MAX, ImVec2(0, 64));
    ImGui::PopStyleColor();

    ImGui::Text("Output");
    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(1.0f, 0.f, 0.0f, 1.0f));
    ImGui::PlotHistogram("R", histogramOut[0], 256, 0, nullptr, FLT_MAX, FLT_MAX, ImVec2(0, 64));
    ImGui::PopStyleColor();
    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.0f, 1.f, 0.0f, 1.0f));
    ImGui::PlotHistogram("G", histogramOut[1], 256, 0, nullptr, FLT_MAX, FLT_MAX, ImVec2(0, 64));
    ImGui::PopStyleColor();
    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.f, 0.f, 1.f, 1.0f));
    ImGui::PlotHistogram("B", histogramOut[2], 256, 0, nullptr, FLT_MAX, FLT_MAX, ImVec2(0, 64));
    ImGui::PopStyleColor();
}

void dip::ui::drawFilter() {
    if (ImGui::InputInt("Kernel Size", &kernel_size, 2, 8)) {
        kernel_size = std::max(kernel_size, 1);
    }
}


void dip::ui::set_command(const std::string &cmd) {
    strncpy_s(command, cmd.c_str(), cmd.size());
    _dirty = true;
}

void dip::ui::write_output(const std::string &output) {
    for (int i = HistoryLength - 1; i > 1; --i) {
        history[i] = history[i - 1];
    }
    history[1] = output;
}
