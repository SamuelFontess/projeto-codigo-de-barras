// codigo_barras.c
#include "codigo_compartilhado.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NUM_DIGITOS 7
#define TAM_CODIGO 67

// Funções auxiliares
static int calcular_soma_ponderada(const char *identificador) {
    int soma = 0;
    for (int i = 0; i < NUM_DIGITOS; i++) {
        int peso = (i % 2 == 0) ? 3 : 1;
        soma += peso * (identificador[i] - '0');
    }
    return soma;
}

// Funções para manipulação de identificadores
bool validar_identificador(const char *identificador) {
    if (strlen(identificador) != NUM_DIGITOS + 1) {
        return false;
    }
    for (int i = 0; i < NUM_DIGITOS + 1; i++) {
        if (!isdigit(identificador[i])) {
            return false;
        }
    }
    char digito_verificador = calcular_digito_verificador(identificador);
    return digito_verificador == identificador[NUM_DIGITOS];
}

char calcular_digito_verificador(const char *identificador) {
    int soma = calcular_soma_ponderada(identificador);
    int proximo_multiplo = ((soma + 9) / 10) * 10;
    return (char)((proximo_multiplo - soma) + '0');
}

// Funções para codificação e decodificação de códigos de barras
void codificar_identificador(const char *identificador, char *codigo_barras) {
    const char *l_code[] = L_CODE;
    const char *r_code[] = R_CODE;

    strcpy(codigo_barras, "101"); // Marcador inicial
    for (int i = 0; i < 4; i++) {
        strcat(codigo_barras, l_code[identificador[i] - '0']);
    }
    strcat(codigo_barras, "01010"); // Marcador central
    for (int i = 4; i < 8; i++) {
        strcat(codigo_barras, r_code[identificador[i] - '0']);
    }
    strcat(codigo_barras, "101"); // Marcador final
}

bool decodificar_codigo_barras(const char *codigo_barras, char *identificador) {
    if (strlen(codigo_barras) != TAM_CODIGO) {
        return false;
    }
    const char *l_code[] = L_CODE;
    const char *r_code[] = R_CODE;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 10; j++) {
            if (strncmp(&codigo_barras[3 + i * 7], l_code[j], 7) == 0) {
                identificador[i] = '0' + j;
                break;
            }
        }
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 10; j++) {
            if (strncmp(&codigo_barras[32 + i * 7], r_code[j], 7) == 0) {
                identificador[4 + i] = '0' + j;
                break;
            }
        }
    }
    identificador[8] = '\0';
    return true;
}

// Funções para manipulação de arquivos PBM
ImagemPBM *criar_imagem_pbm(int largura, int altura) {
    ImagemPBM *imagem = (ImagemPBM *)malloc(sizeof(ImagemPBM));
    if (!imagem) return NULL;

    imagem->largura = largura;
    imagem->altura = altura;
    imagem->pixels = (int **)malloc(altura * sizeof(int *));
    if (!imagem->pixels) {
        free(imagem);
        return NULL;
    }
    for (int i = 0; i < altura; i++) {
        imagem->pixels[i] = (int *)calloc(largura, sizeof(int));
        if (!imagem->pixels[i]) {
            for (int j = 0; j < i; j++) {
                free(imagem->pixels[j]);
            }
            free(imagem->pixels);
            free(imagem);
            return NULL;
        }
    }
    return imagem;
}

void liberar_imagem_pbm(ImagemPBM *imagem) {
    if (!imagem) return;
    for (int i = 0; i < imagem->altura; i++) {
        free(imagem->pixels[i]);
    }
    free(imagem->pixels);
    free(imagem);
}

bool salvar_imagem_pbm(const char *nome_arquivo, ImagemPBM *imagem) {
    FILE *arquivo = fopen(nome_arquivo, "w");
    if (!arquivo) return false;

    fprintf(arquivo, "P1\n%d %d\n", imagem->largura, imagem->altura);
    for (int i = 0; i < imagem->altura; i++) {
        for (int j = 0; j < imagem->largura; j++) {
            fprintf(arquivo, "%d ", imagem->pixels[i][j]);
        }
        fprintf(arquivo, "\n");
    }
    fclose(arquivo);
    return true;
}

ImagemPBM *carregar_imagem_pbm(const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) return NULL;

    char tipo[3];
    int largura, altura;
    if (fscanf(arquivo, "%2s", tipo) != 1 || strcmp(tipo, "P1") != 0) {
        fclose(arquivo);
        return NULL;
    }
    if (fscanf(arquivo, "%d %d", &largura, &altura) != 2) {
        fclose(arquivo);
        return NULL;
    }

    ImagemPBM *imagem = criar_imagem_pbm(largura, altura);
    if (!imagem) {
        fclose(arquivo);
        return NULL;
    }

    for (int i = 0; i < altura; i++) {
        for (int j = 0; j < largura; j++) {
            if (fscanf(arquivo, "%d", &imagem->pixels[i][j]) != 1) {
                liberar_imagem_pbm(imagem);
                fclose(arquivo);
                return NULL;
            }
        }
    }
    fclose(arquivo);
    return imagem;
}