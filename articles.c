 /**
 * 
 * @file orders.c
 * @author Diogo Miranda
 * @date January 7, 2023 9:34 AM
 * @brief Funcionalidades de gestão de artigos
 * 
 * Este ficheiro contém todas as estruturas, funções de CRUD, memória dinâmica,
 * persistência de dados e listagens referentes aos artigos. 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strings.h"
#include "articles.h"
#include "input.h"

/**
 * Esta função inicializa o array (articles) com memória dinâmica da estrutura Articles
 * @param articles
 */
void setupArticles(Articles *articles) {
    articles->articles = (Article*) malloc(ARTICLES_INITIAL_SIZE * sizeof (Article));
    articles->counter = 0;
    articles->size = ARTICLES_INITIAL_SIZE;
}

/**
 * Esta função guarda para um ficheiro binário o contador atual
 * e o array de artigos da variável Articles.
 * @param articles
 * @param file
 */
void saveArticles(Articles articles, char *file) {

    int i;
    FILE *fp = fopen(file, "wb");
    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }

    fwrite(&articles.counter, sizeof (int), 1, fp);
    for (int i = 0; i < articles.counter; i++) {
        fwrite(&articles.articles[i], sizeof (Article), 1, fp);
    }

    fclose(fp);

    puts("\nGuardado com sucesso.");

}

/**
 * Esta função carrega para a memória os dados dos artigos guardados no ficheiro.
 * @param articles 
 * @param file ponteiro da variável com o nome e tipo do ficheiro a serem
 * @return 1 - caso conseguiu carregar os dados, 0 - caso falhou ao carregar 
 * os dados
 */
int loadArticles(Articles *articles, char *file) {
    int successful = 0;

    FILE *fp = fopen(file, "rb");
    if (fp != NULL) {

        fread(&articles->counter, sizeof (int), 1, fp);

        if (articles->counter > 0) {
            articles->articles = (Article*) malloc(articles->counter * sizeof (Article));
            fread(articles->articles, sizeof (Article), articles->counter, fp);

            articles->size = articles->counter;

            successful = 1;
            printf("Carregado com sucesso.");
        }
        fclose(fp);
    }


    if (!successful) {
        fp = fopen(file, "wb");
        if (fp != NULL) {
            setupArticles(articles);
            fclose(fp);
            printf("Foi criado um ficheiro de artigos, pois não havia um.");
            successful = 1;
        }
    }

    return successful;
}

/**
 * Imprime as informações de um artigo
 * @param article
 */
void printArticle(Article article) {

    printf("\nCódigo: %d", article.code);
    printf("\nNome: %s", article.name);
    printf("\nTamanho mínimo: %d", article.sizes.min);
    printf("\nTamanho máximo: %d", article.sizes.max);
    printf("\nTotal vendido: %d", article.sold);
    puts("\n");
}

/**
 * Imprime todos os artigos e suas respectivas informações
 * @param articles
 */
void printArticles(Articles articles) {
    if (articles.counter > 0) {
        int i;
        for (i = 0; i < articles.counter; i++) {
            puts("");
            printArticle(articles.articles[i]); //meter print article para imprimir os produtos
        }
    } else {
        puts("\n"ERROR_LIST_EMPTY);
    }
}

/**
 * Apaga um artigo
 * @param article
 */
void deleteArticle(Article *article) {
    article->code = 0;
    strcpy(article->name, "");
    article->sizes.min = article->sizes.max = 0;
    article->sold = 0;
    printf(DELETE_SUCESSFUL);
    puts("\n");
}

/**
 * Procura um artigo
 * @param articles
 * @param code Código do artigo a procurar
 * @return Caso o artigo seja encontrado, retorna o valor da posição do artigo,
 * caso contrário retorna -1
 */
int searchArticle(Articles articles, int code) {
    for (int i = 0; i < articles.counter; i++) {
        if (articles.articles[i].code == code) {
            return i;
        }
    }
    return -1;
}

/**
 * Esta função irá criar um artigo com a sua respectiva informação
 * @param articles
 * @return o novo valor do contador da variável do tipo articles
 */
int createArticle(Articles *articles) {
    puts("\n");
    int code = getInt(MIN_CODE_ARTICLE, MAX_CODE_ARTICLE, MSG_GET_ARTICLE_CODE);

    if (searchArticle(*articles, code) == -1) {
        articles->articles[articles->counter].code = code;

        getString(articles->articles[articles->counter].name, MAX_ARTICLE_NAME,
                MSG_GET_ARTICLE_NAME);

        articles->articles[articles->counter].sizes.min = getInt(MIN_SIZE_ARTICLE, MAX_SIZE_ARTICLE, MSG_GET_ARTICLE_MIN_SIZE);

        articles->articles[articles->counter].sizes.max = getInt(articles->articles[articles->counter].sizes.min, MAX_SIZE_ARTICLE, MSG_GET_ARTICLE_MAX_SIZE);

        articles->articles[articles->counter].sold = 0;

        printf(CREATE_SUCESSFUL);
        puts("\n");
        return articles->counter++;
    }
    return -1;
}

/**
 * Esta função irá atualizar os valores atuais de um respetivo artigo face
 * aos novos valores inseridos. 
 * @param article
 */
void updateArticle(Article *article) {
    puts("\n");
    getString(article->name, MAX_ARTICLE_NAME, MSG_GET_ARTICLE_NAME);
    article->sizes.min = getInt(MIN_SIZE_ARTICLE, MAX_SIZE_ARTICLE, MSG_GET_ARTICLE_MIN_SIZE);
    article->sizes.max = getInt(article->sizes.min, MAX_SIZE_ARTICLE, MSG_GET_ARTICLE_MAX_SIZE);
    printf(UPDATE_SUCESSFUL);
    puts("\n");
}

