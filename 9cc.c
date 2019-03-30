#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

enum {
    ND_NUM = 256
};

typedef struct Node {
    int ty;
    struct Node lhs;
    struct Node rhs;
    int val;
} Node;

Node *new_node(int ty, Node *lhs, Node *rhs) {
    Node *node = malloc(sizeof(Node));
    node->ty = ty;
    node->lhs = lhs;
    node->rhs = rhs;
    return node;
}

Node *new_node_num(int val){
    Node *node = malloc(sizeof(Node));
    node->ty = ND_NUM;
    node->val = val;
    return node;
}

int consume(int ty) {
    if (tokens[pos].ty != ty)
        return 0;
    pos++;
    return 1;
}

Node *add() {
    Node *node = mul();

    for(;;) {
        if(consume('+'))
            node = new_node('+', node, mul());
        else if (consume('-'))
            node = new_node(('-', node, mul()));
        else
            return node;
    }
}

Node *mul() {
    Node *node = term();

    for(;;) {
        if (consume('*'))
            node = new_node('*', node, term());
        else if (consume('/'))
            node = new_node('/', node, term());
        else
            return node;
    }
}


enum {
    TK_NUM = 256,
    TK_EOF,
};

typedef struct {
    int ty;
    int val;
    char *input;
} Token;

Token tokens[100];

void tokenize(char *p) {
    int i = 0;

    while (*p) {
        if (isspace(*p)) {
            p++;
            continue;
        }

        if (*p == '+' || *p == '-') {
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

void error(int i) {
    fprintf(stderr, "unexpected token %s\n", tokens[i].input);
    exit(1);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "wrong number of argument\n");
        return 1;
}

tokenize(argv[1]);

printf(".intel_syntax noprefix\n");
printf(".global _main\n");
printf("_main:\n");

if (tokens[0].ty != TK_NUM)
    error(0);
printf("  mov rax, %d\n", tokens[0].val);

int i = 1;
while (tokens[i].ty != TK_EOF) {
    if (tokens[i].ty == '+') {
        i++;
        if (tokens[i].ty != TK_NUM)
            error(i);
        printf("  add rax, %d\n", tokens[i].val);
        i++;
        continue;
    }

    else if (tokens[i].ty == '-') {
        i++;
        if (tokens[i].ty != TK_NUM)
            error(i);
        printf("  sub rax, %d\n", tokens[i].val);
        i++;
        continue;
    }
    else
        error(i);
    }

    printf("  ret\n");
    return 0;
}
