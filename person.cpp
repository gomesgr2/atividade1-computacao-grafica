#include "person.hpp"

#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/rotate_vector.hpp>

void Person::initializeGL(GLuint program) {
  terminateGL();

  m_program = program;
  m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
  m_rotationLoc = abcg::glGetUniformLocation(m_program, "rotation");
  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");

  m_rotation = 0.0f;
  m_translation = glm::vec2(0, -0.7f);
  m_velocity = glm::vec2(0);

  std::array<glm::vec2, 16> positions{

      // Corpo da pessoa
      glm::vec2{-7.0f, +24.0f}, glm::vec2{0.0f, +030.0f},
      glm::vec2{+7.0f, +24.0f}, glm::vec2{0.0f, +10.0f},

      // Cabeça da pessoa
      glm::vec2{-10.0f, +40.0f}, glm::vec2{0.0f, +40.43f},
      glm::vec2{+10.0f, +40.0f}, glm::vec2{0.0f, +30.0f},

      // Pernas da pessoa
      glm::vec2{-20.0f, 0.0f}, glm::vec2{0.0f, +10.0f}, glm::vec2{0.1f, +6.8f},
      glm::vec2{+20.0f, 0.0f},

      // Braços da pessoa
      glm::vec2{-21.0f, +38.0f}, glm::vec2{-24.0f, +035.0f},
      glm::vec2{+21.0f, +38.0f}, glm::vec2{+24.0f, +35.0f}

  };

  for (auto &position : positions) {
    position /= glm::vec2{15.5f, 15.5f};
  }

  const std::array indices{// Corpo da pessoa
                           0, 1, 3, 1, 2, 3,
                           // Cabeça da pessoa
                           4, 5, 7, 5, 6, 7,
                           // Pernas da pessoa
                           8, 9, 10, 9, 10, 11,

                           // Braços da pessoa
                           12, 13, 0, 14, 15, 2

  };

  abcg::glGenBuffers(1, &m_vbo);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glGenBuffers(1, &m_ebo);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
  abcg::glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  GLint positionAttribute{abcg::glGetAttribLocation(m_program, "inPosition")};

  abcg::glGenVertexArrays(1, &m_vao);

  abcg::glBindVertexArray(m_vao);

  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

  abcg::glBindVertexArray(0);
}

void Person::paintGL(const GameData &gameData) {
  if (gameData.m_state != State::Playing) return;

  abcg::glUseProgram(m_program);

  abcg::glBindVertexArray(m_vao);

  abcg::glUniform1f(m_scaleLoc, m_scale);
  abcg::glUniform1f(m_rotationLoc, m_rotation);
  abcg::glUniform2fv(m_translationLoc, 1, &m_translation.x);
  abcg::glUniform4fv(m_colorLoc, 1, &m_color.r);
  abcg::glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_INT, nullptr);

  abcg::glBindVertexArray(0);

  abcg::glUseProgram(0);
}

void Person::terminateGL() {
  abcg::glDeleteBuffers(1, &m_vbo);
  abcg::glDeleteBuffers(1, &m_ebo);
  abcg::glDeleteVertexArrays(1, &m_vao);
}

void Person::update(const GameData &gameData, float deltaTime) {
  if (gameData.m_input[static_cast<size_t>(Input::Left)]) {
    if ((m_translation.x - 2.0f * deltaTime) > -1) {
      m_translation.x = m_translation.x - 2.0f * deltaTime;
    }
  }

  if (gameData.m_input[static_cast<size_t>(Input::Right)]) {
    if ((m_translation.x - 2.0f * deltaTime) < 1) {
      m_translation.x = m_translation.x + 2.0f * deltaTime;
    }
  }
}