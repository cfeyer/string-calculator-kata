.PHONY: check clean

PRODUCT_H_FILES=./include/String_Calculator.h ./include/Add_Observer_Interface.h ./include/Tokenizer.h ./include/Tokenizer_Interface.h
PRODUCT_CPP_FILES=./src/String_Calculator.cpp ./src/Tokenizer.cpp
TEST_CPP_FILES=./test/test.cpp ./test/Tokenizer_Tests.cpp

check: ./bin/test
	./bin/test

./bin/test: $(PRODUCT_H_FILES) $(PRODUCT_CPP_FILES) $(TEST_CPP_FILES) | ./bin
	$(CXX) -std=c++17 $^ -I./include -lgmock -lgtest -lgmock_main -o $@

./bin:
	mkdir ./bin

clean:
	rm -rf ./bin
