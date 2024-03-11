#include <stdio.h>
void swap(int *a, int *b){
    int t = *a;
    *a = *b;
    *b = t;
}
void printPow(int powers[], int rank[], int n){
    for(int i = 0; i < n; i++)
        printf("%d(rank %d)%c", powers[i], rank[i], " \n"[i == n - 1]);
}
int main(){
    int n, t, m;
    scanf("%d%d%d", &n, &t, &m);
    int location[n];//use label to find location
    int label[n];//use location to find label
    int powers[n];//the power of ith labeled person
    for(int i = 0; i < n; i++){
        scanf("%d", &powers[i]);
        label[i] = i;
        location[i] = i;
    }
    int incident, ai, qi, bi, mi, attack = 0;
    for(int i = 0; i < t; i++){
        scanf("%d", &incident);
        if(incident == 1){
            scanf("%d", &ai);
            ai--;
            int loc = location[ai];
            #ifdef debug
            printf("tag = %d, loc = %d\n", ai, loc);
            #endif
            if(loc == 0) continue;//already at first place
            int prevLabel = label[loc - 1];
            powers[ai] = powers[prevLabel];
            location[ai] = loc - 1;
            location[prevLabel] = loc;
            label[loc] = prevLabel;
            label[loc - 1] = ai;
            attack++;
        }
        else if(incident == 2){
            for(int i = 0; i < n; i++){
                int tag = label[i];
                powers[tag] += (n - 1 - i);
            }
        }
        else if(incident == 3){
            scanf("%d", &qi);
            #ifdef debug
            printf("finding smallest >= %d\n", qi);
            #endif
            int low = 0, high = n - 1, idx = -1;
            while(low <= high){
                int mid = (low + high) >> 1;
                int tag = label[mid];
                if(powers[tag] >= qi){
                    idx = tag;
                    low = mid + 1;
                }
                else high = mid - 1;
            }
            if(idx == -1) printf("0 0\n");
            else printf("%d %d\n", location[idx] + 1, idx + 1);
        }
        else{//report increase from last m attack
            scanf("%d%d", &bi, &mi);

        }
        #ifdef debug
        printPow(powers, location, n);
        #endif
    }
}