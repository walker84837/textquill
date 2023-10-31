# g++ main.cpp -I./include/opencv4 -I./include/tesseract 
CXX = g++
CXXFLAGS = -Ofast -Wall -Wextra -pedantic -I./include/opencv4 -I ./include/tesseract
LIBS = -ltesseract -lopencv_core -lopencv_imgcodecs -lopencv_imgproc
SRCS = src/main.cpp
EXEC = bin/cpp-ocr-tool.app

.PHONY: all clean

all: $(EXEC)

$(EXEC): $(SRCS)
	$(CXX) $(CXXFLAGS) $< -o $@ $(LIBS)

clean:
	rm -f src/$(EXEC)
