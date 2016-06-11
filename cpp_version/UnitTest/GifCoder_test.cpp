#include <fstream>
#include <iostream>
#include "../GifCoder/GifCoder.h"
#include "../GifCoder/BMPReader.h"
using namespace std;
int main(){
	//cout<<sizeof(unsigned short int)<<endl;
	ofstream outputfile;
	outputfile.open("gif_output",ofstream::binary|ofstream::out|ofstream::trunc);
	GIFCoder gifcoder(outputfile);
	BMPReader bmpreader;
	bmpreader.openBMP("result000002.bmp");
	gifcoder.setHeight((unsigned short int)bmpreader.getHeight());
	gifcoder.setWidth((unsigned short int)bmpreader.getWidth());
	gifcoder.initial();
	gifcoder.start();
	gifcoder.setInfiniteLoop();
	gifcoder.setTime(100);
	gifcoder.addGraph(&bmpreader);
	bmpreader.openBMP("result000003.bmp");
	gifcoder.addGraph(&bmpreader);
	bmpreader.openBMP("result000004.bmp");
	gifcoder.addGraph(&bmpreader);
	bmpreader.openBMP("result000005.bmp");
	gifcoder.addGraph(&bmpreader);
	bmpreader.openBMP("result000006.bmp");
	gifcoder.addGraph(&bmpreader);
	bmpreader.openBMP("result000007.bmp");
	gifcoder.addGraph(&bmpreader);
	bmpreader.openBMP("result000008.bmp");
	gifcoder.addGraph(&bmpreader);
	gifcoder.finish();
	outputfile.close();
}