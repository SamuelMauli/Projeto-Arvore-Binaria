#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura que representa um nó da árvore binária de busca (BST)
typedef struct no {
    int matricula;          // Chave única de identificação do funcionário
    char nome[50];          // Nome do funcionário
    char cargo[50];         // Cargo ocupado
    float salario;          // Remuneração
    struct no *esq;         // Ponteiro para subárvore esquerda (matrículas menores)
    struct no *dir;         // Ponteiro para subárvore direita (matrículas maiores)
} No;

/**
 * Cria e inicializa um novo nó da árvore.
 * 
 * @param matricula Chave de identificação
 * @param nome Nome completo
 * @param cargo Cargo atual
 * @param salario Salário atual
 * @return Ponteiro para o nó alocado. Espaço deve ser liberado posteriormente com free()
 */
No* criar_no(int matricula, char *nome, char *cargo, float salario) {
    No *novo = (No*)malloc(sizeof(No));
    novo->matricula = matricula;
    strcpy(novo->nome, nome);       // Copia string para o campo nome
    strcpy(novo->cargo, cargo);     // Copia string para o campo cargo
    novo->salario = salario;
    novo->esq = novo->dir = NULL;   // Inicializa ponteiros como NULL
    return novo;
}

/**
 * Insere um novo funcionário na árvore BST mantendo a propriedade de ordenação.
 * Duplicatas são detectadas e rejeitadas.
 * 
 * @param raiz Raiz da subárvore atual
 * @param matricula Chave para inserção
 * @return Ponteiro para a raiz atualizada da subárvore
 */
No* inserir(No *raiz, int matricula, char *nome, char *cargo, float salario) {
    if (raiz == NULL) return criar_no(matricula, nome, cargo, salario); // Árvore vazia: cria novo nó
    
    // Navega pela árvore comparando matrículas
    if (matricula < raiz->matricula)
        raiz->esq = inserir(raiz->esq, matricula, nome, cargo, salario); // Insere à esquerda se menor
    else if (matricula > raiz->matricula)
        raiz->dir = inserir(raiz->dir, matricula, nome, cargo, salario); // Insere à direita se maior
    else
        printf("Matricula existente!\n"); // Matrícula duplicada
    
    return raiz; // Retorna a raiz (estrutura imutável neste nível)
}

/**
 * Busca um funcionário pela matrícula usando busca binária.
 * 
 * @param raiz Raiz da subárvore atual
 * @param matricula Chave de busca
 * @return Ponteiro para o nó encontrado ou NULL se não existir
 */
No* buscar(No *raiz, int matricula) {
    if (raiz == NULL || raiz->matricula == matricula) // Caso base: encontrou ou árvore vazia
        return raiz;
    
    // Decide em qual subárvore continuar a busca
    if (matricula < raiz->matricula)
        return buscar(raiz->esq, matricula); // Busca na esquerda se menor
    else
        return buscar(raiz->dir, matricula); // Busca na direita se maior
}

/**
 * Exibe os dados de um funcionário formatados.
 * 
 * @param func Ponteiro para o nó contendo os dados. Se NULL, não imprime nada.
 */
void exibir_funcionario(No *func) {
    if (func != NULL) {
        printf("\nMatricula: %d\n", func->matricula);
        printf("Nome: %s\n", func->nome);
        printf("Cargo: %s\n", func->cargo);
        printf("Salario: R$%.2f\n\n", func->salario);
    }
}

/**
 * Percorre a árvore em ordem (esquerda-raiz-direita), listando funcionários em ordem crescente de matrícula.
 * 
 * @param raiz Raiz da subárvore atual
 */
void listar_em_ordem(No *raiz) {
    if (raiz != NULL) {
        listar_em_ordem(raiz->esq);   // Visita subárvore esquerda
        exibir_funcionario(raiz);     // Processa nó atual
        listar_em_ordem(raiz->dir);   // Visita subárvore direita
    }
}

/**
 * Encontra o nó com menor matrícula em uma subárvore (usado para sucessor in-order).
 * 
 * @param no Raiz da subárvore
 * @return Ponteiro para o nó com menor valor
 */
No* menor_valor(No *no) {
    No *atual = no;
    while (atual && atual->esq != NULL) // Percorre até o nó mais à esquerda
        atual = atual->esq;
    return atual;
}

/**
 * Remove um nó da árvore mantendo a propriedade BST.
 * Três casos: nó folha, nó com um filho, nó com dois filhos.
 * 
 * @param raiz Raiz da subárvore atual
 * @param matricula Chave para remoção
 * @return Ponteiro para a raiz atualizada da subárvore
 */
