
FROM alpine:3.19 AS builder

#Instala ferramentas de compilação corretas do Alpine
RUN apk add --no-cache \
    build-base \
    boost-dev \
    asio-dev

WORKDIR /app

#Copia os arquivos do projeto
COPY . .

#executa o makefile (o binário será gerado nativamente para o ambiente Alpine)
RUN make


FROM alpine:3.19

WORKDIR /app

#instala apenas as dependências de execução em runtime
RUN apk add --no-cache \
    libstdc++ \
    boost-system

#copia apenas o executável final gerado na Etapa 1
COPY --from=builder /app/servidor .

EXPOSE 8000

CMD ["./servidor"]