//
// Created by radisa on 9.10.25..
//

#include <engine/graphics/GraphicsController.hpp>
#include <engine/graphics/OpenGL.hpp>
#include <engine/platform/PlatformController.hpp>
#include <engine/platform/Window.hpp>
#include <engine/resources/ResourcesController.hpp>

#include "MainController.hpp"

#include <GUIController.hpp>
#include <X11/X.h>
#include <spdlog/spdlog.h>

#include "../../engine/libs/glad/include/glad/glad.h"

namespace app {

    class MainPlatformEventObserver : public engine::platform::PlatformEventObserver {
    public:
        void on_mouse_move(engine::platform::MousePosition position) override;
    };

    void MainPlatformEventObserver::on_mouse_move(engine::platform::MousePosition position) {
        auto gui_controller = engine::core::Controller::get<GUIController>();
        if (!gui_controller->is_enabled()) {
            auto camera = engine::core::Controller::get<engine::graphics::GraphicsController>()->camera();
            camera->rotate_camera(position.dx, position.dy);
        }
    }

    std::string_view MainController::name() const {
        return "app::MainController";
    }

    void MainController::initialize() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        platform->register_platform_event_observer(std::make_unique<MainPlatformEventObserver>());
        engine::graphics::OpenGL::enable_depth_testing();

