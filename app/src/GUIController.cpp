//
// Created by radisa on 11.10.25..
//

#include <engine/graphics/GraphicsController.hpp>
#include <engine/platform/PlatformController.hpp>

#include "GUIController.hpp"

#include <imgui.h>

namespace app {

    std::string_view GUIController::name() const {
        return "app::GuiController";
    }

    void GUIController::poll_events() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        if (platform->key(engine::platform::KeyId::KEY_F2).state() == engine::platform::Key::State::JustPressed) {
            set_enable(!is_enabled());
        }
    }

    void GUIController::initialize() {
        set_enable(false);
    }

    void GUIController::draw() {
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
        graphics->begin_gui();

        ImGui::Begin("Main GUI");
        ImGui::End();
        graphics->end_gui();
    }
} // app
