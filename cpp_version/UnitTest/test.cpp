#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>

using namespace std;

int main(){
	stringstream ss;
	string sr;
	unsigned char bt=0x0f;
	ss<<setw(2)<<setfill('0')<<hex<<(unsigned int)bt;
	ss>>sr;
	cout<<sr<<endl;
}