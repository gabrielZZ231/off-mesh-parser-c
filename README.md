# Processador de Malhas OFF em C (Face-Vértice)

Este projeto consiste em um parser desenvolvido em C que implementa a estrutura de dados topológica do tipo **Face-Vértice**. Seu objetivo principal é processar e carregar malhas poligonais genéricas (incluindo triângulos, quadriláteros ou polígonos mistos). A ferramenta foi construída como parte de um trabalho prático para a disciplina de Computação Gráfica, sob a orientação do professor Icaro Cunha.

A aplicação lê as coordenadas espaciais e as relações topológicas a partir de arquivos 3D no formato padrão `.off` (Object File Format), instanciando-os na memória. O projeto também entrega as seguintes funcionalidades:

- **Relatório de Conectividade:** Exibe um resumo estrutural direto no terminal, contendo os dados gerais e informações de topologia da malha lida.
- **Geração Dinâmica de SVG:** Executa a projeção e o desenho das formas em 2D nativamente, salvando o resultado em arquivos vetoriais (`.svg`). Isso é feito do zero, sem a necessidade de importar nenhuma biblioteca gráfica externa para o C.

## A Estrutura Face-Vértice

Nesta representação, o mapeamento do objeto é feito através de duas listas principais que se comunicam através de índices:

1. **Lista de Vértices:** Guarda as coordenadas exatas no espaço 3D (x, y, z) de cada ponto. Além disso, mantém uma lista auxiliar dinâmica que aponta para quais faces estão utilizando aquele vértice.
2. **Lista de Faces:** Contém uma referência (os índices) dos vértices que formam o seu polígono. Essa abordagem permite montar figuras com três, quatro ou N lados facilmente.

## Como Compilar e Executar

Para compilar o código fonte, utilize o GCC ou um compilador compatível com C em seu terminal:

```bash
gcc malha_face_vertice.c -o processador_malhas
```

E então, rode a aplicação:

```bash
./processador_malhas
```
*(No Windows, execute através de `.\processador_malhas.exe` ou `processador_malhas.exe`)*

Ao ser executado, o script já lerá os arquivos em anexo (`triangles.off` e `hand-hybrid.off`), gerando seus resumos no terminal e exportando automaticamente os desenhos `.svg`.
