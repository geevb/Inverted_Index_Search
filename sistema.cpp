
#include "sistema.h"

void Sistema::preencherArvores() {
	std::cout << "Preenchendo Arvores...\n";
    DIR *dir;
    struct dirent *ent;
    std::ifstream arquivo;
    std::string palavra;
    
    dir = opendir ("manpages/");
    if (dir != NULL) {
        while ((ent = readdir (dir)) != NULL) {
        	// Remover extensão do arquivo manpage.
        	std::string nomeComando = removerExtensao(ent->d_name);
            switch (ent->d_type) {
            case DT_REG:
            {

                // Preenchendo Árvore principal com os nomes dos comandos.
                arvoreB->insert(std::pair<std::string, std::string>(nomeComando, ent->d_name));

                // Preenchendo Árvore secundária com as palavras de cada manpage.
                std::string local_arquivo = "manpages/" + (std::string)ent->d_name;
            	arquivo.open(local_arquivo);
            	if (!arquivo.is_open()) { std::cout << "Arquivo não foi aberto!"; exit(1); }

            	while (arquivo >> palavra) {
            		// Interador para percorrer todos os nodos da árvore.
            		// Se não encontrar, adicionar um novo nodo com uma lista dentro.
            		btree::btree_map< std::string, std::list<std::string> >::iterator it = arvoreBS->find(palavra);
            		if (it == arvoreBS->end()) {
            			std::list<std::string> lista_arquivos;
            			lista_arquivos.push_back((std::string)ent->d_name);
            			arvoreBS->insert(std::pair< std::string, std::list<std::string> >(palavra, lista_arquivos));
            		} else { // Se encontrar o nodo, apenas inserir o nome da página no final da lista do nodo.
            			it->second.push_back((std::string)ent->d_name);
            		}           
           		}
           		arquivo.close();
           		break;
            }

            default:
                break;
      	    }
        }

        // Remover todas as duplicidades de dentro das listas de cada nodo da árvore.
        for(btree::btree_map< std::string, std::list<std::string> >::iterator it = arvoreBS->begin(); it != arvoreBS->end(); it++) {
        		it->second.sort();
                it->second.unique();
        }

        std::cout << "Preenchimento Finalizado! \n";
        std::cout << "Tamanho Arv. Primaria: " << arvoreB->size() << std::endl;
        std::cout << "Tamanho Arv. Secundaria: " << arvoreBS->size() << std::endl;
        closedir (dir);

    } else {
        std::cout << "Diretório não encontrado" << std::endl;
        exit (1);
    }
}


void Sistema::efetuarPesquisa() {
	int resp = 0;
	while (resp != 4) {
		resp = gui->perguntarTipoPesquisa();
		if (resp == 4) { exit(0); } // Finalizar execução do programa.
		if (resp == 3) { std::string palavra1 = gui->perguntarChavePesquisa(); std::string palavra2 = gui->perguntarSegundaChavePesquisa(); pesquisarChavesConjuntivas(palavra1, palavra2); } 
			else {
			std::string chave = gui->perguntarChavePesquisa();
    		if (resp == 1) { pesquisarChavePrimaria(chave); }
    		if (resp == 2) { pesquisarChaveSecundaria(chave); }
    		}
		}
}


void Sistema::pesquisarChavePrimaria(std::string chavePrimaria) {
	std::ifstream nome_arquivo;
	std::string linha;
	nome_arquivo.open("arquivo_arvore.txt");
	bool achou_palavra = false;

	while(getline(nome_arquivo, linha)) {
    	if (linha.find(chavePrimaria, 0) != std::string::npos) {
        	std::string comando = "cat manpages/" + chavePrimaria + ".*";
        	std::cout << "ManPage de " << chavePrimaria << std::endl;
    		std::cout << "\n";
        	std::cout << system((comando).c_str()); //Comando CAT, caso exista mais de um comando com mesmo nome, será apresentado todos os arquivos.
        	std::cout << "\n";
        	achou_palavra = true;
        	break;
    	}
	}
	nome_arquivo.close();
	if (!achou_palavra) std::cout << "ManPage não encontrada! \n";
}


void Sistema::pesquisarChaveSecundaria(std::string chaveSecundaria) {
	std::ifstream nome_arquivo("arvoresecundaria.txt");
	std::string linha;
	std::string primeiraPalavra;
	bool achou_palavra = false;

	while(getline(nome_arquivo, linha)) {
		std::string primeira_palavra = linha.substr(0, linha.find(" "));
		if (primeira_palavra == chaveSecundaria){
			std::cout << "Páginas que contém a palavra: " << chaveSecundaria << std::endl;
    		if (linha.find(chaveSecundaria, 0) != std::string::npos) {
    			std::cout << linha << std::endl;
        		achou_palavra = true;
    		}
		}
	}
	nome_arquivo.close();
	if (!achou_palavra) std::cout << "Palavra não encontrada! \n";
}


