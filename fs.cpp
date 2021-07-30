/**
 * Implemente aqui as funções dos sistema de arquivos que simula EXT3
 */

//  g++ main.cpp fs.cpp -o main -lpthread
// .main

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <string.h>

#include "fs.h"

using namespace std;

void initFs(string fsFileName, int blockSize, int numBlocks, int numInodes)
{

  printf("\n \n");

  cout << "Nome: " << fsFileName << endl;
  cout << "Tamanho dos blocos          (T): " << blockSize << endl;
  cout << "Numero de blocos            (N): " << numBlocks << endl;
  cout << "Numero de Inodes            (I): " << numInodes << endl;

  FILE *arquivo = fopen(fsFileName.c_str(), "w+b"); // Leitura do arquivo
  INODE vetorInodes;

  int tamanhoMapaBits = ceil(((float)numBlocks) / 8); // 4 bytes para representar 32 blocos
  char mapaBits = 0x01;

  int tamanhoVetorInodes = sizeof(INODE) * numInodes;

  cout << "Tamanho do Mapa de Bits     (MB): " << tamanhoMapaBits << endl;
  cout << "Tamanho do vetor de Inodes  (VI): " << tamanhoVetorInodes << endl;
  printf("\n \n");

  vetorInodes.IS_USED = 1; // quando é usado e quando não é?
  vetorInodes.IS_DIR = 1;

  for (int i = 0; i < sizeof(vetorInodes.NAME); i++)
  {
    if (i == 0)
    {
      vetorInodes.NAME[i] = '/';
    }
    else
    {
      vetorInodes.NAME[i] = 0;
    }
  }

  vetorInodes.SIZE = 0;

  for (int i = 0; i < sizeof(vetorInodes.DIRECT_BLOCKS); i++)
  {
    vetorInodes.DIRECT_BLOCKS[i] = 0;
    vetorInodes.INDIRECT_BLOCKS[i] = 0;
    vetorInodes.DOUBLE_INDIRECT_BLOCKS[i] = 0;
  }

  int tamanhoVetorBlocos = blockSize * numBlocks;

  if (arquivo == NULL)
  {
    cout << "Erro ao abrir o arquivo!" << arquivo << endl;
    system("pause");
    exit(1);
  }
  else
  {
    char complemento = 0;

    fwrite(&blockSize, sizeof(char), 1, arquivo);
    fwrite(&numBlocks, sizeof(char), 1, arquivo);
    fwrite(&numInodes, sizeof(char), 1, arquivo);

    fwrite(&mapaBits, sizeof(char), 1, arquivo);

    for (int i = 0; i < tamanhoMapaBits - 1; i++)
    {
      fwrite(&complemento, sizeof(char), 1, arquivo);
    }

    for (int i = 0; i < numInodes; i++)
    {
      if (i == 0)
      {
        fwrite(&vetorInodes, sizeof(INODE), 1, arquivo);
      }
      else
      {
        vetorInodes.IS_USED = 0;
        vetorInodes.IS_DIR = 0;
        vetorInodes.NAME[0] = 0;
        vetorInodes.SIZE = 0;

        fwrite(&vetorInodes, sizeof(INODE), 1, arquivo);
      }
    }

    fwrite(&complemento, sizeof(char), 1, arquivo);

    for (int i = 0; i < tamanhoVetorBlocos; i++)
    {
      fwrite(&complemento, sizeof(char), 1, arquivo);
    }
  }

  fclose(arquivo);
}