/**
 * Esta função irá aumentar dinâmicamente o tamanho do array (articles)
 * da variável do tipo articles
 * @param articles
 */
void expandArticles(Articles *articles) {
    Article *temp = (Article*) realloc(articles->articles, sizeof (Article) * (articles->size * 2));
    if (temp != NULL) {
        articles->size *= 2;
        articles->articles = temp;
    }
}

/**
 * Esta função começa por verificar se há espaço para inserir um novo artigo.
 * Caso não tenha, chama a função para expandir o tamanho do array,
 * para possibilitar a criação do artigo.
 * Em seguida, é chamada a função de criar um artigo,
 * que irá retornar ou o valor da novo posição do artigo ou
 * -1 caso o artigo já exista. 
 * @param articles
 */
void createArticles(Articles *articles) {

    if (articles->counter == articles->size) {
        expandArticles(articles);
    }

    if (articles->counter < articles->size) {
        if (createArticle(articles) == -1) {
            puts(ERROR_ARTICLE_ALREADY_EXISTS);
        }
    } else {
        puts(ERROR_LIST_FULL);
    }
}

/**
 * Esta função verifica face ao código do artigo inserido pelo utilizador, 
 * se o artigo existe ou não.
 * Caso exista, é chamada a função de imprimir informações sobre o artigo.
 * @param articles
 */
void searchArticles(Articles articles) {
    int tempCode, code;
    tempCode = getInt(MIN_CODE_ARTICLE, MAX_CODE_ARTICLE, MSG_GET_ARTICLE_CODE);
    code = searchArticle(articles, tempCode);
    if (code != -1) {
        printArticle(articles.articles[code]); //mesma coisa que o outro comentario de cima
    } else {
        puts(ERROR_ARTICLE_DOESNT_EXISTS);
    }
}

/**
 * Esta função verifica face ao código do artigo inserido pelo utilizador, 
 * se o artigo existe ou não.
 * Caso exista, é chamada a função de atualizar artigo.
 * @param articles
 */
void updateArticles(Articles *articles) {
    int tempCode, code;
    tempCode = getInt(MIN_CODE_ARTICLE, MAX_CODE_ARTICLE, MSG_GET_ARTICLE_CODE);
    code = searchArticle(*articles, tempCode);
    if (code != -1) {
        updateArticle(&articles->articles[code]);
    } else {
        puts(ERROR_ARTICLE_DOESNT_EXISTS);
    }
}

/**
 * Esta função verifica face ao código do artigo inserido pelo utilizador, 
 * se o artigo existe ou não.
 * Caso exista, é chamada a função de apagar o artigo.
 * @param articles
 */
void deleteArticles(Articles *articles) {
    int tempCode, tempCounter;
    tempCode = getInt(MIN_CODE_ARTICLE, MAX_CODE_ARTICLE, MSG_GET_ARTICLE_CODE);
    tempCounter = searchArticle(*articles, tempCode);
    if (tempCounter != -1) {
        if (articles->articles[tempCounter].sold == 0) {
            int i;
            for (i = tempCounter; i < articles->counter - 1; i++) {
                articles->articles[i] = articles->articles[i + 1];
            }
            deleteArticle(&articles->articles[i]);
            articles->counter--;
        } else {
            puts(ERROR_ARTICLE_HAS_BEEN_SOLD);
        }
    } else {
        puts(ERROR_ARTICLE_DOESNT_EXISTS);
    }
}

/**
 * Imprime o menu de gestão de artigos de calçado
 */
void printArticlesMenu() {
    printf("\n--- Gestão de Artigos de Calçado ---");
    printf("\n1. - Inserir");
    printf("\n2. - Procurar");
    printf("\n3. - Atualizar");
    printf("\n4. - Remover");
    printf("\n5. - Listar");
    printf("\n6. - Guardar");
    printf("\n7. - Carregar");
    printf("\n0 - Sair");
    //printf("\nAlunos: %d/%d", articles.counter, MAX_ARTICLES_LIST);
}

/**
 * Obtem o tamanho mínimo do respectivo artigo
 * @param article
 * @return o tamanho mínimo
 */
int getArticleMinSize(Article article) {
    return article.sizes.min;
}

/**
 * Obtem o tamanho máximo do respectivo artigo
 * @param article
 * @return o tamanho máximo
 */
int getArticleMaxSize(Article article) {
    return article.sizes.max;
}

/**
 * Obtem o artigo que mais vendeu
 * @param articles
 * @return o código do artigo
 */
int getMostSoldArticle(Articles articles) {
    int articleCode;
    int totalSells = -1;
    for (int i = 0; i < articles.counter; i++) {
        if (articles.articles[i].sold > totalSells) {
            totalSells = articles.articles[i].sold;
            articleCode = articles.articles[i].code;
        }
    }
    return articleCode;
}

/**
 * Obtem o artigo que menos vendeu
 * @param articles
 * @return o código do artigo
 */
int getFewestSoldArticle(Articles articles) {
    int articleCode;
    int totalSells = 2147483647; // INT_MAX
    for (int i = 0; i < articles.counter; i++) {
        if (articles.articles[i].sold < totalSells) {
            totalSells = articles.articles[i].sold;
            articleCode = articles.articles[i].code;
        }
    }
    return articleCode;
}


/**
 * Atualiza a quantidade de vendas do respectivo artigo
 * @param quantity quantidade vendido
 * @param article artigo a alterar o valor
 */
void setArticleSold(int quantity, Article *article) {
    article->sold = +quantity;
}

/**
 * Liberta o bloco de memória apontado por articles
 * @param articles ponteiro da variável do tipo articles
 */
void freeArticles(Articles *articles) {
    free(articles->articles);
}
