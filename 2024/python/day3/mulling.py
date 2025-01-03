import re

def find_mulls(input):
	mulls = []
	regex = r"mul\(\d+,\d+\)"

	matches = re.finditer(regex, input, re.MULTILINE)

	for matchNum, match in enumerate(matches, start=1):
		mulls.append(match.group())

	return mulls

def calculate_mulls(mulls):
	dotp = 0
	regex = r"mul\((\d+),(\d+)\)"
	rgx = re.compile(regex)

	for m in mulls:
		match = rgx.match(m)
		if not match:
			continue
		dotp += int(match.group(1)) * int(match.group(2))

	return dotp

def main():
	section = "xmul(2,4)%&mul[3,7]!@^do_not_mul(5,5)+mul(32,64]then(mul(11,8)mul(8,5))"
	mulls = find_mulls(section)
	print(calculate_mulls(mulls))

if __name__ == "__main__":
	main()
