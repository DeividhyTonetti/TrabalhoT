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

/*
    -> Tamanho dos blocos (T)   : char 1 byte - 8 bits;               -> OK (blockSize)
    -> Número de blocos   (N)   : char 1 byte - 8 bits;               -> OK (numBlocks)
    -> Número de Inodes   (I)   : char 1 byte - 8 bits;               -> OK (numInodes)
    -> Mapa de Bits             : char 1 bit por bloco { ceil(N/8) }; -> OK (vetorMapaBits)
    -> Vetor de Inodes          : char { sizeof(inode) * I };         -> NO (vetorInodes)
    -> Indice do Inode Dir Raiz : char 1 byte - 8 bits;               -> NO ("/")
    -> Vetor de Blocos          : char { T * N };                     -> NO ()
    
*/

void initFs(string fsFileName, int blockSize, int numBlocks, int numInodes)
{

  printf("\n \n");

  cout << "Nome: " << fsFileName << endl;
  cout << "Tamanho dos blocos          (T): " << blockSize << endl;
  cout << "Numero de blocos            (N): " << numBlocks << endl;
  cout << "Numero de Inodes            (I): " << numInodes << endl;

  FILE *arquivo = fopen(fsFileName.c_str(), "w+b"); // Leitura do arquivo
  INODE vetorInodes;

  int tamanhoMapaBits = ceil( ((float) numBlocks) / 8); // 4 bytes para representar 32 blocos
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

void addFile(std::string fsFileName, std::string filePath, std::string fileContent)
{
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
