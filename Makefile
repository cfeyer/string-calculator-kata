.PHONY: check

check: ./bin/test
	./bin/test

./bin/test: ./test/test.cpp | ./bin
	$(CXX) $^ -lgtest -o $@

./bin:
	mkdir ./bin
