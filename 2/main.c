#include <stdio.h>
#include <stdlib.h>
#define N 100005
typedef struct Deck{
    int data;
    struct Deck *next;
}Deck;
Deck *gen(int u, Deck *n){
    Deck *new = (Deck*)malloc(sizeof(Deck));
    new->data = u;
    new->next = n;
    return new;
}
void printSingle(Deck *deck){
    Deck *cur = deck;
    while(cur){
        printf(" %d", cur->data);
        cur = cur->next;
    }
    return;
}
void printAll(Deck *heads[], int lens[], int n){
    for(int i = 0; i < n; i++){
        printf("%d", lens[i]);
        printSingle(heads[i]);
        printf("\n");
    }
}
void add(int u, Deck **head, Deck **tail, int *len){
    Deck *new = gen(u, *head);
    *head = new;
    if(new->next == NULL) *tail = *head;
    *len += 1;
}
void remov(Deck **head, Deck **tail, int *len){
    *head = (*head)->next;
    if(*head == NULL) *tail = NULL;
    *len -= 1;
}
void concat(Deck **h1, Deck **t1, int *l1, Deck **h2, Deck **t2, int *l2){
    if(*l1 == 0)
        return;
    (*t1)->next = *h2;
    *h2 = *h1;
    *l2 += *l1;
    *h1 = NULL, *t1 = NULL, *l1 = 0;
    return;
}
void merge(Deck **h1, Deck **t1, int *l1, Deck **h2, Deck **t2, int *l2){
    if(*l1 == 0) return;
    if(*l2 == 0){
        *h2 = *h1, *t2 = *t1, *l2 = *l1;
        *h1 = NULL, *t1 = NULL, *l1 = 0;
        return;
    }
    Deck *ptr1 = *h1, *ptr2 = *h2;
    int cnt = 0;
    Deck *tmph = gen(-1, NULL), *tmpcur = tmph;
    while(ptr1 && ptr2){
        if(cnt & 1){
            tmpcur->next = ptr2;
            tmpcur = tmpcur->next;
            ptr2 = ptr2->next;
        }
        else{
            tmpcur->next = ptr1;
            tmpcur = tmpcur->next;
            ptr1 = ptr1->next;
        }
        cnt++;
    }
    if(ptr1){
        tmpcur->next = ptr1;
        tmpcur = *t1;
    }
    else if(ptr2){
        tmpcur->next = ptr2;
        tmpcur = *t2;
    }
    *h2 = tmph->next;
    *l2 += *l1;
    *h1 = NULL, *t1 = NULL, *l1 = 0;
    *t2 = tmpcur;
    return;
}
int main(){
    int n, m, t, i, j, u;
    scanf("%d%d", &n, &m);
    Deck *heads[N] = {0};
    Deck *tails[N] = {0};
    int lens[N] = {0};
    for(int op = 0; op < m; op++){
        scanf("%d", &t);
        #ifdef debug
        printf("\n%dth operation = %d\n", op, t);
        #endif
        switch(t){
            case 1:
                scanf("%d%d", &i, &u);
                i--;
                add(u, &heads[i], &tails[i], &lens[i]);
                break;
            case 2:
                scanf("%d", &i);
                i--;
                remov(&heads[i], &tails[i], &lens[i]);
                break;
            case 3:
                scanf("%d%d", &i, &j);
                i--, j--;
                concat(&heads[i], &tails[i], &lens[i], &heads[j], &tails[j], &lens[j]);
                break;
            case 4:
                scanf("%d%d", &i, &j);
                i--, j--;
                merge(&heads[i], &tails[i], &lens[i], &heads[j], &tails[j], &lens[j]);
                break;
        }
        #ifdef debug
        printAll(heads, lens, n);
        #endif
    }
    printAll(heads, lens, n);
    return 0;
}