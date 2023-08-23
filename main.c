#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <time.h>

#define TRUE 1
#define FALSE 0

char lin_dup[70] = "==================================================================\n";
char lin_sim[70] = "------------------------------------------------------------------\n";

typedef struct {
    char nome[50];
    char telefone[20];
    char email[50];
    int excluido;
} Contato;

// Função para adicionar um novo contato
void adicionarContato(FILE *arquivo, const Contato *contato) {
    fwrite(contato, sizeof(Contato), 1, arquivo);
    fflush(arquivo); // Certifica-se de que os dados sejam gravados no arquivo imediatamente
}

// Função para listar todos os contatos
void listarContatos(FILE *arquivo) {
    Contato contato;
    int contador = 0;
    rewind(arquivo); // Volta para o início do arquivo antes de ler
    printf("Indice\tNome\tTelefone\tEmail\n");
    printf("---------------------------------------------\n");
    while (fread(&contato, sizeof(Contato), 1, arquivo) == 1) {
        contador++;
        if(contato.excluido == 0){
            printf("%d\t%s\t%s\t%s\n", contador, contato.nome, contato.telefone, contato.email);
        }
    }
}

void atualizarContato(Contato *contato, const char *novoNome, const char *novoTelefone, const char *novoEmail) {
    strcpy(contato->nome, novoNome);
    strcpy(contato->telefone, novoTelefone);
    strcpy(contato->email, novoEmail);
}


void excluirContato(FILE *arquivo, int posicao) {
    Contato contato;
    fseek(arquivo, posicao * sizeof(Contato), SEEK_SET);
    fread(&contato, sizeof(Contato), 1, arquivo);

    // Marcar o contato como excluído
    contato.excluido = 1;

    fseek(arquivo, posicao * sizeof(Contato), SEEK_SET);
    fwrite(&contato, sizeof(Contato), 1, arquivo);
    fflush(arquivo);
}

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
    printf("  S - Sair\n");
    printf("  ----------------------\n");
    printf("\n  Escolha uma opção: ");
}



int main()
{
    setlocale(LC_ALL, "Portuguese");
    int posicao; // Variável para armazenar a posição do contato a ser atualizado
    char op1;

    FILE *arquivo;

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

        if (op1 == 's' || op1 == 'S')
        {
            printf("\n  ** Fim do aplicativo **");
            sleep(1);
            break;
        }
        switch(toupper(op1))
        {
            case 'C':
            	arquivo = fopen("agenda.dat", "ab+");
                printf("\n%s", lin_sim);
                printc("Adicionar contato", 66);
                printf("\n%s\n", lin_sim);

                printf("Nome: ");
                scanf("%s", contato.nome);
                printf("Telefone: ");
                scanf("%s", contato.telefone);
                printf("Email: ");
                scanf("%s", contato.email);
                contato.excluido = 0;

                adicionarContato(arquivo, &contato);
                fclose(arquivo);
                //sleep(5);
            break;
            case 'R':
            	arquivo = fopen("agenda.dat", "r+");
                printf("\n%s", lin_sim);
                printc("Visualizar contato", 66);
                printf("\n%s\n", lin_sim);

                listarContatos(arquivo);
                fclose(arquivo);
                break;

            case 'U':
            	arquivo = fopen("agenda.dat", "rb+");
                printf("\n%s", lin_sim);
                printc("Atualizar contato", 66);
                printf("\n%s\n", lin_sim);

                listarContatos(arquivo);

                printf("\nDigite o índice do contato que deseja atualizar: ");
                scanf("%d", &posicao);
                posicao--; // Ajuste para índice (considerando que a lista é exibida a partir de 1)

                fseek(arquivo, posicao * sizeof(Contato), SEEK_SET);
                fread(&contato, sizeof(Contato), 1, arquivo);

                printf("\nContato atual:\n");
                printf("\n\tNome:%s\n\tTelefone: %s\n\tEmail: %s\n", contato.nome, contato.telefone, contato.email);

                printf("\nDigite o novo nome: ");
                char novoNome[50];
                scanf("%s", novoNome);

                printf("Digite o novo telefone: ");
                char novoTelefone[20];
                scanf("%s", novoTelefone);

                printf("Digite o novo email: ");
                char novoEmail[50];
                scanf("%s", novoEmail);

                atualizarContato(&contato, novoNome, novoTelefone, novoEmail);

                fseek(arquivo, posicao * sizeof(Contato), SEEK_SET);
                fwrite(&contato, sizeof(Contato), 1, arquivo);
                fflush(arquivo);

                printf("\nContato atualizado com sucesso!\n");
                fclose(arquivo);
                break;
            case 'D':
		arquivo = fopen("agenda.dat", "rb+");
		printf("\n%s", lin_sim);
		printc("Apagar contato", 66);
		printf("\n%s\n", lin_sim);
		
		listarContatos(arquivo);
		
		printf("\nDigite o índice do contato que deseja excluir: ");
		scanf("%d", &posicao);
		posicao--; // Ajuste para índice (considerando que a lista é exibida a partir de 1)
		
		rewind(arquivo); // Reposiciona o cursor para o início do arquivo
		excluirContato(arquivo, posicao);
		
		printf("\nContato excluído com sucesso!\n");
		
		fclose(arquivo);
                break;
            default:
                printf("Digite uma opção valida\n");
        }
    } while(op1);

    return 0;
}
