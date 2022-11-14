# Projeto 1 : Cliente e Servidor TCP

Esse projeto trata-se de um conjunto de Cliente/Servidor com a finalidade
de treinar e aperfeiçoar o conhecimento sobre Redes de Computadores.

Para gerar os executaveis utilize o comando:

Uso: `make`

Para limpar:

Uso: `make clean`

## Servidor

Uso: `./servidor`

> O servidor utiliza o endereço localhost como padrão.

## Cliente

Uso: `./cliente <IPaddress>`

## Funcionalidade

O servidor atende um cliente por vez. A cada conexão ele envia um hello ao cliente, recebe uma mensagem e encerra a conexão.

O cliente por sua vez, se conecta com o servidor, recebe uma mensagem, aguarda o usuário digitar uma mensagem e a envia ao servidor.

> O cliente e o servidor utilizam uma porta padrão para se comunicarem, no código fonte é utilizado a porta '2022'.
