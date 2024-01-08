all: clean compile run

compile: main.cpp directory.cpp regFile.cpp os.cpp softLink.cpp
	@echo "-------------------------------------------"
	@echo "Compiling..."
	@g++ -std=c++14 -o test main.cpp directory.cpp regFile.cpp os.cpp softLink.cpp

run:
	@echo "-------------------------------------------"
	@echo "Running the tests...."
	@echo "======================================================================="
	@./test
	@echo "======================================================================="
	@echo "Completed tests...."

clean:
	@echo "-------------------------------------------"
	@echo "Cleaning..."
	@rm -f *.o
	@rm -f test