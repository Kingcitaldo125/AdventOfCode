from math import hypot

def print_map(map):
	for row in map:
		for r in row:
			print(r,end='')
		print('')
	print('')

def read_input(filepath="input.txt"):
	pairs = []
	max_x = 0
	max_y = 0
	lines = None

	with open(filepath) as f:
		lines = f.readlines()

	for line in lines:
		f,s = line.split(',')

		fint = int(f)
		sint = int(s)

		max_x = max(max_x, fint)
		max_y = max(max_y, sint)

		pairs.append([fint, sint])

	map = [['.' for j in range(max_x + 1)] for i in range(max_y + 1)]

	return [map, pairs, max_x, max_y]

def get_neighbors(map, x, y, max_x, max_y):
	neighbors = []

	distance = lambda x1,y1,x2,y2: hypot(x1 - x2, y1 - y2)
	sort_key = lambda item: distance(item[0],item[0],max_x,max_y)

	for i in range(x - 1, x + 2):
		if i < 0 or i >= len(map[0]) or i == x:
			continue
		if map[y][i] == '#':
			continue

		neighbors.append([i,y])

	for i in range(y - 1, y + 2):
		if i < 0 or i >= len(map) or i == y:
			continue
		if map[i][x] == '#':
			continue

		neighbors.append([x,i])

	# start with the neighbors closest to the target
	return sorted(neighbors,key=sort_key)

def byte_steps(map, visited, x, y, steps, max_x, max_y):
	coordstr = str([x,y])
	visited.add(coordstr)

	map[y][x] = 'O'

	if x == max_x and y == max_y:
		return steps

	minsteps = max_x * max_y
	for n in get_neighbors(map, x, y, max_x, max_y):
		if str(n) in visited:
			continue

		bs = byte_steps(map, visited, n[0], n[1], steps + 1, max_x, max_y)
		minsteps = min(minsteps, bs)

	if minsteps == max_x * max_y:
		map[y][x] = '.'

	return minsteps

def main():
	map, pairs, max_x, max_y = read_input()

	for p in pairs:
		x,y = p
		map[y][x] = '#'

	visited = set([])
	bsteps = byte_steps(map, visited, 0, 0, 0, max_x, max_y)
	print_map(map)
	print("byte steps:", bsteps)

if __name__ == "__main__":
	main()
