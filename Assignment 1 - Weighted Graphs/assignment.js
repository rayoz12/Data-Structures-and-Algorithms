class weightedGraph {

    constructor() {
        this.adj_list = [];
    }

    add_vertex(v) {
        this.adj_list[v] = [];
    }

    add_edge(u, v, weight) {
        this.adj_list[v][u] = weight;
        this.adj_list[u][v] = weight;
    }

    remove_vertex(u) {
        this.adj_list.splice(u, 1);
        //remove other vertex
    }

    remove_edge(u, v) {
        this.adj_list[u][v] = 0;
        this.adj_list[v][u] = 0;
    }

    set_edge_weight(u, v, weight) {
        this.adj_list[v][u] = weight;
        this.adj_list[u][v] = weight;
    }

    get_edge_weight(u, v) {
        return this.adj_list[v][u];
    }

    degree(u) {
        return this.adj_list[u].length;
    }

    num_vertices() {
        return this.adj_list.length;
    }
    
    has_vertex(u) {
        return !(this.adj_list[u] === undefined);
    }
}

function TS_ASSERT(a) {
    console.log("asserting A:", a);
}

function TS_ASSERT_EQUALS(a, b) {
    console.log("asserting equals A:", a, " and B:", b, ". Is equal: ", a == b);
}

function testAddVertex(){
		
    const g = new weightedGraph();
    r = Math.floor(Math.random() * 10)%20;
    for (i = 0; i < r; ++i) g.add_vertex(i);
    
    console.log(g.num_vertices(),r);		
}

function testHasVertex(){
		
    const g = new weightedGraph();
    r = Math.floor(Math.random() * 10)%20;
    for (i = 0; i < r; ++i) g.add_vertex(i);
    
    for (i = 0; i < r; i++){
        TS_ASSERT(g.has_vertex(i));
    }		
}

function testRemoveVertex(){
    
    const g = new weightedGraph();
    
    let r = Math.floor(Math.random() * 10)%20;
    let s = Math.floor(Math.random() * 10)%r;
    
    for (let i = 0; i < r; ++i) g.add_vertex(i);
    
    TS_ASSERT_EQUALS(g.num_vertices(),r);
    for (let i = 0; i < r; i++){
        TS_ASSERT(g.has_vertex(i));
    }
    
    for (let i = 0; i < s; ++i) g.remove_vertex(i);
    
    TS_ASSERT_EQUALS(g.num_vertices(), r-s);
    for (let i = 0; i < s; ++i){
        TS_ASSERT(!g.has_vertex(i));
    }
    for (let i = s; i < r; ++i){
        TS_ASSERT(g.has_vertex(i));
    }
    
}

//testAddVertex();
//testHasVertex();
testRemoveVertex();