No* remover(No *raiz, int matricula) {
    if (raiz == NULL) return raiz; // Árvore vazia

    // Busca o nó a ser removido
    if (matricula < raiz->matricula)
        raiz->esq = remover(raiz->esq, matricula);
    else if (matricula > raiz->matricula)
        raiz->dir = remover(raiz->dir, matricula);
    else { // Nó encontrado
        // Caso 1: Nó folha ou com apenas um filho
        if (raiz->esq == NULL) {
            No *temp = raiz->dir;
            free(raiz);
            return temp;
        } else if (raiz->dir == NULL) {
            No *temp = raiz->esq;
            free(raiz);
            return temp;
        }
        
        // Caso 2: Nó com dois filhos - substitui pelo sucessor in-order (menor da direita)
        No *temp = menor_valor(raiz->dir);
        // Copia dados do sucessor
        raiz->matricula = temp->matricula;
        strcpy(raiz->nome, temp->nome);
        strcpy(raiz->cargo, temp->cargo);
        raiz->salario = temp->salario;
        // Remove o sucessor recursivamente
        raiz->dir = remover(raiz->dir, temp->matricula);
    }
    return raiz;
}

/**
 * Limpa o buffer de entrada para evitar problemas com entradas inválidas.
 */
void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF); // Lê todos os caracteres até encontrar nova linha ou EOF
}

/**
 * Libera toda a memória alocada para a árvore usando percurso pós-ordem.
 * 
 * @param raiz Raiz da subárvore atual
 */
void liberar_arvore(No *raiz) {
    if (raiz != NULL) {
        liberar_arvore(raiz->esq); // Libera subárvore esquerda
        liberar_arvore(raiz->dir); // Libera subárvore direita
        free(raiz);                // Libera nó atual
    }
}

/**
 * Função principal com menu interativo para gerenciamento de funcionários.
 * Operações: inserção, atualização, busca, listagem, remoção e saída.
 */
int main() {
    No *arvore = NULL;       // Raiz da árvore inicializada como NULL
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
        limpar_buffer(); // Limpa buffer após scanf para evitar conflitos com fgets

        switch(opcao) {
            case 1: // Inserção
                printf("\nMatricula: ");
                scanf("%d", &matricula);
                limpar_buffer();
                printf("Nome: ");
                fgets(nome, 50, stdin);
                nome[strcspn(nome, "\n")] = '\0'; // Remove '\n' do final
                printf("Cargo: ");
                fgets(cargo, 50, stdin);
                cargo[strcspn(cargo, "\n")] = '\0';
                printf("Salario: ");
                scanf("%f", &salario);
                limpar_buffer();
                arvore = inserir(arvore, matricula, nome, cargo, salario);
                break;

            case 2: { // Atualização
                printf("\nMatricula para atualizar: ");
                scanf("%d", &matricula);
                limpar_buffer();
                No *func = buscar(arvore, matricula);
                if (func) {
                    printf("Novo nome (%s): ", func->nome);
                    fgets(nome, 50, stdin);
                    nome[strcspn(nome, "\n")] = '\0';
                    if (strlen(nome)) strcpy(func->nome, nome); // Atualiza se não for vazio

                    printf("Novo cargo (%s): ", func->cargo);
                    fgets(cargo, 50, stdin);
                    cargo[strcspn(cargo, "\n")] = '\0';
                    if (strlen(cargo)) strcpy(func->cargo, cargo);

                    printf("Novo salario (%.2f): ", func->salario);
                    char entrada[20];
                    fgets(entrada, 20, stdin);
                    if (strlen(entrada)) sscanf(entrada, "%f", &func->salario); // Atualiza se houver entrada
                } else {
                    printf("Funcionario nao encontrado!\n");
                }
                break;
            }

            case 3: // Busca
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

            case 4: // Listagem ordenada
                printf("\n=== Funcionarios cadastrados ===\n");
                listar_em_ordem(arvore);
                break;

            case 5: // Remoção
                printf("\nMatricula para remover: ");
                scanf("%d", &matricula);
                limpar_buffer();
                arvore = remover(arvore, matricula);
                break;

            case 6: // Encerramento
                liberar_arvore(arvore);
                printf("\nSistema encerrado!\n");
                break;

            default:
                printf("\nOpcao invalida!\n");
        }
    } while (opcao != 6);

    return 0;
}
