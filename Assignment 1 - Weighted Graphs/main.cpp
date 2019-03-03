#include <iostream>
#include <vector>
#include "weighted_graph.hpp"

void TS_ASSERT_EQUALS(int a, int b) {
    std::cout << "assert call equal: " << a << "," << b << ". Result: " << ((a == b) ? "True":"False") << std::endl;
}

void TS_ASSERT(bool a) {
    std::cout << "assert call assert: " << a << ". Result: " << ((a == true) ? "True":"False") << std::endl;
}

void TS_ASSERT_DIFFERS(int a, int b) {
    std::cout << "assert call differs: " << a << "," << b << ". Result: " << ((a != b) ? "True":"False") << std::endl;
}

void TS_ASSERT_LESS_THAN(int a, int b) {
	std::cout << "assert call less than: " << a << "," << b << ". Result: " << ((a < b) ? "True":"False") << std::endl;
}

template <typename vertex>
std::vector<std::pair<vertex, vertex> > random_tree(const std::vector<vertex>& vertices){

	std::vector<std::pair<vertex, vertex> > tree_edges;
	if (!vertices.empty()){
		std::vector<vertex> connected;
		std::vector<vertex> unconnected;
		connected.push_back(vertices[0]);
	

	
		for (int i = 1; i < vertices.size(); ++i) unconnected.push_back(vertices[i]);
	
		while (connected.size() < vertices.size()){
	
			int index1 = std::rand()%connected.size();
			int index2 = std::rand()%unconnected.size();
			vertex u = connected[index1];
			vertex v = unconnected[index2];
			tree_edges.push_back({u,v});
			unconnected.erase(unconnected.begin() + index2);
			connected.push_back(v);
	
		}
	}
	return tree_edges;

}

void testGetEdgeWeight(){
	
	weighted_graph<int> g;
	int r = std::rand()%20;
	
	int re[r][r];
	
	for (int i = 0; i < r; ++i){
		for (int  j = 0; j < r; ++j){
			re[i][j] = 0;
		}
	}
	
	for (int i = 0; i < r; ++i){
		for (int j = i+1; j < r;  ++j){
			if (std::rand()%2 == 1){
				int weight = (std::rand()%10) + 1;
				re[i][j] = weight;
				re[j][i] = weight;
			}
		}
	}
	
	for (int i = 0; i < r; ++i){
		g.add_vertex(i);
	}
	
	for (int i = 0; i < r; ++i){
		for (int j = i+1; j < r;  ++j){
			if (re[i][j] > 0){
				g.add_edge(i,j,re[i][j]);
			}
		}
	}
	
	for (int i = 0; i < r; ++i){
		for (int j = i+1; j < r;  ++j){
			if (re[i][j] > 0){
				TS_ASSERT_EQUALS(g.get_edge_weight(i,j), re[i][j]);
				TS_ASSERT_EQUALS(g.get_edge_weight(j,i), re[j][i]);
			}
		}
	}		
	
}


void testDegree(){	

	weighted_graph<int> g;
	int r = std::rand()%20;
	
	int re[r][r];
	
	for (int i = 0; i < r; ++i){
		for (int  j = 0; j < r; ++j){
			re[i][j] = 0;
		}
	}
	
	for (int i = 1; i < r; ++i){
		for (int j = i+1; j < r;  ++j){
			if (std::rand()%2 == 1){
				int weight = (std::rand()%10) + 1;
				re[i][j] = weight;
				re[j][i] = weight;
			}
		}
	}
	
	for (int i = 0; i < r; ++i){
		g.add_vertex(i);
	}
	
	for (int i = 0; i < r; ++i){
		for (int j = i+1; j < r;  ++j){
			if (re[i][j] > 0){
				g.add_edge(i,j,re[i][j]);
			}
		}
	}

	for (int i = 0; i < r; ++i){
		int edge_count = 0;
		for (int j = 0; j < r;  ++j){
			if (re[i][j] > 0){
				edge_count++;
			}
		}
		TS_ASSERT_EQUALS(g.degree(i), edge_count);
	}
}

