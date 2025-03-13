FROM gcc

WORKDIR /CodigoDeBarras

COPY . .

RUN gcc -c codigo_compartilhado.c -o codigo_compartilhado.o
RUN gcc -c extrair_identificador.c -o extrair_identificador.o
RUN gcc -c gerar_codigo_barras.c -o gerar_codigo_barras.o

RUN gcc -o ProgramaExtrair codigo_compartilhado.o extrair_identificador.o
RUN gcc -o ProgramaGerar codigo_compartilhado.o gerar_codigo_barras.o

CMD ["./ProgramaGerar"]