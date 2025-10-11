//
// Created by radisa on 9.10.25..
//

#include "MyApp.hpp"

#include <GUIController.hpp>
#include <MainController.hpp>
#include <spdlog/spdlog.h>

namespace app {
    void MyApp::app_setup() {
        spdlog::info("App setup");
        auto main_controller = register_controller<app::MainController>();
        auto gui_controller  = register_controller<app::GUIController>();
        main_controller->after(engine::core::Controller::get<engine::core::EngineControllersEnd>());
        gui_controller->after(main_controller);
    }
} // app
