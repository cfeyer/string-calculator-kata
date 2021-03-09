.PHONY: check

check: ./bin/test
	./bin/test

./bin/test: ./test/test.cpp | ./bin
	$(CXX) $^ -o $@

./bin:
	mkdir ./bin
