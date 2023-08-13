CXX = g++
CXXFLAGS = -O3 -std=c++17 -Wall -Wextra -pedantic -I/usr/include/opencv4
LIBS = -ltesseract -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs -lopencv_photo
SRCS = main.cpp
EXEC = cpp-ocr-tool.app

.PHONY: all clean

all: $(EXEC)

$(EXEC): $(SRCS)
	$(CXX) $(CXXFLAGS) $< -o $@ $(LIBS)

clean:
	rm -f $(EXEC)
