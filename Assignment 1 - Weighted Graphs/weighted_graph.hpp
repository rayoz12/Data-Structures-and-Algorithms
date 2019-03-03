
#ifndef WEIGHTED_GRAPH_H
#define WEIGHTED_GRAPH_H

//A large selection of data structures from the standard
//library. You need not feel compelled to use them all,
//but as you can't add any, they're all here just in case.
#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <array>
#include <list>
#include <forward_list>
#include <deque>
#include <map>



template <typename vertex>
class weighted_graph {

	private:
	//You will need to add some data members here
	//to actually r epresent the graph internally,
	//and keep track of whatever you need to.
	
	//std::vector<std::vector<vertex> > adj_list;
	std::map<vertex, std::map<vertex, int> > vertices; //holds the data that represents the graph
	int edgeCount = 0;//total edges
	int totalWeight = 0;//totalweight in the graph, could be determined at runtime but this seems like a more performant solution.
	void insertionSort(std::vector<vertex>& arr);//used in DFT and BFT
	
	
	//The graph_iterator class provides an iterator
	//over the vertices of the graph.
	//This is one of the harder parts, so if you're
	//not too comfortable with C++ leave this for last.
	//If you are, there are many ways of doing this,
	//as long as it passes the tests, it's okay.
	
	class graph_iterator {
		
		private:
		
		//You may need data members here.
		int position = 0;
		vertex* pointerPosition;
		std::vector<vertex> vertexVector;
				
		public:
			graph_iterator(const weighted_graph &);
			graph_iterator(const weighted_graph &, size_t);
			~graph_iterator();
			graph_iterator operator=(const graph_iterator&);
			bool operator==(const graph_iterator&) const;
			bool operator!=(const graph_iterator&) const;
			graph_iterator operator++();
			graph_iterator operator++(int);
			const vertex operator*();
			const vertex* operator->();
	};
	
	
	//The neighbour_iterator class provides an iterator
	//over the neighbours of a given vertex. This is
	//probably harder (conceptually) than the graph_iterator.
	//Unless you know how iterators work.
	class neighbour_iterator {
			
		private:
		
		//You may need data members here.
		int position = 0;
		std::vector<std::pair<vertex, int> > vertexVector; 
		vertex vertexSelected;
		
		
		public:
			
			//neighbour_iterator(const neighbour_iterator&);
			neighbour_iterator(const weighted_graph &, const vertex&);
			neighbour_iterator(const weighted_graph &, const vertex&, size_t);
			~neighbour_iterator();
			neighbour_iterator operator=(const neighbour_iterator& it);
			bool operator==(const neighbour_iterator&) const;
			bool operator!=(const neighbour_iterator&) const;
			neighbour_iterator operator++();
			neighbour_iterator operator++(int);			
			const std::pair<vertex, int> operator*();
			const std::pair<vertex, int>* operator->();
	};
	
	public:
	
	
	weighted_graph(); //A constructor for weighted_graph. It should start empty.
	~weighted_graph(); //A destructor. Depending on how you do things, this may
					   //not be necessary.
					   
	
	
	bool are_adjacent(const vertex&, const vertex&) const; //Returns true if the two vertices are
														   //adjacent, false otherwise.
	bool has_vertex(const vertex&) const; //Returns true if the passed in vertex is 
										  //a vertex of the graph, false otherwise.
	
	void add_vertex(const vertex&); //Adds the passed in vertex to the graph (with no edges).
	void add_edge(const vertex&, const vertex&, const int&); //Adds an edge between the two vertices
															 //with the given weight (as an int).
	
	void remove_vertex(const vertex&); //Removes the given vertex. Should also clear any incident edges.
	void remove_edge(const vertex&, const vertex&); //Removes the edge between the two vertices, if it exists.
	void set_edge_weight(const vertex&, const vertex&, const int&); //Changes the edge weight between the two
																	//vertices to the new weight (the int).
	
	int get_edge_weight(const vertex&, const vertex&) const; //Returns the weight on the edge between the two vertices.
	int degree(const vertex&) const; //Returns the degree of the vertex.
	int weighted_degree(const vertex&); //Returns the sum of the weights on all the edges incident to the vertex.
	int num_vertices() const; //Returns the total number of vertices in the graph.
	int num_edges() const; //Returns the total number of edges in the graph (just the count, not the weight).
	int total_weight(); //Returns the sum of all the edge weights in the graph.
	
	std::vector<vertex> get_vertices() const; //Returns a vector containing all the vertices.
	std::vector<vertex> get_neighbours(const vertex&) const; //Returns a vector containing the neighbours of the given vertex.

