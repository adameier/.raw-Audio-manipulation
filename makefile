CC=g++
TAGS= -std=c++11

driver:
	$(CC) -o samp driver.cpp $(TAGS)

test:
	$(CC) -o tests tests.cpp $(TAGS)
	./tests

clean:
	@rm -f samp
	@rm -f tests
	@rm -f out.raw
