#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX 4001
typedef struct Node{
    char data;
    struct Node* next;
}Node;
typedef struct Stack{
    int size;
    Node *head;
}Stack;
void initStack(Stack *st){
    st->size = 0;
    st->head = NULL;
}
Node* genNode(char tok){
    Node *node = (Node*)malloc(sizeof(Node));
    node->data = tok, node->next = NULL;
    return node;
}
void push(Stack *st, char tok){
    Node *newNode = genNode(tok);
    st->size += 1;
    newNode->next = st->head;
    st->head = newNode;
}
bool isEmpty(Stack *st){
    return !(st->size);
}
char pop(Stack *st){
    // if(!st->size) return 0;
    st->size -= 1;
    char c = st->head->data;
    st->head = (st->head)->next;
    return c;
}
int priorty(char c){
    if(c == '*' || c == '/' || c == '%') return 2;
    else if(c == '+' || c == '-') return 1;
    return -1;
}
void infixToPostfix(char *infix, char postfix[]){
    Stack st;
    initStack(&st);
    int len = strlen(infix);
    for(int i = 0; i < len; i++){
        if(isdigit(infix[i])){
            char chr[2]; chr[0] = infix[i], chr[1] = 0;
            strcat(postfix, chr);
        }
        else if(infix[i] == '(') push(&st, infix[i]);
        else if(infix[i] == ')'){
            while(!isEmpty(&st) && st.head->data != '('){
                char chr[2]; chr[0] = pop(&st), chr[1] = 0;
                strcat(postfix, chr);
            }
            pop(&st);
        }
        else{
            while(!isEmpty(&st) && priorty(infix[i]) <= priorty(st.head->data)){
                char chr[2]; chr[0] = pop(&st), chr[1] = 0;
                strcat(postfix, chr);
            }
            push(&st, infix[i]);
        }
    }
    while(!isEmpty(&st)){
        char chr[2]; chr[0] = pop(&st), chr[1] = 0;
        strcat(postfix, chr);
    }
}
typedef struct numNode{
    long long data;
    struct numNode* next;
}numNode;
typedef struct numStack{
    int size;
    numNode *head;
}numStack;
void initNumStack(numStack *st){
    st->size = 0;
    st->head = 0;
}
void numPush(numStack *st, long long num){
    numNode *node = (numNode*)malloc(sizeof(numNode));
    node->data = num, node->next = st->head;
    st->head = node;
    st->size += 1;
    return;
}
long long numPop(numStack *st){
    if(!st->size) return 0;
    long long num = st->head->data;
    st->head = st->head->next;
    st->size -= 1;
    return num;
}
long long Calc(long long n1, char tok, long long n2){
    if(tok == '+') return n1 + n2;
    else if (tok == '-') return n1 - n2;
    else if (tok == '*') return n1 * n2;
    else if(tok == '/') return n1 / n2;
    else return n1 % n2;
}
long long Eval(char *postfix){
    numStack st;
    initNumStack(&st);
    int len = strlen(postfix);
    for(int i = 0; i < len; i++){
        if(isdigit(postfix[i])) numPush(&st, (long long)(postfix[i] - '0'));
        else{
            long long n2 = numPop(&st);
            long long n1 = numPop(&st);
            long long res = Calc(n1, postfix[i], n2);
            #ifdef debug
            printf("tmp res = %lld\n", res);
            #endif
            numPush(&st, res);
        }
    }
    return numPop(&st);
}
int main(){
    char infix[MAX];
    char postfix[MAX] = {0};
    while(scanf("%s", infix) != EOF){
        infixToPostfix(infix, postfix);
        printf("%s=", postfix);
        printf("%lld", Eval(postfix));
        printf("\n");
        memset(infix, 0, MAX);
        memset(postfix, 0, MAX);
    }
}