#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <time.h>

#define TRUE 1
#define FALSE 0

typedef struct {
    char nome[50];
    char telefone[20];
    char email[50];
} Contato;

// Função para adicionar um novo contato
void adicionarContato(FILE *arquivo, const Contato *contato) {
    fwrite(contato, sizeof(Contato), 1, arquivo);
    fflush(arquivo); // Certifica-se de que os dados sejam gravados no arquivo imediatamente
}

// Função para listar todos os contatos
void listarContatos(FILE *arquivo) {
    Contato contato;
    rewind(arquivo); // Volta para o início do arquivo antes de ler
    while (fread(&contato, sizeof(Contato), 1, arquivo) == 1) {
        printf("Nome: %s\nTelefone: %s\nEmail: %s\n\n", contato.nome, contato.telefone, contato.email);
    }
}

int buscarContato(FILE *arquivo, const char *nomeBusca) {
    Contato contato;
    while (fread(&contato, sizeof(Contato), 1, arquivo) == 1) {
        if (strcmp(contato.nome, nomeBusca) == 0) {
            return 1; // Contato encontrado
        }
    }
    return 0; // Contato não encontrado
}

void atualizarContato(FILE *arquivo, const char *nomeBusca, const Contato *novoContato) {
    Contato contato;
    long posicao = 0;
    int encontrado = 0; // Flag para indicar se o contato foi encontrado

    while (fread(&contato, sizeof(Contato), 1, arquivo) == 1) {
        if (strcmp(contato.nome, nomeBusca) == 0) {
            encontrado = 1; // Marca como encontrado
            printf("\nContato encontrado:\n");
            printf("Nome: %s\n", contato.nome);
            printf("Telefone: %s\n", contato.telefone);
            printf("Email: %s\n", contato.email);

            char opcao;
            printf("Deseja alterar este contato? (s/n): ");
            scanf(" %c", &opcao);

            if (opcao == 's' || opcao == 'S') {
                printf("Novo nome: ");
                scanf("%s", novoContato->nome);
                printf("Novo telefone: ");
                scanf("%s", novoContato->telefone);
                printf("Novo email: ");
                scanf("%s", novoContato->email);

                // Move o cursor para a posição do contato a ser atualizado
                fseek(arquivo, posicao * sizeof(Contato), SEEK_SET);
                fwrite(novoContato, sizeof(Contato), 1, arquivo);

                printf("Contato atualizado com sucesso.\n");
            } else {
                printf("Atualização cancelada.\n");
            }

            break; // Saímos do loop, pois o contato foi encontrado
        }
        posicao++; // Atualiza a posição
    }

    if (!encontrado) {
        printf("\nContato não encontrado.\n");
    }
}


// Função para deletar um contato
void deletarContato(FILE *arquivo, const char *nomeBusca) {
    Contato contato;
    FILE *temp = tmpfile();

    while (fread(&contato, sizeof(Contato), 1, arquivo) == 1) {
        if (strcmp(contato.nome, nomeBusca) != 0) {
            fwrite(&contato, sizeof(Contato), 1, temp);
        }
    }

    rewind(temp);
    fclose(arquivo);

    arquivo = fopen("agenda.dat", "wb");
    while (fread(&contato, sizeof(Contato), 1, temp) == 1) {
        fwrite(&contato, sizeof(Contato), 1, arquivo);
    }

    fclose(temp);
}

char lin_dup[70] = "==================================================================\n";
char lin_sim[70] = "------------------------------------------------------------------\n";


void printc(char *s, int tam)
{
    int ne = (tam - strlen(s)) / 2;
    while (ne > 0)
    {
        printf(" ");
        ne--;
    }
    printf(s);
}

void sleep(time_t delay)
{
    time_t timer0, timer1;
    time(&timer0);
    do
    {
        time(&timer1);
    } while ((timer1 - timer0) < delay);
}

int menu_principal()
{
    printf("\n%s", lin_dup);
    printc("AGENDA TELEFÔNICA", 66);
    printf("\n%s\n", lin_dup);
    printf("  C - Adicionar contato\n");
    printf("  R - Visualizar contato\n");
    printf("  U - Atualizar contato\n");
    printf("  D - Apagar contato\n");
    printf("  F - Sair\n");
    printf("  ----------------------\n");
    printf("\n  Escolha uma opção: ");
}



int main()
{
    setlocale(LC_ALL, "Portuguese");
    char op1, op2;

    FILE *arquivo = fopen("agenda.dat", "ab+");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    int opcao;
    Contato contato;

    do
    {
        menu_principal();
        fflush(stdin);
        op1 = getchar();

        system("cls || clear");

        if (op1 == 'f' || op1 == 'F')
        {
            printf("\n  ** Fim do aplicativo **");
            sleep(1);
            break;
        }
        switch(toupper(op1))
        {
            case 'C':
                printf("\n%s", lin_sim);
                printc("Adicionar contato", 66);
                printf("\n%s\n", lin_sim);

                printf("Nome: ");
                scanf("%s", contato.nome);
                printf("Telefone: ");
                scanf("%s", contato.telefone);
                printf("Email: ");
                scanf("%s", contato.email);
                adicionarContato(arquivo, &contato);

                printf("\n\n");
                //sleep(5);
            break;
            case 'R':
                printf("\n%s", lin_sim);
                printc("Visualizar contato", 66);
                printf("\n%s\n", lin_sim);

                listarContatos(arquivo);

                break;

            case 'U':
                printf("\n%s", lin_sim);
                printc("Atualizar contato", 66);
                printf("\n%s\n", lin_sim);

                printf("Digite o nome do contato que deseja atualizar: ");
                char nomeBusca[50];
                scanf("%s", nomeBusca);
                atualizarContato(arquivo, nomeBusca, &contato);
                break;
            default:
                printf("Digite uma opção valida\n");
        }
    } while(op1);

    fclose(arquivo);

    return 0;
}

