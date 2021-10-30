# Atividade 1 - Computação Gráfica : Jogo 2D

### Este jogo foi feito utilizando o algoritmos do jogo de asteroids apresentado na semana 5 como base.

#### Estrutura de arquivos 

![image](https://user-images.githubusercontent.com/70021084/139538921-5720171e-3ecd-4560-bfc5-50c7f1fb4b45.png)
  

### Classes dos elementos da cena do jogo

Como foi feito no projeto dos asteroids, separamos os elementos de cena do jogo em classes :
  
**Person**: classe que representa a pessoa (VAO, VBO e atributos como translação, orientação e velocidade).
   
**Balls**: classe que gerencia as bolas. Como feito na classe Asteroids (classe está no jogo asteroids da semana 5), a classe Balls contém uma lista de instâncias de uma estrutura Ball, que define o VBO e proprieadades de uma bola.


**main.cpp**

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


