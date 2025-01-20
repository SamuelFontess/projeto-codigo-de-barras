#include "codigo_compartilhado.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NUM_DIGITOS 7
#define TAM_CODIGO 67

bool verificar_codigo_barras(ImagemPBM *imagem, char *codigo_barras) {
    int largura = imagem->largura;
    int altura = imagem->altura;

    // Determinar o espaçamento lateral e altura do código baseado nos pixels
    int inicio_x = -1, fim_x = -1;
    for (int x = 0; x < largura; x++) {
        for (int y = 0; y < altura; y++) {
            if (imagem->pixels[y][x] == 1) {
                if (inicio_x == -1) inicio_x = x;
                fim_x = x;
            }
        }
    }

    if (inicio_x == -1 || fim_x == -1) return false; // Nenhum código encontrado

    int largura_area = (fim_x - inicio_x + 1) / TAM_CODIGO;

    // Verificar e reconstruir o código de barras
    for (int i = 0; i < TAM_CODIGO; i++) {
        int soma_pixels = 0;
        for (int x = inicio_x + i * largura_area; x < inicio_x + (i + 1) * largura_area; x++) {
            for (int y = 0; y < altura; y++) {
                soma_pixels += imagem->pixels[y][x];
            }
        }
        codigo_barras[i] = (soma_pixels > (altura * largura_area / 2)) ? '1' : '0';
    }
    codigo_barras[TAM_CODIGO] = '\0';
    return true;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <nome_arquivo_pbm>\n", argv[0]);
        return 1;
    }

    const char *nome_arquivo = argv[1];
    ImagemPBM *imagem = carregar_imagem_pbm(nome_arquivo);
    if (!imagem) {
        fprintf(stderr, "Erro: Não foi possível carregar o arquivo %s ou o arquivo não é um PBM válido.\n", nome_arquivo);
        return 1;
    }

    char codigo_barras[TAM_CODIGO + 1];
    if (!verificar_codigo_barras(imagem, codigo_barras)) {
        fprintf(stderr, "Erro: Nenhum código de barras foi encontrado na imagem.\n");
        liberar_imagem_pbm(imagem);
        return 1;
    }

    char identificador[NUM_DIGITOS + 2];
    if (!decodificar_codigo_barras(codigo_barras, identificador)) {
        fprintf(stderr, "Erro: O código de barras encontrado é inválido ou corrompido.\n");
        liberar_imagem_pbm(imagem);
        return 1;
    }

    printf("Identificador extraído: %s\n", identificador);
    liberar_imagem_pbm(imagem);
    return 0;
}