	//typedef typename std::vector<vertex>::iterator graph_iterator;
	weighted_graph<vertex>::graph_iterator begin(); //Returns a graph_iterator pointing to the start of the vertex set.
	weighted_graph<vertex>::graph_iterator end(); //Returns a graph_iterator pointing to one-past-the-end of the vertex set.
	
		
	neighbour_iterator neighbours_begin(const vertex&); //Returns a neighbour_iterator pointing to the start
														//of the neighbour set for the given vertex.
	neighbour_iterator neighbours_end(const vertex&); //Returns a neighbour_iterator pointing to one-past-the-end
													  //of the neighbour set for the given vertex.

	std::vector<vertex> depth_first(const vertex&); //Returns the vertices of the graph in the order they
													//are visited in by a depth-first traversal starting at
													//the given vertex.
	std::vector<vertex> breadth_first(const vertex&); //Returns the vertices of the graph in the order they
													  //are visisted in by a breadth-first traversal starting
													  //at the given vertex.
	
	weighted_graph<vertex> mst(); //Returns a minimum spanning tree of the graph.
	
};

//Define all your methods down here (or move them up into the header, but be careful you don't double up).
//Although these are just the same names copied from above, you may find a few more clues in the full
//method headers. Note also that C++ is sensitive to the order you declare and define things in - you
//have to have it available before you use it.

template <typename vertex> weighted_graph<vertex>::graph_iterator::graph_iterator(const weighted_graph & g){
	vertexVector = g.get_vertices();
	pointerPosition = &vertexVector[0];
	position = 0;
}
template <typename vertex> weighted_graph<vertex>::graph_iterator::graph_iterator(const weighted_graph & g, size_t start_pos){
	vertexVector = g.get_vertices();
	pointerPosition = &vertexVector[(int)start_pos];
	position = start_pos;
}
template <typename vertex> weighted_graph<vertex>::graph_iterator::~graph_iterator(){}

template <typename vertex> void weighted_graph<vertex>::insertionSort(std::vector<vertex>& arr) {
	int arrSize = arr.size();
	for (int i = 1 ; i < arrSize; i++) {
		int x = arr[i];
		int pos = i-1;
		while (pos >= 0 && arr[pos] > x){
			arr[pos + 1] = arr[pos];
			pos--;
		}
		arr[pos + 1] = x;
	}
}

template <typename vertex> typename weighted_graph<vertex>::graph_iterator weighted_graph<vertex>::graph_iterator::operator=(const graph_iterator& it){ 
	position = it.postion;
	return *this;
}
template <typename vertex> bool weighted_graph<vertex>::graph_iterator::operator==(const graph_iterator& it) const { 
	return position == it.position;
}
template <typename vertex> bool weighted_graph<vertex>::graph_iterator::operator!=(const graph_iterator& it) const { 
	return position != it.position;
}
template <typename vertex> typename weighted_graph<vertex>::graph_iterator weighted_graph<vertex>::graph_iterator::operator++(){ 
	++position;
	return *this;
}
template <typename vertex> typename weighted_graph<vertex>::graph_iterator weighted_graph<vertex>::graph_iterator::operator++(int){ 
	//fix this by creating a new constructor
	position++;
	return *this;
}
template <typename vertex> const vertex weighted_graph<vertex>::graph_iterator::operator*(){ 
	return vertexVector[position];
}
template <typename vertex> const vertex* weighted_graph<vertex>::graph_iterator::operator->(){ 
	return vertexVector[position];
}

template <typename vertex> weighted_graph<vertex>::neighbour_iterator::neighbour_iterator(const weighted_graph & g, const vertex& u) {
	//vertexVector = g.get_neighbours(u);
	for (auto v : g.get_neighbours(u)) {
		int weight = g.get_edge_weight(u, v);
		vertexVector.push_back(std::pair<vertex, int> (v, weight));
	}
	//pointerPosition = &vertexVector[0];
	position = 0;
	vertexSelected = u;
}
template <typename vertex> weighted_graph<vertex>::neighbour_iterator::neighbour_iterator(const weighted_graph & g, const vertex& u, size_t start_pos) {
	//vertexVector = g.get_neighbours(u);
	for (auto v : g.get_neighbours(u)) {
		int weight = g.get_edge_weight(u, v);
		vertexVector.push_back(std::pair<vertex, int> (v, weight));
	}
	//pointerPosition = &vertexVector[(int) start_pos];
	position = start_pos;
	vertexSelected = u;
}
template <typename vertex> weighted_graph<vertex>::neighbour_iterator::~neighbour_iterator() {}
template <typename vertex> typename weighted_graph<vertex>::neighbour_iterator weighted_graph<vertex>::neighbour_iterator::operator=(const neighbour_iterator& it) { 
	position = it.postion;
	return *this;
}
template <typename vertex> bool weighted_graph<vertex>::neighbour_iterator::operator==(const neighbour_iterator& it) const { 
	return position == it.position;
}
template <typename vertex> bool weighted_graph<vertex>::neighbour_iterator::operator!=(const neighbour_iterator& it) const { 
	return position != it.position;
}
template <typename vertex> typename weighted_graph<vertex>::neighbour_iterator weighted_graph<vertex>::neighbour_iterator::operator++() {
	++position;
	return *this;
}
template <typename vertex> typename weighted_graph<vertex>::neighbour_iterator weighted_graph<vertex>::neighbour_iterator::operator++(int){ 
	//fix this by creating a new constructor
	position++;
	return *this;
}			
template <typename vertex> const std::pair<vertex, int> weighted_graph<vertex>::neighbour_iterator::operator*(){ 
	return vertexVector[position];
}
template <typename vertex> const std::pair<vertex, int>* weighted_graph<vertex>::neighbour_iterator::operator->(){ 
	return new std::pair<vertex, int> (vertexVector[position].first, vertexVector[position].second);
}

