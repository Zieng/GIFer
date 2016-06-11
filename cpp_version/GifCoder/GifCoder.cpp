#include <iostream>
#include <assert.h>
#include <string.h>
#include "GifCoder.h"

using namespace std;

void GIF_Header::version_89a(){
	signature[0]='G';
	signature[1]='I';
	signature[2]='F';
	version[0]='8';
	version[1]='9';
	version[2]='a';
}

void GIF_Header::output(ostream& os){
	os<<signature[0]<<signature[1]<<signature[2];
	os<<version[0]<<version[1]<<version[2];
}

void Logical_Screen_Descriptor::default_set(){
	packed_field= 0x70;
	background_color_index=0x00;
	pixel_aspect_ratio=0x00;
}

void Logical_Screen_Descriptor::output(ostream& os){
	cout<<logical_screen_width<<" "<<logical_screen_height<<endl;
	os.write((const char*)&logical_screen_width,sizeof(unsigned short int));
	os.write((const char*)&logical_screen_height,sizeof(unsigned short int));
	os.write((const char*)&packed_field,sizeof(BYTE));
	os.write((const char*)&background_color_index,sizeof(BYTE));
	os.write((const char*)&pixel_aspect_ratio,sizeof(BYTE));
}

void Image_Descriptor::default_set(){
	image_separator=0x2C;
	image_left_position=0x0000;
	image_top_position=0x0000;
	packed_field=0x87;
}

void Image_Descriptor::output(ostream &os){
	os.write((const char*)&image_separator,sizeof(BYTE));
	os.write((const char*)&image_left_position,sizeof(unsigned short int));
	os.write((const char*)&image_top_position,sizeof(unsigned short int));
	os.write((const char*)&image_width,sizeof(unsigned short int));
	os.write((const char*)&image_height,sizeof(unsigned short int));
	os.write((const char*)&packed_field,sizeof(BYTE));
}

void Graphic_Control_Extension::default_set(){
	extension_introducer=0x21;
	graphic_control_label=0xF9;
	block_size=0x04;
	packed_field=0x08;
	delay_time=0x000a;
	transparent_color_index=0x00;
	block_terminator=0x00;
}

void Graphic_Control_Extension::output(ostream& os){
	os.write((const char*)&extension_introducer,sizeof(BYTE));
	os.write((const char*)&graphic_control_label,sizeof(BYTE));
	os.write((const char*)&block_size,sizeof(BYTE));
	os.write((const char*)&packed_field,sizeof(BYTE));
	os.write((const char*)&delay_time,sizeof(unsigned short int));
	os.write((const char*)&transparent_color_index,sizeof(BYTE));
	os.write((const char*)&block_terminator,sizeof(BYTE));	
}

void Tailer::default_set(){
	GIF_tailer=0x3B;
}

void Tailer::output(ostream& os){
	os.write((const char*)&GIF_tailer,sizeof(BYTE));	
}

void Application_Extension::set_infinite_loop(){
	extension_introducer=0x21;
	application_extension_label=0xff;
	block_size=0x0b;
	strcpy((char*)application_identifier,"NETSCAPE2.0");
	sub_block_size=0x03;
	loop_block_id=0x01;
	loop_count=0;
	block_terminator=0x00;
}

void Application_Extension::output(std::ostream &os){
	os.write((const char*)&extension_introducer,sizeof(BYTE));
	os.write((const char*)&application_extension_label,sizeof(BYTE));
	os.write((const char*)&block_size,sizeof(BYTE));
	os.write((const char*)&application_identifier,sizeof(BYTE)*11);
	os.write((const char*)&sub_block_size,sizeof(BYTE));
	os.write((const char*)&loop_block_id,sizeof(BYTE));
	os.write((const char*)&loop_count,sizeof(unsigned short int));
	os.write((const char*)&block_terminator,sizeof(BYTE));
}

GIFCoder::GIFCoder(std::ostream& os):os_(os){
	if(os_.good()){
	}
	else{
		cerr<<"GIFCoder::Broken Flow"<<endl;
	}	
}

void GIFCoder::initial(){
	header_.version_89a();
	screen_descriptor_.default_set();
	image_descriptor_.default_set();
	control_extension_.default_set();
	tailer_.default_set();
	dataBlock_.initial();
}

void GIFCoder::setHeight(unsigned short int height){
	screen_descriptor_.logical_screen_height=height;
	image_descriptor_.image_height=height;
}

void GIFCoder::setWidth(unsigned short int width){
	screen_descriptor_.logical_screen_width=width;
	image_descriptor_.image_width=width;
}

void GIFCoder::setTime(unsigned short int time){
	control_extension_.delay_time=time;
}

void GIFCoder::start(){
	header_.output(os_);
	screen_descriptor_.output(os_);
}

void GIFCoder::addGraph(BMPReader* dataSource){
	dataSource_=dataSource;
	assert((unsigned short int)dataSource_->getHeight()==image_descriptor_.image_height);
	assert((unsigned short int)dataSource_->getWidth()==image_descriptor_.image_width);
	control_extension_.output(os_);
	image_descriptor_.output(os_);
	unsigned char* palette=dataSource_->getPalette();
	os_.write((const char*)palette,sizeof(unsigned char)*256*3);
	dataBlock_.initial();
	unsigned char pixel;
	while(dataSource_->hasPixelLeft()){
		if(dataSource_->getNextPixel(&pixel)){
			dataBlock_.inputPixelData(pixel);
		}
		else{
			cerr<<"GIFCoder::Get Pixel Failed!"<<endl;
		}
	}
	dataBlock_.inputFinish();
	unsigned char codelength=0x08;
	os_.write((const char*)&codelength,sizeof(unsigned char));
	while(dataBlock_.hasDataLeft()){
		unsigned char size;
		unsigned char* data;
		size=(unsigned char)dataBlock_.getDataBlock(data);
		os_.write((const char*)&size,sizeof(unsigned char));
		os_.write((const char*)data,sizeof(unsigned char)*size);
	}
	dataBlock_.deinitial();
}

void GIFCoder::setInfiniteLoop(){
	loop_control_.set_infinite_loop();
	loop_control_.output(os_);
}

void GIFCoder::finish(){
	tailer_.output(os_);
}