#pragma once
typedef unsigned char BYTE_MY;// 1
typedef unsigned short WORD_MY;//2
typedef unsigned int DWORD_MY;//4
typedef long LONG_MY;

typedef struct  tagBITMAPFILEHEADER_MY {
	unsigned char bfType[2];//�ļ����ͣ�������0x424D�����ַ���BM��
	unsigned char bfSize[4];//�ļ���С
	unsigned char bfReserved1[2];//������
	unsigned char bfReserved2[2];//������
	unsigned char bfOffBits[4];//���ļ�ͷ��ʵ��λͼ���ݵ�ƫ���ֽ���
}BITMAPFILEHEADER_MY;

typedef struct tagBITMAPINFOHEADER_MY {
	DWORD_MY biSize;//��Ϣͷ��С
	LONG_MY biWidth;//ͼ����
	LONG_MY biHeight;//ͼ��߶�
	WORD_MY biPlanes;//λƽ����������Ϊ1
	WORD_MY biBitCount;//ÿ����λ��
	DWORD_MY  biCompression; //ѹ������
	DWORD_MY  biSizeImage; //ѹ��ͼ���С�ֽ���
	LONG_MY  biXPelsPerMeter; //ˮƽ�ֱ���
	LONG_MY  biYPelsPerMeter; //��ֱ�ֱ���
	DWORD_MY  biClrUsed; //λͼʵ���õ���ɫ����
	DWORD_MY  biClrImportant; //��λͼ����Ҫ��ɫ����
}BITMAPINFOHEADER_MY;

typedef struct tagRGBQUAD_MY {
	BYTE_MY rgbBlue; //����ɫ����ɫ����
	BYTE_MY rgbGreen; //����ɫ����ɫ����
	BYTE_MY rgbRed; //����ɫ�ĺ�ɫ����
	BYTE_MY rgbReserved; //����ֵ
}RGBQUAD_MY;

typedef struct tagIMAGEDATA_MY
{
	BYTE_MY blue;
	BYTE_MY green;
	BYTE_MY red;
}IMAGEDATA_MY;