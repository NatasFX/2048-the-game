# 2048 The Game

Um jogo de 2048 feito em C com [Allegro 5](https://liballeg.org/). Este é um
projeto de arquivo: o código e os assets originais foram preservados como
foram encontrados, inclusive suas datas de modificação.

## Gameplay

![Colagem de momentos do gameplay de 2048 The Game](assets/gameplay-montage.jpg)

## Executar

No Ubuntu/Debian, instale as dependências de compilação:

```sh
sudo apt install build-essential pkg-config liballegro5-dev
```

Depois, na raiz do projeto:

```sh
make
./allegro
```

`make run` compila, quando necessário, e inicia o jogo.

## Controles

- Use o mouse ou as setas esquerda/direita para escolher a coluna.
- Use a seta para baixo para acelerar a queda.
- Pressione `Esc` para abrir o menu de pausa.

O placar local é criado como `ranking.txt` ao executar o jogo e não é
versionado.

## Estrutura

- `allegro.c` — fonte original do jogo.
- `source/` — fontes, imagens, animações e áudio usados pelo jogo.
- `Makefile` — atalho atual para compilação local, sem alterar a fonte.

## Nota sobre licenciamento

Não há uma licença explícita nos arquivos originais. Portanto, este
repositório não concede permissão adicional para reutilização do código ou
dos assets. As faixas musicais presentes em `source/mus/` permanecem sujeitas
aos direitos de seus respectivos titulares.
