//
// Created by radisa on 9.10.25..
//

#ifndef MAINCONTROLLER_HPP
#define MAINCONTROLLER_HPP
#include <engine/core/Controller.hpp>

namespace app {
    class MainController : public engine::core::Controller {
    public:
        std::string_view name() const override;

    private:
        void initialize() override;
    };
} // app

#endif //MAINCONTROLLER_HPP
