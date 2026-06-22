# ==========================================
# ETAPA 1: Compilação (Build) usando Alpine
# ==========================================
FROM alpine:3.19 AS builder

# Instala ferramentas de compilação corretas do Alpine
RUN apk add --no-cache \
    build-base \
    boost-dev \
    asio-dev

WORKDIR /app

# Copia os arquivos do projeto
COPY . .

# Executa o makefile (o binário será gerado nativamente para o ambiente Alpine)
RUN make

# ==========================================
# ETAPA 2: Imagem Final de Execução (Super Leve)
# ==========================================
FROM alpine:3.19

WORKDIR /app

# Instala apenas as dependências de execução em runtime
RUN apk add --no-cache \
    libstdc++ \
    boost-system

# Copia apenas o executável final gerado na Etapa 1
COPY --from=builder /app/servidor .

EXPOSE 8000

CMD ["./servidor"]