# Atividade 1 - Computação Gráfica : Jogo 2D Fuja das bolas

### Este jogo foi feito utilizando o algoritmos do jogo de asteroids apresentado na semana 5 como base.

#### Estrutura de arquivos 

![image](https://user-images.githubusercontent.com/70021084/139538921-5720171e-3ecd-4560-bfc5-50c7f1fb4b45.png)
  

### Classes dos elementos da cena do jogo

Como foi feito no projeto dos asteroids, separamos os elementos de cena do jogo em classes :
  
**Person**: classe que representa a pessoa (VAO, VBO e atributos como translação, orientação e velocidade).
   
**Balls**: classe que gerencia as bolas. Como feito na classe Asteroids (classe está no jogo asteroids da semana 5), a classe Balls contém uma lista de instâncias de uma estrutura Ball, que define o VBO e proprieadades de uma bola.


### main.cpp

Seguindo o que foi apresentado nas aulas anteriores, temos a **main.cpp** dessa forma

```
#include <fmt/core.h>

#include "abcg.hpp"
#include "openglwindow.hpp"

int main(int argc, char **argv) {
  try {
    abcg::Application app(argc, argv);

    auto window{std::make_unique<OpenGLWindow>()};
    window->setOpenGLSettings({.samples = 4});
    window->setWindowSettings({.width = 600,
                               .height = 600,
                               .showFPS = false,
                               .showFullscreenButton = false,
                               .title = "My Game"});
    app.run(std::move(window));
  } catch (const abcg::Exception &exception) {
    fmt::print(stderr, "{}\n", exception.what());
    return -1;
  }
  return 0;
} 
```


### gamedata.cpp

Estrutura que define o estado atual do jogo
```
#ifndef GAMEDATA_HPP_
#define GAMEDATA_HPP_

#include <bitset>

enum class Input { Right, Left };
enum class State { Playing, GameOver, Win, Init, Menu };

struct GameData {
  State m_state{State::Init};
  std::bitset<2> m_input;
};

#endif

```
m_state pode ser:  
  - Playing  : aplicação em modo jogo
  - GameOver : aplicação na tela de finalização do jogo com a mensagem "Perdeu" e botão para jogar novamente, neste caso a pessoa e as bolas não são exibidas.
  - Win : aplicação na tela de finalização do jogo com a mensagem "Ganhou" e botão para jogar novamente, neste caso a pessoa e as bolas não são exibidas.
  - Init : aplicação na tela de inicialização aparecendo botão com a mensagem "Jogar".
  - Menu : aplicação de escolha da dificuldade do jogo com os botões : Fácil, Médio e Difícil.

m_input : 
  Máscara de bits de eventos de estado, cujo os únicos estados possíveis são para a esquerda e direita (movimentações possiveis do objeto pessoa)


### openglwindow.hpp

O conteúdo de openglwindow.hpp ficará assim :

```
#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <imgui.h>

#include <random>

#include "abcg.hpp"
#include "person.hpp"
#include "balls.hpp"


class OpenGLWindow : public abcg::OpenGLWindow {
 protected:
  void handleEvent(SDL_Event& event) override;
  void initializeGL() override;
  void paintGL() override;
  void paintUI() override;
  void resizeGL(int width, int height) override;
  void terminateGL() override;

 private:
  GLuint m_starsProgram{};
  GLuint m_objectsProgram{};

  int m_viewportWidth{};
  int m_viewportHeight{};
  int m_balls_quant{};

  GameData m_gameData;
  Person m_person;
  Balls m_balls;


  abcg::ElapsedTimer m_restartWaitTimer;

  ImFont* m_font{};

  std::default_random_engine m_randomEngine;

  void initialize();
  void update();
  void checkCollisions();
  void checkWinCondition();
};

#endif
```

### openglwindow.cpp :

- **OpenGLWindow::handleEvent**
Como a pessoa só poderá se mover horizontalmente, então teremos apenas os movimentos para direita e esquerda do teclado :
```
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

```
**Note : que o m_input é atualizado a cada evento **








