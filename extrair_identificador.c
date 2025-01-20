#include "codigo_compartilhado.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#endif

#define NUM_DIGITOS 7
#define TAM_CODIGO 67

bool verificar_codigo_barras(ImagemPBM *imagem, char *codigo_barras) {
    int largura = imagem->largura;
    int altura = imagem->altura;

    // Find the actual code region by detecting the first and last black pixels
    int inicio_x = -1, fim_x = -1;
    int linha_media = altura / 2;  // Use middle row for better detection
    
    // Find start and end positions
    for (int x = 0; x < largura; x++) {
        if (imagem->pixels[linha_media][x] == 1) {
            if (inicio_x == -1) inicio_x = x;
            fim_x = x;
        }
    }

    if (inicio_x == -1 || fim_x == -1) return false;

    // Calculate width per bit based on the total width of the code
    double largura_bit = (double)(fim_x - inicio_x + 1) / TAM_CODIGO;

    // Read each bit using the calculated width
    for (int i = 0; i < TAM_CODIGO; i++) {
        int x_inicio = inicio_x + (int)(i * largura_bit);
        int x_fim = inicio_x + (int)((i + 1) * largura_bit);
        
        int pretos = 0, total = 0;
        for (int x = x_inicio; x < x_fim; x++) {
            for (int y = altura/4; y < (3*altura)/4; y++) {  // Use middle 50% of height
                total++;
                if (imagem->pixels[y][x] == 1) pretos++;
            }
        }
        
        codigo_barras[i] = (pretos > total/2) ? '1' : '0';
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

    // Using binary mode for stdout to avoid encoding issues
    #ifdef _WIN32
    _setmode(_fileno(stdout), _O_BINARY);
    #endif
    
    printf("Identificador extraído: %s\n", identificador);
    fflush(stdout);
    
    liberar_imagem_pbm(imagem);
    return 0;
}