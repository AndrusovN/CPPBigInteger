CC=g++ -std=c++20
CFLAGS=-Wall -Wextra -Wpedantic -Werror
TESTFLAGS=-lgtest -pthread --coverage
OUTPUT=tests
SOURCES=$(OUTPUT).cpp biginteger.cpp rational.cpp exceptions.cpp
INFOS=$(SOURCES:.cpp=.info)
GCDAS=$(SOURCES:.cpp=.gcda)
GCNOS=$(SOURCES:.cpp=.gcno)
TOTAL_INFO=info_merger.info
COVERAGE_FOLDER=coverage_report
COVERAGE_REPORT_MAINPAGE=index.html
OUTPUT_STREAM=/dev/null

build: clean $(SOURCES)
	$(CC) $(SOURCES) $(CFLAGS) $(TESTFLAGS) -O2 -o $(OUTPUT).o

clean:
	rm -f $(OUTPUT).o
	rm -f $(GCDAS)
	rm -f $(GCNOS)
	rm -f $(INFOS)
	rm -f $(TOTAL_INFO)
	rm -rf $(COVERAGE_FOLDER)

test: build $(SOURCES)
	./$(OUTPUT).o
	lcov -t $(OUTPUT) -o $(TOTAL_INFO) -c -d . >> $(OUTPUT_STREAM)
	genhtml -o $(COVERAGE_FOLDER) $(TOTAL_INFO) >> $(OUTPUT_STREAM)
	xdg-open $(COVERAGE_FOLDER)/$(COVERAGE_REPORT_MAINPAGE) >> $(OUTPUT_STREAM)

