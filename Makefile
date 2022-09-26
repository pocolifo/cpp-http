OUTPUT=main.out

build:
	clang++ $(shell find src -type f -name *.cpp) -std=c++17 -o $(OUTPUT)

clean:
	-rm $(OUTPUT)

test: build
	./$(OUTPUT)

.PHONY: build clean test