void addFile(string fsFileName, string filePath, string fileContent)
{
  FILE *arquivo = fopen(fsFileName.c_str(), "r+b");
  
  typedef struct
  {
    int TAMANHO_BLOCOS; 
    int NUM_BLOCOS;  
    int NUM_INODES;         
  } CARACTER;


  if (arquivo == NULL)
  {
    cout << "Erro ao abrir o arquivo!" << arquivo << endl;
    system("pause");
    exit(1);
  }
  else 
  {
    char buffer[10];
    int posicaoInodes;

    CARACTER novoModelo;
    INODE vetorInodes;

    fseek(arquivo, 0, SEEK_SET); 
    fread(buffer, sizeof(char), 3, arquivo); // 1 por 1

    // Pego os 3 primeiros bytes do arquivo (char -> 8 bits)
    novoModelo.TAMANHO_BLOCOS = buffer[0]; 
    novoModelo.NUM_BLOCOS     = buffer[1];
    novoModelo.NUM_INODES     = buffer[2];

    int tamanhoMapaBits    = ceil(((float) novoModelo.NUM_BLOCOS) / 8);
    int tamanhoVetorBlocos = novoModelo.TAMANHO_BLOCOS * novoModelo.NUM_BLOCOS;
    int tamanhoVetorInodes = sizeof(INODE) * novoModelo.NUM_INODES;

// Vou desolocar 4 bytes (TamBlocos, numBlocos, NumInodes, mapaBits)
    fseek(arquivo, 4, SEEK_SET);
// Quantos Inodes eu tenho? - 132

    filePath.erase(0, 1);
// Qual Inode está ocupado
    for (int i = 0; i < novoModelo.NUM_INODES; i++)
    {
      char valorInode;
      fread(&valorInode, sizeof(vetorInodes), 1, arquivo);

      if (!valorInode) 
      {
        vetorInodes.IS_USED = 0x01;
        vetorInodes.IS_DIR = 0x00;

        strcpy(vetorInodes.NAME, filePath.c_str());
        
        vetorInodes.SIZE = 0x00;
        vetorInodes.DIRECT_BLOCKS[0] = 0;
        vetorInodes.INDIRECT_BLOCKS[0] = 0;
        vetorInodes.DOUBLE_INDIRECT_BLOCKS[0] = 0;

        fseek(arquivo, -sizeof(vetorInodes), SEEK_CUR);
        fwrite(&vetorInodes, sizeof(INODE), 1, arquivo);

        printf("Antes: %x \n", ftell(arquivo));

        while(i < novoModelo.NUM_INODES - 1) {
          printf("No while: %x \n", ftell(arquivo));
          fread(&valorInode, sizeof(vetorInodes), 1, arquivo);
          i++;
        }

        printf("Depois: %x \n", ftell(arquivo));

        int pastaRaiz = 0x01;
        fwrite(&pastaRaiz, sizeof(char), 1, arquivo);
        fwrite(&fileContent, sizeof(char), novoModelo.TAMANHO_BLOCOS, arquivo);

        break;
      }


    }
    
    // cout << "------------------------ Parametros iniciais ------------------" << endl << endl;
    // cout << "Nome do Arquivo:            " << fsFileName << endl;
    // cout << "Nome da Pasta:              " << filePath << endl;
    // cout << "Conteudo:                   " << fileContent << endl;
    // cout << "Tamanho do Mapa de BITS:    " << tamanhoMapaBits << endl;
    // cout << "Tamanho do vetor de Blocos: " << tamanhoVetorBlocos << endl;
    // cout << "Tamanho do vetor de Inodes: " << tamanhoVetorInodes << endl;
    // cout << "Tamanho do Inode:           " << sizeof(vetorInodes) << endl << endl;
    // // cout << "Posicao do InodeLivre:      " << posicaoInodeLivre << endl << endl;
    // cout << "--------------------------------------------------------------" << endl << endl;

    // cout << "------------------------ Primeiros 3 bytes --------------------" << endl << endl;
    // cout << "Tamanho dos blocos: " << novoModelo.TAMANHO_BLOCOS << endl;
    // cout << "Numero de blocos:   " << novoModelo.NUM_BLOCOS << endl;
    // cout << "Numero de Inodes:   " << novoModelo.NUM_INODES << endl << endl;
    // cout << "--------------------------------------------------------------" << endl;
    
  }
}

void addDir(std::string fsFileName, std::string dirPath)
{
}

void remove(std::string fsFileName, std::string path)
{
}

void move(std::string fsFileName, std::string oldPath, std::string newPath)
{
}
