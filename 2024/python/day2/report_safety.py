def read_input(filename):
	lines = []
	with open(filename) as f:
		lines = [i.strip('\n') for i in f.readlines()]
	return lines

def is_increasing(report):
	if len(report) < 2:
		return True

	last = report[0]
	for i in range(1,len(report)):
		if report[i] <= last:
			return False
		last = report[i]

	return True

def is_decreasing(report):
	if len(report) < 2:
		return True

	last = report[0]
	for i in range(1,len(report)):
		if report[i] >= last:
			return False
		last = report[i]

	return True

def is_safe(report):
	if not is_decreasing(report) and not is_increasing(report):
		return False

	for i in range(len(report) - 1):
		cur = report[i]
		next = report[i + 1]
		diff = abs(cur - next)
		if diff < 1 or diff > 3:
			return False

	return True

def main():
	lines = read_input("input.txt")
	count = 0
	for l in lines:
		if is_safe([int(i) for i in l.split(' ')]):
			count += 1
	print(count,"safe reports")

if __name__ == "__main__":
	main()
