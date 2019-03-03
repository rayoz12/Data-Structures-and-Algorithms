#include <iostream>
#include "weighted_graph.hpp"
#include "graph_algorithms.cpp"
//#include "tests/test_helper.cpp"


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

template <typename vertex>
unsigned component_index(const weighted_graph<vertex>& graph, const std::vector<std::vector<int>>& components){

	for (int i = 0; i < components.size(); ++i){
		if (graph.has_vertex(*components[i].begin())){
			return i;
		}
	}
	
	return -1;

}

template <typename vertex>
std::map<vertex, int> compute_tree_distances(const weighted_graph<vertex>& t, const vertex& start_vertex){

	std::map<vertex, int> distances;
	
	distances[start_vertex] = 0;
	auto bfs_order = breadth_first(t, start_vertex);
	
	auto parent = bfs_order.begin();
	auto child = bfs_order.begin();
	++child;
	
	while (child != bfs_order.end()){
		if (t.are_adjacent(*child, *parent)){ 
			distances[*child] = distances[*parent] + t.get_edge_weight(*child, *parent);
			++child;
		}
		else {
			++parent;
		}
	}
	
	return distances;

}

template <typename vertex_t>
struct bc_vertex{
	vertex_t v;
	bool isCut;
	unsigned _id;
	
	bc_vertex(vertex_t vertex, bool is_cut, unsigned id) : v(vertex), isCut(is_cut), _id(id){}
	
	inline bool operator==(const bc_vertex<vertex_t>& other) const {
	    return other.v == v && _id == other._id;
    }
};

namespace std {
    template <typename vertex>
    struct hash<bc_vertex<vertex>> {
        inline size_t operator()(const bc_vertex<vertex>& x) const {
            return x.v.size() + 1;
        }
    };
}

template<typename vertex>
weighted_graph<bc_vertex<vertex>> random_block_cut_tree(const std::vector<vertex>& components){

	weighted_graph<bc_vertex<vertex>> bc_tree;
	std::vector<bc_vertex<vertex>> cut_vertices;
	std::vector<bc_vertex<vertex>> block_vertices;
	
	size_t count = 0;
	size_t cut_vertex_count = 0;

	while (count < components.size()){

		auto next = bc_vertex<vertex>(components[count], false, count);
		bc_tree.add_vertex(next);			
		++count;	

		if (block_vertices.size() > 0){

			if (std::rand()%2 && cut_vertex_count > 0){
				//use existing cut vertex
				bc_tree.add_edge(cut_vertices[std::rand()%cut_vertices.size()], next, 1);
			}
			else {
				//add new cut vertex
				auto cut_vertex = bc_vertex<vertex>(vertex(), true, cut_vertex_count);
				bc_tree.add_vertex(cut_vertex);
				bc_tree.add_edge(cut_vertex, next, 1);
				cut_vertices.push_back(cut_vertex);
				++cut_vertex_count;
				bc_tree.add_edge(cut_vertex, block_vertices[std::rand()%block_vertices.size()], 1);
				
			}

		}
		block_vertices.push_back(next);

	}
	
	return bc_tree;

}

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

void testIsConnnectedEmptyGraph(){

	weighted_graph<int> g;

	TS_ASSERT(is_connected(g));

}

void testIsConnectedConnectedGraph(){

	weighted_graph<int> g;

	auto r = (std::rand()%20) + 5;

	for (auto i = 0; i < r; ++i){
		g.add_vertex(i);
	}

	std::vector<int> vertices(g.begin(), g.end());

	auto min_edges = random_tree(vertices);

	for (auto e : min_edges){
		g.add_edge(e.first, e.second, (std::rand()%10) + 1);
	}

	auto extra_edges = random_tree(vertices);

	for (auto e : extra_edges){
		if (!g.are_adjacent(e.first, e.second)){
			g.add_edge(e.first, e.second, (std::rand()%10) + 1);
		}
	}

	TS_ASSERT(is_connected(g));

}

void testIsConnectedSimpleDisconnectedGraph(){

	weighted_graph<int> g;

	int r = std::rand()%20 + 2;

	for (int i = 0; i < r; ++i){
		g.add_vertex(i);
	}

	TS_ASSERT(!is_connected(g));

}