void testTotalWeight(){

	weighted_graph<int> g;
	int r = std::rand()%20;
	int total_weight = 0;

	int re[r][r];

	for (int i = 0; i < r; ++i){
		for (int  j = 0; j < r; ++j){
			re[i][j] = 0;
		}
	}

	for (int i = 1; i < r; ++i){
		for (int j = i+1; j < r;  ++j){
			if (std::rand()%2 == 1){
				int weight = (std::rand()%10) + 1;
				re[i][j] = weight;
				re[j][i] = weight;
				total_weight += weight;
			}
		}
	}

	for (int i = 0; i < r; ++i){
		g.add_vertex(i);
	}

	for (int i = 0; i < r; ++i){
		for (int j = i+1; j < r;  ++j){
			if (re[i][j] > 0){
				g.add_edge(i,j,re[i][j]);
			}
		}
	}

	TS_ASSERT_EQUALS(g.total_weight(), total_weight);
}

void testGetVertices(){

	weighted_graph<int> g;
	int r = std::rand()%20;
	int s = std::rand()%r;

	for (int i = 0; i < r; ++i) g.add_vertex(i);

	std::vector<int> v = g.get_vertices();
	TS_ASSERT_EQUALS(g.num_vertices(), v.size());
	for (int i = 0; i < v.size(); ++i){
		TS_ASSERT(g.has_vertex(v[i]));
	}

	for (int i = 0; i < v.size(); ++i){
		for (int j = i + 1; j < v.size(); ++j){
			TS_ASSERT_DIFFERS(v[i], v[j]);
		}
	}

	for (int i = 0; i < s; ++i) g.remove_vertex(i);

	v = g.get_vertices();
	TS_ASSERT_EQUALS(g.num_vertices(), v.size());
	for (int i = 0; i < v.size(); ++i){
		TS_ASSERT(g.has_vertex(v[i]));
	}

	for (int i = 0; i < v.size(); ++i){
		for (int j = i + 1; j < v.size(); ++j){
			TS_ASSERT_DIFFERS(v[i], v[j]);
		}
	}		
}

