#include <ctype.h> //usado toupper
#include <stdio.h>
#include <stdlib.h> //usado system
#include <string.h> //usado strcmp

struct TpFarma {
  char Nome[21];
  float Preco;
  int QEstoque;
};

typedef struct TpFarma TpFARMA;

TpFARMA RgFarma;

FILE *ArqFarma;

long int Tamanho = 21 * sizeof(char) + sizeof(float) + sizeof(int);

long int TArquivo() {
  fseek(ArqFarma, 0, 2);
  long int R = ftell(ArqFarma) / Tamanho;
  return R;
}

//--------FUNCAO QUE VERIFICA SE O NOME JA EXISTE NO ARQUIVO
int Verifica_Existe(char *Nome) {

  FILE *ArqFarma =
      fopen("Farmacos.dat", "rb"); // Abrir o arquivo no modo de leitura
  if (ArqFarma == NULL) {
    perror("Erro ao abrir o arquivo");
    exit(EXIT_FAILURE);
  }

  char Farmaco[21];
  int Achou = 0;

  fseek(ArqFarma, 0, 0);
  while (fread(&RgFarma, Tamanho, 1, ArqFarma) == 1) {
    strcpy(Farmaco, RgFarma.Nome); // Copiar o nome do registro para a variável
                                   // Farmaco (Farmaco = RgFarma.Nome)
    if (strcmp(Farmaco, Nome) ==
        0) { // Comparar o nome do registro com o nome recebido como parâmetro
      Achou = 1;
      break;
    }
  }

  fclose(ArqFarma);
  return Achou; // Retorne 1 se encontrou o registro, 0 caso contrário
}

void Incluir() {
  char R;
  do {
    // system("cls");
    printf("*** inclusao ***\n\n");

    printf("Nome: ");
    scanf("%s", RgFarma.Nome);

    // Verificar se o nome já existe
    if (Verifica_Existe(RgFarma.Nome) == 1) {
      printf("Nome já cadastrado!\n");
    } else {
      printf("Preco: ");
      scanf("%f", &RgFarma.Preco);
      printf("Estoque: ");
      scanf("%d", &RgFarma.QEstoque);
      fseek(ArqFarma, 0, 2);
      fwrite(&RgFarma, Tamanho, 1, ArqFarma);
    }

    printf("\nNova inclusao? S/N ");
    scanf(" %c", &R);
    R = toupper(R);
  } while (R != 'N');
}

void Excluir() {

  // dado o nome do farmaco, deixar a quantidade em -1
  // não permitir exclusão de um farmaco que não existe

  if (TArquivo() != 0) {
    fclose(ArqFarma); // modo a + b
    ArqFarma = fopen("Farmacos.dat", "r+b");
    // system("cls");
    printf("*** excluir ***\n\n");
    fseek(ArqFarma, 0, 0);
    printf("Qual farmaco? ");
    char Farmaco[21];
    scanf("%s", Farmaco);
    int Achou = 0;
    do {
      fread(&RgFarma, Tamanho, 1, ArqFarma);
      if (strcmp(RgFarma.Nome, Farmaco) == 0) {
        Achou = 1;
        printf("Nome: %s\n", RgFarma.Nome);
      }
    } while (!feof(ArqFarma) && (Achou == 0));
    if (Achou == 0)
      printf("Registro inexistente!");
    else {
      // Dados com o preço negativo estao como excluidos
      RgFarma.Preco = -10;
      fseek(ArqFarma, -Tamanho, 1);
      fwrite(&RgFarma, Tamanho, 1, ArqFarma);
      printf(">>> Exclusao efetuada com sucesso! <<<\n");
    }
    // system("pause");
    fclose(ArqFarma);
    ArqFarma = fopen("Farmacos.dat", "a+b");
  } else {
    printf("Arquivo Vazio.  Nao existem dados a alterar.");
  }
  // system("pause");
}

