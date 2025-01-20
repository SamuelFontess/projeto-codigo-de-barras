# Projeto de Leitura de Código de Barras EAN-8

Este projeto em C permite a geração e leitura de códigos de barras no formato EAN-8 a partir de imagens PBM (Portable Bitmap). Ele inclui funcionalidades para codificar identificadores em códigos de barras e decodificar códigos de barras de imagens PBM.

## Estrutura do Projeto

- `extrair_identificador.c`: Contém a lógica para extrair e decodificar o código de barras de uma imagem PBM.
- `gerar_codigo_barras.c`: Contém a lógica para gerar uma imagem PBM de um código de barras a partir de um identificador.
- `codigo_compartilhado.h`: Cabeçalho com definições e declarações de funções compartilhadas.
- `codigo_compartilhado.c`: Implementação das funções compartilhadas.

## Compilação

Para compilar os arquivos, você pode usar o GCC (ou qualquer outro compilador C). Por exemplo:

```sh
gcc -o extrair_identificador extrair_identificador.c codigo_compartilhado.c
gcc -o gerar_codigo_barras gerar_codigo_barras.c codigo_compartilhado.c
```

## Uso

### Extração de Identificador

Para extrair um identificador de uma imagem PBM contendo um código de barras EAN-8:

```sh
./extrair_identificador <nome_arquivo_pbm>
```

Exemplo:

```sh
./extrair_identificador codigo_barras.pbm
```

### Geração de Código de Barras

Para gerar uma imagem PBM de um código de barras EAN-8 a partir de um identificador:

```sh
./gerar_codigo_barras <identificador> [espacamento] [largura_area] [altura] [nome_arquivo]
```

Parâmetros:
- `identificador`: Identificador de 8 dígitos.
- `espacamento` (opcional): Espaçamento ao redor do código de barras (padrão: 4).
- `largura_area` (opcional): Largura de cada área de bit (padrão: 3).
- `altura` (opcional): Altura do código de barras (padrão: 50).
- `nome_arquivo` (opcional): Nome do arquivo de saída (padrão: `codigo_barras.pbm`).

Exemplo:

```sh
./gerar_codigo_barras 12345670
```

## Estrutura de Dados

### ImagemPBM

Estrutura para armazenar informações de uma imagem PBM:

```c
typedef struct {
    int largura;
    int altura;
    int **pixels; // Matriz de pixels (1 para preto, 0 para branco)
} ImagemPBM;
```

## Funções Principais

### Codificação e Decodificação

- `void codificar_identificador(const char *identificador, char *codigo_barras)`: Codifica um identificador em um código de barras EAN-8.
- `bool decodificar_codigo_barras(const char *codigo_barras, char *identificador)`: Decodifica um código de barras EAN-8 em um identificador.

### Manipulação de Imagens PBM

- `ImagemPBM *criar_imagem_pbm(int largura, int altura)`: Cria uma nova imagem PBM.
- `void liberar_imagem_pbm(ImagemPBM *imagem)`: Libera a memória de uma imagem PBM.
- `bool salvar_imagem_pbm(const char *nome_arquivo, ImagemPBM *imagem)`: Salva uma imagem PBM em um arquivo.
- `ImagemPBM *carregar_imagem_pbm(const char *nome_arquivo)`: Carrega uma imagem PBM de um arquivo.

## Licença

Este projeto está licenciado sob a licença MIT. Veja o arquivo [LICENSE](LICENSE) para mais detalhes.
