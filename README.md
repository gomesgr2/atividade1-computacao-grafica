# Atividade 1 - Computação Gráfica : Jogo 2D Escape das bolas

#### Estrutura de arquivos 

![image](https://user-images.githubusercontent.com/70021084/139538921-5720171e-3ecd-4560-bfc5-50c7f1fb4b45.png)
  

### Classes dos elementos da cena do jogo

Como foi feito no projeto dos asteroids, separamos os elementos de cena do jogo em classes :
  
**Person**: classe que representa a pessoa (VAO, VBO e atributos como translação, orientação e velocidade).
   
**Balls**: classe que gerencia as bolas. Como feito na classe Asteroids (classe está no jogo asteroids da semana 5), a classe Balls contém uma lista de instâncias de uma estrutura Ball, que define o VBO e proprieadades de uma bola.


### Principais implementações 

### gamedata.cpp :

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

- **Manipulação de estados na função paintUI** 
```
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
```
Para definirmos as dificuldades do jogo mudaremos o m_balls_quant : variável que é responsável pela quantidade de bolas que serão criadas na classe Balls.

- **Verificação se o usuário ganhou** 
```
void OpenGLWindow::checkWinCondition() {
  // caso o tempo acabe o estado do jogo mudará para Win
  if (m_restartWaitTimer.elapsed() > 30) {
    m_gameData.m_state = State::Win;
    m_restartWaitTimer.restart();
  }
}

```


### Person :
 Para criação da pessoa foi utilizado o geogebra para auxiliar na criação dos vértices :
 
![2021-10-22_19-45](https://user-images.githubusercontent.com/70021084/139541289-b3b5a5f5-e32c-44e1-9831-0ee9449e1859.png)

Assim na função **initializeGL** na classe Person ficou dessa forma :

```
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
```

Para garantir a movimentação do objeto, foi feito :

```
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

```
### Balls :
  - **Balls::initializeGL** 
```
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

```
Observe que a lista bolas é criada a partir da variavel quantity, o valor desta variável é definida de acordo com o nível selecionado pelo usuario. 

- **Balls::update**

```
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
```

Na função update a atualização da translação da bola é feita sempre negativamente ao eixo y, criando assim o efeito de queda. Caso a bola ultrapasse a viewport, ou seja, saia da tela, temos então que iremos atualizar a posição da bola somando +2 no eixo y fazendo que ela volte a aparecer na tela, além disso, é gerado um m_translation.x aleatório entre (-1, 1) para garantir que as bolas variem de posição a cada queda.



## Autor

- Gabriel Gomes de Oliveira Costa
- Ra : 11201921471

