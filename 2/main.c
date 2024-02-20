#include <stdio.h>
#include <stdlib.h>
#define N 100005
typedef struct Deck{
    int data;
    struct Deck *prev;
    struct Deck *next;
}Deck;
int main(){
    int n, m, t, i, j, u;
    scanf("%d%d", &n, &m);
    Deck *heads[N] = {0};
    Deck *tail[N] = {0};
    int lens[N] = {0};
    for(int idx = 0; idx < m; idx++){
        scanf("%d", &t);
        #ifdef debug
        printf("\ntype = %d\n", t);
        #endif
        if(t == 1){
            scanf("%d%d", &i, &u);
            i--;
            #ifdef debug
            printf("add %d to %dth deck\n", u, i);
            #endif
            Deck *new = (Deck*)malloc(sizeof(Deck));
            new->data = u;
            if(heads[i] == NULL){
                heads[i] = new;
                tail[i] = heads[i];
                new->next = NULL;
            }
            else{
                new->next = heads[i];
                heads[i] = new;
            }
            lens[i]++;
        }//1 add
        else if(t == 2){
            scanf("%d", &i);
            i--;
            heads[i] = heads[i]->next;
            if(heads[i] == NULL)
                tail[i] = NULL;
            lens[i]--;
        }//2 remove
        else if(t == 3){
            scanf("%d%d", &i, &j);
            i--, j--;
            if(heads[i] == NULL) continue;
            else{
                tail[i]->next = heads[j];
                heads[j] = heads[i];
                tail[j] = tail[i];
            }
            heads[i] = tail[i] = NULL;
            lens[j] += lens[i];
            lens[i] = 0;
        }//3, move to top
        else{
            scanf("%d%d", &i, &j);
            i--, j--;
            Deck *tmphead = NULL, *tmpcur = NULL;
            Deck *ptri = heads[i], *ptrj = heads[j];
            int cnt = 0;
            while(ptri && ptrj){
                if(cnt & 1){
                    if(tmphead == NULL){
                        tmphead = ptrj;
                        tmpcur = tmphead;
                    }
                    else{
                        tmpcur->next = ptrj;
                        tmpcur = tmpcur->next;
                    }
                    ptrj = ptrj->next;
                }
                else{
                    if(tmphead == NULL){
                        tmphead = ptri;
                        tmpcur = tmphead;
                    }
                    else{
                        tmpcur->next = ptri;
                        tmpcur = tmpcur->next;
                    }
                    ptri = ptri->next;
                }
                cnt++;
            }
            while(ptri){
                if(tmphead == NULL){
                    tmphead = ptri;
                    tmpcur = tmphead;
                }
                else{
                    tmpcur->next = ptri;
                    tmpcur = tmpcur->next;
                }
                ptri = ptri->next;
            }
            while(ptrj){
                if(tmphead == NULL){
                    tmphead = ptrj;
                    tmpcur = tmphead;
                }
                else{
                    tmpcur->next = ptrj;
                    tmpcur = tmpcur->next;
                }
                ptrj = ptrj->next;
            }
            heads[j] = tmphead;
            tail[j] = tmpcur;
            lens[j] += lens[i];
            lens[i] = 0;
        }//4
    }// m op
    for(int i = 0; i < n; i++){
        printf("%d", lens[i]);
        for(int j = 0; j < lens[i]; j++){
            printf(" %d", heads[i]->data);
            heads[i] = heads[i]->next;
        }
        printf("\n");
    }//print
    return 0;
}