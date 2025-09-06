# GameHackLab — Laboratório ético de estudo em C++

Este projeto oferece um ambiente seguro para estudar conceitos básicos, sem violar termos de uso e sem tocar em processos de terceiros.

O laboratório é composto por:
- target_app: app "alvo" com servidor TCP em 127.0.0.1:39111 (estado: health/coins/level).
- trainer_app: cliente que autentica (nonce+token), lê estado e envia patch autorizado.
- CMake + C++17, sem bibliotecas externas.

Compilar:
  mkdir build && cd build
  cmake .. -DCMAKE_BUILD_TYPE=Release
  cmake --build . --config Release

Uso:
  1) ./target_app
  2) ./trainer_app

Autor: MicDog (Michael Douglas)
Licença: MIT
