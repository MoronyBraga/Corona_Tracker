#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <conio.h>
#include <time.h>

int login();
int cadastro();

//Variaveis globais
char nome[30], cpf[11], tel[12], end[100], cep[10], email[30], data[10], com[30];

//tipo End
typedef struct end {
    char rua[30], num[10], bairro[30], cidade[20], estado[2];
} End;

//tipo Nasc, nao possui o ano, este esta declarado dentro da funcao registro
typedef struct nasc {
    char dia[2], mes[2];
}Nasc;

/* Funcao responsavel por fazer o registro de novos pacientes.
Na pratica, ela abrie dois arquivos no diretorio do programa, um para os dados
gerais de pacientes e outro para armazenar CEP, idade e, se possuir, comorbidades
dos pacientes que estejam no grupo de risco */

void registro(){
    //Variaveis locais;
    FILE* fp;   // FILE* arquivos
    FILE* sec;
    int ani, op1, op = 1;   //numericos usados em operacoes dentro da funcao. ani  o ano de nascimento
    Nasc nasc;
    End end;

    fflush(stdin);

    //Abre os arquivos no modo append
    fp = fopen("files/regis.txt", "a");
    sec = fopen("files/sec_saude.txt", "a");

    // inicio do loop while, esse loop vai repetir a edição dos arquivos caso o usuario deseje registrar mais de um paciente na mesma sessao
    while(op == 1){

        printf("\n  Digite as Informações do Paciente nos campos abaixo:\n\n");
        // Dados do paciente
        printf("  - Nome: ");
        gets(nome);
        fprintf(fp, "%s\n", nome);

        printf("  - CPF: ");
        gets(cpf);
        fprintf(fp, "%s\n", cpf);

        printf("  - Telefone: ");
        gets(tel);
        fprintf(fp, "%s\n", tel);

        printf("  - Endereco:\n");

        printf("   Rua: ");
        gets(end.rua);
        fprintf(fp, "%s, ", end.rua);

        printf("   Número: ");
        gets(end.num);
        fprintf(fp, "%s, ", end.num);

        printf("   Bairro: ");
        gets(end.bairro);
        fprintf(fp, "%s, ", end.bairro);

        printf("   Cidade: ");
        gets(end.cidade);
        fprintf(fp, "%s, ", end.cidade);

        printf("   Estado: ");
        gets(end.estado);
        fprintf(fp, "%s, ", end.estado);

        printf("   CEP: ");
        gets(cep);
        fprintf(fp, "%s\n", cep);

        printf("  - Data de nascimento:\n ");
        printf("  Dia: ");
        scanf("%s", &nasc.dia);
        fprintf(fp, "%s/", nasc.dia);

        printf("   Mês: ");
        scanf("%s", &nasc.mes);
        fprintf(fp, "%s/", nasc.mes);

        printf("   Ano: ");
        scanf("%d", &ani);

        time_t t = time(NULL);
        struct tm tm = *localtime(&t);

        int idade = 0;

        fprintf(fp, "%d\n", ani);

        idade = (tm.tm_year + 1900) - ani;//operacao de calculo da idade, baseada no ano de nascimento

        printf("  - E-mail do paciente: ");
        scanf("%s", email);
        fprintf(fp, "%s\n", email);

        printf("  - Data do diagnóstico: ");
        scanf("%s", data);
        fprintf(fp, "%s\n", data);

        printf("  - Possui comorbidade? [sim: 1/ nao: 0]: ");
        scanf("%d", &op1);

        if(op1 == 1){ // essa condicional toma o valor da variavel op1 para dar continuidade na coleta caso o paciente possua comorbidades
            fflush(stdin);
            printf("  Informe a(s) comorbidade(s): ");
            gets(com);
            system("cls");
        }

        if(idade >= 65 || op1 == 1){   //se o paciente possuir comorbidades ou tiver mais de 65 anos, a idade e cep serao salvos no segundo arquivo
            fprintf(sec, "Idade: ");
            fprintf(sec, "%d\n", idade);
            fprintf(sec, "CEP: %s\n", cep);
            if (op1 == 0){
                fprintf(sec, "%c", '\n');
            }
            if(op1 == 1){
                fprintf(sec, "Comorbidade: %s\n\n", com); //caso o paciente tenha a comorbidade, ela serÃ¡ tambem registrada no arquivo
                fprintf(fp, "%s\n", com);
            }
        }

        fprintf(fp, "%c", '\n'); // insersao de um salto de linha no arquivo

        printf("\n  Deseja cadastrar mais um paciente? [sim: 1/ nao: 0]: "); // opcao para um novo registro; define-se se o usuario deseja fazer um novo registro e iniciar uma iteracao do loop while
        scanf("%d", &op);
        fflush(stdin);
    }
    fclose(sec);
    fclose(fp);  //fechamento dos arquivos
    system("cls");
}

