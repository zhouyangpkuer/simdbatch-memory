f_raw = open("skew_raw.dat", "r")
f_draw = open("avgtail-zipf.csv", "w")
f_draw2 = open("99tail-zipf.csv", "w")


for i in xrange(0,11):
	a = 0.0
	b = 0.0
	c = 0.0
	d = 0.0

	l = f_raw.readline().split()
	l = f_raw.readline().split()
	l = f_raw.readline().split()
	a += float(l[3])
	l = f_raw.readline().split()
	c += float(l[3])

	l = f_raw.readline().split()
	l = f_raw.readline().split()
	l = f_raw.readline().split()
	b += float(l[3])
	l = f_raw.readline().split()
	d += float(l[3])



	l = f_raw.readline().split()
	l = f_raw.readline().split()
	l = f_raw.readline().split()
	a += float(l[3])
	l = f_raw.readline().split()
	c += float(l[3])

	l = f_raw.readline().split()
	l = f_raw.readline().split()
	l = f_raw.readline().split()
	b += float(l[3])
	l = f_raw.readline().split()
	d += float(l[3])


	l = f_raw.readline().split()
	l = f_raw.readline().split()
	l = f_raw.readline().split()
	a += float(l[3])
	l = f_raw.readline().split()
	c += float(l[3])

	l = f_raw.readline().split()
	l = f_raw.readline().split()
	l = f_raw.readline().split()
	b += float(l[3])
	l = f_raw.readline().split()
	d += float(l[3])


	f_draw.write(str(i * 3.0 / 10) + "\t" + str(a / 3) + "\t" +  str(b / 3) + "\n")
	f_draw2.write(str(i * 3.0 / 10) + "\t" + str(c / 3) + "\t" +  str(d / 3) + "\n")

f_raw.close()
f_draw.close()
f_draw2.close()






f_raw = open("simdsize_raw.dat", "r")
f_draw = open("avgtail-simdrownum.csv", "w")
f_draw2 = open("99tail-simdrownum.csv", "w")


for i in xrange(0,11):
	a = 0.0
	b = 0.0
	c = 0.0
	d = 0.0


	l = f_raw.readline().split()
	l = f_raw.readline().split()
	l = f_raw.readline().split()
	a += float(l[3])
	l = f_raw.readline().split()
	c += float(l[3])

	l = f_raw.readline().split()
	l = f_raw.readline().split()
	l = f_raw.readline().split()
	b += float(l[3])
	l = f_raw.readline().split()
	d += float(l[3])

	if(i == 0):
		f_draw.write(str(100) + "\t" + str(a) + "\t" +  str(b) + "\n")
		f_draw2.write(str(100) + "\t" + str(c) + "\t" +  str(d) + "\n")
	else:
		f_draw.write(str(i * 500) + "\t" + str(a) + "\t" +  str(b) + "\n")
		f_draw2.write(str(i * 500) + "\t" + str(c) + "\t" +  str(d) + "\n")

f_raw.close()
f_draw.close()
f_draw2.close()





f_raw = open("frsize_raw.dat", "r")
f_draw = open("avgtail-algomem.csv", "w")
f_draw2 = open("99tail-algomem.csv", "w")


for i in xrange(0,12):
	a = 0.0
	b = 0.0
	c = 0.0
	d = 0.0

	l = f_raw.readline().split()
	l = f_raw.readline().split()
	l = f_raw.readline().split()
	a += float(l[3])
	l = f_raw.readline().split()
	c += float(l[3])

	l = f_raw.readline().split()
	l = f_raw.readline().split()
	l = f_raw.readline().split()
	b += float(l[3])
	l = f_raw.readline().split()
	d += float(l[3])

	f_draw.write(str(2 ** (i - 2)) + "\t" + str(a) + "\t" +  str(b) + "\n")
	f_draw2.write(str(2 ** (i - 2)) + "\t" + str(c) + "\t" +  str(d) + "\n")

f_raw.close()
f_draw.close()
f_draw2.close()



