#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct no {
    int matricula;
    char nome[50];
    char cargo[50];
    float salario;
    struct no *esq;
    struct no *dir;
} No;

No* criar_no(int matricula, char *nome, char *cargo, float salario) {
    No *novo = (No*)malloc(sizeof(No));
    novo->matricula = matricula;
    strcpy(novo->nome, nome);
    strcpy(novo->cargo, cargo);
    novo->salario = salario;
    novo->esq = novo->dir = NULL;
    return novo;
}

No* inserir(No *raiz, int matricula, char *nome, char *cargo, float salario) {
    if (raiz == NULL) return criar_no(matricula, nome, cargo, salario);
    
    if (matricula < raiz->matricula)
        raiz->esq = inserir(raiz->esq, matricula, nome, cargo, salario);
    else if (matricula > raiz->matricula)
        raiz->dir = inserir(raiz->dir, matricula, nome, cargo, salario);
    else
        printf("Matricula existente!\n");
    
    return raiz;
}

No* buscar(No *raiz, int matricula) {
    if (raiz == NULL || raiz->matricula == matricula)
        return raiz;
    
    if (matricula < raiz->matricula)
        return buscar(raiz->esq, matricula);
    else
        return buscar(raiz->dir, matricula);
}

void exibir_funcionario(No *func) {
    if (func != NULL) {
        printf("\nMatricula: %d\n", func->matricula);
        printf("Nome: %s\n", func->nome);
        printf("Cargo: %s\n", func->cargo);
        printf("Salario: R$%.2f\n\n", func->salario);
    }
}

void listar_em_ordem(No *raiz) {
    if (raiz != NULL) {
        listar_em_ordem(raiz->esq);
        exibir_funcionario(raiz);
        listar_em_ordem(raiz->dir);
    }
}

No* menor_valor(No *no) {
    No *atual = no;
    while (atual && atual->esq != NULL)
        atual = atual->esq;
    return atual;
}

No* remover(No *raiz, int matricula) {
    if (raiz == NULL) return raiz;

    if (matricula < raiz->matricula)
        raiz->esq = remover(raiz->esq, matricula);
    else if (matricula > raiz->matricula)
        raiz->dir = remover(raiz->dir, matricula);
    else {
        if (raiz->esq == NULL) {
            No *temp = raiz->dir;
            free(raiz);
            return temp;
        } else if (raiz->dir == NULL) {
            No *temp = raiz->esq;
            free(raiz);
            return temp;
        }
        
        No *temp = menor_valor(raiz->dir);
        raiz->matricula = temp->matricula;
        strcpy(raiz->nome, temp->nome);
        strcpy(raiz->cargo, temp->cargo);
        raiz->salario = temp->salario;
        raiz->dir = remover(raiz->dir, temp->matricula);
    }
    return raiz;
}

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void liberar_arvore(No *raiz) {
    if (raiz != NULL) {
        liberar_arvore(raiz->esq);
        liberar_arvore(raiz->dir);
        free(raiz);
    }
}

int main() {
    No *arvore = NULL;
    int opcao, matricula;
    char nome[50], cargo[50];
    float salario;

    do {
        printf("\n=== MENU ===\n");
        printf("1. Adicionar funcionario\n");
        printf("2. Atualizar dados\n");
        printf("3. Buscar funcionario\n");
        printf("4. Listar todos\n");
        printf("5. Remover funcionario\n");
        printf("6. Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        limpar_buffer();

        switch(opcao) {
            case 1:
                printf("\nMatricula: ");
                scanf("%d", &matricula);
                limpar_buffer();
                printf("Nome: ");
                fgets(nome, 50, stdin);
                nome[strcspn(nome, "\n")] = '\0';
                printf("Cargo: ");
                fgets(cargo, 50, stdin);
                cargo[strcspn(cargo, "\n")] = '\0';
                printf("Salario: ");
                scanf("%f", &salario);
                limpar_buffer();
                arvore = inserir(arvore, matricula, nome, cargo, salario);
                break;

            case 2: {
                printf("\nMatricula para atualizar: ");
                scanf("%d", &matricula);
                limpar_buffer();
                No *func = buscar(arvore, matricula);
                if (func) {
                    printf("Novo nome (%s): ", func->nome);
                    fgets(nome, 50, stdin);
                    nome[strcspn(nome, "\n")] = '\0';
                    if (strlen(nome)) strcpy(func->nome, nome);

                    printf("Novo cargo (%s): ", func->cargo);
                    fgets(cargo, 50, stdin);
                    cargo[strcspn(cargo, "\n")] = '\0';
                    if (strlen(cargo)) strcpy(func->cargo, cargo);

                    printf("Novo salario (%.2f): ", func->salario);
                    char entrada[20];
                    fgets(entrada, 20, stdin);
                    if (strlen(entrada)) sscanf(entrada, "%f", &func->salario);
                } else {
                    printf("Funcionario nao encontrado!\n");
                }
                break;
            }

            case 3:
                printf("\nMatricula para buscar: ");
                scanf("%d", &matricula);
                limpar_buffer();
                No *resultado = buscar(arvore, matricula);
                if (resultado) {
                    exibir_funcionario(resultado);
                } else {
                    printf("Funcionario nao encontrado!\n");
                }
                break;

            case 4:
                printf("\n=== Funcionarios cadastrados ===\n");
                listar_em_ordem(arvore);
                break;

            case 5:
                printf("\nMatricula para remover: ");
                scanf("%d", &matricula);
                limpar_buffer();
                arvore = remover(arvore, matricula);
                break;

            case 6:
                liberar_arvore(arvore);
                printf("\nSistema encerrado!\n");
                break;

            default:
                printf("\nOpcao invalida!\n");
        }
    } while (opcao != 6);

    return 0;
}