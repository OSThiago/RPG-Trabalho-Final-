#include<stdlib.h>
#include<stdio.h>
#include<windows.h>
#include<string.h>
#include <time.h>
#include <conio.h>
//teclas para controlar o jogo
#define ACIMA 72
#define ABAIXO 80
#define DIREITA 77
#define ESQUERDA 75
#define ENTER 13
#define BACKSPACE 8
//Cores para o texto
#define vermelho "\x1b[31m"
#define verde "\x1b[32m"
#define amarelo "\x1b[33m"
#define azul "\x1b[34m"
#define rosa "\x1b[35m"
#define ciano "\x1b[36m"
#define reset "\x1b[0m"

#define NOVO_JOGO 1
#define CONTINUAR 2
#define COMO_JOGAR 3
#define SAIR 4

//Estrutura de dados para o personagem a ser escolhido
struct personagem {
	char nome[50], classe[20];
	char atq1Nome[20],atq2Nome[20],atq3Nome[20];
	int vida , poder, arma;
	int ataque1, ataque2, ataque3;
	struct mochila *mochila;
	float  armadura;
};

//Estrutura de daods para o inventário (relacionada a do personagem)
struct mochila {
	int pocoes;
	char armas[20];
};

//Estrutura dos dados dos inimigos a serem enfrentados
struct inimigos {
	int vida, dano;
	char nome[20];
};

