# Projeto 4 : Chat entre clientes

Esse projeto trata-se de um conjunto de Cliente/Servidor com a finalidade
de treinar e aperfeiçoar o conhecimento sobre Redes de Computadores.

Para gerar os executaveis utilize o comando:

`make`

Para limpar:

`make clean`

## Servidor:

uso: `./servidor <Port>`

> O servidor utiliza o endereço localhost como padrão.

## Cliente:

uso: `./cliente <IPaddress> <Port_1> <Port_2>`

## Funcionalidade

O servidor concorrente envia um “Hello” ao cliente, aguarda receber mensagens e as enviam de volta em forma de “eco”.
Após terminar as mensagens, o servidor finaliza a conexão.

O cliente por sua vez, trata-se de um Multiplexador de Entrada/Saída, ele se conecta com dois servidores e ainda recebe uma mensagem através do terminal.
A multiplaxação funciona de forma a permitir que ele consiga alternar entre a leitura de dados de 3 fontes distintas e ao envio de pacotes com dois servidores.

## Exemplo

Execução do cliente, dado a entrada in.txt:

`./cliente 127.0.0.1 3000 3001 < in.txt > out.txt`
