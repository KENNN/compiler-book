#include "9cc.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

Node *assign() {
    Node *node = add();
    while (consume('='))
        node = new_node('=', node, assign());
    return node;
}

Node *stmt() {
    Node *node = assign();
    if (!consume(';'))
        error("Found not ';' token %s", tokens[pos].input);
}

void program() {
    int i = 0;
    while (tokens[pos].ty != TK_EOF)
        code[i++] = stmt();
    code[i] = NULL;
}

void tokenize(char *p) {
    int i = 0;

    while (*p) {
        if (isspace(*p)) {
            p++;
            continue;
        }

        if ('a' <= *p && *p <= 'z') {
            tokens[i].ty = TK_IDENT;
            tokens[i].input = p;
            i++;
            p++;
            continue;
        }

        if (*p == '+' || *p == '-' || *p == '*' || *p == '/' || *p == '(' || *p == ')') {
            tokens[i].ty = *p;
            tokens[i].input = p;
            i++;
            p++;
            continue;
        }

        if (isdigit(*p)) {
            tokens[i].ty = TK_NUM;
            tokens[i].input = p;
            tokens[i].val = strtol(p, &p, 10);
            i++;
            continue;
        }

        fprintf(stderr, "can't tokenize %s\n", p);
        exit(1);
    }

    tokens[i].ty = TK_EOF;
    tokens[i].input = p;
}