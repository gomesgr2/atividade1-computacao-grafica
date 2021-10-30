# Atividade 1 - Computação Gráfica : Jogo 2D Fuja das bolas

### Este jogo foi feito utilizando o algoritmos do jogo de asteroids apresentado na semana 5 como base.

#### Estrutura de arquivos 

![image](https://user-images.githubusercontent.com/70021084/139538921-5720171e-3ecd-4560-bfc5-50c7f1fb4b45.png)
  

### Classes dos elementos da cena do jogo

Como foi feito no projeto dos asteroids, separamos os elementos de cena do jogo em classes :
  
**Person**: classe que representa a pessoa (VAO, VBO e atributos como translação, orientação e velocidade).
   
**Balls**: classe que gerencia as bolas. Como feito na classe Asteroids (classe está no jogo asteroids da semana 5), a classe Balls contém uma lista de instâncias de uma estrutura Ball, que define o VBO e proprieadades de uma bola.


#### Principais implementações 

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
**Note : que o m_input é atualizado a cada evento**

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
Para definirmos as dificuldades do jogo mudaremos o m_balls_quant, que é responsável pela quantidade de bolas que serão criadas na classe Balls.

