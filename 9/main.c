#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX 4005
enum Type{num, op};
typedef struct Token{
    int type;
    char data[20];
    struct Token *next;
}Token;
typedef struct numNode{
    long long data;
    struct numNode* next;
}numNode;
typedef struct charNode{
    char data;
    struct charNode *next;
}charNode;
charNode *genCNode(char c, charNode *next){
    charNode *node = (charNode*)malloc(sizeof(charNode));
    node->data = c, node->next = next;
    return node;
}
void charPush(charNode **head, char c){
    *head = genCNode(c, *head);
}
char charPop(charNode **head){
    // if(*head == NULL) return 0;
    char c = (*head)->data;
    charNode *nxt = (*head)->next;
    free(*head);
    *head = nxt;
    return c;
}
int priorty(char c){
    if(c == '*' || c == '/') return 3;
    else if(c == '%') return 2;
    else if(c == '+' || c == '-') return 1;
    return -1;
}
Token* genTok(char *str, int type){
    Token *nt = (Token*)malloc(sizeof(Token));
    memset(nt->data, 0, sizeof(nt->data));
    strcpy(nt->data, str);
    nt->type = type;
    nt->next = NULL;
    return nt;
}
Token* inToPost(char *infix){
    charNode *st = NULL;
    Token *post = genTok("", -1), *cur = post;
    int len = strlen(infix), i = 0;
    while(i < len){
        if(isdigit(infix[i])){
            char str[20] = {0};
            int s = 0;
            str[s++] = infix[i];
            printf("%c", infix[i]);
            i++;
            while(i < len && isdigit(infix[i])){
                str[s++] = infix[i];
                printf("%c", infix[i]);
                i++;
            }
            cur->next = genTok(str, num);
            cur = cur->next;
        }
        else{
            if(infix[i] == '(') charPush(&st, infix[i]);
            else if(infix[i] == ')'){
                while(st && st->data != '('){
                    char opstr[2] = {0}; opstr[0] = charPop(&st);
                    printf("%c", opstr[0]);
                    cur->next = genTok(opstr, op);
                    cur = cur->next;
                }
                charPop(&st);
            }
            else{
                while(st && priorty(st->data) >= priorty(infix[i])){
                    char opstr[2] = {0}; opstr[0] = charPop(&st);
                    printf("%c", opstr[0]);
                    cur->next = genTok(opstr, op);
                    cur = cur->next;
                }
                charPush(&st, infix[i]);
            }
            i++;
        }
    }
    while(st){
        char opstr[2] = {0}; opstr[0] = charPop(&st);
        printf("%c", opstr[0]);
        cur->next = genTok(opstr, op);
        cur = cur->next;
    }
    return post->next;
}
void printTok(Token *tok){
    while(tok){
        printf("type = %d, data = %s\n", tok->type, tok->data);
        tok = tok->next;
    }
}
long long Calc(long long n1, char tok, long long n2){
    if(tok == '%' && n2 != 0) return n1 % n2;
    else if (tok == '-') return n1 - n2;
    else if (tok == '*') return n1 * n2;
    else if(tok == '/' && n2 != 0) return n1 / n2;
    else return n1 + n2;
}
numNode *genNNode(long long num, numNode *next){
    numNode *node = (numNode*)malloc(sizeof(numNode));
    node->data = num, node->next = next;
    return node;
}
void numPush(numNode **head, long long num){
    *head = genNNode(num, *head);
}
long long numPop(numNode **head){
    if(*head == NULL) return 0ll;
    long long num = (*head)->data;
    numNode *nxt = (*head)->next;
    free(*head);
    *head = nxt;
    return num;
}
long long Eval(Token *tok){
    numNode *st = NULL;
    while(tok){
        if(tok->type == num){
            numPush(&st, (long long)(atoll(tok->data)));
        }
        else{
            long long n2 = numPop(&st), n1 = numPop(&st);
            long long ret = Calc(n1, tok->data[0], n2);
            #ifdef debug
            printf("ret = %lld\n", ret);
            #endif
            numPush(&st, ret);
        }
        Token *nxt = tok->next;
        free(tok);
        tok = nxt;
    }
    return numPop(&st);
}
int main(){
    char infix[MAX];
    for(int i = 0; i < 3; i++){
        scanf("%s", infix);
        Token *tok = inToPost(infix);
        #ifdef debug
        printTok(tok);
        #endif
        printf("=%lld\n", Eval(tok));
    }
}
