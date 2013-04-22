#include <stdio.h>
#include "trie.h"

#define COMMA 36
#define SQUOTE 37
#define DQUOTE 38
#define UNDERSCORE 39
#define PLUS 40
#define MINUS 41
#define OTHER 42

typedef struct word {
    int instances;
    char *s;
} word;

int charHash(char c) {
    if ('a' <= c  && c <= 'z')
        return c - 'a';
    if ('A' <= c && c <= 'Z')
        return c - 'A';
    if ('0' <= c && c <= '9')
        return c - '0';
    switch (c) {
        case ',' : return COMMA;
        case '\'': return SQUOTE;
        case '"' : return DQUOTE;
        case '_' : return UNDERSCORE;
        case '+' : return PLUS;
        case '-' : return MINUS;
        default  : return OTHER;
    }
}

void printInt(void *i) {
    printf("%d\n", *(int *)i);
}

void printWord(void *s) {
    struct word *w = s;
    printf("%s : %d\n", w->s, w->instances);
}

word *makeWord(char *s) {
    word *w = (word *)malloc(sizeof(word));
    if (w) {
        w->s = strdup(s);
        w->instances = 1;
    }
    return w;
}

void freeWord(void *w0) {
    word *w = (word *)w0;
    if (w) free(w->s);
}

void incrementWord(void *w0) {
    word *w = (word *)w0;
    if (w) w->instances++;
}

void countInstances(FILE *fp) { 
    // tokenizes the stream and adds tokens to the trie
    char c, buf[256];
    int i = 0;
    Trie *t = trie_alloc(charHash, freeWord, printWord);
    while (true) {
        c = fgetc(fp);
        if (c == EOF) break;
        if (charHash(c) == OTHER) {
            if (i > 0) {
                buf[i] = '\0';
                if (!trie_update(t, buf, incrementWord))
                    trie_insert(t, buf, makeWord(buf));
                i = 0;
            }
        } 
        else {
            buf[i++] = c;
            buf[i] = '\0';
        }
    }
    trie_print(t);
    trie_free(t);
}

int main(int argc, const char **argv) {
    if (argc == 1) {
        fprintf(stderr, "Error: please enter a filename\n");
        return 0;
    }
    FILE *fp = fopen(argv[1], "r");
    if (fp)
        countInstances(fp);
    else
        printf("Error opening file\n");
    return 0;
}
