#include "balls.hpp"

#include <cppitertools/itertools.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

void Balls::initializeGL(GLuint program, int quantity) {
  terminateGL();
  m_randomEngine.seed(
      std::chrono::steady_clock::now().time_since_epoch().count());

  m_program = program;
  m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
  m_rotationLoc = abcg::glGetUniformLocation(m_program, "rotation");
  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");

  m_balls.clear();
  m_balls.resize(quantity);

  // iniciando as bolas com m_translation.x aleatório e um m_translation.y = -1,
  // garantindo que as bolas sempre vão começar acima do viewport, garantindo
  // assim a dinâmica do jogo
  for (auto &ball : m_balls) {
    ball = createBall();
    ball.m_translation = {m_randomDist(m_randomEngine), -1.0f};
  }
}

void Balls::paintGL(const GameData &gameData) {
  if (gameData.m_state != State::Playing) return;

  abcg::glUseProgram(m_program);

  for (const auto &ball : m_balls) {
    abcg::glBindVertexArray(ball.m_vao);

    abcg::glUniform4fv(m_colorLoc, 1, &ball.m_color.r);
    abcg::glUniform1f(m_scaleLoc, ball.m_scale);
    abcg::glUniform1f(m_rotationLoc, ball.m_rotation);

    for (auto i : {-2, 0, 2}) {
      for (auto j : {-2, 0, 2}) {
        abcg::glUniform2f(m_translationLoc, ball.m_translation.x + j,
                          ball.m_translation.y + i);

        abcg::glDrawArrays(GL_TRIANGLE_FAN, 0, 3);
      }
    }

    abcg::glBindVertexArray(0);
  }

  abcg::glUseProgram(0);
}

void Balls::terminateGL() {
  for (auto ball : m_balls) {
    abcg::glDeleteBuffers(1, &ball.m_vbo);
    abcg::glDeleteVertexArrays(1, &ball.m_vao);
  }
}

void Balls::update(float deltaTime) {
  auto &re{m_randomEngine};

  for (auto &ball : m_balls) {
    // atualizando a translação das bolas fazendo elas transladarem sempre
    // negativamente ao eixo y, criando o efeito de queda
    ball.m_translation.y = ball.m_translation.y - 1.0f * deltaTime;

    // caso a bola ultrapassou o viewport somamos +2 e acrescentamos a bola um
    // m_translation.x aleatório para garantir que as bolas variem as posições
    // de queda
    if (ball.m_translation.y < -1.0f) {
      ball.m_translation.x = m_randomDist(re);
      ball.m_translation.y += 2.0f;
    }
  }
}

Balls::Ball Balls::createBall(glm::vec2 translation, float scale) {
  Ball ball;

  auto &re{m_randomEngine};

  std::uniform_real_distribution<float> randomIntensity(0.5f, 1.0f);
  ball.m_color = glm::vec4(1) * 4.0f;

  ball.m_color.a = 1.0f;
  ball.m_rotation = 0.0f;
  ball.m_scale = scale;
  ball.m_translation = translation;

  ball.m_angularVelocity = m_randomDist(re);

  glm::vec2 direction{m_randomDist(re), -1};
  ball.m_velocity = glm::normalize(direction) / 2.0f;

  const auto sides{15};
  double radius = 0.5;

  std::vector<glm::vec2> positions(0);
  positions.emplace_back(0, 0);
  const auto step{M_PI * 2 / sides};
  for (const auto angle : iter::range(0.0, M_PI * 2, step)) {
    positions.emplace_back(std::cos(angle) * radius, std::sin(angle) * radius);
  }
  positions.push_back(positions.at(1));

  abcg::glGenBuffers(1, &ball.m_vbo);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, ball.m_vbo);
  abcg::glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec2),
                     positions.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  GLint positionAttribute{abcg::glGetAttribLocation(m_program, "inPosition")};

  abcg::glGenVertexArrays(1, &ball.m_vao);

  abcg::glBindVertexArray(ball.m_vao);

  abcg::glBindBuffer(GL_ARRAY_BUFFER, ball.m_vbo);
  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glBindVertexArray(0);
  return ball;
}