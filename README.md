# off-mesh-parser-c

Um programa em C que lê arquivos de malhas 3D no formato Object File Format (.off) utilizando uma estrutura de dados de Face-Vértice, e exporta projeções 2D dessas malhas em formato SVG.

## Arquivos
- `malha_face_vertice.c`: O código-fonte principal. Ele lê os arquivos `.off` e os exporta para `.svg`.
- `triangles.off`: Um arquivo de malha de exemplo contendo uma geometria simples de triângulos.
- `hand-hybrid.off`: Um arquivo de malha de exemplo contendo uma geometria mais complexa de uma mão.
- `triangles.svg`: Projeção 2D gerada a partir do `triangles.off`.
- `hand-hybrid.svg`: Projeção 2D gerada a partir do `hand-hybrid.off`.

## Como Compilar e Executar
Utilize o GCC ou qualquer compilador C padrão para compilar o programa:
```bash
gcc malha_face_vertice.c -o malha
```

Para executar o programa:
```bash
./malha
```
(No Windows: `malha.exe`)

O programa irá carregar automaticamente os arquivos `triangles.off` e `hand-hybrid.off`, imprimir um resumo dos seus vértices e faces no terminal, e gerar os arquivos `.svg` correspondentes.
