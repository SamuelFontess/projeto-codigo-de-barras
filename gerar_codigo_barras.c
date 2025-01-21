#include "codigo_compartilhado.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LARGURA_AREA_PADRAO 3
#define ALTURA_PADRAO 50
#define ESPACAMENTO_PADRAO 4
#define NOME_ARQUIVO_PADRAO "codigo_barras.pbm"
#define TAM_CODIGO 67

void preencher_codigo_barras(ImagemPBM *imagem, const char *codigo_barras, int largura_area, int altura, int espacamento) {
    int largura_total = strlen(codigo_barras) * largura_area;
    int inicio_x = espacamento;
    int inicio_y = espacamento;

    for (int i = 0; i < strlen(codigo_barras); i++) {
        int cor = codigo_barras[i] - '0';
        for (int y = inicio_y; y < inicio_y + altura; y++) {
            for (int x = inicio_x + i * largura_area; x < inicio_x + (i + 1) * largura_area; x++) {
                imagem->pixels[y][x] = cor;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <identificador> [espacamento] [largura_area] [altura] [nome_arquivo]\n", argv[0]);
        return 1;
    }

    const char *identificador = argv[1];
    int espacamento = (argc > 2) ? atoi(argv[2]) : ESPACAMENTO_PADRAO;
    int largura_area = (argc > 3) ? atoi(argv[3]) : LARGURA_AREA_PADRAO;
    int altura = (argc > 4) ? atoi(argv[4]) : ALTURA_PADRAO;
    const char *nome_arquivo = (argc > 5) ? argv[5] : NOME_ARQUIVO_PADRAO;

    if (!validar_identificador(identificador)) {
        fprintf(stderr, "Erro: Identificador inválido! Certifique-se de que ele tem 8 dígitos válidos.\n");
        return 1;
    }

    // Verificar se o arquivo já existe
    FILE *arquivo_existente = fopen(nome_arquivo, "r");
    if (arquivo_existente != NULL) {
        fclose(arquivo_existente);
        char resposta;
        printf("O arquivo %s já existe. Deseja sobrescrevê-lo? (s/n): ", nome_arquivo);
        scanf(" %c", &resposta);
        if (resposta != 's' && resposta != 'S') {
            fprintf(stderr, "Erro: arquivo resultante já existe.\n");
            return 1;
        }
    }

    char codigo_barras[TAM_CODIGO + 1];
    codificar_identificador(identificador, codigo_barras);

    int largura_imagem = strlen(codigo_barras) * largura_area + 2 * espacamento;
    int altura_imagem = altura + 2 * espacamento;

    ImagemPBM *imagem = criar_imagem_pbm(largura_imagem, altura_imagem);
    if (!imagem) {
        fprintf(stderr, "Erro: Não foi possível criar a imagem PBM.\n");
        return 1;
    }

    preencher_codigo_barras(imagem, codigo_barras, largura_area, altura, espacamento);

    if (!salvar_imagem_pbm(nome_arquivo, imagem)) {
        fprintf(stderr, "Erro: Não foi possível salvar a imagem no arquivo %s.\n", nome_arquivo);
        liberar_imagem_pbm(imagem);
        return 1;
    }

    printf("Código de barras salvo em %s\n", nome_arquivo);
    liberar_imagem_pbm(imagem);
    return 0;
}