void main(){

    setlocale(LC_ALL, "Portuguese");
    int log = 1, op;
    printf("                ---------- CORONA TRACKER -----------\n\n");


    while(op != 3){
        op = 0;
        printf("\n    Como deseja prosseguir?:\n\n ");  //MENU do programa
        printf("   Cadastrar novo paciente: 1\n");
        printf("    Trocar de conta: 2\n");
        printf("    Encerrar programa: 3\n");
        op = getch();

        switch(op){ //o switch opera com os valores da tabela ASCII dado por seus respectivos digitos pelo teclado

        case 49:
            if(log == 0){  //so sera permitido um registro se o usuario estiver logado
                registro();
            } else {
                log = login();
                if(log != 0){
                    log = cadastro(); // caso o usuario nao possua uma conta, deve cadastrar-se
                }
            }
            break;
        case 50:
            log = 1;
            log = login(); //opcao para troca de conta sem precisar reiniciar o programa
            if(log != 0){
                    log = cadastro(); // caso o usuario nao possua uma conta, deve cadastrar-se
                }
            break;
        case 51:
             //opcao de fechamento do programa
            system("cls");
            printf("\n    Sessão finalizada. Obrigado por utilizar o Corona Tracker.\n"); //mensagem de saida
            printf("\n    Pressione Enter");
            getch();
            system("cls");
            op = 3;
            break;
        default:
            system("cls");
            printf("\n    Opção inválida.\n");
        }
    }
}

int cadastro(){ // cadastro de nova conta
    FILE* fp;
    char username[30], password[30], aux[30], i;
    int op = 0, s = 0, j;

    fflush(stdin);

    while(op != 1){
        system("cls");
        fp = fopen("files/logs.txt", "r");  // o arquivo e aberto para leitura
        while(op != 1){
            j = 0;
            printf("\n  Novo nome de usuário: ");
            gets(username);  //username recebe o novo nome de usuario

            for(int c = 0; username[c] != '\0'; c++){
                j++;
            }
            if (j < 1){
                printf("\n    Usuário inválido.\n");
                printf("\n    Pressione Enter\n");
                getch();
                system("cls");
            } else {
                break;
            }
        }

        while((i = getc(fp)) != EOF){ //o loop verifica se o nome ja existe no arquivo
            fscanf(fp, "%s", aux);
            if(strcmp(aux, username) == 0){
                    s++;
            }
        }
        if(s > 0){
            printf("\n  Nome de usuário indisponível! Tente novamente.\n"); //se sim ira pedir outro nome
            s = 0;
        } else {
            printf("\n  Nome de usuário disponível.\n");//se nao o loop sera interrompido com a mensagem de disponibilidade do nome
            fflush(stdin);
            fclose(fp);
            break;
        }
    }

    fp = fopen("files/logs.txt", "a"); //nome sera escrito no arquivo
    fprintf(fp, "%s\n", username);

	op = 0;


	while(op != 1){
        s = 0;
		printf("\n  Nova senha: "); //pede-se entao uma senha que sera inserida logo abaixo do nome de usuario seguida de um salto de linha
    	gets(password);
    	for(int c = 0; password[c] != '\0'; c++){
            s++;
		}
		if (s < 1){
    		printf("\n    Senha inválida.\n");
    		printf("\n    Pressione Enter\n");
    		getch();
            system("cls");
		} else {
			break;
		}
	}
	fprintf(fp, "%s\n\n", password);

    fclose(fp);

    fflush(stdin);

    system("cls");
    return 0;
}


int login(){ //funcao de login de usuario

    FILE* fp;
    char username[30], password[30], str2[30];
    int i, s = 0, op = 0, op2 = 0;

    system("cls");
    printf("\n  Para ter acesso, entre com seu login: \n");

    while(op == 0){
        fp = fopen("files/logs.txt", "r"); //arquivo aberto para leitura

        printf("\n    Nome de usuário: ");
        gets(username);

        char str[30];
        while((i = getc(fp)) != EOF){ //verifica se o nome existe

            fscanf(fp, "%s", str);
            if(strcmp(str, username) == 0){
                s++;
                fscanf(fp, "%s", str2); //caso o nome exista, a variavel sera atribuida a variavel str2 a string que esta logo abaixo do nome de usuario, ou seja, a senha
                break;
            }
        }
        if(s == 0){
            printf("\n  Usuário inexistente. Deseja fazer um novo cadastro? [sim: 1/não: 0]: ");
            scanf("%d", &op);  //caso nao exista, a funcao ira perguntar se se deseja criar uma nova conta
            if(op == 1){
                fclose(fp);
                return 1; //se sim retorna 1, se nao o loop sera repetido
            }
            fflush(stdin);

        } else {
            break;
        }
    }

    while(op2 == 0){
        printf("    Senha: "); //caso o nome de usuario exista no arquivo, sera pedida a senha
        gets(password);

        if(strcmp(str2, password) != 0){ //compara a str2 atribuida acima com a senha fornecida
            printf("Senha inválida\n"); // se houver erro, essa mensajem sera impressa
        } else {
            break; //caso esteja correta, o loop sera quabrado
        }
    }
    fclose(fp); //fechamento do arquivo
    system("cls");
    printf("\n    Bem vindo %s\n\n", username); //mensagem de boas vindas com o nome do usuario logado

    fflush(stdin);
    return 0;  //fim da funcao
}

