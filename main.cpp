#include <queue>
#include <pthread.h>
#include <vector>
#include <set>
// compile : g++ -o a.out main.cpp -std=c++11 -lpthread
using namespace std;
#define INF 18446744073709551615LL
#define CORE_MAX 96
typedef pair<uint64_t, uint64_t> pairs;
vector<pairs> edges[10000000];
uint64_t vt, eg, prbs;
// 100 is max problem size
uint64_t hs[100], result[100];
set<uint64_t> sets[100];
pthread_t thds[100];
void * run(void * data);
void addEdge(vector <pairs> adjeg[], uint64_t u, uint64_t v, uint64_t wt)
{
    adjeg[u].push_back({v, wt}); adjeg[v].push_back({u, wt});
}
void threads(uint64_t problem, uint64_t k)
{
    for(uint64_t i = 0; i<problem; i++)
        pthread_create(&thds[i], NULL, run, (void*)(i + CORE_MAX*k));
    for(uint64_t i = 0; i<problem; i++)
        pthread_join(thds[i], NULL);
}

bool checks(uint64_t posit,uint64_t data){
    if(sets[data].find(posit) != sets[data].end()) return true;
}

void * run(void * data){
    uint64_t min = INF;
    for(auto j : sets[(uint64_t)data]){
        priority_queue< pairs, vector<pairs>, greater<pairs> > pq;
        vector<uint64_t> dist(vt, INF);
        dist[j] = 0;
        pq.push({0, j});
        while(!pq.empty()){
            pq.pop();
            if(checks(pq.top().second, (uint64_t)data) && pq.top().second != j)
                break;
            for (auto x : edges[pq.top().second])
                if (dist[x.first] > dist[pq.top().second] + x.second) {
                    dist[x.first] = dist[pq.top().second] + x.second;
                    pq.push({dist[x.first], x.first});
                }
        }
        uint64_t check = (uint64_t)data;
        for(auto k : sets[check]){
            if(j==k) continue;
            if(dist[k] < min)
                min = dist[k];
        }
        if(result[check] > min)
            result[check] = min;
    }
}
int main(int argc, char ** argv){
    uint64_t house, m;
    FILE * input = fopen("input.txt", "r");
    FILE * output = fopen("output.txt", "w");
    fscanf(input, "%llu", &vt);
    fscanf(input, "%llu", &eg);
    for(int i = 0; i < eg; i++){
        uint64_t v1, v2, d;
        fscanf(input, "%llu %llu %llu", &v1, &v2, &d);
        addEdge(edges, v1, v2, d);
    }
    fscanf(input, "%llu", &prbs);
    for(uint64_t i = 0; i < prbs; i++){
        fscanf(input, "%llu", &hs[i]);
        for(int j = 0; j < hs[i]; j++){
            fscanf(input, "%llu", &house);
            sets[i].insert(house);
        }
        result[i] = INF;
    }
    if(prbs <= CORE_MAX) threads(prbs,0);
    else{
        for(m=0;m<prbs / CORE_MAX; m++) threads(CORE_MAX, m);
        threads(prbs % CORE_MAX, m);
    }
    for(int i = 0; i < prbs; i++)
        fprintf(output, "%llu\n", result[i]);
    fclose(input);
    fclose(output);
    return 0;
}