void Alterar() {
  if (TArquivo() != 0) {
    fclose(ArqFarma); // modo a + b
    ArqFarma = fopen("Farmacos.dat", "r+b");
    // system("cls");
    printf("*** alterar ***\n\n");
    fseek(ArqFarma, 0, 0);
    printf("Qual farmaco? ");
    char Farmaco[21];
    scanf("%s", Farmaco);
    int Achou = 0;
    do {
      fread(&RgFarma, Tamanho, 1, ArqFarma);
      //------------------------------------------ VERIFICA SE NAO FOI EXCLUIDO
      if (strcmp(RgFarma.Nome, Farmaco) == 0 && RgFarma.Preco >= 0) {
        Achou = 1;
        printf("Nome: %s\n", RgFarma.Nome);
        printf("Valor: %f\n", RgFarma.Preco);
        printf("Estoque: %d\n", RgFarma.QEstoque);
      }
    } while (!feof(ArqFarma) && (Achou == 0));
    if (Achou == 0)
      printf("Registro inexistente!");
    else {
      char Opcao;
      //--------MELHORIA 1: MENU PARA ESCOLHER QUAL CARACTERISTICA ALTERAR
      printf("Qual caracteristica deseja que seja alterada? \n\n");
      printf("N - Nome \n");
      printf("P - Preco \n");
      printf("Q - Quantidade \n");
      printf("T - Todos \n");
      scanf(" %c", &Opcao);
      Opcao = toupper(Opcao);
      switch (Opcao) {
      case 'N':
        printf("Qual o novo nome? \n");
        scanf("%s", RgFarma.Nome);
        break;
      case 'P':
        printf("Qual o novo preco? \n");
        scanf("%f", &RgFarma.Preco);
        break;
      case 'Q':
        printf("Qual a nova quantidade? \n");
        scanf("%d", &RgFarma.QEstoque);
        break;
      case 'T':
        printf("Qual o novo nome? \n");
        scanf("%s", RgFarma.Nome);

        printf("Qual o novo preco? \n");
        scanf("%f", &RgFarma.Preco);

        printf("Qual a nova quantidade? \n");
        scanf("%d", &RgFarma.QEstoque);
        break;
      }

      fseek(ArqFarma, -Tamanho, 1);
      fwrite(&RgFarma, Tamanho, 1, ArqFarma);
      printf(">>> Alteracao efetuada com sucesso! <<<\n");
    }
    // system("pause");
    fclose(ArqFarma);
    ArqFarma = fopen("Farmacos.dat", "a+b");
  } else {
    printf("Arquivo Vazio.  Nao existem dados a alterar.");
  }
  // system("pause");
  return;
}

void Consultar() {
  // system("cls");
  printf("*** consulta ***\n\n");
  fseek(ArqFarma, 0, 0);
  printf("Qual farmaco? ");
  char Farmaco[21];
  scanf("%s", Farmaco);
  int Achou = 0;
  do {
    fread(&RgFarma, Tamanho, 1, ArqFarma);
    //------------------------------------------ VERIFICA SE NAO FOI EXCLUIDO
    if (strcmp(RgFarma.Nome, Farmaco) == 0 && RgFarma.Preco >= 0) {
      Achou = 1;
      printf("Nome: %s\n", RgFarma.Nome);
      printf("Valor: R$%.2f\n", RgFarma.Preco);
      printf("Estoque: %d\n", RgFarma.QEstoque);
    }
  } while (!feof(ArqFarma) && (Achou == 0));
  if (Achou == 0)
    printf("Registro inexistente!");
  // system("pause");
  return;
}

void LTodos() {
  //--------MELHORIA 2: MSG DE ERRO CASO NAO EXISTA NENHUM REGISTRO
  if (TArquivo() != 0) {
    // system("cls");
    printf("*** lista todos ***\n\n");
    fseek(ArqFarma, 0, 0);
    do {
      fread(&RgFarma, Tamanho, 1, ArqFarma);
      if (!feof(ArqFarma)) {
        //------------------------------------------ VERIFICA SE NAO FOI
        //EXCLUIDO
        if (RgFarma.Preco >= 0) {
          printf("Nome: %s\n", RgFarma.Nome);
          printf("Valor: R$%.2f\n", RgFarma.Preco);
          printf("Estoque: %d\n", RgFarma.QEstoque);
          printf("***\n\n");
        }
      }
    } while (!feof(ArqFarma));
    // system("pause");
  } else {
    printf("Arquivo Vazio. Nao existem dados para serem mostrados.");
  }
}

int main() {
  ArqFarma = fopen("Farmacos.dat", "a+b");
  char Opcao;
  do {
    // system("cls");
    printf("\n\n> > > Pague Pouco < < < \n\n");
    printf("Que deseja fazer? \n\n");
    printf("I - Incluir \n");
    printf("E - Excluir \n");
    printf("A - Alterar \n");
    printf("C - Consultar \n");
    printf("T - Listar Todos \n");
    printf("S - Sair \n\n");
    scanf(" %c", &Opcao);
    Opcao = toupper(Opcao);
    switch (Opcao) {
    case 'I':
      Incluir();
      break;
    case 'E':
      Excluir();
      break;
    case 'A':
      Alterar();
      break;
    case 'C':
      Consultar();
      break;
    case 'T':
      LTodos();
      break;
    }
  } while (Opcao != 'S');
  fclose(ArqFarma);
  return 0;
}