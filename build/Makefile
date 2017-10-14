ifeq ($(TERM_PROGRAM),Apple_Terminal)
	CC=g++
	CXXFLAGS= -I. -std=c++11 -O3
else
	CC=g++ -pthread 
	CXXFLAGS= -I . -I /usr/include -std=c++11 -O3
endif

COMMON_DEPS =  ../YRShell.h ../utility/CircularQ.h ../utility/IntervalTimer.h ../utility/Dictionary.h ../utility/YRShellDefines.h ../utility/YRShellInterpreter.h ../utility/HiResTimer.h ../utility/Sliceable.h ../utility/BufferedSerial.h  ../utility/HardwareSpecific.h
COMMON_OBJ = ../YRShell.o ../utility/IntervalTimer.o  ../utility/Dictionary.o ../utility/YRShellInterpreter.o ../utility/Sliceable.o ../utility/HiResTimer.o  ../utility/BufferedSerial.o ../utility/HardwareSpecific.o
SHELL_DEPS = $(COMMON_DEPS)
SHELL_OBJ = main.o  $(COMMON_OBJ)
TEST_DEPS = catch.h testSupport.h $(COMMON_DEPS)
TEST_OBJ = testall.o testSupport.o $(COMMON_OBJ)


%.o: %.cpp $(TEST_DEPS)
	$(CC) -c -o $@ $< $(CXXFLAGS)

all: testall yrshell doc testall runtest

testall: $(TEST_OBJ)
	$(CC)  -o $@ $^ $(CXXFLAGS)

yrshell: $(SHELL_OBJ)
	$(CC)  -o $@ $^ $(CXXFLAGS)

doc: doxygen.log

doxygen.log: Doxyfile $(TEST_DEPS)  doxygen.h
	doxygen > doxygen.log

runtest: testresult

testresult:
	./testall -s > testresult

.PHONY: clean

clean:
	rm -rf html
	rm -f testall
	rm -f yrshell
	rm -f testresult
	rm -f doxygen.log
	rm -f *.o
	rm -f ../*.o
	rm -f ../utility/*.o

