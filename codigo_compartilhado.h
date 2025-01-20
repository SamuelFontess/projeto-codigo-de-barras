// codigo_barras.h
#ifndef CODIGO_BARRAS_H
#define CODIGO_BARRAS_H

#include <stdbool.h>

#define L_CODE {"0001101", "0011001", "0010011", "0111101", "0100011", "0110001", "0101111", "0111011", "0110111", "0001011"}
#define R_CODE {"1110010", "1100110", "1101100", "1000010", "1011100", "1001110", "1010000", "1000100", "1001000", "1110100"}

// Struct para armazenar informações de uma imagem PBM
typedef struct {
    int largura;
    int altura;
    int **pixels; // Matriz de pixels (1 para preto, 0 para branco)
} ImagemPBM;

// Funções para manipulação de identificadores
bool validar_identificador(const char *identificador);
char calcular_digito_verificador(const char *identificador);

// Funções para codificação e decodificação de códigos de barras
void codificar_identificador(const char *identificador, char *codigo_barras);
bool decodificar_codigo_barras(const char *codigo_barras, char *identificador);

// Funções para manipulação de arquivos PBM
ImagemPBM *criar_imagem_pbm(int largura, int altura);
void liberar_imagem_pbm(ImagemPBM *imagem);
bool salvar_imagem_pbm(const char *nome_arquivo, ImagemPBM *imagem);
ImagemPBM *carregar_imagem_pbm(const char *nome_arquivo);

#endif // CODIGO_BARRAS_H