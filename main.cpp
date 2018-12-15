#include <cstdio>
#include <algorithm>
#include <queue>
#include <cstdlib>
#include <pthread.h>
#include <vector>
#include <functional>
#include <set>
// 컴파일 법 : g++ -o a.out main.cpp -std=c++11
using namespace std;

#define MAX_VER 10000005
#define MAX_HOUSE 64
#define MAX_PROBLEM 100
#define INF 18446744073709551615LL
#define MAX_CORE 96

vector<pair<uint64_t, uint64_t> > edge[MAX_VER];
uint64_t num_of_vertices, num_of_edge, num_of_problem, num_of_house[MAX_PROBLEM], result[MAX_PROBLEM];
set<uint64_t> house[MAX_PROBLEM];
bool problem_solved[MAX_PROBLEM];
pthread_t thd[MAX_PROBLEM];
int used_thd_count;



void * run(void * data){
    uint64_t pnum = (uint64_t)data;
    for(auto j : house[pnum]){

        priority_queue<pair<uint64_t, uint64_t>, vector<pair<uint64_t, uint64_t> >, greater<pair<uint64_t, uint64_t> > > pq;
        pq.push({0, j});

        vector<uint64_t> distance(num_of_vertices, INF);
        vector<bool> flag(num_of_vertices, false);
        distance[j] = 0;

        while(!pq.empty()){
//            auto temp = pq.top(); temp 넣은건 속도 개선용이었나봄. 결과에 이상없음
  //          pq.pop();
    //        uint64_t now = temp.second;
            uint64_t now = pq.top().second;
            pq.pop();

            if (flag[now]) continue;
            else flag[now] = true;

            auto it = house[pnum].find(now);

            if(now != j && it != house[pnum].end())
                break;

            //if(temp.first > result[pnum])
             //   break;

            /*for(int k=0; k<edge[now].size(); k++){

                uint64_t next = edge[now][k].first;
                if(flag[next]) continue;

                uint64_t weight = edge[now][k].second;

                if(distance[next] > distance[now] + weight){

                    distance[next] = distance[now] + weight;
                    pq.push({distance[next], next});
                }
            }*/
            for (auto x : edge[now]) {
                // Get vertex label and weight of current adjacent
                // of u.
//                int v = x.first;
                uint64_t next = x.first;
                if(flag[next]) continue;
//                int weight = x.second;
                uint64_t weight = x.second;

                // If there is shorted path to v through u.
                if (distance[next] > distance[now] + weight) {
                    // Updating distance of v
                    distance[next] = distance[now] + weight;
                    pq.push({distance[next], next});
                }
            }
        }

        uint64_t min = INF;

        for(auto k : house[pnum]){
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

    fscanf(input, "%llu", &num_of_vertices);
    fscanf(input, "%llu", &num_of_edge);

    for(int i = 0; i < num_of_edge; i++){

        uint64_t v1, v2, d;
        fscanf(input, "%llu %llu %llu", &v1, &v2, &d);

        edge[v1].push_back({v2, d});
        edge[v2].push_back({v1, d});
    }
    fscanf(input, "%llu", &num_of_problem);


    for(uint64_t i = 0; i < num_of_problem; i++){

        fscanf(input, "%llu", &num_of_house[i]);

        for(int j = 0; j < num_of_house[i]; j++){
            uint64_t temp;
            fscanf(input, "%llu", &temp);
            house[i].insert(temp);
        }
        result[i] = INF;
    }


    if(num_of_problem <= MAX_CORE){
        for(uint64_t i = 0; i < num_of_problem; i++)
            pthread_create(&thd[i], NULL, run, (void*)i);

        for(uint64_t i = 0; i < num_of_problem; i++)
            pthread_join(thd[i], NULL);
    }
    else{
        int i;
        for(i = 0; i < num_of_problem / MAX_CORE; i++){
            for(uint64_t j = 0; j < MAX_CORE; j++)
                pthread_create(&thd[j], NULL, run, (void*)(j + i * MAX_CORE));
            for(uint64_t j = 0; j < MAX_CORE; j++)
                pthread_join(thd[j], NULL);
        }
        for(uint64_t j = 0; j < num_of_problem % MAX_CORE; j++)
            pthread_create(&thd[j], NULL, run, (void *)(j + i * MAX_CORE));
        for(uint64_t j = 0; j < num_of_problem % MAX_CORE; j++)
            pthread_join(thd[j], NULL);

        // for(uint64_t i = 0; i < MAX_CORE; i++)
        //    pthread_create(&thd[i], NULL, run, (void *)i);

        // for(uint64_t i = 0 ; i < MAX_CORE; i++)
        //    pthread_join(thd[i], NULL);

        // for(uint64_t i = MAX_CORE; i < num_of_problem; i++)
        //    pthread_create(&thd[i - MAX_CORE], NULL, run, (void*)i);

        // for(uint64_t i = MAX_CORE; i < num_of_problem; i++)
        //    pthread_join(thd[i - MAX_CORE], NULL);
    }

    for(int i = 0; i < num_of_problem; i++){
        fprintf(output, "%llu\n", result[i]);
    }
    fclose(input);
    fclose(output);
    return 0;
}
















