from collections import defaultdict

def read_input(filepath="input.txt"):
	page_orderings = []
	page_rules = []

	with open(filepath) as f:
		lines = [i.strip('\n') for i in f.readlines()]
		for l in lines:
			if "|" in l:
				page_orderings.append(l)
			elif "," in l:
				page_rules.append(l)

	return [page_orderings, page_rules]

def valid_rules(ordering_map, page_rules):
	valids = []
	for rule_line in page_rules:
		line = rule_line.split(',')
		valid = True
		for i in range(len(line)):
			if not valid:
				break
			item = line[i]
			for j in range(0,i):
				if line[j] in ordering_map[item]:
					valid = False
					break

		if valid:
			valids.append(rule_line)

	return valids

def get_middle_num(line):
	if ',' not in line:
		return -1
	items = line.split(',')
	return int(items[len(items) // 2])

def main():
	page_orderings, page_rules = read_input()
	ordering_map = defaultdict(set)

	for ordering in page_orderings:
		l,r = ordering.split('|')
		ordering_map[l].add(r)

	total = 0
	for rule in valid_rules(ordering_map, page_rules):
		mid = get_middle_num(rule)
		total += mid

	print("total =",total)

if __name__ == "__main__":
	main()
