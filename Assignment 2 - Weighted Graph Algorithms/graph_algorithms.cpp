#ifndef GRAPH_ALGS
#define GRAPH_ALGS

#include <map>
#include <vector>
#include <queue>
#include <stack>
#include <list>
#include <deque>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <iostream>
#include <utility>
#include <algorithm>
#include <limits>
#include "weighted_graph.hpp"
#include "easy_weighted_graph_algorithms.cpp"

//this just returns a graph of a depth first traversal, used in connected components.
template <typename vertex> 
weighted_graph<vertex> depth_first_graph(const weighted_graph<vertex>& g, const vertex& start_vertex) {

	weighted_graph<vertex> df_graph;
	std::unordered_set<vertex> visited;
	std::stack<vertex> unprocessed;
	
	unprocessed.push(start_vertex);
	
	while (!unprocessed.empty()){
		
		vertex u = unprocessed.top();
		unprocessed.pop();
		//check if it's been visited
		if (visited.count(u) == 0){
			visited.insert(u); //mark as visited
			df_graph.add_vertex(u); //register vertex in graph
			std::priority_queue<vertex> pq; //used to store neighbors in queue order.
			
			//here we can get edge weight and add to df_graph.
			for (auto n_it = g.cneighbours_begin(u); n_it != g.cneighbours_end(u); ++n_it){
				pq.push(n_it->first);
				//get edge weight here and add it to the graph, not sure if we'll have to handle the fact that it will be added twice....
				
				//check if this neighbor vertex is already in the graph and if not add it.
				//I won't do the check in code because weighted_grah.hpp already does it for us.
				df_graph.add_vertex(n_it->first);
				//get edge weight
				int weight = n_it->second;
				//add edge with weight
				df_graph.add_edge(u, n_it->first, weight);
			}
			
			//place all neighbors into unprocessed stack.
			while (!pq.empty()){
				unprocessed.push(pq.top());
				pq.pop();
			}
		}
		
	}	
	return df_graph;
}

//Returns true if the graph is connected, false otherwise.
template <typename vertex>
bool is_connected(const weighted_graph<vertex>& g){
	//call BFS or DFT and check the number of vertecies it returns.
	auto firstVertex = g.begin();
	if (firstVertex == g.end()) //check if the graph is empty
		return true;
	std::vector<vertex> verticesVisited = breadth_first(g, *firstVertex); //do a BFT traversal.
	return verticesVisited.size() == g.num_vertices(); //if the traversal visited all the vertcies that means that all of them are connected and this condition would return true.
	//return false;
}

//Returns a vector of weighted graphs, where each weighted graph is a connected
//component of the input graph.
template <typename vertex>
std::vector<weighted_graph<vertex>> connected_components(const weighted_graph<vertex>& g){
	auto returnVector = std::vector<weighted_graph<vertex>>();
	
	auto firstVertex = g.begin();
	if (firstVertex == g.end())
		return returnVector;
		
	//alogrithim is:
	//look through all vertcies
	//if not visited vertex start a dft from it.
	//in the dft at the current vertex copy all the edges into a subgraph g verticies.
	//
		
	int verticesNum = g.num_vertices();
	int visitedVertices = 0;
	std::map<vertex, bool> marked;
	//set all as not marked, commented out cos i changed the way the check works in the for loop below.
	//for (auto it : g)
		//marked[it] = false;
	
	for (auto it : g) {
		if (marked.find(it) == marked.end()) { //if the vertex hasn't been visited (if the key isn't in the map)
			weighted_graph<vertex> subGraph = depth_first_graph(g, it); //uses the first function to perform a DFT and return it's graph.
			//we need to go though this sub graph and mark the ones present as marked
			for (auto subIt : subGraph) {
				marked[subIt] = true;
			}
			//mark this vertex.
			marked[it] = true;
			returnVector.push_back(subGraph); //add the component to the vector being returned.
		}
	}
	
	return returnVector;
}

