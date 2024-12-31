def read_input(filename="input.txt"):
	lines = None

	with open(filename) as f:
		lines = f.readlines()

	patterns = set([i.strip('\n') for i in lines[0].split(', ')])
	designs = [i.strip('\n') for i in lines[2:]]

	return [patterns,designs]

def onsen_towels(patterns, designs):
	onsen_count = 0

	for design in designs:
		found_pattern = True

		for j in range(len(design)):
			for k in range(j,len(design) - 1):
				part = design[j:k+1]
				if part in patterns:
					found_pattern = True
					break
				found_pattern = False

		if found_pattern:
			print("found pattern",design)
			onsen_count += 1

	return onsen_count

def main():
	patterns, designs = read_input()
	print("patterns:",patterns)
	print("designs:",designs)
	ot = onsen_towels(patterns, designs)
	print(f"{ot} towel patterns possible")

if __name__ == "__main__":
	main()
