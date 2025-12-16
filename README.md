*Driver de Caractere – Gerador de Números Pseudoaleatórios*

Esse é um driver de caractere que eu desenvolvi para o meu projeto de Sistemas Operacionais. A ideia é simples: sempre que você lê o dispositivo, ele gera e retorna um número pseudoaleatório.

Esse trabalho foi feito para entender melhor como funciona a criação de drivers no Linux e como gerar números pseudoaleatórios no kernel, que é basicamente o que esse driver faz.

O que esse projeto faz?

Esse driver permite que você leia um número pseudoaleatório de um arquivo especial no sistema (em /dev/gerador). Ele usa um algoritmo simples de congruência linear para gerar os números e é bem básico, mas foi uma boa oportunidade para aprender como criar drivers no Linux.

Como usar?
Passo 1: Instalar as dependências

Antes de compilar, se você ainda não tem as dependências necessárias, execute o comando abaixo para instalar os pacotes essenciais para compilar módulos no kernel:

sudo apt update
sudo apt install build-essential linux-headers-$(uname -r)

Passo 2: Compilar o driver

Depois de instalar as dependências, basta compilar o driver com o make:

make


Isso vai gerar o arquivo gerador.ko, que é o módulo que você vai carregar no kernel.

Passo 3: Carregar o módulo

Agora, é só carregar o módulo com o comando insmod:

sudo insmod gerador.ko salt=10


O parâmetro salt pode ser passado para variar a sequência dos números gerados. Se você não passar, ele usa o valor padrão de 1.

Passo 4: Criar o arquivo de dispositivo

Você também vai precisar criar o dispositivo em /dev para conseguir acessar o driver:

sudo mknod /dev/gerador c 240 0
sudo chmod 666 /dev/gerador


Com isso, o dispositivo /dev/gerador estará disponível para leitura.

Passo 5: Testar

Agora, para testar, basta rodar o comando cat:

cat /dev/gerador


Isso vai gerar um número pseudoaleatório. A cada vez que você rodar o cat, um número diferente será gerado.

Passo 6: Remover o driver

Quando terminar, você pode remover o módulo e o dispositivo com:

sudo rmmod gerador
sudo rm /dev/gerador
make clean

Como funciona?

O driver gera números pseudoaleatórios usando um algoritmo simples de congruência linear. Quando você lê o dispositivo, ele gera um número novo e retorna esse número como uma string, que pode ser visualizada no terminal.

Eu usei o salt como um parâmetro de inicialização, para garantir que a sequência de números não seja a mesma toda vez que o módulo for carregado. Ele é combinado com o valor de jiffies, que é um contador do sistema, para garantir que a sequência mude ao longo do tempo.

Observações

A função read é onde o número aleatório é gerado e retornado.

A escrita no dispositivo não é utilizada, mas a função write foi implementada para manter a interface de driver de caractere.

O offset é usado para evitar loop infinito quando você utiliza o comando cat para ler o dispositivo.

Licença

Este projeto é licenciado sob a GPL v2.