//Returns a map of the vertices of the weighted graph g and their distances from
//the given starting vertex v.
template <typename vertex> 
std::map<vertex, int> dijkstras(const weighted_graph<vertex>& g, const vertex& v){
	//https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
	//https://stackoverflow.com/questions/17898342/priority-queue-in-dijkstras-algorithm?utm_medium=organic&utm_source=google_rich_qa&utm_campaign=google_rich_qa
	//int inf = std::numeric_limits<int>::infinity(); //This doesn't work.... so i'm making do with the line below.
	int inf = std::numeric_limits<int>::max() - 99999;
	//int inf = 999999;
	std::map<vertex, int> distances;
	std::map<vertex, bool> marked;
	int vertciesMarked = 0;
	//syntax taken from easy weighted graph algos.
	std::priority_queue<std::pair<vertex, int>, std::vector<std::pair<vertex, int> >, std::greater<std::pair<vertex, int> >> vertexPQ;
	
	//set all distances to infitity
	for (auto it : g) {
		if (it == v)
			distances[it] = 0;
		else
			distances[it] = inf;
		marked[it] = false;
		
		vertexPQ.push(std::pair<vertex, int> (it, distances[it]));
	}
	while (!vertexPQ.empty()) {
		vertex u = vertexPQ.top().first;
		vertexPQ.pop();
		for (auto it = g.cneighbours_begin(u); it != g.cneighbours_end(u); ++it) {
			//if (marked[*it])
				//continue;
			vertex neighbour = it->first;
			int weight = it->second;
			if (distances[neighbour] > (distances[u] + weight)) {
				distances[neighbour] = distances[u] + weight;
				vertexPQ.push(std::pair<vertex, int> (neighbour, weight));
			}
		}
	}	
	return distances;
	//return std::map<vertex, int>();
}

//I tried to use maps in place of arrays however it always failed. :(
//this is a modified DFT that uses the depth and low points to check if a vertex is an articulation point.
template <typename vertex>
void findArticulationPoints(vertex i, int d, const weighted_graph<vertex>& g, bool visited[], int depth[], int low[], vertex parent[], std::vector<vertex>& points) {
    visited[i] = true, //visit this vertex and set intial values
    depth[i] = d;
    low[i] = d;
    int child_count = 0;
    bool is_articulation = false;
    for (auto n = g.cneighbours_begin(i); n != g.cneighbours_end(i); ++n) { //iterate though all neighbours
		vertex neighbour = n->first;
        if (!visited[neighbour]) { //only act on unvisited vertices
            parent[neighbour] = i; //set parent of neighbour vertex as we have already visited a previous one that is the parent.
            findArticulationPoints(neighbour, d + 1, g, visited, depth, low, parent, points); //do a traversal from this point again.
            child_count = child_count + 1;
			//check if the vertex is an articulation point.
            if (low[neighbour] >= depth[i])
                is_articulation = true;
            low[i] = std::min(low[i], low[neighbour]);
        }
        else if (neighbour != parent[i])
            low[i] = std::min(low[i], depth[neighbour]);
    }
    if ((parent[i] >= 0 && is_articulation) || //This is for a general articulation point. conditions: does have a parent and is determined to be an articulation point.
        (parent[i] <= -1 && child_count > 1))	//This condition checks if the vertex is a root. conditions: no parent and there are at least 2 children
            points.push_back(i);
}


//Returns a vector containing all the articulation points of the
//input weighted graph g.
template <typename vertex>
std::vector<vertex> articulation_points(const weighted_graph<vertex>& g){
	
	//declare all the arrays.
	const int vertexNum = g.num_vertices();
	bool visited[vertexNum];
	int depth[vertexNum], low[vertexNum];
	vertex parent[vertexNum];
	std::vector<vertex> points;
	
	if (g.num_vertices() == 0) //check if this is an empty graph.
		return points;
	
	//initialise the arrays.
	for (int i=0;i<vertexNum;i++) {
		visited[i] = false;
		depth[i] = 0;
		low[i] = 0;
		parent[i] = -1; //negative integer as a sentinel value
	}
	
	vertex firstVertex = *(g.begin());//grab the first vertex to iterate from.
	findArticulationPoints(firstVertex, 0, g, visited, depth, low, parent, points); //start the recursion loop of finding articulation points.
	
	//this is the easy crap way, it works but isn't the best.
	/*
	for (auto v: g) {
		weighted_graph<vertex> copyGraph = g;
		copyGraph.remove_vertex(v);
		if (!is_connected(copyGraph))
			points.push_back(v);
	}
	*/
	return points;
}


#endif
