from sympy.logic.boolalg import to_cnf
from array import *
#from sympy.logic.boolalg import Xor
from sympy.abc import A,B,C
f = open("write_output0.txt","r")
line = f.readlines()
s = line[0].split(" ")
# a = ["" for x in range(int(s[1]))]
# b = ["" for x in range(int(s[1]))]
# g = ["" for x in range(int(s[1]))]
input = int(s[0])
output = int(s[1])
f.close
for num in range(output):	
	i = 1
	string  = "write_output" + str(num) + ".txt"
	#print(string)
	f = open(string,"r")
	line = f.readlines()
	while line[i][0] == "O":
		s = line[i].split("=")
		t = "~" + s[1]
		a = to_cnf(s[1],True)
		b = ~to_cnf(s[1],True)
		j = 1
		while line[i+j][1] == line[i][1]:
			s = line[i+j].split("=")
			x = to_cnf(s[1],True)
			a = a  & x
			b = b  & ~x
			j = j + 1
		# print(str(a))
		# print(str(b))
		i = i + j;
	s = line[i].split(" ")
	# c = ["" for x in range(int(s[1]))]
	# d = ["" for x in range(int(s[1]))]
	input = int(s[0])
	i = i+1
	while (i)<(len(line)) and line[i][0] == "O":
		s = line[i].split("=")
		t = "~" + s[1]
		c = to_cnf(s[1],True)
		d = ~to_cnf(s[1],True)
		j = 1
		while (i+j)<(len(line)-1) and line[i+j][1] == line[i][1]:
			s = line[i+1].split("=")
			x = to_cnf(s[1],True)
			c = c | x
			d = d | ~ x
			j = j + 1
		# print(str(c))
		# print(str(d))
		i = i + j;
	f.close()
	string  = "final_cnf" + str(num)
	f = open(string,"w")
	g = a & d |  b  &  c
	cnf = str(to_cnf(g))
	count = 1;
	f.write("p cnf ")
	f.write(str(input))
	f.write(" ")
	for y in range(len(cnf)):
		if cnf[y] == "&":
			count = count + 1;
	f.write(str(count))
	f.write("\n")
	for z in range(len(cnf)):
		if cnf[z] == "(" or cnf[z] == ")" or cnf[z] == "|" or cnf[z] == "I":
			continue
		elif cnf[z] == "&":
			f.write("0 \n")
		elif cnf[z] == "~":
			f.write("-")
		else:
			f.write(cnf[z])
	#print(cnf)
	f.close()

# s = "(((((I0&~(I1&I2))|(I1&I2))&(~(I1&I2)))&~(~I1))|(~I1))"
# a = to_cnf(s,True)
# s = "I0"
# b = to_cnf(s,True)
# c[1] = str(to_cnf(a ^ b))
# print(c[1])
