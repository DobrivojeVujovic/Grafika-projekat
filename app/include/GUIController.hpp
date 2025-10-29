//
// Created by radisa on 11.10.25..
//
#ifndef GUICONTROLLER_HPP
#define GUICONTROLLER_HPP
#include<engine/core/Controller.hpp>

namespace app {

    class GUIController : public engine::core::Controller {
    public:
        std::string_view name() const override;

    private:
        void poll_events() override;

        void initialize() override;

        void draw() override;
    };

} // app

#endif //GUICONTROLLER_HPP
