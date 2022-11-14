# Projeto 2 : Servidor TCP Concorrente

Esse projeto trata-se de um conjunto de Cliente/Servidor com a finalidade
de treinar e aperfeiçoar o conhecimento sobre Redes de Computadores.

Para gerar os executaveis utilize o comando:

Uso: `make`

Para limpar:

Uso: `make clean`

## Servidor

Uso: `./servidor <Port> <Backlog>`

> O servidor utiliza o endereço localhost como padrão.

## Cliente

Uso: `./cliente <IPaddress> <Port>`

## Script

O script desenvolvido executa de forma simultanea 10 clientes.

Uso: `./script.sh <PORT>`

> É utilizado localhost como endereço para o servidor.

## Funcionalidade

O servidor concorrente possui uma lista de comandos que obtem informação nativa do sistema Linux em que o cliente está em execução.
Após o emparelhamento com o cliente, ele envia 3 desses comandos de forma aleatoria, espera o retorno do comando e salva em "result.txt" com as informações do cliente.
Ele é capaz de atender a multiplos clientes de forma concorrente, tendo o Backlog informado como parametro.

O cliente por sua vez, se conecta com o servidor, rebece 3 comandos para serem executados localmente e encaminha a saida dos comandos ao servidor.
Terminado essa operação, o cliente é encerrado.
