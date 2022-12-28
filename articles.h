
/**
 * @file:   articles.h
 * @author: Diogo Miranda
 * @date December 26, 2022
 * @brief Estruturas, constantes e prototipagem de funções
 * 
 * Este ficheiro contêm todas as estruturas e constantes para o funcionamento do registo de artigos.
 * Contêm também as prototipagens das funções que são acedidas pelo ficheiro main.c 
 */


#ifndef ARTICLES_H
#define ARTICLES_H

#define ARTICLES_DB_FILE  "articles.bin"

// STRINGS
#define ERROR_ARTICLE_DOESNT_EXISTS "Esse artigo não existe."
#define ERROR_ARTICLE_ALREADY_EXISTS "Esse artigo já existe."
#define ERROR_ARTICLE_HAS_BEEN_SOLD "O artigo não pode ser apagado, pois já foi encomendado anteriormente."

// CONSTANTS
#define ARTICLES_INITIAL_SIZE 5

#define MIN_CODE_ARTICLE 0
#define MAX_CODE_ARTICLE 9999
#define MSG_GET_ARTICLE_CODE "Insira o código de artigo [0 - 9999]: "

#define MAX_ARTICLE_NAME 31
#define MSG_GET_ARTICLE_NAME "Insira o nome do artigo: "

#define MIN_TYPE 0
#define MAX_TYPE 2
#define MSG_GET_ARTICLE_TYPE "Insira o tipo do artigo [0 - Sapato, 1 - Sandália, 2 - Bota]: "

#define MIN_SIZE_ARTICLE 34
#define MAX_SIZE_ARTICLE 48
#define MSG_GET_ARTICLE_MIN_SIZE "Insira o tamanho mínimo do artigo [Minímo possível - 34]: "
#define MSG_GET_ARTICLE_MAX_SIZE "Insira o tamanho máximo do artigo [Máximo possível - 48]: "

// TYPEDEF

typedef enum {
    Sapatos, Sandálias, Botas
} Types;

typedef struct {
    int min, max;
} Sizes;

typedef struct {
    int code;
    char name[MAX_ARTICLE_NAME];
    Types type; // 0 - Sapatos, 1 - Sandálias, 2 - Botas
    Sizes sizes;
    int sold;
} Article;

typedef struct {
    int counter, size;
    Article *articles;
} Articles;

// FUNCTIONS
void setupArticles(Articles *articles);
void saveArticles(Articles articles, char *file);
int loadArticles(Articles *articles, char *file);
void freeArticles(Articles *articles);

// CRUD
void createArticles(Articles *articles);
int searchArticle(Articles articles, int code);
void searchArticles(Articles articles);
void printArticles(Articles articles);
void updateArticles(Articles *articles);
void deleteArticles(Articles *articles);

// GETS & SETS
int getArticleMinSize(Article article);
int getArticleMaxSize(Article article);
void setArticleSold(int quantity, Article *article);
int getMostSoldArticle(Articles articles);
int getFewestSoldArticle(Articles articles);
int getSoldQuantityType(int type, Articles articles);

// MENUS
void printArticlesMenu();

#endif /* ARTICLES_H */

