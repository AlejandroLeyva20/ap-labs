def get_length(lst):
	tot = 0
	for data in lst:
		if type(data) == list:
			tot += get_length(data)
		else:
			tot += 1
	return tot

lst = [1,2,3,4,5]
print(get_length(lst))