void testIsConnectedDisconnectedGraph(){

	weighted_graph<int> g;

	int r = std::rand()%10 + 2;
	int s = std::rand()%10 + 2;
	int t = std::rand()%10 + 2;

	int i = 0;
	std::vector<int> c1;
	std::vector<int> c2;
	std::vector<int> c3;

	for (; i < r; ++i){
		c1.push_back(i);
		g.add_vertex(i);
	}

	for (; i < s + r; ++i){
		c2.push_back(i);
		g.add_vertex(i);
	}

	for (; i < t + s + r; ++i){
		c3.push_back(i);
		g.add_vertex(i);
	}

	auto e1 = random_tree(c1);
	auto e2 = random_tree(c2);
	auto e3 = random_tree(c3);

	for (auto e : e1){
		g.add_edge(e.first, e.second, (std::rand()%10) + 1);
	}

	for (auto e : e2){
		g.add_edge(e.first, e.second, (std::rand()%10) + 1);
	} 

	for (auto e : e3){
		g.add_edge(e.first, e.second, (std::rand()%10) + 1);
	}

	TS_ASSERT(!is_connected(g));

}

void testConnectedComponentsEmptyGraph(){

	weighted_graph<int> g;
	TS_ASSERT_EQUALS(connected_components(g).size(), 0);

}

void testConnectedComponentsSimpleDisconnectedGraph(){

	weighted_graph<int> g;

	int r = std::rand()%20 + 1;

	for (int i = 0; i < r; ++i){
		g.add_vertex(i);
	}

	std::vector<bool> found(r, false);

	for (auto h : connected_components(g)){
		found[*h.begin()] = true;
	}

	TS_ASSERT(connected_components(g).size() == g.num_vertices());

	for (auto i = 0; i < r; ++i){
		TS_ASSERT(found[i]); //Vertex i found.
	}

}

void testConnectedComponentsConnectedGraph(){

	weighted_graph<int> g;

	auto r = (std::rand()%20) + 5;

	for (auto i = 0; i < r; ++i){
		g.add_vertex(i);
	}

	std::vector<int> vertices(g.begin(), g.end());

	auto min_edges = random_tree(vertices);

	for (auto e : min_edges){
		g.add_edge(e.first, e.second, (std::rand()%10) + 1);
	}

	auto extra_edges = random_tree(vertices);

	for (auto e : extra_edges){
		if (!g.are_adjacent(e.first, e.second)){
			g.add_edge(e.first, e.second, (std::rand()%10) + 1);
		}
	}

	auto computed_components = connected_components(g);

	TS_ASSERT_EQUALS(computed_components.size(), 1);
	auto component = computed_components[0];
	TS_ASSERT_EQUALS(component.num_vertices(), g.num_vertices());

	for (auto v : g) TS_ASSERT(component.has_vertex(v));
	for (auto v : component) TS_ASSERT(g.has_vertex(v));

	TS_ASSERT_EQUALS(component.num_edges(), g.num_edges());
	for (auto e : min_edges) TS_ASSERT(component.are_adjacent(e.first, e.second));

}
/*
void testConnectedComponentsDisconnectedGraph(){

	weighted_graph<int> g;
	int vert_total = 0;

	std::vector<std::vector<int>> components;

	auto num_components = std::rand()%10 + 2;

	for (auto i = 0; i < num_components; ++i){

		std::vector<int> component;
		auto r = std::rand()%5 + 2;
		for (auto j = vert_total; j < vert_total + r; ++j){
			component.push_back(j);
		}
		vert_total += component.size();
		components.push_back(component);

	}

	std::vector<std::vector<std::pair<int, int>>> edges;
	for (auto i = 0; i < num_components; ++i){

		std::vector<std::pair<int, int>> component_edges = random_tree(components[i]);
		edges.push_back(component_edges);

	}

	for (auto c : components){
		for (auto v : c){
			g.add_vertex(v);
		}
	}

	for (auto c_e : edges){
		for (auto e : c_e){
			g.add_edge(e.first, e.second, std::rand()%5 + 1);
		}
	}

	auto computed_components = connected_components(g);

	for (auto h : computed_components){
		auto source_component = component_index(h, components);
		for (auto v : components[source_component]){
			TS_ASSERT(h.has_vertex(v));
		}

		TS_ASSERT_EQUALS(h.num_vertices(), components[source_component].size());

		for (auto e : edges[source_component]){
			TS_ASSERT(h.are_adjacent(e.first, e.second));
		}

		TS_ASSERT_EQUALS(h.num_edges(), edges[source_component].size());

	}

}

void testDijkstrasEmptyGraph(){

	weighted_graph<int> g;
	auto distances = dijkstras(g, 0); //The 0 shouldn't really do anything here, it's just standing in for the parameter.
	TS_ASSERT(distances.empty());

}
*/
void testDijkstras(){

	weighted_graph<int> g;

	auto r = (std::rand()%20) + 5;

	for (auto i = 0; i < r; ++i){
		g.add_vertex(i);
	}

	std::vector<int> vertices(g.begin(), g.end());

	auto min_edges = random_tree(vertices);

	for (auto e : min_edges){
		g.add_edge(e.first, e.second, (std::rand()%10) + 1);
	}

	auto shortest_path_tree_weight = g.total_weight();

	auto start_vertex = vertices[std::rand()%vertices.size()];

	auto actual_distances = compute_tree_distances(g, start_vertex);

	auto extra_edges = random_tree(vertices);

	for (auto e : extra_edges){
		if (!g.are_adjacent(e.first, e.second)){
			g.add_edge(e.first, e.second, (std::rand()%10) + shortest_path_tree_weight + 1);
		}
	}

	auto computed_shortest_paths = dijkstras(g, start_vertex);

	//TS_ASSERT_EQUALS(actual_distances, computed_shortest_paths);
	std::cout << "Actual Distances: \n";
	for(auto elem : actual_distances)
	{
	   std::cout << elem.first << " " << elem.second << "\n";
	}
	std::cout << "computed Distances: \n";
	for(auto elem : computed_shortest_paths)
	{
	   std::cout << elem.first << " " << elem.second << "\n";
	}

	for (auto p : actual_distances){
		TS_ASSERT_EQUALS(computed_shortest_paths[p.first], p.second);
	}

}

