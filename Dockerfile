FROM nginx:alpine-slim

# O ponto (.) indica para o Docker copiar TUDO da pasta atual para dentro do Nginx
COPY . /usr/share/nginx/html/

EXPOSE 80