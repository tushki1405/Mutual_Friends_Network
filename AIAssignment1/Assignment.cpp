/*****************************************************************
--------Foundation of Artificial Intelligence
--------Homework 1
--------Submitted by : Tushar Gupta
******************************************************************/
#include <iostream>
#include <queue>
#include <stack>
#include <list>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <sstream>

using namespace std;

//Structure for every node of graph
struct node
{
	std::string name;
	int nodeNumber;
	int parentNode;
	map<int, int> adjacent;
	int cost;
	int depth;
	bool traversed;
	node() {}
	node(std::string n, int num)
	{
		name=n;
		nodeNumber=num;
		traversed=false;
		parentNode = -1;
		cost=-1;
		depth = -1;
	}
};

//Priority queue compare function for BFS
class compareNodeDepth
{
public:
	bool operator()(node& n1, node& n2)
	{
		if(n1.depth > n2.depth) return true;
		else if(n1.depth == n2.depth && n1.name > n2.name) return true;
		return false;
	}
};

//Priority queue compare function for DFS
class compareNodeR
{
public:
	bool operator()(node& n1, node& n2)
	{
		if(n1.name < n2.name) return true;
		return false;
	}
};

//Priority queue compare function for UCS
class compareNodeCost
{
public:
	bool operator()(node& n1, node& n2)
	{
		if(n1.cost > n2.cost) return true;
		else if(n1.cost == n2.cost && n1.name > n2.name) return true;
		return false;
	}
};

