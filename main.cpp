/*
#include <iostream> // 내가 찾던것!! 언디렉션
#include <utility>
#include <queue>
using namespace std;
# define INF 0x3f3f3f3f

// iPair ==> Integer Pair
typedef pair<int, int> iPair;

// To add an edge
void addEdge(vector <pair<int, int> > adj[], int u,
                                     int v, int wt)
{
    adj[u].push_back(make_pair(v, wt));
    adj[v].push_back(make_pair(u, wt));
}


// Prints shortest paths from src to all other vertices
void shortestPath(vector<pair<int,int> > adj[], int V, int src)
{
    // Create a priority queue to store vertices that
    // are being preprocessed. This is weird syntax in C++.
    // Refer below link for details of this syntax
    // http://geeksquiz.com/implement-min-heap-using-stl/
    priority_queue< iPair, vector <iPair> , greater<iPair> > pq;

    // Create a vector for distances and initialize all
    // distances as infinite (INF)
    vector<int> dist(V, INF);

    // Insert source itself in priority queue and initialize
    // its distance as 0.
    pq.push(make_pair(0, src));
    dist[src] = 0;

    // Looping till priority queue becomes empty (or all
    //distances are not finalized)
    while (!pq.empty())
    {
        // The first vertex in pair is the minimum distance
        // vertex, extract it from priority queue.
        // vertex label is stored in second of pair (it
        // has to be done this way to keep the vertices
        // sorted distance (distance must be first item
        // in pair)
        int u = pq.top().second;
        pq.pop();

        // Get all adjacent of u.
        for (auto x : adj[u])
        {
            // Get vertex label and weight of current adjacent
            // of u.
            int v = x.first;
            int weight = x.second;

            // If there is shorted path to v through u.
            if (dist[v] > dist[u] + weight)
            {
                // Updating distance of v
                dist[v] = dist[u] + weight;
                pq.push(make_pair(dist[v], v));
            }
        }
    }

    // Print shortest distances stored in dist[]
    printf("Vertex Distance from Source\n");
    for (int i = 0; i < V; ++i)
        printf("%d \t\t %d\n", i, dist[i]);
}

// Driver program to test methods of graph class
int main()
{
    int V = 9;
    vector<iPair > adj[V];

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

    addEdge(adj, 1, 2, 5);
    addEdge(adj, 2, 3, 6);
    addEdge(adj, 1, 3, 1);

    shortestPath(adj, V, 3);

    // heap에다가 배열 선언해야할듯?
    return 0;
}
*/

/* 얘는 디렉션 있는 버젼
#include <cstdio>
#include <vector>
#include <set>
#include <queue>
#include <algorithm>

using namespace std;
#define ii pair<int,int>
#define pb push_back
int n;int e,source;
vector<ii> g[100000];
int dist[100000];
bool marked[100000];
void apply_dijkstra()
{
    set<ii > s;
    s.insert(ii(0,source));
    dist[source] = 0;marked[source] = 1;
    while(!s.empty())
    {
        ii p = *s.begin();
        s.erase(p);
        marked[p.second] = 2;
        for(int i=0;i<g[p.second].size();i++)
            if(marked[g[p.second][i].second]==0)
            {
                s.insert(ii(dist[p.second]+g[p.second][i].first,g[p.second][i].second));
                marked[g[p.second][i].second] = 1;
                dist[g[p.second][i].second] = dist[p.second]+g[p.second][i].first;
            }
            else if(marked[g[p.second][i].second]==1 && dist[g[p.second][i].second] > dist[p.second]+g[p.second][i].first)
            {
                s.erase(ii(dist[g[p.second][i].second],g[p.second][i].second));
                s.insert(ii(dist[p.second]+g[p.second][i].first,g[p.second][i].second));
                dist[g[p.second][i].second] =  dist[p.second]+g[p.second][i].first;
            }
    }
    printf("Shortest Distances are\n");
    for(int i=1;i<=n;i++)
    {
        printf("Node %d - %d\n",i,dist[i]);
    }
    printf("\n");
}

int main()
{
    printf("Enter the number of vertices:\n");
    scanf("%d",&n);
    printf("Enter the number of edges:\n");
    int e;scanf("%d",&e);
    printf("Enter the edges and their weights:\n");
    for(int i=0;i<e;i++)
    {
        int x,y,w;
        scanf("%d%d%d",&x,&y,&w);
        g[x].pb(ii(w,y));
    }
    printf("Enter the source:\n");
    scanf("%d",&source);
    apply_dijkstra();
    return 0;
}
 */