CC=g++
CFLAGS=-Wall -Wextra -Wpedantic -Werror
TESTFLAGS=-lgtest -pthread --coverage
OUTPUT=main
SOURCES=$(OUTPUT).cpp
COVERAGE_FOLDER=coverage_report
COVERAGE_REPORT_MAINPAGE=index.html
OUTPUT_STREAM=/dev/null

build: clean $(SOURCES)
	$(CC) $(SOURCES) $(CFLAGS) $(TESTFLAGS) -o $(OUTPUT).o

clean:
	rm -rf $(OUTPUT).o
	rm -rf $(OUTPUT).gcda
	rm -rf $(OUTPUT).gcno
	rm -rf $(OUTPUT).info
	rm -rf $(COVERAGE_FOLDER)

test: build $(SOURCES)
	./$(OUTPUT).o
	lcov -t $(OUTPUT) -o $(OUTPUT).info -c -d . >> $(OUTPUT_STREAM)
	genhtml -o $(COVERAGE_FOLDER) $(OUTPUT).info >> $(OUTPUT_STREAM)
	xdg-open $(COVERAGE_FOLDER)/$(COVERAGE_REPORT_MAINPAGE) >> $(OUTPUT_STREAM)

