ifeq ($(TERM_PROGRAM),Apple_Terminal)
	CC=g++
	CXXFLAGS= -I. -std=c++11
else
	CC=g++ -pthread 
	CXXFLAGS= -I . -I /usr/include -std=c++11
endif

COMMON_DEPS =  ../YRShell.h ../utility/CircularQ.h ../utility/IntervalTimer.h ../utility/Dictionary.h ../utility/YRShellDefines.h ../utility/YRShellInterpreter.h
COMMON_OBJ = ../YRShell.o ../utility/IntervalTimer.o  ../utility/Dictionary.o ../utility/YRShellInterpreter.o
SHELL_DEPS = $(COMMON_DEPS)
SHELL_OBJ = main.o  $(COMMON_OBJ)
TEST_DEPS = catch.h testSupport.h $(COMMON_DEPS)
TEST_OBJ = test.o testSupport.o $(COMMON_OBJ)


%.o: %.cpp $(TEST_DEPS)
	$(CC) -c -o $@ $< $(CXXFLAGS)

all: test yrshell doc test runtest

test: $(TEST_OBJ)
	$(CC)  -o $@ $^ $(CXXFLAGS)

yrshell: $(SHELL_OBJ)
	$(CC)  -o $@ $^ $(CXXFLAGS)

doc: doxygen.log

doxygen.log: Doxyfile $(TEST_DEPS)  doxygen.h
	doxygen > doxygen.log

runtest: testresult

testresult:
	./test > testresult

.PHONY: clean

clean:
	rm -rf html
	rm -f test
	rm -f shell
	rm -f testresult
	rm -f doxygen.log
	rm -f *.o
	rm -f ../*.o
	rm -f ../utility/*.o

