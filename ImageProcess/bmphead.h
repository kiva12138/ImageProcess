#pragma once
typedef unsigned char BYTE_MY;// 1
typedef unsigned short WORD_MY;//2
typedef unsigned int DWORD_MY;//4
typedef long LONG_MY;

typedef struct  tagBITMAPFILEHEADER_MY {
	unsigned char bfType[2];//文件类型，必须是0x424D，即字符“BM”
	unsigned char bfSize[4];//文件大小
	unsigned char bfReserved1[2];//保留字
	unsigned char bfReserved2[2];//保留字
	unsigned char bfOffBits[4];//从文件头到实际位图数据的偏移字节数
}BITMAPFILEHEADER_MY;

typedef struct tagBITMAPINFOHEADER_MY {
	DWORD_MY biSize;//信息头大小
	LONG_MY biWidth;//图像宽度
	LONG_MY biHeight;//图像高度
	WORD_MY biPlanes;//位平面数，必须为1
	WORD_MY biBitCount;//每像素位数
	DWORD_MY  biCompression; //压缩类型
	DWORD_MY  biSizeImage; //压缩图像大小字节数
	LONG_MY  biXPelsPerMeter; //水平分辨率
	LONG_MY  biYPelsPerMeter; //垂直分辨率
	DWORD_MY  biClrUsed; //位图实际用到的色彩数
	DWORD_MY  biClrImportant; //本位图中重要的色彩数
}BITMAPINFOHEADER_MY;

typedef struct tagRGBQUAD_MY {
	BYTE_MY rgbBlue; //该颜色的蓝色分量
	BYTE_MY rgbGreen; //该颜色的绿色分量
	BYTE_MY rgbRed; //该颜色的红色分量
	BYTE_MY rgbReserved; //保留值
}RGBQUAD_MY;

typedef struct tagIMAGEDATA_MY
{
	BYTE_MY blue;
	BYTE_MY green;
	BYTE_MY red;
}IMAGEDATA_MY;