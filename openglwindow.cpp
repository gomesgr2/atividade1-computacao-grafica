#include "openglwindow.hpp"

#include <imgui.h>

#include <string>

#include "abcg.hpp"

using namespace std;

void OpenGLWindow::handleEvent(SDL_Event &event) {
  // Teremos apenas os movimentos para direita e esquerda do teclado
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_gameData.m_input.set(static_cast<size_t>(Input::Left));
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_gameData.m_input.set(static_cast<size_t>(Input::Right));
  }
  if (event.type == SDL_KEYUP) {
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Left));
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Right));
  }
}

void OpenGLWindow::initializeGL() {
  ImGuiIO &io{ImGui::GetIO()};
  auto filename{getAssetsPath() + "Inconsolata-Medium.ttf"};
  m_font = io.Fonts->AddFontFromFileTTF(filename.c_str(), 60.0f);
  if (m_font == nullptr) {
    throw abcg::Exception{abcg::Exception::Runtime("Cannot load font file")};
  }

  m_objectsProgram = createProgramFromFile(getAssetsPath() + "objects.vert",
                                           getAssetsPath() + "objects.frag");

  abcg::glClearColor(0, 0, 0, 1);

#if !defined(__EMSCRIPTEN__)
  abcg::glEnable(GL_PROGRAM_POINT_SIZE);
#endif

  m_randomEngine.seed(
      std::chrono::steady_clock::now().time_since_epoch().count());
  // inicializa o jogo
  m_gameData.m_state = State::Init;
}

void OpenGLWindow::update() {
  float deltaTime{static_cast<float>(getDeltaTime())};

  if (m_gameData.m_state == State::Playing) {
    checkCollisions();
    checkWinCondition();
  }

  m_person.update(m_gameData, deltaTime);
  m_balls.update(deltaTime);
}

void OpenGLWindow::paintGL() {
  update();

  abcg::glClear(GL_COLOR_BUFFER_BIT);
  abcg::glViewport(0, 0, m_viewportWidth, m_viewportHeight);

  m_balls.paintGL(m_gameData);
  m_person.paintGL(m_gameData);
}

void OpenGLWindow::paintUI() {
  abcg::OpenGLWindow::paintUI();
  const auto size{ImVec2(500, 500)};
  const auto position{ImVec2((m_viewportWidth - size.x) / 2.0f,
                             (m_viewportHeight - size.y) / 2.0f)};
  ImGui::SetNextWindowPos(position);
  ImGui::SetNextWindowSize(size);
  ImGuiWindowFlags flags{};

  // As flags serão relativas ao estado do jogo
  if (m_gameData.m_state == State::Playing) {
    flags = {ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar |
             ImGuiWindowFlags_NoInputs};

  } else {
    flags = {ImGuiWindowFlags_NoDecoration};
  }

  ImGui::Begin(" ", nullptr, flags);
  ImGui::PushFont(m_font);

  // tela de finalização do jogo, mensagem final será relativa ao estado
  if (m_gameData.m_state == State::GameOver ||
      m_gameData.m_state == State::Win) {
    const char *message =
        m_gameData.m_state == State::GameOver ? "Perdeu" : "Ganhou";
    ImGui::Text(message);
    if (ImGui::Button("Jogar\nNovamente", ImVec2(500, 200))) {
      m_gameData.m_state = State::Menu;
    }
  }
  // telas de inicialização
  else if (m_gameData.m_state == State::Init) {
    if (ImGui::Button("Jogar", ImVec2(300, 80))) {
      m_gameData.m_state = State::Menu;
    }
  }
  // menu de inicialização, as dificuldades do jogo foram baseadas nas
  // quantidades de bolas que serão criadas, para isso foi definida uma variável
  // de nome m_balls_quant que muda de acordo com o nivel
  else if (m_gameData.m_state == State::Menu) {
    if (ImGui::Button("Facil", ImVec2(300, 80))) {
      m_gameData.m_state = State::Playing;
      m_balls_quant = 3;
      initialize();
    }
    if (ImGui::Button("Médio", ImVec2(300, 80))) {
      m_gameData.m_state = State::Playing;
      m_balls_quant = 4;
      initialize();
    }
    if (ImGui::Button("Dificil", ImVec2(300, 80))) {
      m_gameData.m_state = State::Playing;
      m_balls_quant = 6;
      initialize();
    }
  } else {
    ImGui::Text("Você tem 30 seg");
  }

  ImGui::PopFont();
  ImGui::End();
}

void OpenGLWindow::resizeGL(int width, int height) {
  m_viewportWidth = width;
  m_viewportHeight = height;

  abcg::glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLWindow::terminateGL() {
  abcg::glDeleteProgram(m_starsProgram);
  abcg::glDeleteProgram(m_objectsProgram);

  m_person.terminateGL();
  m_balls.terminateGL();
}

void OpenGLWindow::checkCollisions() {
  // função responsavel por verificar se ocorreu uma colisão entre a pessoa e as
  // bolas
  for (const auto &ball : m_balls.m_balls) {
    const auto ballTranslation{ball.m_translation};
    const auto distance{glm::distance(m_person.m_translation, ballTranslation)};

    if (distance < m_person.m_scale * 0.9f + ball.m_scale * 0.85f) {
      m_gameData.m_state = State::GameOver;
      m_restartWaitTimer.restart();
    }
  }
}

void OpenGLWindow::initialize() {
  // função que inicializa m_person e m_balls
  m_person.initializeGL(m_objectsProgram);
  m_balls.initializeGL(m_objectsProgram, m_balls_quant);
}

void OpenGLWindow::checkWinCondition() {
  // caso o tempo acabe o estado do jogo mudará para Win
  if (m_restartWaitTimer.elapsed() > 30) {
    m_gameData.m_state = State::Win;
    m_restartWaitTimer.restart();
  }
}