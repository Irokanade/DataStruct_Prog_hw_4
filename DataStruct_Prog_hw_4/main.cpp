//
//  main.cpp
//  DataStruct_Prog_hw_4
//
//  Created by Michael Leong on 10/12/2022.
//

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#define ARRAYBUFFER 256

using namespace std;

class Node {
public:
    int vertex;
    class Node *next;
    
    //creating new node
    //without param default to zero
    Node() {
        this->vertex = 0;
        this->next = NULL;
    }
    
    Node(int v) {
        this->vertex = v;
        this->next = NULL;
    }
};

class Graph {
public:
    int numVertices;
    class Node **adjLists;
    
    //creating graph
    Graph(int vertices) {
        this->numVertices = vertices;
        this->adjLists = new Node*[vertices];
        
        for(int i = 0; i < vertices; i++) {
            this->adjLists[i] = NULL;
        }
    }
    
    void addEdge(int s, int d) {
        //add edge from s to d
        Node *newNode = new Node(d);
        newNode->next = this->adjLists[s];
        this->adjLists[s] = newNode;
        
        //add edge from d to s
        newNode = new Node(s);
        newNode->next = this->adjLists[d];
        this->adjLists[d] = newNode;
    }
    
    void printGraph() {
        for(int i = 0; i < this->numVertices; i++) {
            class Node *temp = this->adjLists[i];
            cout << "\n Vertex " << i << "\n: ";
            while (temp) {
                cout << temp->vertex << " -> ";
                temp = temp->next;
            }
            cout << '\n';
        }
    }
    
    void dfs(int v, bool visited[]) {
        class Node *tempList = this->adjLists[v];
        class Node *temp = tempList;
        
        visited[v] = true;
        
        while(temp != NULL) {
            int connectedVertex = temp->vertex;
            
            if(!visited[connectedVertex]) {
                dfs(connectedVertex, visited);
            }
            
            temp = temp->next;
        }
    }
    
    void dfsCycle(int u, int p, int colour[], int par[], int &cycleNumber) {
        //node is already visited
        if(colour[u] == 2) {
            return;
        }
        
        //partially visited node found (new cycle found)
        if(colour[u] == 1) {
            cycleNumber++;
            
            return;
        }
        
        //store parent of u
        par[u] = p;
        
        //mark as partially visited
        colour[u] = 1;
        class Node *tempList = this->adjLists[u];
        class Node *temp = tempList;
        while(temp != NULL) {
            int connectedVertex = temp->vertex;
            
            if(connectedVertex == par[u]) {
                temp = temp->next;
                continue;
            }
            
            //cout << connectedVertex << ' ';
            dfsCycle(connectedVertex, u, colour, par, cycleNumber);
            
            temp = temp->next;
            
        }
        
        //mark as fully visited
        colour[u] = 2;
    }
    
    int numberOfConnectedComponents() {
        //mark all the vertices as not visited
        bool* visited = new bool[numVertices];
        for (int i = 0; i < numVertices; i++) {
            visited[i] = false;
        }
        
        //number of connected components
        int count = 0;
        
        for (int i = 0; i < numVertices; i++) {
            if (visited[i] == false) {
                dfs(i, visited);
                count++;
            }
        }
        return count;
    }
    
    int numberOfSimpleCycles() {
        int colour[numVertices];
        int par[numVertices];
        
        //init tables
        for(int i = 0; i < numVertices; i++) {
            colour[i] = 0;
        }
        for(int i = 0; i < numVertices; i++) {
            par[i] = 0;
        }
        
        int cycleNumber = 0;
        
        for(int i = 0; i < numVertices; i++) {
            if(colour[i] == 0) {
                //cout << "\nstart: " << i << ' ';
                dfsCycle(i, 0, colour, par, cycleNumber);
                //cout << '\n';
            }
        }
        
        return cycleNumber;
    }
};

int main() {
    int numOfGraphs = 0;
    int n = 0; //number of nodes
    int m = 0; //number of inputs
    string nodeStringToCheck; //list of nodes to check in string form
    int checkNodeList[ARRAYBUFFER]; //list of nodes to check in array form
    
    string mystring;
    
    //using ifstream to open file
    ifstream myfile ("test.txt");
    
    if ( myfile.is_open() ) {
        myfile >> numOfGraphs;
        
        //cout << "repeat: " << numOfGraphs << '\n';
        
        class Graph *graphList[numOfGraphs];
        
        for(int i = 0; i < numOfGraphs; i++) {
            myfile >> n;
            myfile >> m;
            
            n++;
            
            graphList[i] = new Graph(n);
            
            for(int j = 0; j < m; j++) {
                int src = 0;
                int dst = 0;
                
                myfile >> src;
                myfile >> dst;
                
                graphList[i]->addEdge(src, dst);
            }
            
            getline(myfile, nodeStringToCheck); // skip a line
            getline(myfile, nodeStringToCheck); // list of nodes to check
            //cout << "nonce: " << nodeStringToCheck << '\n';
            
            //get nodes each separated by spacebar
            int checkNodeListIndex = 0;
            //using stringstream and redirection >> operator to parse values
            istringstream iss(nodeStringToCheck);
            while(iss >> checkNodeList[checkNodeListIndex]) {
                checkNodeListIndex++;
            }
            
            //test print
            /*cout << "node list to check: ";
            for(int i = 0; i < checkNodeListIndex; i++) {
                cout << checkNodeList[i] << ' ';
            }
            cout << '\n';*/
            
            //graphList[i]->printGraph();
            //minus 1 coz teachers graph start from index 1
            int connectedCompNum = graphList[i]->numberOfConnectedComponents()-1;
            int simpleCycleNum = graphList[i]->numberOfSimpleCycles();
            int nonSimpleCycleNum = connectedCompNum - simpleCycleNum;
            int numOfRingsToOpen = (nonSimpleCycleNum-1)*(nonSimpleCycleNum>0) + (simpleCycleNum);
            
            //cout << "number of connected components: " << connectedCompNum << '\n';
            //cout << "number of rings to open: " << numOfRingsToOpen << '\n';
            //cout << "number of simple cycles: " << simpleCycleNum << '\n';
            
            cout << connectedCompNum << '\n';
            cout << "Yes\n";
            cout << numOfRingsToOpen << '\n';
            
        }
        
        //read until eof
        while(myfile) {
            getline(myfile, nodeStringToCheck);
        }
        
        myfile.close();
        
    } else {
        cout << "error reading test.txt\nmake sure test.txt is in the same folder as the executable\n";
    }

    return 0;
}
