//
// Created by radisa on 9.10.25..
//

#include <engine/platform/PlatformController.hpp>
#include <engine/platform/Window.hpp>

#include "MainController.hpp"

#include <X11/X.h>
#include <spdlog/spdlog.h>

namespace app {
    std::string_view MainController::name() const {
        return "app::MainController";
    }

    bool MainController::loop() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        if (platform->key(engine::platform::KeyId::KEY_ESCAPE).state() == engine::platform::Key::State::JustPressed) {
            return false;
        } else {
            return true;
        }

    }

    void MainController::initialize() {
        spdlog::info("MainController initialized");
    }
} // app
