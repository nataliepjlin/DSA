#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX 4005
typedef struct charNode{
    char data;
    struct charNode* next;
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
void infixToPostfix(char *infix, char postfix[]){
    charNode *st = NULL;
    int len = strlen(infix);
    int idx = 0;
    for(int i = 0; i < len; i++){
        if(isdigit(infix[i])){
            postfix[idx++] = infix[i];
            printf("%c", postfix[idx - 1]);
        }
        else if(infix[i] == '('){
            charPush(&st, infix[i]);
        }
        else if(infix[i] == ')'){
            while(st && st->data != '('){
                postfix[idx++] = ' ';
                postfix[idx++] = charPop(&st);
                printf("%c", postfix[idx - 1]);
            }
            charPop(&st);//pop left bracket
        }
        else{//op
            while(st && priorty(st->data) >= priorty(infix[i])){
                postfix[idx++] = ' ';
                postfix[idx++] = charPop(&st);
                printf("%c", postfix[idx - 1]);
            }
            postfix[idx++] = ' ';
            charPush(&st, infix[i]);
        }
    }//for loop
    while(st){
        postfix[idx++] = ' ';
        postfix[idx++] = charPop(&st);
        printf("%c", postfix[idx - 1]);
    }
}
typedef struct numNode{
    long long data;
    struct numNode *next;
}numNode;
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
long long Calc(long long n1, char tok, long long n2){
    if(tok == '%' && n2 != 0) return n1 % n2;
    else if (tok == '-') return n1 - n2;
    else if (tok == '*') return n1 * n2;
    else if(tok == '/' && n2 != 0) return n1 / n2;
    else return n1 + n2;
}
long long Eval(char *postfix){
    numNode *st = NULL;
    int i = 0, len = strlen(postfix);
    long long tmp = 0;
    while(i < len){
        if(isdigit(postfix[i])) tmp = tmp * 10 + (postfix[i] - '0');
        else if(postfix[i] == ' '){
            #ifdef debug
            printf("tmp = %lld\n", tmp);
            #endif
            numPush(&st, tmp);
            tmp = 0;
        }
        else{
            long long n2 = numPop(&st), n1 = numPop(&st);
            long long ret = Calc(n1, postfix[i], n2);
            #ifdef debug
            printf("ret = %lld\n", ret);
            #endif
            numPush(&st, ret);
            i++;
        }
        i++;
    }
    return numPop(&st);
}
int main(){
    char infix[MAX];
    for(int i = 0; i < 3; i++){
        scanf("%s", infix);
        char postfix[MAX * 2] = {0};
        infixToPostfix(infix, postfix);
        printf("=");
        #ifdef debug
        printf("\npostfix = %s\n", postfix);
        #endif
        printf("%lld\n", Eval(postfix));
    }
}