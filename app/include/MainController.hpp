//
// Created by radisa on 9.10.25..
//

#ifndef MAINCONTROLLER_HPP
#define MAINCONTROLLER_HPP
#include <engine/core/Controller.hpp>
#include <glm/glm.hpp>

namespace app {
    class MainController final : public engine::core::Controller {
    public:
        std::string_view name() const override;

    private:
        void initialize() override;

        bool loop() override;

        void update_camera();

        void update() override;

        void draw_fireplace();

        void draw_hut();

        void draw_axe();

        void begin_draw() override;

        void draw() override;

        void end_draw() override;

    public:
        glm::vec3 m_moon_direction{1.0f, 1.0f, 1.0f};
        glm::vec3 m_moon_color{1.0f, 1.0f, 1.0f};
        glm::vec3 m_moon_ambient{0.1f, 0.1f, 0.1f};
        glm::vec3 m_moon_specular{0.5f, 0.5f, 0.5f};
        float m_moon_brightness{0.9f};

    };
} // app

#endif //MAINCONTROLLER_HPP