void testGetNeighbours(){

	weighted_graph<int> g;
	int r = std::rand()%20;

	int re[r][r];

	for (int i = 0; i < r; ++i){
		for (int  j = 0; j < r; ++j){
			re[i][j] = 0;
		}
	}

	for (int i = 1; i < r; ++i){
		for (int j = i+1; j < r;  ++j){
			if (std::rand()%2 == 1){
				int weight = (std::rand()%10) + 1;
				re[i][j] = weight;
				re[j][i] = weight;
			}
		}
	}

	for (int i = 0; i < r; ++i){
		g.add_vertex(i);
	}

	for (int i = 0; i < r; ++i){
		for (int j = i+1; j < r;  ++j){
			if (re[i][j] > 0){
				g.add_edge(i,j,re[i][j]);
			}
		}
	}

	std::vector<int> vertices = g.get_vertices();
	for (int i = 0; i < vertices.size(); ++i){
		std::vector<int> n_i = g.get_neighbours(vertices[i]);
		TS_ASSERT_EQUALS(g.degree(vertices[i]), n_i.size());
		for (int j = 0; j < n_i.size(); ++j){
			TS_ASSERT(g.are_adjacent(vertices[i], n_i[j]));
		}

		for (int j = 0; j < n_i.size(); ++j){
			for (int k = j + 1; k < n_i.size(); ++k){
				TS_ASSERT_DIFFERS(n_i[j], n_i[k]);
			}
		}	
	}
}
/*
void testDepthFirst(){

	weighted_graph<int> g;
	int r = std::rand()%20;
	std::unordered_map<int, std::unordered_set<int> > adj_list;

	int re[r][r];

	for (int i = 0; i < r; ++i){
		for (int  j = 0; j < r; ++j){
			re[i][j] = 0;
		}
	}

	for (int i = 1; i < r; ++i){
		for (int j = i+1; j < r;  ++j){
			if (std::rand()%2 == 1){
				int weight = (std::rand()%10) + 1;
				re[i][j] = weight;
				re[j][i] = weight;
			}
		}
	}

	for (int i = 0; i < r; ++i){
		g.add_vertex(i);
	}

	for (int i = 0; i < r; ++i){
		for (int j = i+1; j < r;  ++j){
			if (re[i][j] > 0){
				g.add_edge(i,j,re[i][j]);
			}
		}
	}

	for (int i = 0; i < r; ++i){

		std::unordered_set<int> n;
		for (int j = 0; j < r; ++j){
			if (re[i][j] > 0) n.insert(j);
		}
		adj_list.insert({i,n});

	}

	int start_vertex = std::rand()%r;

	std::vector<int> graph_dfs = g.depth_first(start_vertex);
	std::vector<int> reference_dfs = dfs(adj_list, start_vertex);

	TS_ASSERT_EQUALS(graph_dfs, reference_dfs);

}

void testBreadthFirst(){

	weighted_graph<int> g;
	int r = std::rand()%20;
	std::unordered_map<int, std::unordered_set<int> > adj_list;

	int re[r][r];

	for (int i = 0; i < r; ++i){
		for (int  j = 0; j < r; ++j){
			re[i][j] = 0;
		}
	}

	for (int i = 1; i < r; ++i){
		for (int j = i+1; j < r;  ++j){
			if (std::rand()%2 == 1){
				int weight = (std::rand()%10) + 1;
				re[i][j] = weight;
				re[j][i] = weight;
			}
		}
	}

	for (int i = 0; i < r; ++i){
		g.add_vertex(i);
	}

	for (int i = 0; i < r; ++i){
		for (int j = i+1; j < r;  ++j){
			if (re[i][j] > 0){
				g.add_edge(i,j,re[i][j]);
			}
		}
	}

	for (int i = 0; i < r; ++i){

		std::unordered_set<int> n;
		for (int j = 0; j < r; ++j){
			if (re[i][j] > 0) n.insert(j);
		}
		adj_list.insert({i,n});

	}

	int start_vertex = std::rand()%r;

	std::vector<int> graph_bfs = g.breadth_first(start_vertex);
	std::vector<int> reference_bfs = bfs(adj_list, start_vertex);

	TS_ASSERT_EQUALS(graph_bfs, reference_bfs);

}
*/
void testMST(){

	weighted_graph<int> g;
	int r = std::rand()%20 + 1;

	int re[r][r];

	for (int i = 0; i < r; ++i){
		for (int  j = 0; j < r; ++j){
			re[i][j] = 0;
		}
	}

	for (int i = 0; i < r; ++i){
		g.add_vertex(i);
	}

	std::vector<std::pair<int, int> > tree_edges = random_tree(g.get_vertices());

	for (auto edge : tree_edges){
		g.add_edge(edge.first, edge.second, 1);
		re[edge.first][edge.second] = 1;
		re[edge.second][edge.first] = 1;			
	}

	for (int i = 1; i < r; ++i){
		for (int j = i+1; j < r;  ++j){
			if (std::rand()%2 == 1 && re[i][j] == 0){
				int weight = (std::rand()%10) + 2;
				re[i][j] = weight;
				re[j][i] = weight;
			}
		}
	}


	for (int i = 0; i < r; ++i){
		for (int j = i+1; j < r;  ++j){
			if (re[i][j] > 1){
				g.add_edge(i,j,re[i][j]);
			}
		}
	}

	weighted_graph<int> t = g.mst();

	for (auto e : tree_edges){
		TS_ASSERT(t.are_adjacent(e.first, e.second));
	}

	TS_ASSERT_EQUALS(t.num_vertices(), r);
	TS_ASSERT_EQUALS(t.num_edges(), r-1);
	TS_ASSERT_EQUALS(t.total_weight(), r-1);
	//TS_ASSERT(is_connected(t));
}


void testWeightedDegree() {	

	weighted_graph<int> g;
	int r = std::rand()%20;

	int re[r][r];

	for (int i = 0; i < r; ++i){
		for (int  j = 0; j < r; ++j){
			re[i][j] = 0;
		}
	}

	for (int i = 1; i < r; ++i){
		for (int j = i+1; j < r;  ++j){
			if (std::rand()%2 == 1){
				int weight = (std::rand()%10) + 1;
				re[i][j] = weight;
				re[j][i] = weight;
			}
		}
	}

	for (int i = 0; i < r; ++i){
		g.add_vertex(i);
	}

	for (int i = 0; i < r; ++i){
		for (int j = i+1; j < r;  ++j){
			if (re[i][j] > 0){
				g.add_edge(i,j,re[i][j]);
			}
		}
	}

	for (int i = 0; i < r; ++i){
		int total_edge_weight = 0;
		for (int j = 0; j < r;  ++j){
			total_edge_weight += re[i][j];
		}
		TS_ASSERT_EQUALS(g.weighted_degree(i), total_edge_weight);
	}

}

void testAddVertex(){

	weighted_graph<int> g;
	int r = std::rand()%20;
	for (int i = 0; i < r; ++i) g.add_vertex(i);

	TS_ASSERT_EQUALS(g.num_vertices(),r);		
}

