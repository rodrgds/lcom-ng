# 🐍 LCOM Snake Game - Projeto 2025

## 📘 LCOM Project for group `GRUPO_2LEIC03_1`

**Group members:**

- Carlos Cerqueira (<up202305021@up.pt>)  
- João Ferreira (<up202305204@up.pt>)  
- Manuel Pedro (<up202303997@up.pt>)  
- Pedro Coelho (<up202306714@up.pt>)  

---

## 🎯 Objetivo

Desenvolvemos uma versão avançada e interativa do clássico jogo Snake, utilizando múltiplos dispositivos de I/O estudados ao longo do semestre. A aplicação corre no ambiente LCOM/Minix e suporta tanto modos de jogo **Singleplayer** quanto **Multiplayer (local e remoto)**, com diferentes variações e desafios.


---

## 🧱 Estrutura do Projeto

O projeto está dividido em múltiplos módulos organizados por diretórios:

* `main.c` – ponto de entrada da aplicação e gestão dos diferentes estados (menus, jogo, multiplayer, etc).
* `game/` – contém a lógica principal do jogo.

  * `elements/` – inclui os componentes essenciais como:

    * `apple.c/.h` – lógica da maçã.
    * `freesnake.c/.h` – libertação de memória associada à cobra.
    * `snake.c/.h` – implementação da cobra e sua lógica.
  * `gamestate.c/.h` – gestão do estado do jogo.
  * `score.c/.h` – sistema de pontuação.
  * `UTILS.h` – funções auxiliares e estruturas comuns.
* `input/` – processamento das entradas do utilizador:

  * `input.c/.h` – trata eventos do teclado e rato.
* `menu/` – gestão e navegação dos menus:

  * `menu.c/.h` – lógica principal do menu.
  * `choose_input.c/.h` – seleção do tipo de input.
  * `local_or_remote.c/.h` – escolha entre multiplayer local ou remoto.
  * `select_mode.c/.h` – seleção do modo de jogo.
* `assets/` – recursos visuais do jogo.

  * `img/` – imagens usadas.
  * `xpm/` – versões das imagens em formato XPM.
* `controllers/` – contém os laboratórios realizados ao longo do semestre.

---

## 💻 Dispositivos de I/O Utilizados

- **Timer**: controla a atualização contínua do jogo, velocidade da cobra e tempo entre frames.
- **Teclado**: principal forma de input nos modos clássicos e multiplayer local.
- **Rato**: utilizado na navegação dos menus e controlo da cobra em certos modos.
- **Placa gráfica**: usada para desenhar o jogo, menus, cobras, maçãs e elementos visuais.
- **Serial Port**: usada no modo **Multiplayer remoto** para comunicar entre duas VMs.

---

## 🚀 Modos de Jogo

### Menu Inicial

- **Singleplayer**
- **Multiplayer**
- **Exit** (também acessível com a tecla `ESC` em qualquer menu)

### 🐍 Singleplayer

1. **Clássico**
   - Controlado via teclado (WASD).
   - Movimento em grelha.
   - Cobra pode atravessar paredes (teletransporte).
   - Apenas um tipo de maçã (aumenta comprimento).

2. **Modo `.io`**
   - Escolha entre rato ou teclado.
   - Movimento contínuo (não em grelha).
   - Tipos de maçã:
     - 🍏 **Normal**: aumenta o comprimento.
     - 💙 **Azul**: inverte os controlos.
     - ✨ **Dourada**: aumenta a velocidade.
   - 🍄 **Cogumelo especial**: aumenta o diametro da cobra.

### 👥 Multiplayer

1. **Local**
   - Duas cobras no mesmo PC.
   - Um jogador usa o rato, o outro o teclado.

2. **Remoto**
   - Requer duas VMs configuradas no **VirtualBox**.
   - Ligar a **porta serial** de uma VM à outra.
   - No host:  
     ```bash
     lcom_run proj abc(podia ser outra palavra, tem e de ser algo)
     ```
   - No cliente:  
     ```bash
     lcom_run proj remote
     ```

---

## 🌟 Funcionalidades Diferenciadoras

- **Multiplayer remoto** usando **porta serial**, uma funcionalidade avançada.
- Modos de jogo variados: clássico, io e multiplayer.
- Uso simultâneo de **teclado e rato** como controlos.
- **Movimento contínuo** fora de grelha.
- Vários tipos de maçãs com efeitos únicos.
- Interface com **menus intuitivos** e navegação simples.

