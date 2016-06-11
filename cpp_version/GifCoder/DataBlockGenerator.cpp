#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <memory.h>
#include "DataBlockGenerator.h"

using namespace std;

void DataBlockGenerator::initial(){
	currentCode_=0;
	currentData_.clear();
	newData_.clear();
	data_.clear();
	data_.push_back((unsigned char*)malloc(sizeof(unsigned char)*256));
	output_block_=0;
	current_block_=0;
	current_position_=0;
	least_code_size_=9;
	current_bit_=0;
	memset(data_[current_block_],0,255*sizeof(unsigned char));
	lzw_coder_.initial();
	least_code_size_=lzw_coder_.getBits();
	putData(lzw_coder_.getHeader());
	strparser_<<hex<<setw(2)<<setfill('0');
}

void DataBlockGenerator::inputPixelData(unsigned char data){
	strparser_.clear();
	strparser_<<hex<<setw(2)<<setfill('0')<<(unsigned int)data;
	strparser_>>newData_;
	currentData_.append(newData_);
	least_code_size_=lzw_coder_.getBits();
	//std::cout<<"current data: "<<currentData_<<std::endl;
	unsigned int code=lzw_coder_.getCode(currentData_);
	//std::cout<<"=====\nput data: "<<std::hex<<currentCode_<<std::endl;
	//std::cout<<std::dec<<"dic size: "<<least_code_size_<<" dic size: "<<lzw_coder_.getBits()<<std::endl;
	//std::cout<<"put code: "<<currentData_<<std::endl;
	//std::cout<<std::hex<<"get code: "<<std::hex<<code<<std::dec<<std::endl;
	//std::cout<<"code data: "<<std::hex<<code<<std::endl;
	if(code==0xFFFFFFFF){
		putData(currentCode_);
		currentCode_=(unsigned int)data;
		currentData_=newData_;
		newData_.clear();
	}
	else if(code==0x100){
		putData(currentCode_);
		putData(lzw_coder_.getHeader());
		//std::cout<<"newData: "<<newData_<<std::endl;
		least_code_size_=9;
		currentCode_=lzw_coder_.getCode(newData_);
		currentData_=newData_;
		newData_.clear();
	}
	else{
		newData_.clear();
		currentCode_=code;
	}
}

void DataBlockGenerator::inputFinish(){
	least_code_size_=lzw_coder_.getBits();
	putData(currentCode_);
	putData(lzw_coder_.getTailer());
}

void DataBlockGenerator::putData(unsigned int data){
	//std::cout<<"put data: "<<std::hex<<data<<std::endl;
	int databit=12;
	while(databit>least_code_size_){
		if((data<<16-databit)&0x8000){
			break;
		}
		databit--;
	}
	/*
	std::cout<<"databit : "<<databit<<std::endl;
	std::cout<<"current_position_: "<<current_position_<<"  current_bit_"<<current_bit_<<std::endl;
	std::cout<<"====="<<std::endl;
	for(size_t i=0;i<=current_position_;i++){
		std::cout<<"block data "<<i<<" : "<<std::hex<<(unsigned int)data_[current_block_][i]<<std::endl;
	}
	std::cout<<"====="<<std::endl;
	*/
	int leftbit=databit;
	while(leftbit>0){
		int capacity=8-current_bit_;
		data_[current_block_][current_position_]=data<<current_bit_ | data_[current_block_][current_position_];
		//std::cout<<"put block data: "<<std::hex<<(unsigned int)data_[current_block_][current_position_]<<std::dec<<"  index:"<<current_position_<<" "<<current_block_<<std::endl;
		current_bit_+=leftbit;
		leftbit-=capacity;
		if(current_bit_>=8){
			current_bit_=0;
			current_position_++;
		}
		if(current_position_==0xff){
			current_position_=0;
			current_block_++;
			data_.push_back((unsigned char*)malloc(sizeof(unsigned char)*255));
			memset(data_[current_block_],0,255*sizeof(unsigned char));
		}
		data=data>>capacity;
	}
	/*
	std::cout<<"current_position_: "<<current_position_<<"  current_bit_"<<current_bit_<<std::endl;
	std::cout<<"====="<<std::endl;
	for(size_t i=0;i<=current_position_;i++){
		std::cout<<"block data "<<i<<" : "<<std::hex<<(unsigned int)data_[current_block_][i]<<std::endl;
	}
	std::cout<<"====="<<std::endl;
	*/
}

bool DataBlockGenerator::hasDataLeft(){
	return output_block_!=data_.size();
}

size_t DataBlockGenerator::getDataBlock(unsigned char* &data){
	//std::cout<<"output block "<<output_block_<<std::endl;
	data=data_[output_block_++];
	if(output_block_!=data_.size()){
		return 0xff;
	}
	else{
		return current_position_+1;
	}
}

void DataBlockGenerator::deinitial(){
	for(int i=0;i<data_.size();i++){
		free(data_[i]);
	}
	data_.clear();
}