class Graph:
	def __init__(self):
		self.nodes = {}

	def _add_neighbors(self, node, neighbors):
		for n in neighbors:
			if n not in self.nodes:
				self.add_node(n)
			self.nodes[node].add(n)

	def add_node(self, node, neighbors=set([])):
		if node not in self.nodes:
			self.nodes[node] = set([])
		self._add_neighbors(node, neighbors)

def read_input(filename):
	lines = None
	with open(filename) as f:
		lines = [i.strip('\n') for i in f.readlines()]
	return lines

def dfs(graph, vlist, vstack, ineighbors, item):
	vstack.append(item)

	neighbors = graph.nodes[item]
	intersect = ineighbors.intersection(neighbors)

	for n in intersect:
		if len(vstack) >= 3:
			continue

		dfs(graph, vlist, vstack, ineighbors, n)

	if len(vstack) == 3:
		#print(f"vstack {vstack}")
		foundt = False
		for v in vstack:
			if v[0] == 't':
				foundt = True
				break

		if foundt:
			vlist.append(sorted(vstack))

	if len(vstack) > 0:
		vstack.pop()

	#print("")

def main():
	graph = Graph()
	lines = read_input("input.txt")
	for l in lines:
		first,second = l.split('-')
		graph.add_node(first, {second})
		graph.add_node(second, {first})

	glist = []
	gset = set([])
	for i in graph.nodes:
		vlist = []
		vstack = []

		ineighbors = graph.nodes[i]
		dfs(graph, vlist, vstack, ineighbors, i)

		for v in vlist:
			vstr = str(v)

			if vstr in gset:
				continue

			glist.append(v)
			gset.add(vstr)

	glist.sort()

	print(f"glist contains '{len(glist)}' items:")
	print(f"{glist}")

if __name__ == "__main__":
	main()
