# !/bin/bash
for i in 100 500 1000 1500 2000 2500 3000 3500 4000 4500 5000  
do
	./bin/main $i >> simdmem.dat
done