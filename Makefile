./bin/test: ./test/test.cpp | ./bin
	$(CXX) ./test/test.cpp -o ./bin/test

./bin:
	mkdir ./bin
