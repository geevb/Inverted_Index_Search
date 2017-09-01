
#include "visao.h"

int Visao::perguntarTipoPesquisa() {
	int resp;
	std::cout << "\nEscolha o tipo de pesquisa: \n (1)Nome Comando \n (2)Palavra Chave \n (3)Combinacao de Palavras Chave \n (4)Sair \n\nR: ";
	std::cin >> resp;
	if (resp == 1 || resp == 2 || resp == 3 || resp == 4) { return resp; }
	else { std::cout << "Resposta inválida."; perguntarTipoPesquisa(); }
}

std::string Visao::perguntarChavePesquisa() {
	std::string resp;
	std::cout << "Digite pelo que deseja pesquisar: ";
	std::cin >> resp;
	return resp;
}

std::string Visao::perguntarSegundaChavePesquisa() {
	std::string resp;
	std::cout << "Digite a segunda palavra a pesquisa: ";
	std::cin >> resp;
	return resp;
}