        // GLFWwindow *window_handle = platform->window()->handle_();
        // glfwSetInputMode(window_handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        spdlog::info("MainController initialized");
    }

    bool MainController::loop() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        if (platform->key(engine::platform::KeyId::KEY_ESCAPE).state() == engine::platform::Key::State::JustPressed) {
            return false;
        } else {
            return true;
        }

    }

    void MainController::draw_skybox() {
        auto shader      = engine::core::Controller::get<engine::resources::ResourcesController>()->shader("skybox");
        auto skybox_cube = engine::core::Controller::get<engine::resources::ResourcesController>()->skybox(
                "skybox");

        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();

        shader->set_mat4("projection", graphics->projection_matrix());
        shader->set_mat4("view", graphics->camera()->view_matrix());

        graphics->draw_skybox(shader, skybox_cube);
    }

    void MainController::update_camera() {
        auto gui_controller = engine::core::Controller::get<GUIController>();
        if (gui_controller->is_enabled())
            return;

        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
        auto camera   = graphics->camera();

        float dt                  = platform->dt();
        float cameraSpeedModifier = 1.0f;
        if (platform->key(engine::platform::KEY_LEFT_SHIFT).is_down())
            cameraSpeedModifier = 0.3f;

        if (platform->key(engine::platform::KEY_W).is_down()) {
            camera->move_camera(engine::graphics::Camera::Movement::FORWARD, cameraSpeedModifier * dt);
        }
        if (platform->key(engine::platform::KEY_S).is_down()) {
            camera->move_camera(engine::graphics::Camera::Movement::BACKWARD, cameraSpeedModifier * dt);
        }
        if (platform->key(engine::platform::KEY_A).is_down()) {
            camera->move_camera(engine::graphics::Camera::Movement::LEFT, cameraSpeedModifier * dt);
        }
        if (platform->key(engine::platform::KEY_D).is_down()) {
            camera->move_camera(engine::graphics::Camera::Movement::RIGHT, cameraSpeedModifier * dt);
        }
        if (platform->key(engine::platform::KEY_LEFT_CONTROL).is_down()) {
            camera->move_camera(engine::graphics::Camera::Movement::DOWN, cameraSpeedModifier * dt);
        }
        if (platform->key(engine::platform::KEY_SPACE).is_down()) {
            camera->move_camera(engine::graphics::Camera::Movement::UP, cameraSpeedModifier * dt);
        }
    }

    void MainController::update() {
        update_camera();
    }

    void MainController::draw_fireplace() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto graphics  = engine::core::Controller::get<engine::graphics::GraphicsController>();
        auto platform  = engine::core::Controller::get<engine::platform::PlatformController>();

        engine::resources::Model *fireplace = resources->model("fireplace");
        engine::resources::Shader *shader   = resources->shader("fireplace");

        shader->use();
        shader->set_mat4("projection", graphics->projection_matrix());
        shader->set_mat4("view", graphics->camera()->view_matrix());

        glm::mat4 model = glm::mat4(0.5f);
        shader->set_mat4("model", model);
        shader->set_float("time", platform->frame_time().current);

        fireplace->draw(shader);
    }

    void MainController::draw_hut() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto graphics  = engine::core::Controller::get<engine::graphics::GraphicsController>();
        auto platform  = engine::core::Controller::get<engine::platform::PlatformController>();

        //Model
        engine::resources::Model *hut = resources->model("hut");
        //Shader
        engine::resources::Shader *shader = resources->shader("hut");

        shader->use();
        shader->set_mat4("projection", graphics->projection_matrix());
        shader->set_mat4("view", graphics->camera()->view_matrix());

        glm::mat4 model = glm::mat4(1.0f);
        model           = glm::translate(model, glm::vec3(3.0f, 0.0f, 0.0f));
        shader->set_mat4("model", model);

        //Direkciono svetlo
        shader->set_vec3("dirLight.direction", m_moon_direction);
        shader->set_vec3("dirLight.diffuse", m_moon_color * m_moon_brightness);
        shader->set_vec3("dirLight.ambient", m_moon_ambient * m_moon_brightness);
        shader->set_vec3("dirLight.specular", m_moon_specular * m_moon_brightness);

        //Point svetlo
        shader->set_vec3("pointLight.position", glm::vec3(0.0f, 0.0f, 0.0f));
        // Boje – topla paleta vatre
        shader->set_vec3("pointLight.ambient", glm::vec3(0.15f, 0.05f, 0.01f)); // slabo osvetljenje okoline
        shader->set_vec3("pointLight.diffuse", glm::vec3(1.0f, 0.45f, 0.05f));  // narandžasto/žuto jezgro
        shader->set_vec3("pointLight.specular", glm::vec3(1.0f, 0.9f, 0.7f));   // topli odsjaj

        // Attenuation
        shader->set_float("pointLight.constant", 1.0f);
        shader->set_float("pointLight.linear", 0.14f);
        shader->set_float("pointLight.quadratic", 0.07f);

        shader->set_float("shininess", 0);

        shader->set_float("time", platform->frame_time().current);

        shader->set_vec3("cameraPos", graphics->camera()->Position);

        hut->draw(shader);
    }

    void MainController::draw_axe() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto graphics  = engine::core::Controller::get<engine::graphics::GraphicsController>();
        auto platform  = engine::core::Controller::get<engine::platform::PlatformController>();

        engine::resources::Model *axe     = resources->model("axe");
        engine::resources::Shader *shader = resources->shader("axe");

        shader->use();
        shader->set_mat4("projection", graphics->projection_matrix());
        shader->set_mat4("view", graphics->camera()->view_matrix());

        //Model matrica
        glm::mat4 model = glm::mat4(1.0f);
        model           = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
        model           = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model           = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
        shader->set_mat4("model", model);

        //Direkciono svetlo
        shader->set_vec3("dirLight.direction", m_moon_direction);
        shader->set_vec3("dirLight.diffuse", m_moon_color * m_moon_brightness);
        shader->set_vec3("dirLight.ambient", m_moon_ambient * m_moon_brightness);
        shader->set_vec3("dirLight.specular", m_moon_specular * m_moon_brightness);

        //Point svetlo
        shader->set_vec3("pointLight.position", glm::vec3(0.0f, 0.0f, 0.0f));
        // Boje – topla paleta vatre
        shader->set_vec3("pointLight.ambient", glm::vec3(0.15f, 0.05f, 0.01f)); // slabo osvetljenje okoline
        shader->set_vec3("pointLight.diffuse", glm::vec3(1.0f, 0.45f, 0.05f));  // narandžasto/žuto jezgro
        shader->set_vec3("pointLight.specular", glm::vec3(1.0f, 0.9f, 0.7f));   // topli odsjaj

        // Attenuation
        shader->set_float("pointLight.constant", 1.0f);
        shader->set_float("pointLight.linear", 0.14f);
        shader->set_float("pointLight.quadratic", 0.07f);

        shader->set_float("shininess", 64);

        shader->set_float("time", platform->frame_time().current);

        shader->set_vec3("cameraPos", graphics->camera()->Position);

        axe->draw(shader);
    }

    void MainController::setup_trees(int tree_count, float tree_radius = 1.0f, float inner_radius = 10.0f) {
        if (trees_setup)
            return;
        trees_setup = true;

        tree_model_matrices.reserve(tree_count);

        float radial_step    = tree_radius * 2.5f;
        float current_radius = inner_radius;

        int placed_trees = 0;
        while (placed_trees < tree_count) {

            //Obim trenutnog prstena
            float circumference = 2.0f * glm::pi<float>() * current_radius;

            // Koliko stabala moze stati na taj obim
            int trees_in_ring = static_cast<int>(circumference / (tree_radius * 2.0f));
            if (trees_in_ring < 1)
                trees_in_ring = 1;

            // Raspodela po uglu
            float angle_step = 2.0f * glm::pi<float>() / trees_in_ring;

            float offset = static_cast<float>(rand()) / RAND_MAX * 10.0f;
            for (int i = 0; i < trees_in_ring && placed_trees < tree_count; i++) {
                float angle = i * angle_step + offset;

                float x = cos(angle) * current_radius;
                float z = sin(angle) * current_radius;

                glm::mat4 model = glm::mat4(1.0f);
                model           = glm::translate(model, glm::vec3(x, 0.0f, z));

                // Random rotacija
                float rotation = (float) (rand() % 360);

                model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));

                tree_model_matrices.push_back(model);
                placed_trees++;
            }

            current_radius += radial_step;
        }

    }

    void MainController::draw_trees() {
        setup_trees(200);

        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto graphics  = engine::core::Controller::get<engine::graphics::GraphicsController>();

        engine::resources::Model *tree    = resources->model("tree");
        engine::resources::Shader *shader = resources->shader("tree");

        shader->use();
        shader->set_mat4("projection", graphics->projection_matrix());
        shader->set_mat4("view", graphics->camera()->view_matrix());

        for (int i = 0; i < tree_model_matrices.size(); i++) {
            shader->set_mat4("model", tree_model_matrices[i]);
            tree->draw(shader);
        }
    }

    void MainController::begin_draw() {
        engine::graphics::OpenGL::clear_buffers();
    }

    void MainController::end_draw() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        platform->swap_buffers();
    }

    void MainController::draw() {
        draw_hut();
        draw_fireplace();
        draw_axe();
        draw_trees();

        draw_skybox();
    }

} // app