void testAddEdge(){

	weighted_graph<int> g;
	int r = std::rand()%20;
	int m = 0;

	int re[r][r];

	for (int i = 0; i < r; ++i){
		for (int  j = 0; j < r; ++j){
			re[i][j] = 0;
		}
	}

	for (int i = 0; i < r; ++i){
		for (int j = i+1; j < r;  ++j){
			if (std::rand()%2 == 1){
				int weight = (std::rand()%10) + 1;
				re[i][j] = weight;
				re[j][i] = weight;
				m++;
			}
		}
	}

	for (int i = 0; i < r; ++i){
		g.add_vertex(i);
	}

	for (int i = 0; i < r; ++i){
		for (int j = i+1; j < r;  ++j){
			if (re[i][j] > 0){
				g.add_edge(i,j,re[i][j]);
			}
		}
	}

	TS_ASSERT_EQUALS(g.num_edges(), m);

}

void testGraphIterator(){

	weighted_graph<int> g;
	int r = std::rand()%20;

	int re[r][r];

	for (int i = 0; i < r; ++i){
		for (int  j = 0; j < r; ++j){
			re[i][j] = 0;
		}
	}

	for (int i = 1; i < r; ++i){
		for (int j = i+1; j < r;  ++j){
			if (std::rand()%2 == 1){
				int weight = (std::rand()%10) + 1;
				re[i][j] = weight;
				re[j][i] = weight;
			}
		}
	}

	for (int i = 0; i < r; ++i){
		g.add_vertex(i);
	}

	for (int i = 0; i < r; ++i){
		for (int j = i+1; j < r;  ++j){
			if (re[i][j] > 0){
				g.add_edge(i,j,re[i][j]);
			}
		}
	}		

	std::unordered_set<int> it_vertices;
	for (auto v : g){
		it_vertices.insert(v);
	}

	TS_ASSERT_EQUALS(it_vertices.size(), g.num_vertices());

	for (auto u : g.get_vertices()){
		TS_ASSERT_LESS_THAN(0, it_vertices.count(u));
	}
}

void testNeighbourIterator(){

	weighted_graph<int> g;
	int r = std::rand()%20 + 1;

	int re[r][r];

	for (int i = 0; i < r; ++i){
		for (int  j = 0; j < r; ++j){
			re[i][j] = 0;
		}
	}

	for (int i = 1; i < r; ++i){
		for (int j = i+1; j < r;  ++j){
			if (std::rand()%2 == 1){
				int weight = (std::rand()%10) + 1;
				re[i][j] = weight;
				re[j][i] = weight;
			}
		}
	}

	for (int i = 0; i < r; ++i){
		g.add_vertex(i);
	}

	for (int i = 0; i < r; ++i){
		for (int j = i+1; j < r;  ++j){
			if (re[i][j] > 0){
				g.add_edge(i,j,re[i][j]);
			}
		}
	}

	for (auto u = 0; u < r; u++){

		std::unordered_set<int> it_neighbours;
		for (auto n = g.neighbours_begin(u); n != g.neighbours_end(u); ++n){
			it_neighbours.insert(n->first);
		}

		int u_degree = 0;
		for (int i = 0; i < r; i++){
			if (re[u][i] > 0) u_degree++;
		}

		TS_ASSERT_EQUALS(it_neighbours.size(), u_degree);
		for (auto v : g.get_neighbours(u)){
			TS_ASSERT_LESS_THAN(0, it_neighbours.count(v));
		}

	}

}

int main(){
	
	//std::cout << "testGraphIterator\n";testGraphIterator();
	//std::cout << "MST\n";testMST();
	std::cout << "testNeighbourIterator\n";testNeighbourIterator();
	//std::cout << "testAddVertex\n";testAddVertex();
	//std::cout << "testAddEdge\n";testAddEdge();
	//std::cout << "testGetEdgeWeight\n";testGetEdgeWeight();
	//std::cout << "testDegree\n";testDegree();
	//std::cout << "testWeightedDegree\n";testWeightedDegree(); //crashes here
	//std::cout << "testTotalWeight\n";testTotalWeight();
	//std::cout << "testGetVertices\n";testGetVertices();
	//std::cout << "testGetNeighbours\n";	testGetNeighbours();	
	
	//system("pause");
}

