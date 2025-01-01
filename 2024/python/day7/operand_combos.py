import re
from itertools import combinations_with_replacement as cwr

def read_input(retlist):
	rl = []
	rgx = re.compile(r'^(\d+)[:]\s(.+)$')
	with open('input.txt') as f:
		rl = f.readlines()

	for r in rl:
		match = rgx.match(r)
		if not match:
			continue
		retlist.append([match.group(1),match.group(2)])

def process(inputlist):
	outlist = []
	for itm,r in inputlist:
		# turn raw numbers into a list of numbers/operands
		rlist = []
		rl1 = r.split(' ')

		# Create list of numbers/spaces
		for r in rl1:
			rlist.append(r)
			rlist.append(' ')
		rlist = rlist[:-1]
		#print('rlist:',rlist)

		# Replace spaces with operands
		comb_set = set([])
		cl1 = list(cwr(['+','*']*len(rlist),rlist.count(' ')))
		for c in cl1:
			comb_set.add(c)
		#print('comb_set:',comb_set)

		for opset in comb_set:
			rlistout = []
			rlc = rlist[:]
			clist_idx = 0
			for r in rlc:
				if clist_idx >= len(opset):
					rlistout.append(r)
					continue

				if r != ' ':
					rlistout.append(r)
					continue

				rlistout.append(opset[clist_idx])
				clist_idx += 1

			#print("rlistout:",rlistout)
			opback = ''.join(rlistout)
			#print("opback:",opback)
			outlist.append([itm,opback])
	return outlist

def evaluate(inputlist):
	total = 0

	for itm,r in inputlist:
		res = eval(r)
		print('itm:',itm)
		print('r:',r)
		print('res:',res)
		print('')
		if res != int(itm):
			continue
		total += res

	return total

def main():
	inputlist = []
	read_input(inputlist)
	outlist = process(inputlist)
	print('output:',evaluate(outlist))

if __name__ == "__main__":
	main()
