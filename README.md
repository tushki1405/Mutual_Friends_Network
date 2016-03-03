# Mutual Friend Network
The project involved searching a graph of mutual friends to connect to a stranger. 
The project was implemented as a part of AI coursework. C++ STL was used and the algorithms employed were DFS, BFS, UniCost and A*. 
The statistics were collected and various algorithms were analyzed for optimality, completeness and space & time complexities.

Input
```
<task#> bfs = 1, dfs = 2, ucs = 3

<source> the name of the first node

<destination> the name of the target node

<#nodes> the total number of nodes

<nodes> 
Andy
Bill
Alice
This is a list of names representing the nodes of the graph.

<graph>
0 5 10
5 0 2
10 2 0
The rows and columns correspond to the nodes in the same order. For example, the first row tells us that
Andy is not connected to himself (0), connected to Bill with a value 5, and connected to Alice with a
value 10.
```
#Output
```
<Expansion>
<Output>
<PathCost>
```
