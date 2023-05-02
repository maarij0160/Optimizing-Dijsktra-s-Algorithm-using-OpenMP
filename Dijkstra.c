#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include<omp.h>
#include <time.h>
#include<stdlib.h>


 
// Number of vertices in the graph
#define V 9
int graph[V][V];
// A utility function to find the vertex with minimum
// distance value, from the set of vertices not yet included
// in shortest path tree
int minDistance(int s,int e,int dist[], bool sptSet[])
{
    // Initialize min value
    int mini = INT_MAX, min_index=8;

    for (int v = s; v <= e; v++){
        if (sptSet[v] == false && dist[v] < mini){
            mini = dist[v];

   min_index = v;
 }
//printf("min_ind %d\n",min_index);
}
    return min_index;
}
 void Update(int graph[V][V],int s,int e,int hold,int dist[], bool sptSet[]){

 for (int v = s; v <= e; v++){

           // Update dist[v] only if is not in sptSet,
            // there is an edge from u to v, and total
            // weight of path from src to  v through u is
            // smaller than current value of dist[v]
            if (!sptSet[v] && graph[hold][v] && dist[hold] != INT_MAX && dist[hold] + graph[hold][v] < dist[v]){
               
                dist[v] = dist[hold] + graph[hold][v];

}
}



}
// A utility function to print the constructed distance
// array
void printSolution(int dist[])
{
    printf("Vertex \t\t Distance from Source\n");
    for (int i = 0; i < V; i++)
        printf("%d \t\t\t\t %d\n", i, dist[i]);
}
 
// Function that implements Dijkstra's single source
// shortest path algorithm for a graph represented using
// adjacency matrix representation
void dijkstra(int graph[V][V],int src)
{
    int dist[V]; // The output array.  dist[i] will hold the
                 // shortest
    // distance from src to i
 
    bool sptSet[V]; // sptSet[i] will be true if vertex i is
                    // included in shortest
    // path tree or shortest distance from src to i is
    // finalized
 
    // Initialize all distances as INFINITE and stpSet[] as
    // false


    for (int i = 0; i < V; i++)
        dist[i] = INT_MAX, sptSet[i] = false;
 
    // Distance of source vertex from itself is always 0
    dist[src] = 0;
    int min=INT_MAX;
    int hold;
    int u;

    // Find shortest path for all vertices
float start = omp_get_wtime();

#pragma omp parallel shared(hold) private(u) num_threads(3)
{
min=INT_MAX;
   int x = omp_get_num_threads();
   //printf("Active Threads %d\n",x);
   int chunk = V/x;
   int me = omp_get_thread_num();
 

   int startv = me * chunk;
   int endv = startv + chunk - 1;
    int count = 0;
 

    for (count = 0; count < V - 1; count++) {
        // Pick the minimum distance vertex from the set of
        // vertices not yet processed. u is always equal to
        // src in the first iteration.

         u = minDistance(startv,endv,dist, sptSet);

 
#pragma omp critical
{  
if(min > dist[u]){
min = dist[u];
hold = u;
      }

}


#pragma omp barrier
        // Mark the picked vertex as processed
printf("%d%%\n",hold);
       #pragma omp single
    {

        sptSet[hold] = true;
      }
   

        // Update dist value of the adjacent vertices of the
        // picked vertex.
// #pragma omp critical
//{
        Update(graph,startv,endv,hold,dist,sptSet);
//}
min = INT_MAX;
#pragma omp barrier
    }
 }

float end = omp_get_wtime();
    // print the constructed distance array
    printSolution(dist);
printf("Running time: %f ms\n", (end - start)*1000);
}
 
// driver's code
int main()
{
    /* Let us create the example graph discussed above */
   
// for(int i=0;i<V;i++){


// for(int j=0;j<V;j++){
// scanf("%d",&graph[i][j]);

//}
//}
int graph[V][V] = {{ 0, 4, 0, 0, 0, 0, 0, 8, 0 },
                        { 4, 0, 8, 0, 0, 0, 0, 11, 0 },
                        { 0, 8, 0, 7, 0, 4, 0, 0, 2 },
                        { 0, 0, 7, 0, 9, 14, 0, 0, 0 },
                        { 0, 0, 0, 9, 0, 10, 0, 0, 0 },
                        { 0, 0, 4, 14, 10, 0, 2, 0, 0 },
                        { 0, 0, 0, 0, 0, 2, 0, 1, 6 },
                        { 8, 11, 0, 0, 0, 0, 1, 0, 7 },
                        { 0, 0, 2, 0, 0, 0, 6, 7, 0 } };
    // Function call
    printf("OPEN MP RESULT ON A GRAPH OF %d Vertices\n",V);
    dijkstra(graph,0);
 
    return 0;
}