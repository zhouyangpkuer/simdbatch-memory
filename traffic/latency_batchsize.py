
f_raw = open("latency_batchsize_raw.dat", "r")
f_draw = open("latency_batchsize.txt", "w")

# batchsize = [32, 64, 128, 256, 512, 768, 1024, 1280, 1536, 1792, 2048]

for i in xrange(0,7):
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


	f_draw.write(str(2 ** (i + 5)) + "\t" + str(a) + "\t" + str(b) + "\t" + str(c) + "\t" + str(d) + "\n")

f_raw.close()
f_draw.close()

