//
// Created by radisa on 11.10.25..
//

#include <engine/graphics/GraphicsController.hpp>
#include <engine/platform/PlatformController.hpp>

#include "GUIController.hpp"

#include <MainController.hpp>
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
        auto main     = engine::core::Controller::get<MainController>();
        graphics->begin_gui();

        ImGui::Begin("Main GUI");

        ImGui::Text("Moon Properties");

        ImGui::SliderFloat3("Direction", &main->m_moon_direction[0], -1.0f, 1.0f);
        // main->m_sun_direction = glm::normalize(main->m_sun_direction);

        ImGui::ColorEdit3("Color", &main->m_moon_color[0]);

        static float _shininess = 0.1;
        ImGui::SliderFloat("Shininess", &_shininess, 0.0f, 1.0f);
        main->m_moon_specular = glm::vec3(_shininess, _shininess, _shininess);

        static float _ambient = 0.1;
        ImGui::SliderFloat("Ambient Strength", &_ambient, 0.05f, 0.8f);
        main->m_moon_ambient = glm::vec3(_ambient, _ambient, _ambient);

        ImGui::SliderFloat("Brightness", &main->m_moon_brightness, 0.1f, 1);

        ImGui::End();
        graphics->end_gui();
    }
} // app
