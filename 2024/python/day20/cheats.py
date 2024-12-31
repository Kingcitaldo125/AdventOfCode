from math import hypot
from collections import defaultdict

class Point:
	def __init__(self,x,y,target=None):
		self.x = x
		self.y = y
		if target:
			self.targetx = target.x
			self.targety = target.y
		else:
			self.targetx = -1
			self.targety = -1

	def __str__(self):
		return str(self.x) + "," + str(self.y)

	def __eq__(self,other):
		return self.x == other.x and self.y == other.y

	def __ne__(self,other):
		return self.x != other.x or self.y != other.y

	def distance(self):
		return hypot(self.x - self.targetx, self.y - self.targety)

def print_map(map):
	for row in map:
		for r in row:
			print(r,end='')
		print('')
	print('')

def read_input(filepath="input.txt"):
	lines = None

	with open(filepath) as f:
		lines = f.readlines()

	map = []
	start = None
	target = None
	yctr = 0
	for line in lines:
		if "S" in line:
			start = Point(line.find("S"),yctr,Point(0,0) if not target else target)
			#print("start at",start.x,start.y)
		elif "E" in line:
			target = Point(line.find("E"),yctr)
			if start:
				start = Point(start.x,start.y,target)
			#print("target at",target.x,target.y)

		map.append(list(filter(lambda x: x != '\n',[k for k in line])))
		yctr += 1

	return [map,start,target]

def get_neighbors(map, point):
	neighbors = []
	cheat_neighbors = []
	x = point.x
	y = point.y

	for i in range(x - 1, x + 2):
		if i < 0 or i >= len(map[0]) or i == x:
			continue

		ipoint = Point(i,y)
		if map[y][i] == '#':
			cheat_neighbors.append(ipoint)
			continue

		neighbors.append(ipoint)

	for i in range(y - 1, y + 2):
		if i < 0 or i >= len(map) or i == y:
			continue

		ipoint = Point(x,i)
		if map[i][x] == '#':
			cheat_neighbors.append(ipoint)
			continue

		neighbors.append(ipoint)

	# start with the cheat neighbors and then the neighbors closest to the target
	return [cheat_neighbors, sorted(neighbors,key=lambda it: it.distance())]

def process_options(map, visited, point, used_cheats, t_cheats):
	cheats = []
	neighbors = []
	ncheats,neigh = get_neighbors(map, point)

	for n in neigh:
		if str(n) in visited:
			continue
		neighbors.append(n)

	for c in ncheats:
		if str(c) in used_cheats or t_cheats < 1:
			continue
		cheats.append(c)
		t_cheats -= 1

	return [cheats,neighbors]

def cheat_search(map, visited, used_cheats, point, target, pseconds, t_cheats):
	visited.add(str(point))

	if point == target:
		return pseconds

	cheats,neighbors = process_options(map, visited, point, used_cheats, t_cheats)

	tcc = t_cheats - len(cheats)
	t_cheats = max(0,tcc)
	tp_seconds = 999

	for c in cheats:
		used_cheats.add(str(c))
		res = cheat_search(map, visited, used_cheats, c, target, pseconds + 1, t_cheats)
		tp_seconds = min(tp_seconds, res)

	for n in neighbors:
		res = cheat_search(map, visited, used_cheats, n, target, pseconds + 1, t_cheats)
		tp_seconds = min(tp_seconds, res)

	return tp_seconds

def cheat_engine(map, used_cheats, start, target, cheats, resset, total_seconds):
	for i in range(len(map)):
		for j in range(len(map[0])):
			xvisited = set([])

			res = cheat_search(map, xvisited, used_cheats, start, target, 0, 2)

			if res >= total_seconds:
				continue

			if res not in resset:
				print("res",res)

			resset.add(res)
			cheats[total_seconds - res] += 1

def count_dots(map):
	dots = 0
	for row in map:
		for r in row:
			if r == '.':
				dots += 1
	return dots

def main():
	map,start,target = read_input()
	print("start at",start)
	print("target at",target)
	visited = set([])
	used_cheats = set([])
	#print(f"dots {count_dots(map)}")
	#return

	total_seconds = cheat_search(map,visited,used_cheats,start,target,0,0)
	print(f"total_seconds {total_seconds}")
	#return

	cheats = defaultdict(int)
	resset = set([])
	kctr = 0
	while kctr < count_dots(map):
		cheat_engine(map, used_cheats, start, target, cheats, resset, total_seconds)
		kctr += 1
	print("used_cheats",used_cheats)
	print('')
	print("cheat times",cheats)

if __name__ == "__main__":
	main()
