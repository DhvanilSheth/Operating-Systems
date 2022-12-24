make:
	gcc -o thread thread.c -lpthread

run:
	./thread 0 50 50
	./thread 1 60 60
	./thread 2 70 70
	./thread 3 80 80
	./thread 4 90 90

histogram:
	python3 -u Histogram.py
	
clean:
	rm -f thread
	rm cache_values.txt