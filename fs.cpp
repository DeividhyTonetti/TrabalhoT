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
  
  int TAMANHO_BLOCOS; 
  int NUM_BLOCOS;  
  int NUM_INODES; 
  int MAPA_BITS;        

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

    INODE vetorInodes;

    fseek(arquivo, 0, SEEK_SET); 
    fread(buffer, sizeof(char), 4, arquivo);

    TAMANHO_BLOCOS = buffer[0]; 
    NUM_BLOCOS     = buffer[1];
    NUM_INODES     = buffer[2];
    MAPA_BITS      = buffer[3];

    int tamanhoMapaBits    = ceil(((float) NUM_BLOCOS) / 8);
    int tamanhoVetorBlocos = TAMANHO_BLOCOS * NUM_BLOCOS;
    int tamanhoVetorInodes = sizeof(INODE) * NUM_INODES;

    int qtdInodesLivres;
    char disponivel;

    fseek(arquivo, 4, SEEK_SET);

    filePath.erase(0, 1);

    int tamanhoConteudo = fileContent.length();

    char conteudo[tamanhoConteudo]; 
    strcpy(conteudo, fileContent.c_str());

    // INCLUSÃO do INODE
    for (int i = 0; i < NUM_INODES; i++)
    {
      char valorInode;
      fread(&valorInode, sizeof(vetorInodes), 1, arquivo);

      if (!valorInode) 
      {
        vetorInodes.IS_USED = 0x01;
        vetorInodes.IS_DIR = 0x00;

        strcpy(vetorInodes.NAME, filePath.c_str());
        
        vetorInodes.SIZE = tamanhoConteudo;

        for(int i = 0; i < 3; i++) {
          vetorInodes.DIRECT_BLOCKS[i] = 0; 
          vetorInodes.INDIRECT_BLOCKS[i] = 0;
          vetorInodes.DOUBLE_INDIRECT_BLOCKS[i] = 0;
        }

        fseek(arquivo, 4 + sizeof(vetorInodes), SEEK_SET);
        fwrite(&vetorInodes, sizeof(INODE), 1, arquivo);

        while(i < NUM_INODES - 1) {
          fread(&valorInode, sizeof(vetorInodes), 1, arquivo);
          i++;
        }

        break;
      }
    }

    char vazio = 0x00;

    fwrite(&vazio, sizeof(char), 1, arquivo);
    
    // VERIFICAÇÃO DO MAPA DE BITS, QUAL BLOCO ESTÁ LIVRE!
    int bl = -1;
    for (int i = 7; i >= 0; i--)
    {
      float depara = MAPA_BITS - pow(2.0, i);

      if(depara < 0) {
        bl = i;
      } else {
        MAPA_BITS -= pow(2.0, i);
      }
    }

   // INCLUSÃO DE DADOS NOS BLOCOS DE ACORDO COM O INODE LIVRE
    for(int i = 0; i <= bl; i++) 
    {
      if(i == bl) 
      {
        fwrite(conteudo, sizeof(char), tamanhoConteudo, arquivo);
        break;
      }

      fread(buffer, TAMANHO_BLOCOS, 1, arquivo);
    }

    fseek(arquivo, 3, SEEK_SET);

    int novoMapaBits = ceil(((float) tamanhoConteudo / TAMANHO_BLOCOS));
   
    bitset<8> bset;
    bset[0] = 1;

    for(int i = 1; i <= novoMapaBits; i++) 
    {
      bset[i] = 1;
    }

    novoMapaBits = bset.to_ulong();

    fwrite(&novoMapaBits, sizeof(char), 1, arquivo);
     
    int tamanhoAtualPai;
    
    fseek(arquivo, 12, SEEK_CUR);

    fread(&tamanhoAtualPai, sizeof(char), 1, arquivo);
    fseek(arquivo, 16, SEEK_SET);

    tamanhoAtualPai += 1;

    fwrite(&tamanhoAtualPai, sizeof(char), 1, arquivo);

    fseek(arquivo, 4 + sizeof(vetorInodes) + 13, SEEK_SET);
    
    int novoIndice = ceil(((float) tamanhoConteudo / TAMANHO_BLOCOS));

    for(int i = 1; i <= novoIndice; i++) {
      fwrite(&i, sizeof(char), 1, arquivo);
    }

    fseek(arquivo, 7, SEEK_CUR);
    printf("POsicao1: %x \n", ftell(arquivo));

    cout<< "NUm inodes: " << NUM_INODES << endl;

    for(int i = 0; i < NUM_INODES - 1 - bl; i++) {
      fread(&vazio, sizeof(vetorInodes), 1, arquivo);
    }

    printf("POsicao2: %x - BL: %d \n", ftell(arquivo), bl);
    int teste = fwrite(&bl, sizeof(char), 1, arquivo);

    printf("POsicao2: %x - TESTE: %d \n", ftell(arquivo), teste);
    // bl
    fclose(arquivo);  
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
