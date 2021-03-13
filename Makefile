.PHONY: check clean

PRODUCT_H_FILES=./include/String_Calculator.h ./include/Add_Observer_Interface.h ./include/Tokenizer.h ./include/Tokenizer_Interface.h
PRODUCT_CPP_FILES=./src/String_Calculator.cpp ./src/Tokenizer.cpp
TEST_CPP_FILES=./test/String_Calculator_Tests.cpp ./test/Tokenizer_Tests.cpp ./test/Acceptance_Tests.cpp
TEST_H_FILES=./test/Mock_Tokenizer.h

check: ./bin/test
	./bin/test

./bin/test: $(PRODUCT_H_FILES) $(PRODUCT_CPP_FILES) $(TEST_CPP_FILES) $(TEST_H_FILES) | ./bin
	$(CXX) -std=c++17 $^ -I./include -lgmock -lgtest -lgmock_main -pthread -o $@

./bin:
	mkdir ./bin

clean:
	rm -rf ./bin
