.PHONY: check clean

check: ./bin/test
	./bin/test

./bin/test: ./test/test.cpp ./include/String_Calculator.h ./src/String_Calculator.cpp ./include/Add_Observer_Interface.h ./test/Tokenizer_Tests.cpp | ./bin
	$(CXX) -std=c++17 $^ -I./include -lgtest -lgtest_main -o $@

./bin:
	mkdir ./bin

clean:
	rm -rf ./bin
