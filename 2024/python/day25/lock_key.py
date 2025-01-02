# Key/lock object class
class Kobj:
	def __init__(self,ktype):
		self.rows = []
		self.ktype = ktype

	def __str__(self):
		rowstr = ""
		for r in self.rows:
			rowstr += r
			rowstr += '\n'
		out_type = "Key:\n" if self.ktype == 'k' else "Lock:\n"
		return out_type + str(self.get_values()) + '\n' + rowstr

	def get_type(self):
		return self.ktype

	def add_row(self, row):
		self.rows.append(row)

	def get_values(self):
		vals = []

		for j in range(len(self.rows[0])):
			val = 0
			for i in range(len(self.rows)):
				itm = self.rows[i][j]
				if itm == '#':
					val += 1
			vals.append(val - 1)

		return vals

def read_input(filename):
	lines = None
	with open(filename) as f:
		lines = [i.strip('\n') for i in f.readlines()]
	return lines

def make_kl_obj(items, portion, is_key):
	kobj = Kobj('k' if is_key else 'l')
	#print("Made a new key" if is_key else "Made a new lock")

	for port in portion:
		kobj.add_row(port)

	items.append(kobj)
	del portion[:]
	portion = []

def process_lines(lines):
	items = []
	portion = []
	is_key = False
	is_lock = False
	rowctr = 0

	for l in lines:
		if '.' not in l and '#' not in l: # separator
			make_kl_obj(items, portion, is_key)
			is_key = False
			is_lock = False
			
			rowctr += 1
			continue

		if not is_key and not is_lock:
			if '.' not in l: # new lock
				#print(f"found lock at row {rowctr}")
				is_key = False
				is_lock = True
			elif '#' not in l: # new key
				#print(f"found key at row {rowctr}")
				is_key = True
				is_lock = False

		portion.append(l)
		rowctr += 1

	if len(portion) > 0:
		make_kl_obj(items, portion, is_key)

	return items

def check_lock_key(lock, key):
	lock_vals = lock.get_values()
	key_vals = key.get_values()

	for i in range(len(lock_vals)):
		l1 = lock_vals[i]
		k1 = key_vals[i]
		if int(l1) + int(k1) > 5:
			return False

	return True

def check_fittings(items):
	locks = [i for i in items if i.get_type() == 'l']
	keys = [i for i in items if i.get_type() == 'k']
	total = 0

	for l in locks:
		for k in keys:
			if check_lock_key(l,k):
				total += 1
	return total

def main():
	lines = read_input("input.txt")
	items = process_lines(lines)

	for i in items:
		print(i)

	fres = check_fittings(items)
	print(f"'{fres}' uniquely fitting key/lock pairs")

if __name__ == "__main__":
	main()
