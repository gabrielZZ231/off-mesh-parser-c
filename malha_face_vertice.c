#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ---------------------------------------------------------
// 1. ESTRUTURAS DE DADOS (Face-Vértice)
// ---------------------------------------------------------

typedef struct {
    float x, y, z;      // Posição espacial do vértice
    int *faces;         // Lista com os índices das faces a qual o vértice pertence
    int num_faces;      // Quantidade atual de faces nesta lista
    int capacity;       // Capacidade de memória alocada para o array 'faces'
} Vertice;

typedef struct {
    int *vertices;      // Lista com os índices dos vértices que compõem a face
    int num_vertices;   // Quantidade de vértices nesta face (ex: 3 p/ triângulo, 4 p/ quad)
} Face;

typedef struct {
    Vertice *vertices;  // Tabela de vértices
    int num_vertices;   // Total de vértices na malha
    Face *faces;        // Tabela de faces
    int num_faces;      // Total de faces na malha
} Malha;


// ---------------------------------------------------------
// 2. FUNÇÕES AUXILIARES DE MEMÓRIA
// ---------------------------------------------------------

void adicionar_face_ao_vertice(Vertice *v, int indice_face) {
    if (v->num_faces >= v->capacity) {
        v->capacity = (v->capacity == 0) ? 4 : v->capacity * 2;
        v->faces = realloc(v->faces, v->capacity * sizeof(int));
    }
    v->faces[v->num_faces] = indice_face;
    v->num_faces++;
}

void liberar_malha(Malha *malha) {
    if (!malha) return;

    for (int i = 0; i < malha->num_vertices; i++) {
        if (malha->vertices[i].faces) free(malha->vertices[i].faces);
    }
    free(malha->vertices);

    for (int i = 0; i < malha->num_faces; i++) {
        if (malha->faces[i].vertices) free(malha->faces[i].vertices);
    }
    free(malha->faces);

    free(malha);
}

// ---------------------------------------------------------
// 3. LEITURA DO ARQUIVO OFF
// ---------------------------------------------------------

Malha* ler_arquivo_off(const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        printf("Erro: Nao foi possivel abrir o arquivo %s\n", nome_arquivo);
        return NULL;
    }

    char cabecalho[10];
    if (fscanf(arquivo, "%s", cabecalho) != 1 || strcmp(cabecalho, "OFF") != 0) {
        printf("Erro: O arquivo %s nao esta no formato OFF valido.\n", nome_arquivo);
        fclose(arquivo);
        return NULL;
    }

    Malha *malha = (Malha*)malloc(sizeof(Malha));
    int num_arestas; 
    fscanf(arquivo, "%d %d %d", &malha->num_vertices, &malha->num_faces, &num_arestas);

    malha->vertices = (Vertice*)calloc(malha->num_vertices, sizeof(Vertice));
    malha->faces = (Face*)calloc(malha->num_faces, sizeof(Face));

    for (int i = 0; i < malha->num_vertices; i++) {
        fscanf(arquivo, "%f %f %f", &malha->vertices[i].x, &malha->vertices[i].y, &malha->vertices[i].z);
    }

    for (int i = 0; i < malha->num_faces; i++) {
        int n_vertices_na_face;
        fscanf(arquivo, "%d", &n_vertices_na_face);
        
        malha->faces[i].num_vertices = n_vertices_na_face;
        malha->faces[i].vertices = (int*)malloc(n_vertices_na_face * sizeof(int));

        for (int j = 0; j < n_vertices_na_face; j++) {
            int indice_vertice;
            fscanf(arquivo, "%d", &indice_vertice);
            malha->faces[i].vertices[j] = indice_vertice;
            adicionar_face_ao_vertice(&malha->vertices[indice_vertice], i);
        }
    }

    fclose(arquivo);
    return malha;
}

// ---------------------------------------------------------
// 4. EXPORTAÇÃO PARA SVG (NOVO)
// ---------------------------------------------------------

void exportar_para_svg(Malha *malha, const char *nome_arquivo) {
    if (!malha || malha->num_vertices == 0) return;

    float min_x = malha->vertices[0].x, max_x = malha->vertices[0].x;
    float min_y = malha->vertices[0].y, max_y = malha->vertices[0].y;

    for (int i = 1; i < malha->num_vertices; i++) {
        if (malha->vertices[i].x < min_x) min_x = malha->vertices[i].x;
        if (malha->vertices[i].x > max_x) max_x = malha->vertices[i].x;
        if (malha->vertices[i].y < min_y) min_y = malha->vertices[i].y;
        if (malha->vertices[i].y > max_y) max_y = malha->vertices[i].y;
    }

    float width = max_x - min_x;
    float height = max_y - min_y;
    if (width == 0) width = 1;
    if (height == 0) height = 1;

    float scale_x = 800.0f / width;
    float scale_y = 800.0f / height;
    float scale = (scale_x < scale_y) ? scale_x : scale_y;
    scale *= 0.9f; 

    float offset_x = 400.0f - (min_x + width / 2.0f) * scale;
    float offset_y = 400.0f - (min_y + height / 2.0f) * scale;

    char out_filename[256];
    strncpy(out_filename, nome_arquivo, sizeof(out_filename));
    char *dot = strrchr(out_filename, '.');
    if (dot) *dot = '\0';
    strcat(out_filename, ".svg");

    FILE *f = fopen(out_filename, "w");
    if (!f) {
        printf("Erro ao criar o arquivo SVG %s\n", out_filename);
        return;
    }

    fprintf(f, "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"800\" height=\"800\" style=\"background: white;\">\n");

    for (int i = 0; i < malha->num_faces; i++) {
        fprintf(f, "  <polygon points=\"");
        for (int j = 0; j < malha->faces[i].num_vertices; j++) {
            int v_idx = malha->faces[i].vertices[j];
            float vx = malha->vertices[v_idx].x * scale + offset_x;
            float vy = 800.0f - (malha->vertices[v_idx].y * scale + offset_y);
            fprintf(f, "%.2f,%.2f ", vx, vy);
        }
        fprintf(f, "\" fill=\"#add8e6\" stroke=\"#00008b\" stroke-width=\"1\" stroke-linejoin=\"round\" />\n");
    }

    fprintf(f, "</svg>\n");
    fclose(f);
    printf(">> Imagem 2D gerada com sucesso: %s\n", out_filename);
}

// ---------------------------------------------------------
// 5. PROGRAMA PRINCIPAL
// ---------------------------------------------------------

void imprimir_resumo(Malha *malha, const char *nome_arquivo) {
    if (!malha) return;
    printf("\n--- Resumo de: %s ---\n", nome_arquivo);
    printf("Total de Vertices: %d\n", malha->num_vertices);
    printf("Total de Faces: %d\n", malha->num_faces);
}

int main(int argc, char *argv[]) {
    const char* arquivo1 = "triangles.off";
    const char* arquivo2 = "hand-hybrid.off";

    printf("Carregando as malhas...\n");

    Malha *malha1 = ler_arquivo_off(arquivo1);
    if(malha1) {
        imprimir_resumo(malha1, arquivo1);
        exportar_para_svg(malha1, arquivo1);
        liberar_malha(malha1);
    }

    Malha *malha2 = ler_arquivo_off(arquivo2);
    if(malha2) {
        imprimir_resumo(malha2, arquivo2);
        exportar_para_svg(malha2, arquivo2);
        liberar_malha(malha2);
    }

    return 0;
}