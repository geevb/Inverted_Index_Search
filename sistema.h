#ifndef SISTEMA_H__
#define SISTEMA_H__

#include "btree_map.h"
#include "visao.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdio.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>
#include <list>
#include <iterator>
#include <limits>

typedef btree::btree_map<std::string, std::string> ArvorePrimaria;
typedef btree::btree_map< std::string, std::list<std::string> > ArvoreSecundaria;

class Sistema {
	ArvorePrimaria *arvoreB = new ArvorePrimaria;
	ArvoreSecundaria *arvoreBS = new ArvoreSecundaria;
	Visao *gui = new Visao;
	std::string FOLDER_PATH = "livros/";
 public:
	void preencherArvores();
	void efetuarPesquisa(); // Método p/ selecionar o tipo de Pesquisa.
	void pesquisarChavePrimaria(std::string pChavePrimaria);
	void pesquisarChaveSecundaria(std::string pChaveSecundaria);
	void pesquisarChavesConjuntivas(std::string palavraChave1, std::string palavraChave2);
	void escreverArquivoArvorePrimaria(std::string nomeArquivo);
	void escreverArquivoArvoreSecundaria(std::string nomeArquivo);
	std::string getPath();
	std::string removerExtensao(const std::string& pNomeArquivo);
};

#endif //SISTEMA_H__