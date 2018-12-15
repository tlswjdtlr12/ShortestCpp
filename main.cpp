#include <cstdio>
#include <algorithm>
#include <queue>
#include <cstdlib>
#include <pthread.h>
#include <vector>
#include <functional>
#include <set>
// compile : g++ -o a.out main.cpp -std=c++11
using namespace std;
#define VER_MAX 10000005
#define PRO_MAX 100
#define INF 18446744073709551615LL
#define CORE_MAX 96
typedef pair<uint64_t, uint64_t> pairs;
vector<pairs> edge[VER_MAX];
uint64_t vt, eg, num_of_problem, hs[PRO_MAX], result[PRO_MAX];
set<uint64_t> sets[PRO_MAX];
pthread_t thd[PRO_MAX];
void * run(void * data);

void addEdge(vector <pairs> adj[], uint64_t u, uint64_t v, uint64_t wt)
{
    adj[u].push_back({v, wt});
    adj[v].push_back({u, wt});
}

void threads(uint64_t problem, uint64_t k)
{
    for(uint64_t i = 0; i<problem; i++)
        pthread_create(&thd[i], NULL, run, (void*)(i + CORE_MAX*k));
    for(uint64_t i = 0; i<problem; i++)
        pthread_join(thd[i], NULL);
}

void * run(void * data){
    uint64_t pnum = (uint64_t)data;
    for(auto j : sets[pnum]){
        priority_queue< pairs, vector<pairs>, greater<pairs> > pq;
        pq.push({0, j});
        vector<uint64_t> distance(vt, INF);
        distance[j] = 0;
        while(!pq.empty()){
            uint64_t now = pq.top().second;
            pq.pop();
            if(now != j && sets[pnum].find(now) != sets[pnum].end())
                break;
            for (auto x : edge[now]) {
                uint64_t next = x.first;
                uint64_t weight = x.second;
                if (distance[next] > distance[now] + weight) {
                    distance[next] = distance[now] + weight;
                    pq.push({distance[next], next});
                }
            }
        }
        uint64_t min = INF;
        for(auto k : sets[pnum]){
            if(j==k) continue;
            if(distance[k] < min)
                min = distance[k];
        }
        if(result[pnum] > min)
            result[pnum] = min;
    }
}

int main(int argc, char ** argv){
    FILE * input = fopen("input.txt", "r");
    FILE * output = fopen("output.txt", "w");
    fscanf(input, "%llu", &vt);
    fscanf(input, "%llu", &eg);
    for(int i = 0; i < eg; i++){
        uint64_t v1, v2, d;
        fscanf(input, "%llu %llu %llu", &v1, &v2, &d);
        addEdge(edge, v1, v2, d);
    }
    fscanf(input, "%llu", &num_of_problem);
    for(uint64_t i = 0; i < num_of_problem; i++){
        fscanf(input, "%llu", &hs[i]);
        for(int j = 0; j < hs[i]; j++){
            uint64_t temp;
            fscanf(input, "%llu", &temp);
            sets[i].insert(temp);
        }
        result[i] = INF;
    }

    if(num_of_problem <= CORE_MAX) threads(num_of_problem,0);
    else{
        int m;
        for(m=0;m<num_of_problem / CORE_MAX; m++) threads(CORE_MAX, m);
        threads(num_of_problem % CORE_MAX, m);
    }
    for(int i = 0; i < num_of_problem; i++)
        fprintf(output, "%llu\n", result[i]);
    fclose(input);
    fclose(output);
    return 0;
}