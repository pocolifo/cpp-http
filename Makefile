OUTPUT=a.out

build:
	clang++ $(shell find src -type f) -std=c++17

clean:
	-rm $(OUTPUT)

test: build
	./$(OUTPUT)

.PHONY: build clean test