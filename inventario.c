#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_ITENS 10
#define TAM_NOME 30
#define TAM_TIPO 20


typedef struct {
    char nome[TAM_NOME];
    char tipo[TAM_TIPO];
    int quantidade;
} Item;


void limparBufferEntrada(void);
void lerLinha(char *dest, size_t tam);
void trimNovaLinha(char *s);

int  compararCaseInsensitive(const char *a, const char *b);
int  buscarItem(Item itens[], int qtd, const char *nomeAlvo); 

void inserirItem(Item itens[], int *qtd);
void removerItem(Item itens[], int *qtd);
void listarItens(const Item itens[], int qtd);


int main(void) {
    Item mochila[MAX_ITENS];
    int quantidadeItens = 0;
    int opcao;

    printf("=========================================\n");
    printf("     MOCHILA DE LOOT - INVENTARIO V1\n");
    printf("=========================================\n");
    printf("Capacidade: ate %d itens\n\n", MAX_ITENS);

    do {
        printf("\n--- MENU ---\n");
        printf("1) Inserir item\n");
        printf("2) Remover item\n");
        printf("3) Buscar item\n");
        printf("4) Listar itens\n");
        printf("0) Sair\n");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &opcao) != 1) {
            
            printf("Entrada invalida. Tente novamente.\n");
            limparBufferEntrada();
            continue;
        }
        limparBufferEntrada(); 

        switch (opcao) {
            case 1:
                inserirItem(mochila, &quantidadeItens);
                listarItens(mochila, quantidadeItens); 
                break;
            case 2:
                removerItem(mochila, &quantidadeItens);
                listarItens(mochila, quantidadeItens);
                break;
            case 3: {
                char nomeBusca[TAM_NOME];
                printf("Digite o nome do item para buscar: ");
                lerLinha(nomeBusca, sizeof(nomeBusca));
                int idx = buscarItem(mochila, quantidadeItens, nomeBusca);
                if (idx >= 0) {
                    printf("\nItem encontrado:\n");
                    printf(" - Nome: %s\n", mochila[idx].nome);
                    printf(" - Tipo: %s\n", mochila[idx].tipo);
                    printf(" - Quantidade: %d\n", mochila[idx].quantidade);
                } else {
                    printf("Item \"%s\" nao encontrado na mochila.\n", nomeBusca);
                }
                
                listarItens(mochila, quantidadeItens);
                break;
            }
            case 4:
                listarItens(mochila, quantidadeItens);
                break;
            case 0:
                printf("Encerrando... Bom jogo!\n");
                break;
            default:
                printf("Opcao inexistente. Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}


void limparBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {  }
}


void lerLinha(char *dest, size_t tam) {
    if (fgets(dest, (int)tam, stdin) != NULL) {
        trimNovaLinha(dest);
    } else {
        
        if (tam > 0) dest[0] = '\0';
    }
}

void trimNovaLinha(char *s) {
    size_t n = strlen(s);
    if (n > 0 && s[n - 1] == '\n') s[n - 1] = '\0';
}


int compararCaseInsensitive(const char *a, const char *b) {
    unsigned char ca, cb;
    while (*a && *b) {
        ca = (unsigned char) tolower((unsigned char)*a);
        cb = (unsigned char) tolower((unsigned char)*b);
        if (ca != cb) return (int)ca - (int)cb;
        a++; b++;
    }
    return (int)(unsigned char)tolower((unsigned char)*a)
         - (int)(unsigned char)tolower((unsigned char)*b);
}


int buscarItem(Item itens[], int qtd, const char *nomeAlvo) {
    for (int i = 0; i < qtd; i++) {
        if (compararCaseInsensitive(itens[i].nome, nomeAlvo) == 0) {
            return i; 
        }
    }
    return -1; 
}


void inserirItem(Item itens[], int *qtd) {
    if (*qtd >= MAX_ITENS) {
        printf("Mochila cheia! Remova algo antes de inserir novo item.\n");
        return;
    }

    Item novo;
    printf("Nome do item: ");
    lerLinha(novo.nome, sizeof(novo.nome));

    printf("Tipo (ex.: arma, municao, cura, ferramenta): ");
    lerLinha(novo.tipo, sizeof(novo.tipo));

    printf("Quantidade: ");
    int ok = scanf("%d", &novo.quantidade);
    if (ok != 1 || novo.quantidade < 0) {
        printf("Quantidade invalida. Operacao cancelada.\n");
        limparBufferEntrada();
        return;
    }
    limparBufferEntrada();

    
    int idx = buscarItem(itens, *qtd, novo.nome);
    if (idx >= 0) {
        itens[idx].quantidade += novo.quantidade;
        
        if (strlen(itens[idx].tipo) == 0 && strlen(novo.tipo) > 0) {
            strncpy(itens[idx].tipo, novo.tipo, sizeof(itens[idx].tipo) - 1);
            itens[idx].tipo[sizeof(itens[idx].tipo) - 1] = '\0';
        }
        printf("Quantidade de \"%s\" atualizada para %d.\n",
               itens[idx].nome, itens[idx].quantidade);
        return;
    }

    
    strncpy(itens[*qtd].nome, novo.nome, sizeof(itens[*qtd].nome) - 1);
    itens[*qtd].nome[sizeof(itens[*qtd].nome) - 1] = '\0';

    strncpy(itens[*qtd].tipo, novo.tipo, sizeof(itens[*qtd].tipo) - 1);
    itens[*qtd].tipo[sizeof(itens[*qtd].tipo) - 1] = '\0';

    itens[*qtd].quantidade = novo.quantidade;

    (*qtd)++;
    printf("Item \"%s\" inserido com sucesso.\n", novo.nome);
}


void removerItem(Item itens[], int *qtd) {
    if (*qtd == 0) {
        printf("A mochila esta vazia. Nada para remover.\n");
        return;
    }

    char nomeAlvo[TAM_NOME];
    printf("Digite o nome do item a remover: ");
    lerLinha(nomeAlvo, sizeof(nomeAlvo));

    int idx = buscarItem(itens, *qtd, nomeAlvo);
    if (idx < 0) {
        printf("Item \"%s\" nao encontrado.\n", nomeAlvo);
        return;
    }

    
    for (int i = idx; i < *qtd - 1; i++) {
        itens[i] = itens[i + 1];
    }
    (*qtd)--;

    printf("Item \"%s\" removido com sucesso.\n", nomeAlvo);
}


void listarItens(const Item itens[], int qtd) {
    printf("\n=== ITENS NA MOCHILA (%d/%d) ===\n", qtd, MAX_ITENS);
    if (qtd == 0) {
        printf("(vazio)\n");
        return;
    }

    for (int i = 0; i < qtd; i++) {
        printf("%2d) Nome: %-28s | Tipo: %-12s | Qtde: %d\n",
               i + 1, itens[i].nome, itens[i].tipo, itens[i].quantidade);
    }
}
