def read_input(filename):
	lines = []
	with open(filename) as f:
		lines = [i.strip('\n') for i in f.readlines()]
	return lines

def get_lists(lines):
	list1 = []
	list2 = []

	for line in lines:
		xl = list(filter(lambda x: x != '', line.split(' ')))
		list1.append(int(xl[0]))
		list2.append(int(xl[-1]))

	return [sorted(list1), sorted(list2)]

def calculate_distances(list1, list2):
	dsum = 0
	for i in range(len(list1)):
		diff = abs(list1[i] - list2[i])
		dsum += diff
	return dsum

def main():
	lines = read_input("input.txt")
	list1, list2 = get_lists(lines)
	print(calculate_distances(list1, list2))

if __name__ == "__main__":
	main()
