#ifndef _GIFCODER_H_
#define _GIFCODER_H_

#include <ostream>
#include "BMPReader.h"
#include "DataBlockGenerator.h"

typedef unsigned char BYTE;

struct GIF_Header{
	BYTE signature[3];
	BYTE version[3];
	void version_89a();
	void output(std::ostream& os);
};

struct Logical_Screen_Descriptor{
	unsigned short int logical_screen_width;
	unsigned short int logical_screen_height;
	BYTE packed_field;
	BYTE background_color_index;
	BYTE pixel_aspect_ratio;
	void default_set();
	void output(std::ostream& os);
};

struct Image_Descriptor{
	BYTE image_separator;
	unsigned short int image_left_position;
	unsigned short int image_top_position;
	unsigned short int image_width;
	unsigned short int image_height;
	BYTE packed_field;
	void default_set();
	void output(std::ostream &os);
};

struct Graphic_Control_Extension{
	BYTE extension_introducer;
	BYTE graphic_control_label;
	BYTE block_size;
	BYTE packed_field;
	unsigned short int delay_time;
	BYTE transparent_color_index;
	BYTE block_terminator;
	void default_set();
	void output(std::ostream &os);
};

struct Tailer{
	BYTE GIF_tailer;
	void default_set();
	void output(std::ostream &os);
};

struct Application_Extension{
	BYTE extension_introducer;
	BYTE application_extension_label;
	BYTE block_size;
	BYTE application_identifier[12];
	BYTE sub_block_size;
	BYTE loop_block_id;
	unsigned short int loop_count;
	BYTE block_terminator;
	void set_infinite_loop();
	void output(std::ostream &os);
};


class GIFCoder{
public:
	GIFCoder(std::ostream& os);
	void initial();
	void setHeight(unsigned short int height);
	void setWidth(unsigned short int weight);
	void addGraph(BMPReader* dataSource);
	void setTime(unsigned short int time);
	void setInfiniteLoop();
	void start();
	void finish();
private:
	std::ostream& os_;
	GIF_Header header_;
	Logical_Screen_Descriptor screen_descriptor_;
	Image_Descriptor image_descriptor_;
	Graphic_Control_Extension control_extension_;
	Tailer tailer_;
	BMPReader* dataSource_;
	DataBlockGenerator dataBlock_;
	Application_Extension loop_control_;
};

#endif