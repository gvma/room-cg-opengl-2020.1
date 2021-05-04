# room-opengl-cg-2020.1
Repositório para a matéria de computação gráfica do IC UFAL

É necessário a utilização das bibliotecas que compõem o OpenGL:
- GLUT and GLEW;

Também é necessário um compilador para C++ instalado no computador.

(Os comandos demonstrados abaixo são para o g++, disponível no ubuntu)
Para compilar e rodar o programa (no linux), navegue até a pasta em que foi feito o clone o repositório e rode o seguinte comando:
g++ main.cpp -o main -lglut -lGLU -lGL -lGLEW && ./main

Os controles utilizados para explorarmos a cena do quarto são:

- Mouse: utilizado para movimentar a câmera em torno de seu próprio eixo, possibilitando olhar em todas as direções. Para olhar nas direções, utilize o clique esquerdo do mouse;
- W: movimentar a câmera para frente;
- A: movimentar a câmera para a esquerda;
- S: movimentar a câmera para trás;
- D: movimentar a câmera para direita;
- P: abrir e fechar a porta;
- J: abrir e fechar a janela;
