
#include "visao.h"
#include "sistema.h"

Sistema *sistema = new Sistema;

int main() {
	sistema->preencherArvores();
	sistema->escreverArquivoArvorePrimaria("arquivo_arvore.txt");
	sistema->escreverArquivoArvoreSecundaria("arvoresecundaria.txt");
	sistema->efetuarPesquisa();
	return 0;
}
