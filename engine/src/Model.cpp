
#include <engine/resources/Model.hpp>
#include <engine/resources/Shader.hpp>

namespace engine::resources {

    void Model::draw(const Shader *shader) {
        shader->use();
        for (auto &mesh: m_meshes) {
            mesh.draw(shader);
        }
    }

    void Model::destroy() {
        for (auto &mesh: m_meshes) {
            mesh.destroy();
        }
    }

    void Model::draw_instanced(const Shader *shader, const std::vector<glm::mat4> &model_matrices) {
        shader->use();
        for (auto &mesh: m_meshes) {
            mesh.setup_instances(model_matrices);
            mesh.draw_instanced(shader, model_matrices.size());
        }
    }
}
