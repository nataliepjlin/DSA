#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define N 1000001
typedef struct up_t{
    int u;//counterpart
    long long len;//len with its counterpart
}up_t;
typedef struct down_t{
    int *nexts, *dpath_ids, self_path_id;
    int cnt;//cnt of descendants
    int cur_next;
    long long presum;
}down_t;
void extend_down(int u, down_t *d, long long *path_max, int *path_size, int v, long long len){
    d[u].cnt += 1;
    d[u].dpath_ids = realloc(d[u].dpath_ids, d[u].cnt * sizeof(int));
    d[u].nexts = realloc(d[u].nexts, d[u].cnt * sizeof(int));
    
    int cur_pid = d[u].self_path_id;
    if(d[u].cnt > 1){
        *path_size += 1;//2
        path_max = realloc(path_max, sizeof(long long) * (*path_size));
        path_max[*path_size - 1] = d[u].presum;
        cur_pid = *path_size - 1;
    }
    d[v].self_path_id = cur_pid;
    path_max[cur_pid] += len;//path_max[1] += len
    d[v].presum = path_max[cur_pid];
    d[u].dpath_ids[d[u].cnt - 1] = cur_pid;
    d[u].nexts[d[u].cnt - 1] = v;
    #ifdef debug
    printf("path_max[%d] = %lld\n", cur_pid, path_max[cur_pid]);
    printf("(u, presum, pid) = (%d, %lld, %d); (v, presum, pid) = (%d, %lld, %d)\n", u, d[u].presum, d[u].self_path_id,v, d[v].presum, d[v].self_path_id);
    printf("d[%d].dpath_ids[%d] = %d, .next[%d] = %d\n\n", u, d[u].cnt - 1, d[u].dpath_ids[d[u].cnt - 1], d[u].cnt - 1, d[u].nexts[d[u].cnt - 1]);
    #endif
}
void cut_path_max(down_t *d, long long *path_max){
    path_max[ d->self_path_id ] = d->presum;
    #ifdef debug
    printf("path[%d]'s max reduce to %lld\n", d->self_path_id, path_max[ d->self_path_id ]);
    #endif
}
void pop_down(down_t *d){
    d->cur_next += 1;
}
int main(){
    int n, m, q, u, v, LOG = 0, op, path_size = 1;
    long long *path_max = malloc(sizeof(long long));//record max of each path
    path_max[0] = 0;
    long long len;
    scanf("%d%d%d", &n, &m, &q);
    up_t **up = malloc(sizeof(up_t*) * n);
    down_t *down = malloc(sizeof(down_t) * n);
    while((1 << LOG) <= n) LOG++;
    LOG++;
    for(int i = 0; i < n; i++){
        up[i] = malloc(LOG * sizeof(up_t));
        down[i].cur_next = down[i].cnt = 0, down[i].self_path_id = -1;
        down[i].dpath_ids = NULL, down[i].nexts = NULL;
        down[i].presum = 0;
    }
    down[0].self_path_id = 0;
    for(int i = 0; i < m; i++){
        scanf("%d%d%lld", &u, &v, &len);
        up[v][0].u = u;
        up[v][0].len = len;
        extend_down(u, down, path_max, &path_size, v, len);
    }//set direct ancestor and descendants
    up[0][0].u = 0, up[0][0].len = 0;
    for(int i = 0; i < n; i++){
        for(int j = 1; j < LOG; j++){
            up[i][j].u = up[ up[i][j - 1].u ][j - 1].u;
            up[i][j].len = up[ up[i][j - 1].u ][j - 1].len + up[i][j - 1].len;

        }
    }//binary lifting

    int cur = 0;
    long long ti;
    for(int i = 0; i < q; i++){
        #ifdef debug
        printf("\ncur = %d\n", cur);
        #endif
        scanf("%d", &op);
        if(op == 1){
            if(down[cur].cur_next >= down[cur].cnt) printf("-1\n");
            else{
                #ifdef debug
                printf("down[%d].cur_next = %d\n", cur, down[cur].cur_next);
                printf("down[%d].nexts[0] = %d\n", cur, down[cur].nexts[0]);
                #endif
                cur = down[cur].nexts[ down[cur].cur_next ];
                printf("%d\n", cur);
            }
        }
        else if(op == 2){
            if(cur == 0) printf("-1\n");
            else{
                cut_path_max(&down[cur], path_max);
                cur = up[cur][0].u;
                pop_down(&down[cur]);
                printf("%d\n", cur);
            }
        }
        else if(op == 3){
            scanf("%lld", &ti);
            int ans = cur;
            while(ans != 0 && ti >= up[ans][0].len){
                int low = 0, high = LOG - 1, tmpcur = ans;
                #ifdef debug
                printf("tmpcur = %d\n", tmpcur);
                #endif
                long long len = 0; 
                while(low <= high){
                    int mid = (low + high) >> 1;
                    if(up[tmpcur][mid].len <= ti){
                        ans = up[tmpcur][mid].u;
                        len = up[tmpcur][mid].len;
                        low = mid + 1;
                    }
                    else high = mid - 1;
                }
                #ifdef debug
                printf("len = %lld, ans = %d\n", len, ans);
                #endif
                ti -= len;
            }
            printf("%d\n", ans);
        }
        else if(op == 4){
            ti = 0;
            for(int i = down[cur].cur_next; i < down[cur].cnt; i++){
                if(path_max[ down[cur].dpath_ids[i] ] > ti)
                    ti = path_max[ down[cur].dpath_ids[i] ];
            }
            printf("%lld\n", ti);
        }
        else break;
    }
}