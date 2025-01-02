import re

from collections import defaultdict
from sys import argv

def read_input(filename):
	lines = None
	with open(filename) as f:
		lines = [i.strip('\n') for i in f.readlines()]
	return lines

def process_gates(lines):
	gates = {}

	for l in lines:
		if ':' not in l:
			break
		gate,val = l.split(': ')
		gates[gate] = int(val)

	return gates

def get_parts_from_line(line):
	rgx = re.compile(r'^(.+)\s+(\w\w\w?)\s+(.+)\s+->\s+(.+)$')
	match = rgx.match(line)

	parts = []
	if not match:
		return parts

	for i in range(1,len(match.groups()) + 1):
		parts.append(match.group(i))

	return parts

def process_connection(connections, fgate, op, sgate, resgate):
	if fgate not in connections or sgate not in connections:
		return 1

	if op == "AND":
		connections[resgate] = connections[fgate] & connections[sgate]
	elif op == "OR":
		connections[resgate] = connections[fgate] | connections[sgate]
	elif op == "XOR":
		connections[resgate] = connections[fgate] ^ connections[sgate]

	return 0

def process_local_connections(connections, lines):
	res = 1
	# Walk through the collection of commands trying to process/store
	# values in registers that we've already processed.

	# Place the output values back in the connections for input registers we've seen
	# For input registers we have not seen, save those for later processing.
	# Custom input registers should be populated based on whether a prior command
	# successfully stored a nonzero value in both input registers.

	# A potentially better approach would be to split input registers on
	# the operand and push those input operands as nodes in a graph, with an edge going to the output register.
	# Topological sort could be done in order to process known registers before the unknown registers
	while res == 1:
		res = 0

		for l in lines:
			if '->' not in l:
				continue

			fgate, op, sgate, resgate = get_parts_from_line(l)
			# If one of the two input gates has a nonzero value, save that command for later
			# Otherwise, add the gate value to the connections
			res |= process_connection(connections, fgate, op, sgate, resgate)

def convert_outputs(connections,filter=""):
	bitstr = ""
	for c in connections:
		if not c[0].startswith(filter):
			continue
		bitstr += str(c[-1])
	bitstr = bitstr[::-1]
	print("bitstr",bitstr)
	return int(bitstr,2)

def main():
	if len(argv) < 2:
		return

	lines = read_input(argv[1])
	gates = process_gates(lines)
	connections = defaultdict(int)

	# place the input gates into the connection collection
	for k,v in gates.items():
		connections[k] = v

	process_local_connections(connections, lines)
	scons = sorted(connections.items())
	print(f"connections {scons}")
	print(convert_outputs(scons,filter="z"))

if __name__ == "__main__":
	main()
