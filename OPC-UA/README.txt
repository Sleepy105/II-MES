O repositório da biblioteca open62541 é muito grande para inserir nesta pasta, fica aqui o link: https://github.com/open62541/open62541

-O código exemplo em Visual Studios 2019 assume que, na mesma pasta do projeto, há uma pasta "lib" que inclui o repositório inteiro do link acima.

-As funções implementadas no main.ccp são só por conveniência, havendo, para já, só funções para ler e escrever booleanos e inteiros de 16 bits (inteiros de 32 bits são fáceis de extender).

-Cada nó de OPC-UA é identificado pelo índice (Int16) e identificador (string) que podem ser determinados por qualquer ferramenta que sirva de cliente OPC-UA (UaExpert p. ex.). Os indices e identificadores dos nós não se alteram ao longo do tempo.

-Em falta: ler e escrever em arrays

-Para já não se usou a biblioteca de subscriptions visto que em princípio será usado "polling", no entanto é possível no futuro vir-se a utilizar.