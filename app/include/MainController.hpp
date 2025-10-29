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

        void draw_skybox();

        void update_camera();

        void update() override;

        void draw_fireplace();

        void draw_hut();

        void draw_axe();

        void setup_trees(int tree_count, float tree_radius, float inner_radius);

        void draw_trees();

        void begin_draw() override;

        void draw() override;

        void end_draw() override;

    public
    :
        glm::vec3 m_moon_direction{1.0f, 1.0f, 1.0f};
        glm::vec3 m_moon_color{1.0f, 1.0f, 1.0f};
        glm::vec3 m_moon_ambient{0.1f, 0.1f, 0.1f};
        glm::vec3 m_moon_specular{0.5f, 0.5f, 0.5f};
        float m_moon_brightness{0.9f};

        bool m_trees_setup{false};
        std::vector<glm::mat4> m_tree_model_matrices;

        bool m_axe_visible{true};
        float m_axe_appear_time{0.0f};    // vreme kada je dugme pritisnuto
        float m_axe_disappear_time{0.0f}; // vreme kada je dugme pritisnuto

        bool m_axe_appear_pending{false};
        bool m_axe_disappear_pending{false};
    };
} // app

#endif //MAINCONTROLLER_HPP
