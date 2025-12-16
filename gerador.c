#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define NOME_DISPOSITIVO "gerador"
#define MAJOR_NUMERO 240
#define MINOR_NUMERO 0

// salt passado na carga do modulo para evitar repeticao da sequencia
static unsigned int salt = 1;
module_param(salt, uint, 0644);

// estado interno do gerador pseudo-aleatorio
static unsigned int estado_prng;

//gerador simples baseado em congruencia linear
static unsigned int gerar_numero(void){
    estado_prng = estado_prng * 1103515245 + 12345 + salt;
    return estado_prng;
}

static int gerador_open(struct inode *inode, struct file *file)
{
    return 0;
}

static int gerador_release(struct inode *inode, struct file *file){
    return 0;
}

static ssize_t gerador_read(struct file *file, char __user *buffer,
                         size_t tamanho, loff_t *offset)
{
    char dados[32];
    int tamanho_dados;
    unsigned int numero;

    // evita loop infinito
    if (*offset > 0)
        return 0;

    numero = gerar_numero();
    tamanho_dados = sprintf(dados, "%u\n", numero);

    // copia os dados do kernel para o espaco de usuario
    if (copy_to_user(buffer, dados, tamanho_dados))
        return -EFAULT;

    // atualiza offset para sinalizar fim de leitura
    *offset = tamanho_dados;
    return tamanho_dados;
}

static ssize_t gerador_write(struct file *file, const char __user *buffer,
                          size_t tamanho, loff_t *offset)
{
    // escrita nao e utilizada neste driver
    return tamanho;
}

static struct file_operations operacoes = {
    .owner   = THIS_MODULE,
    .open    = gerador_open,
    .release = gerador_release,
    .read    = gerador_read,
    .write   = gerador_write,
};

static int __init gerador_init(void)
{
    int resultado;

    // inicializa o estado do gerador usando salt 
    estado_prng = salt ^ jiffies;

    // registra o driver com  omajor
    resultado = register_chrdev(MAJOR_NUMERO, NOME_DISPOSITIVO, &operacoes);
    if (resultado < 0)
        return resultado;

    return 0;
}

static void __exit gerador_exit(void)
{
    // remove o driver do sistema
    unregister_chrdev(MAJOR_NUMERO, NOME_DISPOSITIVO);
}

module_init(gerador_init);
module_exit(gerador_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Luiz Iassuo");
MODULE_DESCRIPTION("Driver simples de numeros aleatorios");
