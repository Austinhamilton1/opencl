CXX=g++

SRC=src
INC=include
OBJ=obj
BIN=bin

OBJS=$(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(wildcard $(SRC)/*.cpp))

CXXFLAGS=-I$(INC) -D CL_TARGET_OPENCL_VERSION=300 -g
LDFLAGS=-lOpenCL

.PHONY: all clean

all: $(BIN)/device_query $(BIN)/vadd $(BIN)/dot $(BIN)/life $(BIN)/sobel $(BIN)/gauss $(BIN)/gray $(BIN)/negative

$(BIN)/device_query: $(OBJ)/device_query.o $(OBJS) | $(BIN)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ)/device_query.o $(OBJS) $(LDFLAGS)

$(BIN)/vadd: $(OBJ)/vadd.o $(OBJS) | $(BIN)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ)/vadd.o $(OBJS) $(LDFLAGS)

$(BIN)/dot: $(OBJ)/dot.o $(OBJS) | $(BIN)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ)/dot.o $(OBJS) $(LDFLAGS)

$(BIN)/life: $(OBJ)/life.o $(OBJS) | $(BIN)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ)/life.o $(OBJS) $(LDFLAGS)

$(BIN)/sobel: $(OBJ)/sobel.o $(OBJS) | $(BIN)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ)/sobel.o $(OBJS) $(LDFLAGS)

$(BIN)/gauss: $(OBJ)/gauss.o $(OBJS) | $(BIN)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ)/gauss.o $(OBJS) $(LDFLAGS)

$(BIN)/gray: $(OBJ)/gray.o $(OBJS) | $(BIN)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ)/gray.o $(OBJS) $(LDFLAGS)

$(BIN)/negative: $(OBJ)/negative.o $(OBJS) | $(BIN)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ)/negative.o $(OBJS) $(LDFLAGS)

$(OBJ)/device_query.o: device_query.cpp | $(OBJ)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ)/vadd.o: vadd.cpp | $(OBJ)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ)/dot.o: dot.cpp | $(OBJ)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ)/life.o: life.cpp | $(OBJ)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ)/sobel.o: sobel.cpp | $(OBJ)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ)/gauss.o: gauss.cpp | $(OBJ)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ)/gray.o: gray.cpp | $(OBJ)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ)/negative.o: negative.cpp | $(OBJ)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ)/Buffer.o : $(SRC)/Buffer.cpp | $(OBJ)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ)/CommandQueue.o : $(SRC)/CommandQueue.cpp | $(OBJ)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ)/Context.o : $(SRC)/Context.cpp | $(OBJ)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ)/Device.o: $(SRC)/Device.cpp | $(OBJ)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ)/Kernel.o : $(SRC)/Kernel.cpp | $(OBJ)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ)/Logger.o : $(SRC)/Logger.cpp | $(OBJ)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ)/Platform.o : $(SRC)/Platform.cpp | $(OBJ)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ)/Program.o : $(SRC)/Program.cpp | $(OBJ)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ):
	mkdir -p $@

$(BIN):
	mkdir -p $@

clean:
	rm -rf $(OBJ)
	rm -rf $(BIN)
