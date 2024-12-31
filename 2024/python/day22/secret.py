from sys import argv

def secretnumber(n):
	mod = 16777216

	mul1 = n * 64
	n = (mul1 ^ n) % mod

	div = n // 32
	n = (div ^ n) % mod

	mul2 = n * 2048
	n = (mul2 ^ n) % mod

	return n

def nth_secret(start,n):
	item = start
	for i in range(n):
		item = secretnumber(item)
	return item

def main():
	lines = None
	with open('input.txt') as f:
		lines = [i.strip('\n') for i in f.readlines()]

	sum = 0
	for l in lines:
		n,start = l.split(": ")
		print("n",n,"start",start)
		sum += nth_secret(int(start),2000)
	print(f"sum: {sum}")

if __name__ == "__main__":
	main()
