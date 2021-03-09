./bin/test: ./test/test.cpp | ./bin
	$(CXX) $^ -o $@

./bin:
	mkdir ./bin
