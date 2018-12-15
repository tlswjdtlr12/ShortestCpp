/*
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
            auto temp = pq.top();
            pq.pop();
            uint64_t now = temp.second;

            if (flag[now]) continue;
            else flag[now] = true;

            auto it = house[pnum].find(now);

            if(now != j && it != house[pnum].end())
                break;

            if(temp.first > result[pnum])
                break;

            for(int k=0; k<edge[now].size(); k++){

                uint64_t next = edge[now][k].first;
                if(flag[next]) continue;

                uint64_t weight = edge[now][k].second;

                if(distance[next] > distance[now] + weight){

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

*/




















//#include <set>
//#include <iostream>
//#include <utility>
//#include <queue>
#include <cstdio>
#include <algorithm>
#include <queue>
#include <cstdlib>
#include <pthread.h>
#include <vector>
#include <functional>
#include <set>
using namespace std;
#define MAX_VER 10000005
#define MAX_HOUSE 64
#define MAX_PROBLEM 100
#define INF 18446744073709551615LL

#define MAX_CORE 96

//vector<pair<uint64_t, uint64_t> > edge[MAX_VER];
uint64_t num_of_vertices, num_of_edge, num_of_problem, num_of_house[MAX_PROBLEM], result[MAX_PROBLEM];
set<uint64_t> house[MAX_PROBLEM];
bool problem_solved[MAX_PROBLEM];
pthread_t thd[MAX_PROBLEM];
int used_thd_count;
// iPair ==> Integer Pair
typedef pair<uint64_t, uint64_t> iPair;
vector<iPair > adj[MAX_VER];

// To add an edge
void addEdge(vector <pair<uint64_t, uint64_t> > adj[], uint64_t u, uint64_t v, uint64_t wt)
{
    adj[u].push_back({v, wt});
    adj[v].push_back({u, wt});
}

//shortestPath(adj, V, 3);
// Prints shortest paths from src to all other vertices
//void shortestPath(vector<pair<int,int> > adj[], int V, int src)
void * shortestPath(void * data)
{
    // Create a priority queue to store vertices that
    // are being preprocessed. This is weird syntax in C++.
    // Refer below link for details of this syntax
    // http://geeksquiz.com/implement-min-heap-using-stl/
    uint64_t pnum = (uint64_t)data;
    for(auto j : house[pnum])
    {
        priority_queue<iPair, vector<iPair>, greater<iPair> > pq;

        // Create a vector for distances and initialize all
        // distances as infinite (INF)
        vector<uint64_t> dist(num_of_vertices, INF);
        vector<bool> flag(num_of_vertices, false);

        // Insert source itself in priority queue and initialize
        // its distance as 0.
        pq.push({0, j});
        dist[j] = 0;

        // Looping till priority queue becomes empty (or all
        //distances are not finalized)
        while (!pq.empty()) {
            // The first vertex in pair is the minimum distance
            // vertex, extract it from priority queue.
            // vertex label is stored in second of pair (it
            // has to be done this way to keep the vertices
            // sorted distance (distance must be first item
            // in pair)

//            uint64_t now = pq.top().second;
//            pq.pop();
            auto temp = pq.top();
            pq.pop();
            uint64_t now = temp.second;

            if (flag[now]) continue;
            else flag[now] = true;

            auto it = house[pnum].find(now);

            if(now != j && it != house[pnum].end())
                break;

            if(temp.first > result[pnum])
                break;

            // Get all adjacent of u.
            for (auto x : adj[now]) {
                // Get vertex label and weight of current adjacent
                // of u.
//                int v = x.first;
                uint64_t next = x.first;
                if(flag[next]) continue;
//                int weight = x.second;
                uint64_t weight = x.second;

                // If there is shorted path to v through u.
                if (dist[next] > dist[now] + weight) {
                    // Updating distance of v
                    dist[next] = dist[now] + weight;
                    pq.push({dist[next], next});
                }
            }

        }
        uint64_t min = INF;
        for(auto k : house[pnum]){
            if(j==k) continue;
            if(dist[k] < min) min = dist[k];
        }
        if(result[pnum] > min) result[pnum] = min;


    }
    // Print shortest distances stored in dist[]
//    printf("Vertex Distance from Source\n");
//    for (int i = 0; i < V; ++i)
//        printf("%d \t\t %d\n", i, dist[i]);
}

// Driver program to test methods of graph class
int main()
{
    FILE * input = fopen("input.txt", "r");
    FILE * output = fopen("output.txt", "w");

    // making above shown graph
//    addEdge(adj, 0, 1, 4);
//    addEdge(adj, 0, 7, 8);
//    addEdge(adj, 1, 2, 8);
//    addEdge(adj, 1, 7, 11);
//    addEdge(adj, 2, 3, 7);
//    addEdge(adj, 2, 8, 2);
//    addEdge(adj, 2, 5, 4);
//    addEdge(adj, 3, 4, 9);
//    addEdge(adj, 3, 5, 14);
//    addEdge(adj, 4, 5, 10);
//    addEdge(adj, 5, 6, 2);
//    addEdge(adj, 6, 7, 1);
//    addEdge(adj, 6, 8, 6);
//    addEdge(adj, 7, 8, 7);

    fscanf(input, "%llu", &num_of_vertices);
    fscanf(input, "%llu", &num_of_edge);

    for(int i = 0; i < num_of_edge; i++){
        uint64_t v1, v2, d;
        fscanf(input, "%llu %llu %llu", &v1, &v2, &d);
        addEdge(adj, v1, v2, d);
    }
    fscanf(input, "%llu", &num_of_problem);

//    addEdge(adj, 1, 2, 5);
//    addEdge(adj, 2, 3, 6);
//    addEdge(adj, 1, 3, 1);

    for(uint64_t i = 0; i < num_of_problem; i++){

        fscanf(input, "%llu", &num_of_house[i]);

        for(int j = 0; j < num_of_house[i]; j++){
            uint64_t temp;
            fscanf(input, "%llu", &temp);
            house[i].insert(temp);
        }
        result[i] = INF;
    }

    //shortestPath(adj, V, 3);
    for(uint64_t j=0; j< num_of_problem; j++)
        shortestPath((void*)j);

    for(int i = 0; i < num_of_problem; i++){
        fprintf(output, "%llu\n", result[i]);
    }
    fclose(input);
    fclose(output);
    return 0;
}