template <typename vertex> typename weighted_graph<vertex>::graph_iterator weighted_graph<vertex>::begin() {
	//return this->get_vertices().begin();
	return graph_iterator(*this);
}
template <typename vertex> typename weighted_graph<vertex>::graph_iterator weighted_graph<vertex>::end() {
	//return this->get_vertices().end();
	return graph_iterator(*this, vertices.size());
}

template <typename vertex>	typename weighted_graph<vertex>::neighbour_iterator weighted_graph<vertex>::neighbours_begin(const vertex& u) {
	return neighbour_iterator(*this, u);
}

template <typename vertex>	typename weighted_graph<vertex>::neighbour_iterator weighted_graph<vertex>::neighbours_end(const vertex& u) {
	auto neighbours = get_neighbours(u);
	return neighbour_iterator(*this, u, neighbours.size());
}

template <typename vertex> weighted_graph<vertex>::weighted_graph(){
	//int vertexLength = 10;
	//adj_list.assign(vertexLength, std::vector<vertex>(vertexLength, 0));
}

template <typename vertex> weighted_graph<vertex>::~weighted_graph(){
	
}

template <typename vertex> bool weighted_graph<vertex>::has_vertex(const vertex& u) const {
	return vertices.count(u) != 0;
	//return false;
}
	
template <typename vertex> bool weighted_graph<vertex>::are_adjacent(const vertex& u, const vertex& v) const {
	if ( vertices.find(u) != vertices.end() ) {
		return vertices.at(u).find(v) != vertices.at(u).end();
	} else {
		return false;
	}
	//return false;
}

template <typename vertex> void weighted_graph<vertex>::add_vertex(const vertex& v) {
	//comparitor function
	std::map<vertex, int> newVert;
	vertices.insert(std::make_pair(v, newVert));
}

template <typename vertex> void weighted_graph<vertex>::add_edge(const vertex& u, const vertex& v, const int& weight) {
	vertices[u][v] = weight; 
	vertices[v][u] = weight;
	edgeCount++;
	totalWeight += weight;
}

template <typename vertex> void weighted_graph<vertex>::remove_vertex(const vertex& u) {
	vertices.erase(u);
	//remove from other vertecies too
	for (auto it : vertices) {
		auto vertMap = it.second;
		if (vertMap.find(u) != vertMap.end()) {
			//there is an existing key here, remove it.
			vertMap.erase(u);
		}
	}
}

template <typename vertex> void weighted_graph<vertex>::remove_edge(const vertex& u, const vertex& v) {
	totalWeight -= vertices[u][v];
	vertices[u].erase(v);
	vertices[v].erase(u);
	edgeCount--;
}

template <typename vertex> void weighted_graph<vertex>::set_edge_weight(const vertex& u, const vertex& v, const int& weight) {
	totalWeight -= vertices[u][v];
	vertices[u][v] = weight;
	vertices[v][u] = weight;
	totalWeight += weight;
}

template <typename vertex> int weighted_graph<vertex>::get_edge_weight(const vertex& u, const vertex& v) const {
	return vertices.at(u).at(v);
}

template <typename vertex> int weighted_graph<vertex>::degree(const vertex& u) const {
	return vertices.at(u).size();
	//return 0;
}

template <typename vertex> int weighted_graph<vertex>::weighted_degree(const vertex& u) {
	//sum of all weights on a vertex
	int weightSum = 0;
	auto vertexNode = vertices.at(u);
	for (auto it = vertexNode.begin(); it != vertexNode.end(); it++)
		weightSum += it->second;
	return weightSum;
	//return 0;
}

template <typename vertex> int weighted_graph<vertex>::num_vertices() const {
	return vertices.size();
	//return 0;
}

template <typename vertex> int weighted_graph<vertex>::num_edges() const {
	return edgeCount;
	//return 0;
}

template <typename vertex> int weighted_graph<vertex>::total_weight() {
	return totalWeight;
	//return 0;
}
	
