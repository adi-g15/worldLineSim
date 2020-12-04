@todo - Changing the GraphMatrix to use 10 dimensions now
@todo - Make Graph_Box::data to a pointer (and these will be accordingly deleted when they aren't being referenced by )

# Matrix as a Graph
----

The title mostly says it, i will complete it more as needed by the main project
This is a subproject for the worldline project (a catchy name, but an interesting small simulator for me)

## Purpose -

I needed a matrix like data structure, that can have
1.  expansion and deletetion capabilities like that of a list (ie. no need to copy again and again for expansion as in std::vector (and i am not talking about the capacity vs size here, that would not work for my purpose))

2. access as a vector 2x2 matrix, or atleast fast access

Then, i had read somewhere, _A matrix is inherently a graph, each node connected to 4 adjacent nodes_, though it maybe very simple, but quite intutive it seems.

So, now the point of it, So i created a matrix data structure, based on a graph, that completes the requirements for `point 1 above`, though it doesn't have constant time lookups like std::vector, but for my case, i chose to create it like this.

## Probable Uses -

It will likely be used as the `world plot`, since think, this world will be expanding (continuosly or on need basis, so I didn't wanted `vector<vector>`, that had been quite an overhead for expansion like 40\*40 to 100\*100)
Also, in case of `vector<vector>`, you will likely have it grow in ONLY TWO DIRECTIONS, and I WANTED IT TO GROW EQUALLY IN ALL FOUR DIRECTIONS, now this would, almost always mean, that even for a simple expansion, the `vector<vector>` has to completely allocate to a whole new place
