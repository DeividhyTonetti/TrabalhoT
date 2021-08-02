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
#include <bitset>

#include "fs.h"

using namespace std;

void initFs(string fsFileName, int blockSize, int numBlocks, int numInodes)
{

  printf("\n \n");

  cout << "Nome: " << fsFileName << endl;
  cout << "Tamanho dos blocos          (T): " << blockSize << endl;
  cout << "Numero de blocos            (N): " << numBlocks << endl;
  cout << "Numero de Inodes            (I): " << numInodes << endl;

  FILE *arquivo = fopen(fsFileName.c_str(), "w+b");
  INODE vetorInodes;

  int tamanhoMapaBits = ceil(((float)numBlocks) / 8);
  char mapaBits = 0x01;

  int tamanhoVetorInodes = sizeof(INODE) * numInodes;

  cout << "Tamanho do Mapa de Bits     (MB): " << tamanhoMapaBits << endl;
  cout << "Tamanho do vetor de Inodes  (VI): " << tamanhoVetorInodes << endl;
  printf("\n \n");

  vetorInodes.IS_USED = 1;
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
    int MAPA_BITS;        
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
    fread(buffer, sizeof(char), 4, arquivo);

    novoModelo.TAMANHO_BLOCOS = buffer[0]; 
    novoModelo.NUM_BLOCOS     = buffer[1];
    novoModelo.NUM_INODES     = buffer[2];
    novoModelo.MAPA_BITS      = buffer[3];

    int tamanhoMapaBits    = ceil(((float) novoModelo.NUM_BLOCOS) / 8);
    int tamanhoVetorBlocos = novoModelo.TAMANHO_BLOCOS * novoModelo.NUM_BLOCOS;
    int tamanhoVetorInodes = sizeof(INODE) * novoModelo.NUM_INODES;

    fseek(arquivo, 4, SEEK_SET);

    filePath.erase(0, 1);

    for (int i = 0; i < novoModelo.NUM_INODES; i++)
    {
      char valorInode;
      fread(&valorInode, sizeof(vetorInodes), 1, arquivo);

      if (!valorInode) 
      {
        vetorInodes.IS_USED = 0x01;
        vetorInodes.IS_DIR = 0x00;

        strcpy(vetorInodes.NAME, filePath.c_str());
        
        vetorInodes.SIZE = fileContent.length();
        vetorInodes.DIRECT_BLOCKS[0] = 0;
        vetorInodes.INDIRECT_BLOCKS[0] = 0;
        vetorInodes.DOUBLE_INDIRECT_BLOCKS[0] = 0;

        fseek(arquivo, 4 + sizeof(vetorInodes), SEEK_SET);
        fwrite(&vetorInodes, sizeof(INODE), 1, arquivo);

        while(i < novoModelo.NUM_INODES - 1) {
          fread(&valorInode, sizeof(vetorInodes), 1, arquivo);
          i++;
        }

        int pastaRaiz = 0x00;
        fwrite(&pastaRaiz, sizeof(char), 1, arquivo);

        int inicioBloco = ftell(arquivo);
        int bl = -1;

        for (int j = 7; j >= 0; j--)
        {
          float depara = novoModelo.MAPA_BITS - pow(2.0, j);

          if(depara < 0) {
            bl = j;
          } else {
            novoModelo.MAPA_BITS -= pow(2.0, j);
          }
        }

        float writeBlock = sizeof(filePath) / tamanhoVetorBlocos;    
        
        char conteudo[fileContent.length()]; 
        strcpy(conteudo, fileContent.c_str());

        cout<< "Teste Conteudo: " << conteudo << endl;
        
        // int certo = fwrite(conteudo, sizeof(char), 3, arquivo);

        for(int j = 0; j <= bl; j++) 
        {
          if(j == 0)
          {
            int somaFilho;

            fread(buffer, novoModelo.TAMANHO_BLOCOS, 1, arquivo);
            fseek(arquivo, -novoModelo.TAMANHO_BLOCOS, SEEK_CUR);

            somaFilho = buffer[0] + 1; // Não funciona - Tem que generalizar 
            fwrite(&somaFilho, sizeof(char) * novoModelo.TAMANHO_BLOCOS, 1, arquivo);
            fseek(arquivo, -novoModelo.TAMANHO_BLOCOS, SEEK_CUR);
            // printf("POsicao4: %x \n", ftell(arquivo)); //ok
          }

          if(j == bl) 
          {
            // printf("POsicao5: %x \n", ftell(arquivo));
            // for (int k = 0; k < fileContent.length(); k++)
            // {
              // cout << "ENTREI: " << conteudo[k] << endl;
              printf("POsicao6: %x \n", ftell(arquivo));
              int certo = fwrite(conteudo, sizeof(char), 3, arquivo); // PORQUE ELE NÃo Está incluindo?
               
              cout << "Certo: " << certo << endl;
               
               printf("POsicao7: %x \n", ftell(arquivo));
            // }
          }

          fread(buffer, novoModelo.TAMANHO_BLOCOS, 1, arquivo);
        }

        fseek(arquivo, inicioBloco, SEEK_SET);

        // bitset<8> bset;

        // for (int k = 0; k < novoModelo.NUM_BLOCOS; k++)
        // {
        //   fread(buffer, novoModelo.TAMANHO_BLOCOS, 1, arquivo);

        //   if(buffer[0] != 0) {
        //     bset[k] = 1;
        //   }
        // }

        // int decimal = bset.to_ulong();

        // fseek(arquivo, sizeof(int), SEEK_SET);
        // fwrite(&decimal, sizeof(char), 1, arquivo);

        fclose(arquivo);
        break;
      }
    }  
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