int main() {
	//INITIALIZATIONS & DECLARATIONS
	int inputFlag=1;
	int totalNodes=0;
	int nameStartLine=4;
	int matrixStartLine=0;
	int method=0;
	int ctr=0;
	int readMatrix;
	string startNode;
	string endNode;
	int startNodeNum=0;
	int endNodeNum=0;
	string data;
	string::iterator it;
	list<string> path;
	list<string> traverse;
	list<string>::iterator listIt;
	string traverseString = "";
	string pathString = "";
	vector<node> nodes;
	stringstream readWrite;
	node newNode;

	//Reading data code start
	ifstream inputFile ("input.txt");
	if(inputFile.is_open())
	{
		while(getline(inputFile, data))
		{
			//Reading initial information
			switch (inputFlag)
			{
			case 1:
				readWrite << data;
				readWrite >> method; 
				readWrite.clear();
				break;
			case 2: startNode = data;
				break;
			case 3: endNode = data;
				break;
			case 4:
				readWrite << data;
				readWrite >> totalNodes;
				readWrite.clear();
				matrixStartLine = nameStartLine+ totalNodes;
				break;
			default:
				break;
			}

			//Reading names
			if(inputFlag > nameStartLine && inputFlag <= nameStartLine+totalNodes)
			{
				newNode = node(data,inputFlag-nameStartLine-1);
				nodes.push_back(newNode);
				if(data == startNode)
				{
					startNodeNum=inputFlag-nameStartLine-1;
				}
				if(data == endNode)
				{
					endNodeNum=inputFlag-nameStartLine-1;
				}
			}

			//Reading Matrix
			if(inputFlag > matrixStartLine && inputFlag <= matrixStartLine+totalNodes)
			{
				ctr = 0;
				readWrite << data;
				for(int i=0;i < totalNodes; i++)
				{
					readWrite >> readMatrix;
					readWrite.clear();
					if(readMatrix != 0)
					{
						nodes[inputFlag-matrixStartLine-1].adjacent.insert(pair<int, int>(ctr,readMatrix));
					}
					ctr++;
				}
			}
			inputFlag++;
		}
		inputFile.close();

		//BFS
		//using a priority queue which sorts according to depth and names
		if(method == 1)
		{
			ctr = 0;
			priority_queue<node, vector<node>, compareNodeDepth> qs;
			map<int, int>::iterator n;
			node temp;
			nodes[startNodeNum].depth = 0;
			nodes[startNodeNum].traversed = true;
			nodes[startNodeNum].parentNode = -2;
			qs.push(nodes[startNodeNum]);

			//BFS Traversal and marking parents
			while(!qs.empty())
			{
				temp = qs.top();
				qs.pop();
				traverse.push_back(temp.name);
				if(temp.name == endNode)
					break;
				for( n=temp.adjacent.begin(); n!=temp.adjacent.end(); n++)
				{
					if(nodes[n->first].traversed == false)
					{
						nodes[n->first].parentNode = temp.nodeNumber;
						nodes[n->first].depth = temp.depth + 1;
						nodes[n->first].traversed = true;
						qs.push(nodes[n->first]);
					}
				}
			}
		}

		//DFS
		//Using a priority queue here to sort according to reverse names
		//Check this
		else if(method == 2)
		{
			stack<node> st;
			priority_queue<node, vector<node>, compareNodeR> qs;
			node temp;
			//cout << "StartNode: " << startNodeNum << " ,EndNode: " << endNodeNum << endl;
			map<int, int>::reverse_iterator n;
			nodes[startNodeNum].traversed = true;
			nodes[startNodeNum].parentNode = -2;
			nodes[startNodeNum].depth = 0;
			st.push(nodes[startNodeNum]);
			//DFS Traversal and marking parents
			while(!st.empty())
			{
				temp = st.top();
				st.pop();
				traverse.push_back(temp.name);
				if(temp.name == endNode)
					break;
				for( n=temp.adjacent.rbegin() ; n!=temp.adjacent.rend(); n++)
				{
					if(nodes[n->first].traversed == false)
					{
						nodes[n->first].parentNode = temp.nodeNumber;
						nodes[n->first].traversed = true;
						nodes[n->first].depth = temp.depth + 1;
						qs.push(nodes[n->first]);
					}
					else if(nodes[n->first].traversed == true && nodes[n->first].depth > temp.depth + 1)
					{
						nodes[n->first].parentNode = temp.nodeNumber;
						nodes[n->first].depth = temp.depth + 1;
						qs.push(nodes[n->first]);
					}
				}
				while(!qs.empty())
				{
					st.push(qs.top());
					qs.pop();
				}
			}
		}

		//UCS
		//Using a priority queue which sorts according to cost and name
		else if(method == 3)
		{
			priority_queue <node, vector<node>, compareNodeCost> frontier;
			queue<node> tempQueue;
			map<int, int>::iterator n;
			node temp, temp2;
			nodes[startNodeNum].cost = 0;
			nodes[startNodeNum].traversed = true;
			nodes[startNodeNum].parentNode = -2;
			nodes[startNodeNum].depth = 0;
			frontier.push(nodes[startNodeNum]);
			//UCS Traversal and marking parents
			while(!frontier.empty())
			{
				temp = frontier.top();
				frontier.pop();
				traverse.push_back(temp.name);
				if(temp.name == endNode)
					break;
				for( n=temp.adjacent.begin(); n!=temp.adjacent.end(); n++)
				{ 
					if((nodes[n->first].traversed == false))
					{
						nodes[n->first].parentNode = temp.nodeNumber;
						nodes[n->first].traversed = true;
						nodes[n->first].cost = temp.cost + n->second;
						nodes[n->first].depth = temp.depth + 1;
						frontier.push(nodes[n->first]);
					}
					else if(nodes[n->first].traversed == true && nodes[n->first].cost > temp.cost + n->second)
					{
						while(!frontier.empty())
						{
							temp2 = frontier.top();
							tempQueue.push(temp2);
							frontier.pop();
						}
						while(!tempQueue.empty())
						{
							temp2 = tempQueue.front();
							if(temp2.name == nodes[n->first].name)
							{
								nodes[n->first].parentNode = temp.nodeNumber;
								nodes[n->first].cost = temp.cost + n->second;
								nodes[n->first].depth = temp.depth + 1;
								frontier.push(nodes[n->first]);
							}
							else
							{
								frontier.push(temp2);
							}
							tempQueue.pop();
						}
					}
				}
			}
		}

		//Setting up traversal path and shortest path in strings for ouput
		node outputNode = nodes[endNodeNum];
		path.push_front(outputNode.name);
		ctr = outputNode.adjacent[outputNode.parentNode];
		while(outputNode.parentNode > -1)
		{
			outputNode = nodes[outputNode.parentNode];
			path.push_front(outputNode.name);
			ctr += outputNode.adjacent[outputNode.parentNode];
		}

		//Checking and writing in file
		for(listIt=traverse.begin(); listIt!=traverse.end(); listIt++)
			traverseString+= *listIt + "-";
		if(path.front() == startNode && path.back() == endNode)
		{
			for(listIt=path.begin(); listIt!=path.end(); listIt++)
				pathString+= *listIt + "-";

			ofstream outputfile("output.txt");
			outputfile<< traverseString.substr(0, traverseString.rfind('-'));
			outputfile<< endl;
			outputfile<< pathString.substr(0, pathString.rfind('-'));
			outputfile<< endl;
			outputfile << ctr;
			outputfile.close();
		}
		else
		{
			ofstream outputfile("output.txt");
			//outputfile<< traverseString.substr(0, traverseString.rfind('-'));
			//outputfile<< endl;
			outputfile << "NoPathAvailable";
			outputfile.close();
		}
	}
	else
	{
		ofstream outputfile("output.txt");
		outputfile << "ERROR";
		outputfile.close();
	}
	return 0;
}