void testArticulationPointsEmptyGraph(){

	weighted_graph<int> g;

	auto points = articulation_points(g);

	TS_ASSERT(points.empty());
}

void testArticulationPoints(){

	weighted_graph<int> g;
	auto vert_total = 0;

	std::vector<std::vector<int>> components;

	auto num_components = std::rand()%10 + 2;	
	for (auto i = 0; i < num_components; ++i){

		std::vector<int> component;
		auto r = std::rand()%5 + 3;
		for (auto j = vert_total; j < vert_total + r; ++j){
			component.push_back(j);
		}
		vert_total += component.size();
		components.push_back(component);

	}

	for (auto c : components){
		for (auto v : c) {
			g.add_vertex(v);
		}

		for (int i = 0; i < c.size(); ++i) g.add_edge(c[i], c[(i+1)%c.size()], std::rand()%10 + 1);
		auto extra_edges = random_tree(c);
		for (auto e : extra_edges){
			if (!g.are_adjacent(e.first, e.second)) g.add_edge(e.first, e.second, std::rand()%10 + 1);
		}
	}


	auto block_cut_tree = random_block_cut_tree(components);
	std::unordered_set<int> cuts;

	for (auto v : block_cut_tree){
		if (v.isCut){
			g.add_vertex(vert_total);
			cuts.insert(vert_total);
			std::vector<int> connections;

			for (auto n = block_cut_tree.neighbours_begin(v); n != block_cut_tree.neighbours_end(v); ++n){
				auto component = n->first.v;
				auto rand1 = std::rand()%component.size();
				auto rand2 = rand1;
				while (rand2 == rand1) rand2 = std::rand()%component.size();

				connections.push_back(component[rand1]);
				connections.push_back(component[rand2]);
			}

			for (auto u : connections) g.add_edge(u, vert_total, std::rand()%10 + 1);

			++vert_total;
		}
	}

	auto computed_articulation_points = articulation_points(g);

	TS_ASSERT_EQUALS(computed_articulation_points.size(), cuts.size());
	for (auto v : computed_articulation_points){
		TS_ASSERT(cuts.count(v) > 0);
	}

	for (auto v : cuts){
		bool articulation_point_found = false;
		for (auto u : computed_articulation_points){
			if (u == v) articulation_point_found = true;
		}
		TS_ASSERT(articulation_point_found);
	}
}


int main(){
	//testIsConnnectedEmptyGraph();
	//std::cout<<"testConnectedComponentsConnectedGraph\n"; testConnectedComponentsConnectedGraph(); std::cout << "\n";
	std::cout<<"testDijkstras\n"; testDijkstras(); std::cout << "\n";
	//std::cout<<"testArticulationPointsEmptyGraph\n"; testArticulationPointsEmptyGraph(); std::cout << "\n";
	//std::cout<<"testArticulationPoints\n"; testArticulationPoints(); std::cout << "\n";
}
