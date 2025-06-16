# Algoritmo de Selkow - Tree-to-Tree Editing Problem

Este projeto implementa o algoritmo de Selkow para resolver o problema de edição entre árvores (Tree-to-Tree Editing Problem). O algoritmo calcula a distância mínima de edição entre duas árvores usando operações de inserção, deleção e rotulação.

## Pré-requisitos

Para compilar e executar este projeto, você precisa ter:

- **Compilador GCC (g++)** compatível com C++17
- **Make** instalado no sistema
- Sistema operacional Windows, Linux ou macOS

### Instalação no Windows

1. **MinGW-w64** ou **CodeBlocks com MinGW**
2. **Git Bash** ou **MSYS2** para usar o comando `make`

### Verificando se os pré-requisitos estão instalados

```bash
# Verificar se o g++ está instalado
g++ --version

# Verificar se o make está instalado
make --version
```

## Como Compilar

### Opção 1: Usando Make (Recomendado)

1. Abra o terminal/prompt de comando na pasta do projeto
2. Execute o comando:

```bash
make
```

Este comando irá:
- Compilar todos os arquivos `.cpp` para arquivos objeto (`.o`)
- Linkar os arquivos objeto para criar o executável `programa`

### Opção 2: Compilação Manual

Se preferir compilar manualmente:

```bash
g++ -std=c++17 -Wall -Wextra -g -c main.cpp -o main.o
g++ -std=c++17 -Wall -Wextra -g -c arvore.cpp -o arvore.o
g++ -std=c++17 -Wall -Wextra -g -c custo.cpp -o custo.o
g++ -std=c++17 -Wall -Wextra -g -o programa main.o arvore.o custo.o
```

## Como Executar

Após a compilação bem-sucedida, execute o programa:

### No Windows:
```cmd
programa.exe
```

### No Linux/macOS:
```bash
./programa
```

## Comandos do Makefile

O projeto inclui os seguintes comandos make:

- **`make`** ou **`make all`**: Compila todo o projeto
- **`make clean`**: Remove todos os arquivos objeto (`.o`) e o executável

### Exemplo de uso completo:

```bash
# Compilar o projeto
make

# Executar o programa
./programa

# Limpar arquivos compilados
make clean

# Recompilar do zero
make clean && make
```

## Flags de Compilação

O projeto usa as seguintes flags do compilador:

- **`-std=c++17`**: Usa o padrão C++17
- **`-Wall`**: Habilita avisos comuns
- **`-Wextra`**: Habilita avisos adicionais
- **`-g`**: Inclui informações de debug

## Solução de Problemas

### Erro: "make: command not found"
- **Windows**: Instale Git Bash, MSYS2, ou use Windows Subsystem for Linux (WSL)
- **Linux**: `sudo apt-get install make` (Ubuntu/Debian) ou `sudo yum install make` (RedHat/CentOS)
- **macOS**: Instale Xcode Command Line Tools: `xcode-select --install`

### Erro: "g++: command not found"
- **Windows**: Instale MinGW-w64 ou CodeBlocks
- **Linux**: `sudo apt-get install g++` (Ubuntu/Debian)
- **macOS**: Instale Xcode Command Line Tools

### Problemas de Compilação
1. Verifique se todos os arquivos `.cpp` e `.h` estão presentes
2. Execute `make clean` seguido de `make`
3. Verifique se o compilador suporta C++17

## Sobre o Algoritmo

O **Algoritmo de Selkow** é usado para calcular a distância de edição entre duas árvores. As operações permitidas são:

1. **Inserção de nó**: Adicionar um novo nó à árvore
2. **Deleção de nó**: Remover um nó existente da árvore
3. **Rotulação**: Alterar o rótulo de um nó existente

O algoritmo encontra a sequência de operações de menor custo para transformar uma árvore na outra.

## Licença

Este projeto é desenvolvido para fins educacionais e de pesquisa.
