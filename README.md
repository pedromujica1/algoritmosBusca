# algoritmosBusca
Implementação de Algoritmos de Busca para a matéria de Inteligência Artificial

Esta parte da documentação descreve como acessar as imagens Docker, publicadas no Docker Hub e como executá-las.

## Pré-requisitos

- Docker instalado

O projeto também está disponível online:

- **Front-end:** https://pedromujica1.github.io/algoritmosBusca/
- **API:** https://defensive-elaine-ifpr-d62571ec.koyeb.app/

No entanto caso, queira acessar o projeto em ambiente de desenvolvimento e sugerir alterações, abaixo estão os comandos:

---


# Para rodar a API



## Executar a imagem

```bash
docker run -d -p 18080:18080 --name servidor-rainhas pedromujica1/api-rainhas:latest
```

A API ficará disponível em:

```
http://localhost:18080
```

---

# Front-end

## Publicar a imagem

Substitua `tagname` pela versão desejada.

```bash
docker push pedromujica1/frontend-rainhas:latest
```

## Executar a imagem

```bash
docker run -d -p 80:80 --name container-frontend pedromujica1/frontend-rainhas:latest
```

O front-end ficará disponível em:

```
http://localhost
```

---

# Comandos úteis

## Listar containers

```bash
docker ps
```

## Parar um container

API

```bash
docker stop servidor-rainhas
```

Front-end

```bash
docker stop container-frontend
```

## Remover um container

API

```bash
docker rm servidor-rainhas
```

Front-end

```bash
docker rm container-frontend
```

## Remover uma imagem

```bash
docker rmi pedromujica1/api-rainhas:tagname
```

```bash
docker rmi pedromujica1/frontend-rainhas:tagname
```