//
// Created by radisa on 9.10.25..
//

#include "MainController.hpp"

#include <spdlog/spdlog.h>

namespace app {
    std::string_view MainController::name() const {
        return "app::MainController";
    }

    void MainController::initialize() {
        spdlog::info("MainController initialized");
    }
} // app
