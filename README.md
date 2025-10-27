# imd0039-Àrvores AVL e Robro-Negra
Projeto desenvolvido para a disciplina IMD0039 - Estrutura de Dados II, com o objetivo de implementar um programa que implemente árvores AVL e Rubro-negra.

# 📖 Sobre o Projeto
Este projeto consiste em um programa desenvolvido em C++ para implementar dois tipos de árvore binaria de busca balanceada: AVL e Rubro-negra.

# ⚙️ Pré-requisitos
Para a execução do projeto, você deverá ter os seguintes componentes instalados no seu sistema:

- C++ versão 11 ou superior(compilador gcc/g++)
- CMake 3.16 ou superior
- **[Raylib](https://github.com/raysan5/raylib)**

# 🚀 Como compilar
- Clone o repositório

```bash
    git clone https://github.com/ivysonnn/b-trees.git 
    cd b-trees
```

- Compile o projeto com CMake

```bash
# Crie o repositório para build do projeto
mkdir build

cd build

# Configure o projeto
cmake ..

# Compile o projeto
cmake --build .
```



## </> Modo de Uso

### Executar o programa

| Argumento (`<flag>`) | Descrição |
| :------------------- | :---------- |
| **`-rb`** | Inicia o programa utilizando a estrutura de **Árvore Rubro-Negra**. |
| **`-avl`** | Inicia o programa utilizando a estrutura de **Árvore AVL**. |

**Exemplo:**
```bash
./btree -avl
```

### Dentro do programa

Após a execução inicial (`./btree -rb` ou `./btree -avl`), o programa entrará no modo interativo, onde você pode usar as seguintes teclas:

| Tecla | Comando | Descrição |
| :---- | :-------- | :---------- |
| **\[ESPAÇO]** | **Gerar Aleatório** | Gera um valor aleatório e o **insere** na Árvore. |
| **`A`** ou **`a`** | **Inserir Dado** | Solicita ao usuário um valor para ser **inserido** na árvore. |
| **`R`** ou **`r`** | **Remover Dado** | Solicita ao usuário um valor para ser **removido** da árvore.|

# 👨‍💻 Alunos
Allan Victor - **[@Allan073](https://github.com/Allan073)**

Chistian Daniel - **[@ChisSilva](https://github.com/ChisSilva)**

Ivyson Lucas - **[@ivysonnn](https://github.com/ivysonnn)**
