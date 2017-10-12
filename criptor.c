#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define version 1.02

char * CarregarArquivo(char * filename);
void menu_command(char * command,char * filename,char * chave);
int CriptografarBloco(char * Memory,char * Chave);
int DescriptografarBloco(char * Memory,char * Chave);
void GravarEmArquivo(char * Memory,char * filename);

void main(int args,char * params[]){
	int r_menu=0;
	printf("## Terminal Encryptor %0.2fv ##\n",version);
	if(args < 4){
		printf("Parametros incorretos, Porfavor digite corretamente\n");
		printf("Ex: ./criptor <Comando> <NomeArquivo> <Chave> \n");
		return;
	}
	menu_command(params[1],params[2],params[3]);
}

char * CarregarArquivo(char * filename){
	FILE * wFile;
	char * Memory;
	int tamanho=0;
	wFile = fopen(filename,"rb");
	if(wFile == NULL){
		printf("[F] Arquivo não encontrado. \n");
		return NULL;
	}else{
		printf("[Ok] Arquivo encontrado\n");
		fseek(wFile,0,SEEK_END);
		tamanho = ftell(wFile)+sizeof(int);
		rewind(wFile);
		printf("[**] Tamanho em bytes: %d \n",tamanho);
		Memory = (char*)malloc(tamanho);
		memcpy(&Memory[0],&tamanho,sizeof(int)); 
		if(Memory == NULL){
			printf("[F] Não foi possivel allocar espaço de memoria. \n");
			return NULL;
		}else{
			tamanho = fread(&Memory[sizeof(int)],1,tamanho,wFile);
			if(tamanho < 0){
				printf("[F] Falha em ler os dados. \n");
				return NULL;
			}else{
				printf("[Ok] leitura realizada com sucesso. \n");
				return &Memory[0];
			}
		}
	}
	fclose(wFile);
}

int CriptografarBloco(char * Memory,char * Chave){
	int auxiliar=sizeof(int),auxiliar1=0,tamanho;
	memcpy(&tamanho,&Memory[0],sizeof(int));
	if(strlen(Chave) < 6){
		printf("[F] A chave deve conter no minimo 6 caracteres \n");
		return 0;
	}
	if(tamanho < 10){
		printf("[F] Voce so pode criptografar um bloco de no minimo 10 caracteres \n");
		return 0;
	}
	while(auxiliar != tamanho){
		if(Memory[auxiliar] != 0x00){
			Memory[auxiliar] = Memory[auxiliar] + Chave[auxiliar1];
		}
		auxiliar1++;
		if(auxiliar1 == strlen(Chave)){
			auxiliar1=0;
		}
		auxiliar++;
	}
}

int DescriptografarBloco(char * Memory,char * Chave){
	int auxiliar=sizeof(int),auxiliar1=0,tamanho;
	memcpy(&tamanho,&Memory[0],sizeof(int));
	if(strlen(Chave) < 6){
		printf("[F] A chave deve conter no minimo 6 caracteres \n");
		return 0;
	}
	if(tamanho < 10){
		printf("[F] Voce so pode criptografar um bloco de no minimo 10 caracteres \n");
		return 0;
	}
	while(auxiliar != tamanho){
		if(Memory[auxiliar] != 0x00){
			Memory[auxiliar] = Memory[auxiliar] - Chave[auxiliar1];
		}
		auxiliar1++;
		if(auxiliar1 == strlen(Chave)){
			auxiliar1=0;
		}
		auxiliar++;
	}
}

void GravarEmArquivo(char * Memory,char * filename){
	FILE * wFile;
	int nbytes=0,tamanho=0;
	memcpy(&tamanho,&Memory[0],sizeof(int));
	tamanho = tamanho - sizeof(int);
	wFile = fopen(filename,"wb");
	if(wFile == NULL){
		printf("[F] Falha ao criar arquivo. \n");
		return;
	}else{
		printf("[Ok] Arquivo criado com sucesso. \n");
		nbytes = fwrite(&Memory[sizeof(int)],1,tamanho,wFile);
		if(nbytes < 0){
			printf("[F] Falha em escrever  o arquivo Criptografado. \n");
			return;
		}else{
			printf("[Ok] Arquivo criptografado com sucesso. \n");
		}
	}
	fclose(wFile);
}

void menu_command(char * command,char * filename,char * chave){
	char * Memory = NULL;
	if(strcmp(command,"criptorfile") == 0){
		Memory = CarregarArquivo(filename);
		if(Memory == NULL){
			printf("[F] Nao foi possivel carregar o arquivo. \n");
			return;
		}
		if(CriptografarBloco(Memory,chave) == 0){
			return;
		}
		GravarEmArquivo(Memory,filename);
		return;
	}
	if(strcmp(command,"descriptorfile") == 0){
		Memory = CarregarArquivo(filename);
		if(Memory == NULL){
			printf("[F] Nao foi possivel carregar o arquivo. \n");
			return;
		}
		if(DescriptografarBloco(Memory,chave) == 0){
			return;
		}
		GravarEmArquivo(Memory,filename);
		return;
	}
	printf("Digite  um comando valido. \n");
}