//Função para impressão do texto em determinadas coordenadas da tela
void gotoxy(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

//Declarações
void gotoxy(int x, int y);
void imagemFase3(int logico, struct inimigos inimigos[], struct personagem personagem);
void fase2(struct personagem personagem,int atributoEscolhido, int retornaPersonagem);
void imagemFase2(int logico, struct inimigos inimigos[], struct personagem personagem);
void final_fase(int fase, struct personagem personagem,int retornaPersonagem);
void escolha();
int tecladoEscolha();
void menu();
void opcao_sair ();
void como_jogar();
void iniciar_jogo(struct personagem personagem);
void mochila(struct personagem *personagem);
void imagemFase1 (int logico, struct inimigos inimigos[], struct personagem personagem);
float atacar();
void fase1(struct personagem personagem, int retornaPersonagem);
void continuar (struct personagem personagem);
int tecladoMenu();
void final_fase2(int fase,struct personagem personagem, int atributoEscolhido, int retornaPersonagem);
void opcao_vitoria();

//Função para impressão do texto em caso de vitória em todas as fase (término do jogo);
void opcao_vitoria(){
	char tela_vitoria[100];
	FILE *arqVitoria;
	int y=10;
	
	system("cls");
	
	//Abertura do arquivos para a impressão do texto
	arqVitoria=fopen("arquivos/tela_vitoria.txt","r");
	
	while(fgets(tela_vitoria,100,arqVitoria)!=NULL){
		gotoxy(30,y);
		printf(amarelo"%s"reset,tela_vitoria);
		y++;
	}
	printf("\n\n\n\n\n\n\n\n");
	fclose(arqVitoria);	
	exit(1);
	
}

//Função para execução da terceira fase
void fase3(struct personagem personagem, int atributoEscolhido, int retornaPersonagem) {
	FILE *arq = NULL, *arqInfoInimigos=NULL, *arqContinuar=NULL;
	char titulo[200], ponteiro[100], ponteiro2[100];
	int y=7, x=37,tecla=0, logico=1,danoT=0;
	float critico=0;
	struct inimigos inimigos[5];
	
	system("cls");

	arqInfoInimigos=fopen("arquivos/infoInimigos.txt","r");
	
	//Pesquisa as informações do ALECCHESI no arquivo 
	do{
		fscanf(arqInfoInimigos,"%s",ponteiro);
	}while(strcmp(ponteiro,"Alecchesi"));

	//Leitura dos dados do inimigo
	strcpy(inimigos[2].nome,ponteiro);
	fscanf(arqInfoInimigos, "%d", &inimigos[2].vida);
	fscanf(arqInfoInimigos,"%d", &inimigos[2].dano);

	//Abertura do arquivo para impressão da tela de carregamento da terceira fase
	arq=fopen("arquivos/titulo1.txt","r");
	system("cls");

	do{
		fscanf(arq,"%s",ponteiro2);
	}while(strcmp(ponteiro2,"3"));

	while(fgets(titulo,200,arq)!=NULL && y < 20) {
		gotoxy(x,y);
		printf("%s", titulo);
		y++;
	}

	sleep(1);
	system("cls");
	
	//Sistema de batalha
	while(inimigos[2].vida>0 && personagem.vida>0) {
		tecla=0;
		x=42;
		y=21;
		
		//Sistema da navegação pelas setas
		while(tecla!=ENTER) {
			system("cls");
			
			//Chamada da função de impressão da imagem da fase
			imagemFase3(logico, inimigos, personagem);
			logico=0;
			gotoxy(x,y);
			printf(verde">>"reset);

			tecla=getch();

			if(tecla==ACIMA) {
				if(y>21) {
					y=y-2;
					gotoxy(x,y);
					printf(verde">>"reset);
				} else if(y==21) {
					y=27;
					printf(verde">>"reset);
				}
			} else if(tecla==ABAIXO) {
				if(y<27) {
					y=y+2;
					gotoxy(x,y);
					printf(verde">>"reset);
				} else if(y==27) {
					y=21;
					gotoxy(x,y);
					printf(verde">>"reset);
				}
			} else if(tecla==DIREITA) {
				gotoxy(x, y);
				printf(verde">>"reset);
			} else if(tecla==ESQUERDA) {
				gotoxy(x, y);
				printf(verde">>"reset);
			} else if(tecla!=ACIMA && tecla!=ABAIXO && tecla!=DIREITA && tecla!=ESQUERDA && tecla!=ENTER) {
				gotoxy(x,y);
				printf(verde">>"reset);
			}
		}
		
		//Sistema de ataque do personagem ao inimigo
		if(y==21) {
			
			//Chamada da função para sorteio do valor de dano crítico
			critico=atacar();
			danoT=personagem.ataque1*critico;
			inimigos[2].vida = inimigos[2].vida-danoT;
		} else if(y==23) {
			critico=atacar();
			danoT=personagem.ataque2*critico;
			inimigos[2].vida=inimigos[2].vida-danoT;
		} else if(y==25) {
			critico=atacar();
			danoT=personagem.ataque3*critico;
			inimigos[2].vida=inimigos[2].vida-danoT;
		}else if(y==27){
			imagemFase3(logico, inimigos, personagem);
			mochila(&personagem);
		}

		if(inimigos[2].vida<=0) {
			gotoxy(56,2);
			printf("  0    ");
			sleep(1);
			break;
		}

		gotoxy(59,2);
		printf("%d      ", inimigos[2].vida);

		imagemFase3(logico,inimigos,personagem);
		
		//Sistema de ataque do inimigo ao personagem
		gotoxy(47,19);
		printf(vermelho"  Eh a vez do inimigo!!   "reset);
		sleep(2);
		critico=atacar();
		personagem.vida=personagem.vida-((inimigos[2].dano)*personagem.armadura);
	}
	
	if(personagem.vida<=0) {
		gotoxy(0,23);
		printf(ciano"Vida:"reset"  %d     ", 0);
		
		//Chamada da função de "game over"
		opcao_sair();
	}
	
	//Sistema de salvamento
	arqContinuar=fopen("arquivos/continuar.txt","w");
	fclose(arq);
	fclose(arqInfoInimigos);
	fclose(arqContinuar);
	opcao_vitoria();
}

//Função para impressão da imagem da fase
void imagemFase3(int logico, struct inimigos inimigos[], struct personagem personagem) {
	FILE *arqInimigos = NULL, *arqAtaque = NULL;
	char ponteiro[100], ataque[100];
	int y = 3,x;
	
	//Abertura do arquivo para leitura, pesquisa e impressão da imagem da terceira fase
	arqInimigos = fopen("arquivos/inimigos.txt","r");

	do {
		fscanf(arqInimigos,"%s", ponteiro);

	} while(strcmp(ponteiro,"3")!=0);
	
	// verifica se está abrindo a imagem pela primeira vez
	if(logico == 1) {
		while(fgets(ponteiro,100,arqInimigos)!=NULL && y <= 14) {
			gotoxy(47,y);
			printf("%s", ponteiro);
			y++;
		}

		gotoxy(44,20);
		printf("Oh nao. Seu mestre quer batalhar!");
		gotoxy(46, 21);
		printf(vermelho"     Prepare-se!"reset);
		getch();
		system("cls");
	}

	y=3;
	
	//Rebobinação do arquivo
	rewind(arqInimigos);
	
	//pesquisa uma determinada linha no arquivo
	do {
		fscanf(arqInimigos,"%s", ponteiro);

	} while(strcmp(ponteiro,"3")!=0);
	
	// printa a partir da linha pesquisda
	while(fgets(ponteiro,100,arqInimigos)!=NULL && y <= 17) {
		gotoxy(47,y);
		printf("%s", ponteiro);
		y++;
	}
	
  	y=20; 
   	arqAtaque=fopen("arquivos/ataque.txt","r");
	gotoxy(47,19);
	printf(verde"Eh a sua vez de jogar!!"reset);
	while(fgets(ataque,200,arqAtaque)!=NULL) {
		gotoxy(45,y);
		printf("%s", ataque);
		y++;
	}

   	x = 53;
   	y = 1;
   	
	// printa na tela dados como nome e vida do inimigo, e nome e vida do personagem atual
   	gotoxy(x+1,y);
	printf("%s     \n",inimigos[2].nome);
	gotoxy(x+4,y+1);
	printf("%d     \n",inimigos[2].vida);
   
 	gotoxy(0,21);
	printf("-----------------------------");
	gotoxy(0,22);
	printf(ciano"Personagem:"reset"  %s       ", personagem.nome);
	gotoxy(0,23);
	printf(ciano"Vida:"reset"  %d     ", personagem.vida);
	gotoxy(0,24);
	printf("-----------------------------");
	
	
	fclose(arqInimigos);
	fclose(arqAtaque);
}

// escolher um atributo depois de terminar a fase 2
void final_fase2(int fase,struct personagem personagem, int atributoEscolhido,int retornaPersonagem) { 
	FILE *arqItem=NULL,*arqContinuar= NULL, *arqMochila=NULL;
	char item[50], pulo[15];
	int y=0,x=0,tecla=0, logico=1,atributoEscolhido2=0, logico2 = 1;

	arqItem=fopen("arquivos/item.txt","r");

	system("cls");
	gotoxy(20,5);
	printf("Voce concluiu a fase "amarelo"%d"reset".", fase);
	gotoxy(15,6);
	printf("Selecione o atributo que voce deseja:");
	
	//sistema para escolher um dos 2 atributos ( porção ou defesa ) 
	if(fase==2) {
		do {
			rewind(arqItem);
			tecla=0;
			y=8;

			fscanf(arqItem,"%s", pulo);

			while(fgets(item,50,arqItem)!=NULL && y<=16) {
				gotoxy(28,y);
				printf("%s", item);
				y++;
			}
			gotoxy(25,18);
			printf(rosa" .----------------."reset);
			gotoxy(25,19);
			printf(rosa"| .--------------. |"reset);
			gotoxy(25,20);
			printf(rosa"| |              | |"reset);
			gotoxy(25,21);
			printf(rosa"| |              | |"reset);
			gotoxy(25,22);
			printf(rosa"| |"verde"    Defesa    "rosa"| |"reset);
			gotoxy(25,23);
			printf(rosa"| |"verde"      +10     "rosa"| |"reset);
			gotoxy(25,24);
			printf(rosa"| |              | |"reset);
			gotoxy(25,25);
			printf(rosa"| |              | |"reset);
			gotoxy(25,26);
			printf(rosa"| |              | |"reset);
			gotoxy(25,27);
			printf(rosa"| '--------------' |"reset);
			gotoxy(25,28);
			printf(rosa" '----------------' "reset);
			y=3;
			x=75;

			gotoxy(x,y);
			printf(rosa" .----------------."reset);
			gotoxy(x,y+1);
			printf(rosa"| .--------------. |"reset);
			gotoxy(x,y+2);
			printf(rosa"| |              | |"reset);
			gotoxy(x,y+3);
			printf(rosa"| |              | |"reset);
			gotoxy(x,y+4);
			printf(rosa"| |"vermelho"    Potions   "rosa"| |"reset);
			gotoxy(x,y+5);
			printf(rosa"| |"vermelho"      10      "rosa"| |"reset);
			gotoxy(x,y+6);
			printf(rosa"| |              | |"reset);
			gotoxy(x,y+7);
			printf(rosa"| |              | |"reset);
			gotoxy(x,y+8);
			printf(rosa"| |              | |"reset);
			gotoxy(x,y+9);
			printf(rosa"| '--------------' |"reset);
			gotoxy(x,y+10);
			printf(rosa" '----------------' "reset);
			y=15;
			
			//printa na tela o que está escrito no arquivo
			while(fgets(item,50,arqItem)!=NULL) {
				gotoxy(x+4,y);
				printf("%s", item);
				y++;
			}
			if(logico==1) {
				gotoxy(46,22);
				printf(verde"<<"reset);
				logico=0;
			}
			tecla=getch();

			x=45;
			y=22;
			
			//altera a posição da seta na tela
			if(tecla==DIREITA) {
				x=45;
				y=22;
				gotoxy(x,y);
				printf("   ");
				x=72;
				y=8;
				gotoxy(x,y);
				printf(verde">>"reset);
				logico2 = 2;
			} else if(tecla==ESQUERDA) {
				x=72;
				y=8;
				gotoxy(x,y);
				printf("   ");
				x=45;
				y=22;
				gotoxy(x,y);
				printf(verde"<<"reset);
				logico2 = 1;
			}
		} while(tecla!=ENTER);
		
		//atribui mais defesa ou mais porções nas variaveis de acordo com o escolhido 
		if(logico2 == 1) {	
			personagem.armadura=personagem.armadura-0.10;
			atributoEscolhido2=3;
		} else {
			arqMochila=fopen("arquivos/mochila.txt","w");
			fprintf(arqMochila,"10");
						
			fclose(arqMochila);
			atributoEscolhido2=4;
		}

	}
	
// escreve no arquivo de save o atriburo escolhido
	arqContinuar=fopen("arquivos/continuar.txt","w");
	fprintf(arqContinuar,"%d\n",3);
	fprintf(arqContinuar,"%d\n",atributoEscolhido);
	fprintf(arqContinuar,"%d\n",atributoEscolhido2);
	fprintf(arqContinuar, "%d\n", retornaPersonagem);
	fclose(arqContinuar);
	fase3(personagem, atributoEscolhido, retornaPersonagem);
	
	fclose(arqItem);
}

// fase 2 do jogo
void fase2(struct personagem personagem, int atributoEscolhido, int retornaPersonagem) {
	FILE *arq = NULL, *arqInfoInimigos=NULL;
	char titulo[200], ponteiro[100], ponteiro2[100];
	int y=11, x=40,tecla=0, logico=1,danoT=0;
	float critico=0;
	struct inimigos inimigos[5];

	system("cls");
	
	arqInfoInimigos=fopen("arquivos/infoInimigos.txt","r"); // abre o arquivo para leitura
	
	//pesquisa a string "esqueleto" dentro do arquivo
	do{
		fscanf(arqInfoInimigos,"%s",ponteiro);

	}while(strcmp(ponteiro,"Esqueleto"));
	
	//atribui a vida e o dano nas variaveis da struct inimigos 
	strcpy(inimigos[1].nome,ponteiro);
	fscanf(arqInfoInimigos, "%d", &inimigos[1].vida);
	fscanf(arqInfoInimigos,"%d", &inimigos[1].dano);


	arq=fopen("arquivos/titulo1.txt","r"); 
	system("cls");

	do{
		fscanf(arq,"%s",ponteiro2);
	}while(strcmp(ponteiro2,"2"));

	while(fgets(titulo,200,arq)!=NULL && y < 19) {
		gotoxy(x,y);
		printf("%s", titulo);
		y++;
	}

	sleep(1);
	system("cls");
	while(inimigos[1].vida>0 && personagem.vida>0) {
		tecla=0;
		x=44;
		y=21;
		
		// muda a posiçao da seta na tela
		while(tecla!=ENTER) {
			system("cls");
			imagemFase2(logico, inimigos, personagem);
			logico=0;
			gotoxy(x,y);
			printf(verde">>"reset);

			tecla=getch();

			if(tecla==ACIMA) {
				if(y>21) {
					y=y-2;
					gotoxy(x,y);
					printf(verde">>"reset);
				} else if(y==21) {
					y=27;
					printf(verde">>"reset);
				}
			} else if(tecla==ABAIXO) {
				if(y<27) {
					y=y+2;
					gotoxy(x,y);
					printf(verde">>"reset);
				} else if(y==27) {
					y=21;
					gotoxy(x,y);
					printf(verde">>"reset);
				}
			} else if(tecla==DIREITA) {
				gotoxy(x, y);
				printf(verde">>"reset);
			} else if(tecla==ESQUERDA) {
				gotoxy(x, y);
				printf(verde">>"reset);
			} else if(tecla!=ACIMA && tecla!=ABAIXO && tecla!=DIREITA && tecla!=ESQUERDA && tecla!=ENTER) {
				gotoxy(x,y);
				printf(verde">>"reset);
			}
		}
		//atribui critico no atque do personagem
		if(y==21) {
			critico=atacar();
			danoT=personagem.ataque1*critico;
			inimigos[1].vida=inimigos[1].vida-danoT;
		} else if(y==23) {
			critico=atacar();
			danoT=personagem.ataque2*critico;
			inimigos[1].vida=inimigos[1].vida-danoT;
		} else if(y==25) {
			critico=atacar();
			danoT=personagem.ataque3*critico;
			inimigos[1].vida=inimigos[1].vida-danoT;
		}else if(y==27){
			imagemFase2(logico, inimigos, personagem);
			mochila(&personagem);
		}

		if(inimigos[1].vida<=0) {
			gotoxy(56,2);
			printf("0    ");
			sleep(1);
			break;
		}

		gotoxy(59,2);
		printf("%d      ", inimigos[1].vida);

		imagemFase2(logico,inimigos,personagem);
		gotoxy(47,19);
		// diminui a vida do personagem depois que o inimigo ataca
		printf(vermelho"  Eh a vez do inimigo!!   "reset);
		sleep(2);
		critico=atacar();
		personagem.vida=personagem.vida-((inimigos[1].dano*critico)*personagem.armadura);
	}
	if(personagem.vida<=0) {
		gotoxy(0,23);
		printf(ciano"Vida:"reset"  %d     ", 0);
		opcao_sair();
	}
	fclose(arq);
	fclose(arqInfoInimigos);	
	final_fase2(2,personagem,atributoEscolhido,retornaPersonagem);
}

// printa na tela as imagens dos arquivos relacionados a fase 2 
void imagemFase2(int logico, struct inimigos inimigos[], struct personagem personagem) {
	FILE *arqInimigos = NULL, *arqAtaque = NULL;
	char ponteiro[100], ataque[100];
	int y = 3,x;
	arqInimigos = fopen("arquivos/inimigos.txt","r");
	
	//altera o ponteiro no arquivo ate enconrar o numero 2
	do {
		fscanf(arqInimigos,"%s", ponteiro);

	} while(strcmp(ponteiro,"2")!=0);
	
	// printa na tela o começo da fase 2
	if(logico == 1) {
		while(fgets(ponteiro,100,arqInimigos)!=NULL && y <= 17) {
			gotoxy(52,y);
			printf("%s", ponteiro);
			y++;
		}

		gotoxy(46,20);
		printf("Oh nao. Mais inimigos a frente!");
		gotoxy(46, 21);
		printf(vermelho"Prepare-se para a batalha!"reset);
		getch();
		system("cls");
	}

	y=3;
	rewind(arqInimigos);
	do {
		fscanf(arqInimigos,"%s", ponteiro);

	} while(strcmp(ponteiro,"2")!=0);

	while(fgets(ponteiro,100,arqInimigos)!=NULL && y <= 17) {
		gotoxy(52,y);
		printf("%s", ponteiro);
		y++;
	}

	y=20;

	arqAtaque=fopen("arquivos/ataque.txt","r");
	gotoxy(49,19);
	printf(verde"Eh a sua vez de jogar!!"reset);
	while(fgets(ataque,200,arqAtaque)!=NULL) {
		gotoxy(47,y);
		printf("%s", ataque);
		y++;
	}

	x = 53;
	y = 1;

	gotoxy(x+1,y);
	printf("%s     \n",inimigos[1].nome);
	gotoxy(x+3,y+1);
	printf("%d     \n",inimigos[1].vida);

	gotoxy(0,21);
	printf("-----------------------------");
	gotoxy(0,22);
	printf(ciano"Personagem:"reset"  %s       ", personagem.nome);
	gotoxy(0,23);
	printf(ciano"Vida:"reset"  %d     ", personagem.vida);
	gotoxy(0,24);
	printf("-----------------------------");
	fclose(arqInimigos);
	fclose(arqAtaque);
}

//função de transição da fase 1 para a 2
void final_fase(int fase, struct personagem personagem, int retornaPersonagem) {
	FILE *arqItem=NULL,*arqContinuar= NULL;
	char item[50];
	int y=0,x=0,tecla=0, logico=1,atributoEscolhido=0;

	arqItem=fopen("arquivos/item.txt","r");

	system("cls");
	gotoxy(20,5);
	printf("Voce concluiu a fase "amarelo"%d"reset".", fase);
	gotoxy(15,6);
	printf("Selecione o atributo que voce deseja:");
	
	// esolher um dos 2 atributos
	if(fase==1) {
		do {
			rewind(arqItem);
			tecla=0;
			fgets(item,50,arqItem);
			gotoxy(28,10);
			printf("%s", item);
			gotoxy(25,12);
			printf(rosa" .----------------."reset);
			gotoxy(25,13);
			printf(rosa"| .--------------. |"reset);
			gotoxy(25,14);
			printf(rosa"| |              | |"reset);
			gotoxy(25,15);
			printf(rosa"| |              | |"reset);
			gotoxy(25,16);
			printf(rosa"| |"vermelho"    Ataque    "rosa"| |"reset);
			gotoxy(25,17);
			printf(rosa"| |"vermelho"      +10     "rosa"| |"reset);
			gotoxy(25,18);
			printf(rosa"| |              | |"reset);
			gotoxy(25,19);
			printf(rosa"| |              | |"reset);
			gotoxy(25,20);
			printf(rosa"| |              | |"reset);
			gotoxy(25,21);
			printf(rosa"| '--------------' |"reset);
			gotoxy(25,22);
			printf(rosa" '----------------' "reset);
			y=3;
			x=75;

			gotoxy(x,y);
			printf(rosa" .----------------."reset);
			gotoxy(x,y+1);
			printf(rosa"| .--------------. |"reset);
			gotoxy(x,y+2);
			printf(rosa"| |              | |"reset);
			gotoxy(x,y+3);
			printf(rosa"| |              | |"reset);
			gotoxy(x,y+4);
			printf(rosa"| |"verde"    Defesa    "rosa"| |"reset);
			gotoxy(x,y+5);
			printf(rosa"| |"verde"     +10      "rosa"| |"reset);
			gotoxy(x,y+6);
			printf(rosa"| |              | |"reset);
			gotoxy(x,y+7);
			printf(rosa"| |              | |"reset);
			gotoxy(x,y+8);
			printf(rosa"| |              | |"reset);
			gotoxy(x,y+9);
			printf(rosa"| '--------------' |"reset);
			gotoxy(x,y+10);
			printf(rosa" '----------------' "reset);
			y=15;
			while(fgets(item,50,arqItem)!=NULL && y<=23) {
				gotoxy(x+4,y);
				printf("%s", item);
				y++;
			}
			if(logico==1) {
				gotoxy(46,16);
				printf(verde"<<"reset);
				logico=0;
			}
			tecla=getch();

			x=45;
			y=16;
			
			// altera a posicao da seta na tela
			if(tecla==DIREITA) {
				x=45;
				y=16;
				gotoxy(x,y);
				printf("   ");
				x=72;
				y=8;
				gotoxy(x,y);
				printf(verde">>"reset);
			} else if(tecla==ESQUERDA) {
				x=72;
				y=8;
				gotoxy(x,y);
				printf("   ");
				x=46;
				y=16;
				gotoxy(x,y);
				printf(verde"<<"reset);
			}
		} while(tecla!=ENTER);

		if(x==45 && y==16) {
			
			//atribui mais ataque ao pesonagem
			personagem.ataque1=personagem.ataque1+10;
			personagem.ataque2=personagem.ataque2+10;
			personagem.ataque3=personagem.ataque3+10;
			atributoEscolhido=1;
		} else {
			
			// atribui mais armadura para o personagem
			personagem.armadura=personagem.armadura-0.10;
			atributoEscolhido=2;
		}

	}
	
	//atribui no arquivo de save o atributo escolhido
	arqContinuar=fopen("arquivos/continuar.txt","w");
	fprintf(arqContinuar,"%d\n",2);
	fprintf(arqContinuar,"%d\n",atributoEscolhido);
	fprintf(arqContinuar,"%d\n", retornaPersonagem);
	fclose(arqContinuar);
	fclose(arqItem);

	fase2(personagem,atributoEscolhido,retornaPersonagem);
}

//Função de escolha de personagem
void escolha() {
	FILE *arqEscolha;
	char nome[50], classe[50];
	int vida, armadura, poder, x=9, i;

	arqEscolha=fopen("arquivos/escolha_personagem.txt", "r");

	gotoxy(5,3);
	printf(amarelo"SELECIONE UM CAMPEAO:\n"reset);
	
	// escreve na tela as informações dos personagens  
	for(i=0; i<3; i++) {
		fscanf(arqEscolha, "%s", nome);
		fscanf(arqEscolha, "%s", classe);
		fscanf(arqEscolha, "%d", &vida);
		fscanf(arqEscolha, "%d", &armadura);
		fscanf(arqEscolha, "%d", &poder);

		gotoxy(x,9);
		printf(rosa"_________________________\n"reset);

		gotoxy(x,10);
		if(i==0) {
			printf(rosa"||"reset"  Nome: %s\t"rosa"||"reset, nome);
		} else {
			printf(rosa"||"reset"  Nome: %s\t\t"rosa"||"reset, nome);
		}
		gotoxy(x,11);
		printf(rosa"||"reset"  Classe: %s\t"rosa"||"reset, classe);
		gotoxy(x,12);
		printf(rosa"||"reset"  Vida: %d\t\t"rosa"||"reset, vida);
		gotoxy(x,13);
		printf(rosa"||"reset"  Armadura: %d\t"rosa"||"reset, armadura);
		gotoxy(x,14);
		printf(rosa"||"reset"  Poder Total: %d\t"rosa"||"reset, poder);
		gotoxy(x,15);
		printf(rosa"|/_____________________\\|"reset);

		x=x+40;
	}
	fclose(arqEscolha);
}

//Função para controle do teclado para a função de escolha
int tecladoEscolha() {
	int tecla=0, x=20, y=7;

	system("cls");
	gotoxy(x,y-1);
	printf(verde"|");
	gotoxy(x,y);
	printf("V"reset);
	while(tecla != ENTER) {
		escolha();
		tecla = getch();
		system("cls");
		if(tecla == ACIMA) {
			gotoxy(x,y-1);
			printf(verde"|");
			gotoxy(x,y);
			printf("V"reset);
		} else if(tecla == ABAIXO) {
			gotoxy(x,y-1);
			printf(verde"|");
			gotoxy(x,y);
			printf("V"reset);
		} else if(tecla == 	DIREITA) {
			if(x<100) {
				x=x+40;
				gotoxy(x,y-1);
				printf(verde"|");
				gotoxy(x,y);
				printf("V"reset);
			} else if(x==100) {
				x=20;
				gotoxy(20,y-1);
				printf(verde"|");
				gotoxy(20,y);
				printf("V"reset);
			}
		} else if(tecla == 	ESQUERDA) {
			if(x>20) {
				x=x-40;
				gotoxy(x,y-1);
				printf(verde"|");
				gotoxy(x,y);
				printf("V"reset);
			} else if(x==20) {
				x=100;
				gotoxy(100,y-1);
				printf(verde"|");
				gotoxy(100,y);
				printf("V"reset);
			}
		} else if (tecla!=ACIMA && tecla!=ABAIXO && tecla!=DIREITA && tecla!=ESQUERDA && tecla!=ENTER) {
			gotoxy(x, y-1);
			printf(verde"|");
			gotoxy(x, y);
			printf("V"reset);

		}
	}

	if(x==20) {
		return 1;
	} else if(x==60) {
		return 2;
	} else {
		return 3;
	}
}

//Função para a tela do menu
void menu() {
	int y=4;
	FILE *arqTitulo;
	FILE *arqXY;
	FILE *arqOpcoes;
	char titulo_menu[200],xy[200], opcoes[200];

	arqTitulo=fopen("Arquivos/titulo_menu.txt","r");
	
	while(fgets(titulo_menu,200,arqTitulo)!=NULL) {
		gotoxy(24,y);
		printf(ciano"%s"reset, titulo_menu);
		y++;
	}

	y=11;
	arqXY=fopen("arquivos/xy.txt","r");

	while(fgets(xy,200,arqXY)!=NULL) {
		gotoxy(89,y);
		printf(verde"%s"reset,xy);
		y++;
	}

	y=14;
	arqOpcoes=fopen("arquivos/opcoes_menu.txt","r");

	while(fgets(opcoes,200,arqOpcoes)!=NULL) {
		gotoxy(26,y);
		printf("%s",opcoes);
		y=y+2;
	}

	fclose(arqTitulo);
	fclose(arqXY);
	fclose(arqOpcoes);
}

//Função para o fechamento do programa
void opcao_sair () {
	FILE *arqSair;
	char texto_final[200];
	int y=4;

	system("cls");
	arqSair=fopen("arquivos/texto_final.txt", "r");

	while(fgets(texto_final, 200, arqSair)!=NULL) {
		gotoxy(13,y);
		printf(vermelho"%s"reset, texto_final);
		y++;
	}

	printf("\n\n\n\n\n");

	fclose(arqSair);
	exit(1);
}

//Função para mostra como jogar
void como_jogar() {
	FILE *arq;
	char texto_livro[200];
	int y=2;

	system("cls");
	arq=fopen("arquivos/como_jogar.txt", "r");

	while(fgets(texto_livro,200,arq)!=NULL) {
		gotoxy(3,y);
		printf("%s", texto_livro);
		y++;
	}

	gotoxy(23,20);
	printf(verde">>"reset);
	printf(" VOLTAR");

	getch();

	fclose(arq);
}

//Função que mostra todos os dados do personagem escolhido
void iniciar_jogo(struct personagem personagem) {
	gotoxy(5,3);
	printf("Voce selecionou"amarelo" %s.\n"reset, personagem.nome);
	gotoxy(5,5);
	printf("Estes sao os atributos totais do seu campeao:\n");
	gotoxy(48, 9);
	printf(ciano"Classe:"reset" %s\n", personagem.classe);
	gotoxy(44,11);
	printf(ciano"Pontos de Vida:"reset" %d\n", personagem.vida);
	gotoxy(47,13);
	if(personagem.armadura==0) {
		printf(ciano"Armadura:"reset" Nenhuma\n");
	} else {
		printf(ciano"Armadura:"reset" %.2f\n", personagem.armadura);
	}
	gotoxy(40,15);
	printf(ciano"Pontos de poder totais:"reset" %d", personagem.poder);
	gotoxy(41,17);
	printf(ciano"Dano de ataque magico:"reset" %d", personagem.ataque1);
	gotoxy(40,19);
	printf(ciano"Dano de ataque hibrido:"reset" %d", personagem.ataque2);
	gotoxy(42,21);
	printf(ciano"Dano de ataque fisico:"reset" %d", personagem.ataque3);
	gotoxy(48,23);
	printf(ciano"Arma: "reset"%s", personagem.mochila->armas);

	gotoxy(5,27);
	printf(verde"Pressione alguma tecla para continuar\n"reset);

	getch();
}

//Função para acessar o invetario
void mochila(struct personagem *personagem) {
	FILE *arqMochila=NULL, *arqMochilaAux=NULL;
	char textoMochila[50];
	int tecla=0,aux=0;

	arqMochila=fopen("arquivos/mochila.txt","r");
	arqMochilaAux=fopen("arquivos/mochilaAux.txt","w");

	fflush(stdin);

	fgets(textoMochila,50,arqMochila);

	aux=atoi(textoMochila);
	personagem->mochila->pocoes=atoi(textoMochila);

	gotoxy(48,21);
	printf(verde"      Potion x%d     "reset, aux);
	gotoxy(48,23);
	printf("       %s          ",personagem->mochila->armas);
	gotoxy(48,25);
	printf("                   ");
	gotoxy(48,27);
	printf(vermelho"   VOLTAR(BACKSPACE)   "reset"|"vermelho"   Voce ira perder sua vez."reset);

	gotoxy(42, 27);
	printf("   ");

	do {
		tecla=0;
		gotoxy(42,21);
		printf(verde">>"reset);
		tecla=getch();

		if(tecla==BACKSPACE) {
			break;
		} else if(tecla==ENTER) {
			if(aux>0) {
				aux--;
				personagem->mochila->pocoes--;
				personagem->vida=personagem->vida+40;
				itoa(aux,textoMochila,10);
				fprintf(arqMochilaAux,"%s",textoMochila);
				fclose(arqMochila);
				fclose(arqMochilaAux);
				remove("arquivos/mochila.txt");
				rename("arquivos/mochilaAux.txt", "arquivos/mochila.txt");
			} else {
				break;
			}
		}
	} while(tecla!=BACKSPACE && tecla!=ENTER);

	system("cls");
}

//Função que imprime a imagem da fase 1
void imagemFase1 (int logico, struct inimigos inimigos[], struct personagem personagem) {
	int i=0, y=3,x;
	char imagem_inimigos[100], ataque[200];
	FILE *arqInimigos= NULL, *arqAtaque=NULL;

	arqInimigos=fopen("arquivos/inimigos.txt","r");
	x=53;

	if(logico==1) {

		rewind(arqInimigos);
		y=3;
		gotoxy(x, y);
		printf(verde"   \\\\\\"vermelho"|||"verde"/// "reset);
		y=4;


		while(fgets(imagem_inimigos,200,arqInimigos)!=NULL) {

			gotoxy(x,y);
			if(y==4) {
				printf(ciano" .  "amarelo"%s"reset, imagem_inimigos);
			} else if(y==5) {
				printf(ciano"/ \\"reset" %s", imagem_inimigos);
			} else if(y==6) {
				printf(ciano"\\ /  "reset"%s", imagem_inimigos);
			} else if(y==7) {
				printf(azul" #   "reset"%s", imagem_inimigos);
			} else if(y==9) {
				printf(azul" #"reset"%s", imagem_inimigos);
			} else if(y==10) {
				printf(azul" #"reset"%s", imagem_inimigos);
			} else if(y==11) {
				printf(azul" #   "reset"%s", imagem_inimigos);
			} else if(y==12) {
				printf(azul" #   "reset"%s", imagem_inimigos);
			} else if(y==13) {
				printf(azul" #   "reset"%s", imagem_inimigos);
			} else if(y==14) {
				printf(azul" #"reset"%s", imagem_inimigos);
			} else if(y==15) {
				printf(azul" #"reset"%s", imagem_inimigos);
			} else if(y==16) {
				break;
			} else {
				gotoxy(x,y);
				printf("%s", imagem_inimigos);
			}
			y++;
		}
		gotoxy(46,20);
		printf("Oh nao. Inimigo a frente!");
		gotoxy(46, 21);
		printf(vermelho"Prepare-se para a batalha!"reset);
		getch();
		system("cls");
	}

	x=53;

	rewind(arqInimigos);
	y=1;
	gotoxy(x+4,y);
	printf("%s     \n",inimigos[i].nome);
	gotoxy(x+6,y+1);
	printf("%d     \n",inimigos[i].vida);

	y=3;
	gotoxy(x, y);
	printf(verde"   \\\\\\"vermelho"|||"verde"/// "reset);
	y=4;
	while(fgets(imagem_inimigos,200,arqInimigos)!=NULL) {

		gotoxy(x,y);
		if(y==4) {
			printf(ciano" .  "amarelo"%s"reset, imagem_inimigos);
		} else if(y==5) {
			printf(ciano"/ \\"reset" %s", imagem_inimigos);
		} else if(y==6) {
			printf(ciano"\\ /  "reset"%s", imagem_inimigos);
		} else if(y==7) {
			printf(azul" #   "reset"%s", imagem_inimigos);
		} else if(y==9) {
			printf(azul" #"reset"%s", imagem_inimigos);
		} else if(y==10) {
			printf(azul" #"reset"%s", imagem_inimigos);
		} else if(y==11) {
			printf(azul" #   "reset"%s", imagem_inimigos);
		} else if(y==12) {
			printf(azul" #   "reset"%s", imagem_inimigos);
		} else if(y==13) {
			printf(azul" #   "reset"%s", imagem_inimigos);
		} else if(y==14) {
			printf(azul" #"reset"%s", imagem_inimigos);
		} else if(y==15) {
			printf(azul" #"reset"%s", imagem_inimigos);
		} else if(y==16) {
			break;
		} else {
			gotoxy(x,y);
			printf("%s", imagem_inimigos);
		}
		
		y++;
	}

	y=20;

	arqAtaque=fopen("arquivos/ataque.txt","r");
	gotoxy(49,19);
	printf(verde"Eh a sua vez de jogar!!"reset);
	while(fgets(ataque,200,arqAtaque)!=NULL) {
		gotoxy(47,y);
		printf("%s", ataque);
		y++;
	}
	
	gotoxy(0,21);
	printf("-----------------------------");
	gotoxy(0,22);
	printf(ciano"Personagem:"reset"  %s       ", personagem.nome);
	gotoxy(0,23);
	printf(ciano"Vida:"reset"  %d     ", personagem.vida);
	gotoxy(0,24);
	printf("-----------------------------");
	fclose(arqInimigos);
	fclose(arqAtaque);

}

//Função para sorteio do valor de ataque crítico
float atacar() {
	int numero;

	srand(time(NULL));
	numero = rand()%2;

	if(numero==0) {
		return 1.0;
	} else if(numero==1) {
		return 1.5;
	} else if(numero==2) {
		return 2.0;
	}
	
}

//Função de execução da fase 1
void fase1(struct personagem personagem, int retornaPersonagem) {
	FILE *arq = NULL, *arqInfoInimigos=NULL;
	char titulo[200], ponteiro2[100];
	int y=11, x=40,tecla=0, logico=1, i=0,danoT=0;
	float critico=0;
	struct inimigos inimigos[5];

	arqInfoInimigos=fopen("arquivos/infoInimigos.txt","r");

	fscanf(arqInfoInimigos, "%s", inimigos[i].nome);
	fscanf(arqInfoInimigos, "%d", &inimigos[i].vida);
	fscanf(arqInfoInimigos,"%d", &inimigos[i].dano);

	arq=fopen("arquivos/titulo1.txt","r");
	system("cls");

	do{
		fscanf(arq,"%s",ponteiro2);
	}while(strcmp(ponteiro2,"1"));

	while(fgets(titulo,200,arq)!=NULL && y < 19) {
		gotoxy(x,y);
		printf("%s", titulo);
		y++;
	}

	sleep(1);
	system("cls");
	while(inimigos[0].vida>0 && personagem.vida>0) {
		tecla=0;
		x=44;
		y=21;

		while(tecla!=ENTER) {
			system("cls");
			imagemFase1(logico, inimigos, personagem);
			logico=0;
			gotoxy(x,y);
			printf(verde">>"reset);

			tecla=getch();

			if(tecla==ACIMA) {
				if(y>21) {
					y=y-2;
					gotoxy(x,y);
					printf(verde">>"reset);
				} else if(y==21) {
					y=27;
					printf(verde">>"reset);
				}
			} else if(tecla==ABAIXO) {
				if(y<27) {
					y=y+2;
					gotoxy(x,y);
					printf(verde">>"reset);
				} else if(y==27) {
					y=21;
					gotoxy(x,y);
					printf(verde">>"reset);
				}
			} else if(tecla==DIREITA) {
				gotoxy(x, y);
				printf(verde">>"reset);
			} else if(tecla==ESQUERDA) {
				gotoxy(x, y);
				printf(verde">>"reset);
			} else if(tecla!=ACIMA && tecla!=ABAIXO && tecla!=DIREITA && tecla!=ESQUERDA && tecla!=ENTER) {
				gotoxy(x,y);
				printf(verde">>"reset);
			}
		}

		if(y==21) {
			critico=atacar();
			danoT=personagem.ataque1*critico;
			inimigos[0].vida=inimigos[0].vida-danoT;
		} else if(y==23) {
			critico=atacar();
			danoT=personagem.ataque2*critico;
			inimigos[0].vida=inimigos[0].vida-danoT;
		} else if(y==25) {
			critico=atacar();
			danoT=personagem.ataque3*critico;
			inimigos[0].vida=inimigos[0].vida-danoT;
		}else if(y==27){
			imagemFase1(logico, inimigos, personagem);
			mochila(&personagem);
		}

		if(inimigos[0].vida<=0) {
			gotoxy(59,2);
			printf("0    ");
			sleep(1);
			break;
		}

		gotoxy(59,2);
		printf("%d      ", inimigos[0].vida);

		imagemFase1(logico,inimigos,personagem);
		gotoxy(47,19);

		printf(vermelho"  Eh a vez do inimigo!!   "reset);
		sleep(2);
		critico=atacar();
		personagem.vida=personagem.vida-((inimigos[0].dano*critico)*personagem.armadura);
	}

	if(personagem.vida<=0) {
		opcao_sair();
	}
	
	fclose(arq);
	fclose(arqInfoInimigos);

	final_fase(1,personagem,retornaPersonagem);
	
}

//Função para baertura do arquivo de save e recuperar os dados de partidas interrompidas anteriormente
void continuar (struct personagem personagem) {
	FILE *arq = NULL, *arqMochila=NULL, *arqEscolhido=NULL, *arqPersonagens=NULL, *arqNSave=NULL;
	char arquivo[100], per_escolhido[15], pesquisa[50], save[300];
	int fase=0,atributo=0,atributo2=0, retornaPersonagem, y;

	arq=fopen("arquivos/continuar.txt","r");
	arqPersonagens=fopen("arquivos/infoPersonagens.txt","r");
	arqNSave=fopen("arquivos/tela_continuar.txt","r");
	system("cls");

	y=3;
	
	if(fgets(arquivo,100, arq)==NULL) {
		while(fgets(save,300,arqNSave)!=NULL){
			gotoxy(17,y);
			printf(rosa"%s"reset, save);
			y++;
		}
		gotoxy(0, 20);
		printf(verde"Pressione alguma tecla para voltar"reset);
		getch();
		main();
		fflush(stdin);
	} else {
		fase=atoi(arquivo);
		if(fase==2){
			fscanf(arq,"%d",&atributo);
			fscanf(arq,"%d",&retornaPersonagem);
		}else if(fase==3){
			fscanf(arq,"%d",&atributo);
			fscanf(arq,"%d",&atributo2);
			fscanf(arq,"%d",&retornaPersonagem);
		}
	}

	if(atributo==1) {
		personagem.ataque1=personagem.ataque1+10;
		personagem.ataque2=personagem.ataque2+10;
		personagem.ataque3=personagem.ataque3+10;
	} else if(atributo==2) {
		personagem.armadura=personagem.armadura+10;
	}

	if(atributo2==3) {
		personagem.armadura=personagem.armadura+10;
	} else if(atributo2==4) {
		arqMochila=fopen("arquivos/mochila.txt","w");
		fprintf(arqMochila,"10");
		fclose(arqMochila);
	}

	switch(retornaPersonagem) {
		case 1:
			strcpy(per_escolhido,"Cloud_Strife");
		break;
		case 2:
			strcpy(per_escolhido,"Yuna");
		break;
		case 3:
			strcpy(per_escolhido,"Snow");
		break;
	}
	//reseta arquivo do personagem atual para atribuir o novo personagem escolhido
	arqEscolhido = fopen("arquivos/personagemAtual.txt","w+");
	rewind(arqPersonagens);
	do {
		fscanf(arqPersonagens,"%s", pesquisa);
	} while(strcmp(pesquisa,per_escolhido)!=0);
	strcpy(personagem.nome,pesquisa);
	fscanf(arqPersonagens,"%s",personagem.classe);
	fscanf(arqPersonagens,"%d %f %d %d",&personagem.vida, &personagem.armadura, &personagem.arma,&personagem.poder);
	fscanf(arqPersonagens,"%s %d",personagem.atq1Nome,&personagem.ataque1);
	fscanf(arqPersonagens,"%s %d",personagem.atq2Nome,&personagem.ataque2);
	fscanf(arqPersonagens,"%s %d",personagem.atq3Nome,&personagem.ataque3);
	fscanf(arqPersonagens, "%s", personagem.mochila->armas);

	fprintf(arqEscolhido,"%s\n%s\n",personagem.nome,personagem.classe);
	fprintf(arqEscolhido,"%d\n%f\n%d\n%d\n",personagem.vida, personagem.armadura, personagem.arma, personagem.poder);
	fprintf(arqEscolhido,"%s\n%d\n",personagem.atq1Nome,personagem.ataque1);
	fprintf(arqEscolhido,"%s\n%d\n",personagem.atq2Nome,personagem.ataque2);
	fprintf(arqEscolhido,"%s\n%d\n",personagem.atq3Nome,personagem.ataque3);
	fprintf(arqEscolhido,"%s\n", personagem.mochila->armas);
	
	if(fase==2) {
		fase2(personagem,atributo, retornaPersonagem);
	}else if(fase == 3){
		fase3(personagem,atributo, retornaPersonagem);
	} 

	fclose(arq);
	fclose(arqMochila);
	fclose(arqEscolhido);
	fclose(arqPersonagens);
	fclose(arqNSave);
	
}

//Função para o controle do teclado da tela do menu
int tecladoMenu() {
	int tecla, x = 23, y = 14;

	system("cls");
	gotoxy(x,y);
	printf(verde">>"reset);
	while(tecla != ENTER) {
		menu();
		tecla = getch();
		system("cls");
		if(tecla == ACIMA) {
			if(y > 14) {
				y = y - 2;
				gotoxy(x,y);
				printf(verde">>"reset);
			} else if(y == 14) {
				y=20;
				gotoxy(x,20);
				printf(verde">>"reset);

			}
		} else if(tecla == ABAIXO) {
			if(y < 20) {
				y = y + 2;
				gotoxy(x,y);
				printf(verde">>"reset);
			} else if(y == 20) {
				y=14;
				gotoxy(x,14);
				printf(verde">>"reset);
			}
		} else if(tecla == 	DIREITA) {
			gotoxy(x,y);
			printf(verde">>"reset);
		} else if(tecla == 	ESQUERDA) {
			gotoxy(x,y);
			printf(verde">>"reset);
		} else if (tecla!=ACIMA && tecla!=ABAIXO && tecla!=DIREITA && tecla!=ESQUERDA && tecla!=ENTER) {
			gotoxy(x, y);
			printf(verde">>"reset);
		}
	}

	if(y == 14) {
		return 1;
	} else if ( y == 16) {
		return 2;
	} else if( y == 18) {
		return 3;
	} else {
		return 4;
	}
	
}

//apagar o cursor da tela
void HideCursor() {
	CONSOLE_CURSOR_INFO cursor = {1, FALSE};
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
}

int main() {
	int opcao=0, retornaPersonagem=0;
	char per_escolhido[15], pesquisa[50];
	struct personagem personagem;
	FILE *arqPersonagens = NULL, *arqEscolhido = NULL;
	FILE *arqmochila=NULL, *arqContinuar=NULL;

	arqPersonagens = fopen("arquivos/infoPersonagens.txt","r");
	rewind(arqPersonagens);
	do {
		opcao=0;
		retornaPersonagem=0;
		HideCursor();
		opcao = tecladoMenu();
		fflush(stdin);

		switch(opcao) {
			case NOVO_JOGO:
				arqContinuar=fopen("arquivos/continuar.txt","w");
				fclose(arqContinuar);

				arqmochila=fopen("arquivos/mochila.txt","w");
				fprintf(arqmochila,"10");
				fclose(arqmochila);
				//receber o personagem escolhido
				retornaPersonagem = tecladoEscolha();

				switch(retornaPersonagem) {
					case 1:
						strcpy(per_escolhido,"Cloud_Strife");
						break;
					case 2:
						strcpy(per_escolhido,"Yuna");
						break;
					case 3:
						strcpy(per_escolhido,"Snow");
						break;
				}
				//reseta arquivo do personagem atual para atribuir o novo personagem escolhido
				arqEscolhido = fopen("arquivos/personagemAtual.txt","w+");
				rewind(arqPersonagens);
				do {
					fscanf(arqPersonagens,"%s", pesquisa);
				} while(strcmp(pesquisa,per_escolhido)!=0);

				strcpy(personagem.nome,pesquisa);
				fscanf(arqPersonagens,"%s",&personagem.classe);
				fscanf(arqPersonagens,"%d %f %d %d",&personagem.vida, &personagem.armadura, &personagem.arma,&personagem.poder);
				fscanf(arqPersonagens,"%s %d",&personagem.atq1Nome,&personagem.ataque1);
				fscanf(arqPersonagens,"%s %d",&personagem.atq2Nome,&personagem.ataque2);
				fscanf(arqPersonagens,"%s %d",&personagem.atq3Nome,&personagem.ataque3);
				fscanf(arqPersonagens, "%s", &personagem.mochila->armas);

				fprintf(arqEscolhido,"%s\n%s\n",personagem.nome,personagem.classe);
				fprintf(arqEscolhido,"%d\n%f\n%d\n%d\n",personagem.vida, personagem.armadura, personagem.arma, personagem.poder);
				fprintf(arqEscolhido,"%s\n%d\n",personagem.atq1Nome,personagem.ataque1);
				fprintf(arqEscolhido,"%s\n%d\n",personagem.atq2Nome,personagem.ataque2);
				fprintf(arqEscolhido,"%s\n%d\n",personagem.atq3Nome,personagem.ataque3);
				fprintf(arqEscolhido,"%s\n", personagem.mochila->armas);

				iniciar_jogo(personagem);
				fase1(personagem,retornaPersonagem);
				system("cls");
				fclose(arqPersonagens);
				fclose(arqEscolhido);
				break;
			case CONTINUAR:
				continuar(personagem);
				break;
			case COMO_JOGAR:
				como_jogar();
				break;
			case SAIR:
				opcao_sair();
				break;
		}
		
	} while(opcao != 4);
	return 0;
}
