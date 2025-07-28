#include "glad/glad.h"
#include <GL/gl.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <memory>
#include "render/object.hh"
#include "render/scene.hh"
#include "render/shader.hh"

Scene::Scene(Camera &cam)
    : highlightedObject(nullptr), camera(cam),
      highlightShader("./assets/shaders/highlight.vert",
                      "./assets/shaders/highlight.frag", false) {
  rootObject = std::make_shared<Object>("root", nullptr, Transform());
  // highlightedObject = rootObject;
}
void Scene::instantiate(std::shared_ptr<Model> model) {
  rootObject->instantiate(model);
}

void Scene::update() {
  camera.update();
  if (highlightedObject) {
    auto bounds = highlightedObject->getBounds();
    auto center = bounds.getCenter();
    camera.transform.position =
        -center + glm::vec3(0, 0, 0.5) * glm::length(bounds.getDiagonal());
    camera.transform.rotation =
        glm::quat(glm::vec3(0, 0, -1), camera.transform.position + center);
  }
}

void Scene::render() {
  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);

  rootObject->cacheModelMats(Transform().getModelMat());

  Shader *curShader = nullptr;

  auto renderLambda = [&curShader](Object &o) {
    if (o.mesh != nullptr) {
      glUniformMatrix4fv(curShader->loc("model"), 1, GL_FALSE,
                         &o.getModelMat()[0][0]);
      if (o.mesh->activate()) {
        if (o.mesh->material && curShader->usesMaterial)
          o.mesh->material->activate(*curShader);

        // TODO add imgui option for style
        glDrawElements(GL_TRIANGLES, o.mesh->getAmountTris() * 3,
                       GL_UNSIGNED_INT, 0);
      } else
        std::cerr << "ERROR ACTIVATING " << o.name << std::endl;
    }
    return true;
  };

  glDepthMask(GL_TRUE);
  curShader = &camera.shader;
  camera.shader.activate(camera);
  rootObject->apply(renderLambda);

  glBindVertexArray(0);
}

/* FOR FUTURE CODE ABOUT POST PROCESSING
    screenShader->use();

    static GLuint quadVAO = 0;
    if (!quadVAO) {
      // Generate VAO
      glGenVertexArrays(1, &quadVAO);
      glBindVertexArray(quadVAO);

      unsigned int VBO;
      // Generate VBO
      glGenBuffers(1, &VBO);
      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      const float vertices[] = {
          // positions   // texCoords
          -1.0f, 1.0f, 0.0f,  1.0f,  1.0f, -1.0f,
          1.0f,  0.0f, -1.0f, -1.0f, 0.0f, 0.0f,

          -1.0f, 1.0f, 0.0f,  1.0f,  1.0f, 1.0f,
          1.0f,  1.0f, 1.0f,  -1.0f, 1.0f, 0.0f,
      };
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

      // Position attribute
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                            (void *)0);

      // Texture coordinate attribute
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                            (void *)(2 * sizeof(float)));

      glBindVertexArray(0); // Unbind VAO
    }

    glBindVertexArray(quadVAO);
    glDisable(GL_DEPTH_TEST);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glUniform1i(screenShader->loc("screenTexture"), 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    interface->update(scene);
    */
