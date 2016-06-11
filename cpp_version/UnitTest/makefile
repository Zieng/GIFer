target: GifCoder_test

CC = g++

DataBlockGenerator_test_obj = DataBlockGenerator_test
LZWCoder_test_obj = LZWCoder_test
EnCoder_test_obj = EnCoder_test
GifCoder_test_obj = GifCoder_test

LZWCoder_test:
	$(CC) -o $(LZWCoder_test_obj) ./LZWCoder_test.cpp ../GifCoder/LZWCoder.cpp ../GifCoder/LZWCoder.h

DataBlockGenerator_test:
	$(CC) -o $(DataBlockGenerator_test_obj) ./DataBlockGenerator_test.cpp ../GifCoder/LZWCoder.cpp ../GifCoder/DataBlockGenerator.cpp

EnCoder_test:
	$(CC) -o $(EnCoder_test_obj) ./EnCoder_test.cpp ../GifCoder/LZWCoder.cpp ../GifCoder/DataBlockGenerator.cpp ../GifCoder/BMPReader.cpp -lfreeimage

GifCoder_test:
	$(CC) -o $(GifCoder_test_obj) ./GifCoder_test.cpp ../GifCoder/GifCoder.cpp ../GifCoder/LZWCoder.cpp ../GifCoder/DataBlockGenerator.cpp ../GifCoder/BMPReader.cpp -lfreeimage