template <typename vertex>	std::vector<vertex> weighted_graph<vertex>::get_vertices() const {
	std::vector<vertex> verticiesList;
	for (auto it : vertices)
		verticiesList.push_back(it.first);
	return verticiesList;
	//return std::vector<vertex>();
}

template <typename vertex>	std::vector<vertex> weighted_graph<vertex>::get_neighbours(const vertex& u) const {
	std::vector<vertex> verticiesList;
	auto vertexNode = vertices.at(u);
	for (auto it : vertexNode)
		verticiesList.push_back(it.first);
	return verticiesList;
	//return std::vector<vertex>();
}


template <typename vertex> std::vector<vertex> weighted_graph<vertex>::depth_first(const vertex& start_vertex){
	std::stack<vertex> unprocessed;
	std::vector<vertex> visited;
	unprocessed.push(start_vertex);
	int vertexSize = num_vertices();
	std::vector<bool> marked(vertexSize, false);
	
	//get all neighbours then sort it. sorting(https://edstem.org/courses/1735/discussion/82719?comment=196682) we can use comparison operators
	
	while (!unprocessed.empty()) {
		int u = unprocessed.top();
		unprocessed.pop();
		if (!marked[u]) {
			visited.push_back(u);
			marked[u] = true;
			auto vertexNode = vertices.at(u);
			std::vector<vertex> temp;
			for (auto it = vertexNode.begin(); it != vertexNode.end(); it++)
				temp.push_back(it->first);
			insertionSort(temp); //sort used here to get the lowest label vertex first.
			for (auto i = temp.rbegin(); i < temp.rend(); i++)
				unprocessed.push(*i);
				
			//unprocessed.push(it->first);
		}
	}
	//std::vector<std::pair<vertex, int> >
	return visited;
}


template <typename vertex> std::vector<vertex> weighted_graph<vertex>::breadth_first(const vertex& start_vertex){
	std::queue<int> unprocessed;
	std::vector<int> visited;
	unprocessed.push(start_vertex);
	int vertexSize = num_vertices();
	std::vector<bool> marked(vertexSize, false);
	while (!unprocessed.empty()) {
		int u = unprocessed.front();
		unprocessed.pop();
		if (!marked[u]) {
			visited.push_back(u);
			marked[u] = true;
			auto vertexNode = vertices.at(u);
			std::vector<vertex> temp;
			for (auto it = vertexNode.begin(); it != vertexNode.end(); it++)
				temp.push_back(it->first);
			insertionSort(temp); //sort used here to get the lowest label vertex first.
			for (auto i = temp.begin(); i < temp.end(); i++)
				unprocessed.push(*i);
		}
	}
	
	return visited;	
	//return std::vector<vertex>();
}
	
template <typename vertex>	weighted_graph<vertex> weighted_graph<vertex>::mst() {
	//https://edstem.org/courses/1735/discussion/85604
	weighted_graph<vertex> mst;
	
	std::map<vertex, std::map<vertex, int> > explored;
	
	std::map<vertex, bool> marked; //marks verticies already in the graph.
	//initialise marked
	for (auto vert : get_vertices()) {
		marked[vert] = false;
	}
	//select first vertex from map;
	auto mapIt = vertices.begin();
	auto vertexSelected = mapIt->first;
	
	mst.add_vertex(mapIt->first);
	
	//make a map of a map to hold the parts of the map we've explored and edges discovered.
	//use the map to find the next minimum edges
	
	while (mst.num_vertices() < num_vertices()) {
		//get all reachable vertex and edge weights.
		
		//vertexSelected = *(mapIt);
		//change vertexSelected each iteration.
		//auto connectedTo = get_neighbours(mapIt->first);
		std::map<vertex, int> newVert;
		explored.insert(std::make_pair(vertexSelected, newVert));
		marked[vertexSelected] = true;
		//assign weights of this vertex
		for (auto v : vertices[vertexSelected]) {
		    explored[vertexSelected][v.first] = v.second;
		}
		/*
		for (auto v : connectedTo) {
			explored[vertexSelected][v] = vertices[vertexSelected][v];
		}
		*/
		//get the cheapest edge to a vertex we haven't been to yet.
		vertex minVertexA = vertexSelected;
		vertex minVertexB = vertexSelected;
		int minWeight = 999999;
		for (auto &exploredVert: explored) {
			for (auto &edge : exploredVert.second) {
				if (!mst.has_vertex(edge.first)) {
					if (edge.second < minWeight) {
						minWeight = edge.second;
						minVertexB = edge.first;
						minVertexA = exploredVert.first;
					}
				}
			}
		}
		
		
		//add this edge to mst and update vertexSelected to the new vertex added for the next while .
		mst.add_vertex(minVertexB);
		mst.add_edge(minVertexA, minVertexB, minWeight);
		
		vertexSelected = minVertexB;
		
	}
	return mst;
	
}


#endif