// Leitura em disco p/ memória e então apresentação das informações cruzadas.
void Sistema::pesquisarChavesConjuntivas(std::string palavraChave1, std::string palavraChave2) {
	std::list<std::string> palavra1;
	std::list<std::string> palavra2;
	std::list<std::string> palavras_iguais;

	std::ifstream nome_arquivo("arvoresecundaria.txt");
	std::string palavra;
	std::string linha;

	while(getline(nome_arquivo, linha)) {
		std::string primeira_palavra = linha.substr(0, linha.find(" "));
		if (primeira_palavra == palavraChave1){
			std::istringstream iss(linha);
    		if (linha.find(palavraChave1, 0) != std::string::npos) {
    			while (iss >> palavra) {
    				palavra1.push_back(palavra);
    			}
    		}
		} else if (primeira_palavra == palavraChave2) {
			std::istringstream iss2(linha);
    		if (linha.find(palavraChave2, 0) != std::string::npos) {
    			while (iss2 >> palavra) {
    				palavra2.push_back(palavra);
    			}
    		}
		}
	}

	palavra1.sort();
	palavra2.sort();

	std::set_intersection(palavra1.begin(), palavra1.end(), palavra2.begin(), palavra2.end(), std::back_inserter(palavras_iguais));

	std::cout << "\nArquivos com as palavras: " << "'" << palavraChave1 << "'" << " e " << "'" << palavraChave2 << "'" << " simultaneamente: " << std::endl;

	// Printar na tela o conteúdo da terceira lista criada pela intersecção das duas outras listas.
	for (auto n : palavras_iguais) { std::cout << n << std::endl; }

}

/*
// Pesquisa Conjuntiva em memória primária
void Sistema::pesquisarChavesConjuntivas(std::string palavraChave1, std::string palavraChave2) {
	std::list<std::string> palavra1;
	std::list<std::string> palavra2;
	std::list<std::string> palavras_iguais;
	
	// Criar dois iteradores que apontam para a posição das palavras encontradas na árvore.
	btree::btree_map< std::string, std::list<std::string> >::iterator it1 = arvoreBS->find(palavraChave1);
	btree::btree_map< std::string, std::list<std::string> >::iterator it2 = arvoreBS->find(palavraChave2);

	// Preencher as listas acima com as listas contidas dentro dos nodos apontados pelos iteradores.
	if ( it1 != arvoreBS->end() ) { palavra1 = it1->second; } else { std::cout << "\nPalavra Chave 1 não encontrada!\n"; return; }
	if ( it2 != arvoreBS->end() ) { palavra2 = it2->second; } else { std::cout << "\nPalavra Chave 2 não encontrada!\n"; return; }

	palavra1.sort();
	palavra2.sort();

	// Do começo ao fim das duas lista, preencher a terceira com a intersecção de ambas.
	std::set_intersection(palavra1.begin(), palavra1.end(), palavra2.begin(), palavra2.end(), std::back_inserter(palavras_iguais));

	std::cout << "Arquivos com as palavras: " << "'" << palavraChave1 << "'" << " e " << "'" << palavraChave2 << "'" << " simultaneamente: " << std::endl;
	// Printar na tela o conteúdo da terceira lista.
	for (auto n : palavras_iguais) {
    	std::cout << n << std::endl;
 	}

}
*/

void Sistema::escreverArquivoArvorePrimaria(std::string nomeArquivo) {
        if (arvoreB->empty()) {
        	std::cout << "Arvore Vazia!" << std::endl;
        	exit (1);
        }

        FILE *arquivo = fopen(nomeArquivo.c_str(), "w");
        if (!arquivo) {
        	std::cout << "Arquivo não foi aberto!" << std::endl;
            exit (1);
        }

        // Criar arvore de interação e então percorrer a árvore preenchida
        for(btree::btree_map<std::string, std::string>::iterator it = arvoreB->begin(); it != arvoreB->end(); it++) {
                fprintf(arquivo, "%s\n", it->first.c_str());
        }

        fclose(arquivo);
}


void Sistema::escreverArquivoArvoreSecundaria(std::string nomeArquivo) {
        if (arvoreBS->empty()) {
        	std::cout << "Arvore Vazia!" << std::endl;
        	exit (1);
        }

        FILE *arquivo = fopen(nomeArquivo.c_str(), "w");
        if (!arquivo) {
        	std::cout << "Arquivo não foi aberto!" << std::endl;
            exit (1);
        }

        // Criar arvore de interação e então percorrer a árvore preenchida
        for(btree::btree_map< std::string, std::list<std::string> >::iterator it = arvoreBS->begin(); it != arvoreBS->end(); it++) {
        	fprintf(arquivo, "%s ", it->first.c_str());
                for (auto n : it->second) {
                	fprintf(arquivo, "%s ", n.c_str());
            }
            fprintf(arquivo, "\n");
        }

        fclose(arquivo);
}


std::string Sistema::removerExtensao(const std::string& nomeArquivo) {
    std::size_t pontofinal = nomeArquivo.find_last_of(".");
    if (pontofinal == std::string::npos) return nomeArquivo;
    return nomeArquivo.substr(0, pontofinal); 
}