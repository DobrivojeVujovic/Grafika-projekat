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

    void MainController::update_camera() {
        //[TODO] add movement speed contorls
        auto gui_controller = engine::core::Controller::get<GUIController>();
        if (gui_controller->is_enabled())
            return;

        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
        auto camera   = graphics->camera();

        float dt = platform->dt();
        if (platform->key(engine::platform::KEY_W).is_down()) {
            camera->move_camera(engine::graphics::Camera::Movement::FORWARD, dt);
        }
        if (platform->key(engine::platform::KEY_S).is_down()) {
            camera->move_camera(engine::graphics::Camera::Movement::BACKWARD, dt);
        }
        if (platform->key(engine::platform::KEY_A).is_down()) {
            camera->move_camera(engine::graphics::Camera::Movement::LEFT, dt);
        }
        if (platform->key(engine::platform::KEY_D).is_down()) {
            camera->move_camera(engine::graphics::Camera::Movement::RIGHT, dt);
        }
        if (platform->key(engine::platform::KEY_LEFT_CONTROL).is_down()) {
            camera->move_camera(engine::graphics::Camera::Movement::DOWN, dt);
        }
        if (platform->key(engine::platform::KEY_SPACE).is_down()) {
            camera->move_camera(engine::graphics::Camera::Movement::UP, dt);
        }
    }

    void MainController::update() {
        update_camera();
    }

    void MainController::draw_hut() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto graphics  = engine::core::Controller::get<engine::graphics::GraphicsController>();
        //Model
        engine::resources::Model *hut = resources->model("hut");
        //Shader
        engine::resources::Shader *shader = resources->shader("basicAlphaTest");

        shader->use();
        shader->set_mat4("projection", graphics->projection_matrix());
        shader->set_mat4("view", graphics->camera()->view_matrix());

        glm::mat4 model = glm::mat4(1.0f);
        model           = glm::translate(model, glm::vec3(5.0f, -5.0f, -5.0f));
        shader->set_mat4("model", model);

        shader->set_vec3("sun.direction", glm::vec3(1.0f, 0.0f, 1.0f));
        shader->set_vec3("sun.ambient", glm::vec3(0.9f, 0.9f, 0.9f));
        shader->set_vec3("sun.diffuse", glm::vec3(0.9f, 0.9f, 0.9f));
        hut->draw(shader);
    }

    void MainController::draw_axe() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto graphics  = engine::core::Controller::get<engine::graphics::GraphicsController>();
        //Model
        engine::resources::Model *watchtower = resources->model("axe");
        //Shader
        engine::resources::Shader *shader = resources->shader("phong");

        shader->use();
        shader->set_mat4("projection", graphics->projection_matrix());
        shader->set_mat4("view", graphics->camera()->view_matrix());

        glm::mat4 model = glm::mat4(1.0f);
        model           = glm::scale(model, glm::vec3(0.1f));
        model           = glm::translate(model, glm::vec3(0.0f, -5.0f, -10.0f));
        model           = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        shader->set_mat4("model", model);

        shader->set_vec3("sun.direction", glm::vec3(1.0f, 0.0f, 1.0f));
        shader->set_vec3("sun.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
        shader->set_vec3("sun.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
        shader->set_vec3("sun.specular", glm::vec3(1.0f, 1.0f, 1.0f));

        shader->set_float("shininess", 300);
        shader->set_vec3("cameraPos", graphics->camera()->Position);

        watchtower->draw(shader);
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
        draw_axe();
    }

} // app
