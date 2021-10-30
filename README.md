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


### object.vert

Esse é o shader utilizado na renderização da pessoa e bolas.
```
#version 410

layout(location = 0) in vec2 inPosition;

uniform vec4 color;
uniform float scale;
uniform vec2 translation;

out vec4 fragColor;

void main() {
  vec2 newPosition = inPosition * scale + translation;
  gl_Position = vec4(newPosition, 0, 1);
  fragColor = color;
}

```

### object.frag 

```
#version 410

in vec4 fragColor;

out vec4 outColor;

void main() { outColor = fragColor; }

```



