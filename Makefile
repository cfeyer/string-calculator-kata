.PHONY: check clean

check: ./bin/test
	./bin/test

./bin/test: ./test/test.cpp ./include/String_Calculator.h | ./bin
	$(CXX) $^ -I./include -lgtest -lgtest_main -o $@

./bin:
	mkdir ./bin

clean:
	rm -rf ./bin
