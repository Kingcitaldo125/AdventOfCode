def stones_help(stone_str, n):
	if n < 1:
		return stone_str

	nstr = ""
	for i in stone_str.split(' '):
		itm = int(i)
		#print(f"itm {itm}")

		if itm == 0:
			nstr += "1"
			nstr += " "
		elif len(i) % 2 == 0:
			half = len(i)//2
			first_half = str(int(i[0:half]))
			second_half = str(int(i[half:]))

			#print(f"first_half {first_half}")
			#print(f"second_half {second_half}")

			nstr += first_half
			nstr += " "
			nstr += second_half
			nstr += " "
		else:
			nstr += str(itm * 2024)
			nstr += " "

	if nstr[-1] == " ":
		nstr = nstr[:-1]
	#print(f"nstr {nstr}")
	return stones_help(nstr, n - 1)

def stones(stone_str,n=1):
	return len(stones_help(stone_str,n).split(' '))

def main():
	#print(stones_help("0 1 10 99 999", 1))
	#print(stones_help("125 17", 25))
	print(stones("125 17", 25))

if __name__ == "__main__":
	main()
