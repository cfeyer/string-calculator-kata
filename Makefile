.PHONY: check

check: ./bin/test
	./bin/test

./bin/test: ./test/test.cpp | ./bin
	$(CXX) $^ -lgtest -lgtest_main -o $@

./bin:
	mkdir ./bin
