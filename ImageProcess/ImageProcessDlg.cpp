
// ImageProcessDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "ImageProcess.h"
#include "ImageProcessDlg.h"
#include "afxdialogex.h"
#include "bmphead.h"
#include <fstream>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma warning(disable:4996)

// CImageProcessDlg 对话框

CImageProcessDlg::CImageProcessDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_IMAGEPROCESS_DIALOG, pParent)
	, MethodToDo(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CImageProcessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_COMBOMETHOD, MethodToDo);
	DDX_Control(pDX, IDC_COMBOMETHOD, comboboxmethod);
	DDX_Control(pDX, Image_Result, imageresult);
	DDX_Control(pDX, Image_SOurce, imagesource);
}

BEGIN_MESSAGE_MAP(CImageProcessDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CImageProcessDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CImageProcessDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CImageProcessDlg 消息处理程序

BOOL CImageProcessDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	CRect temprect(0, 0, 2400, 1350);
	CWnd::SetWindowPos(NULL, 0, 0, temprect.Width(), temprect.Height(), SWP_NOZORDER | SWP_NOMOVE);
	CWnd::CenterWindow();

	// 初始化图片
	CBitmap sourceBitmap;
	//sourceBitmap.LoadBitmap(IDB_BITMAP_SOURCE);
	sourceBitmap.m_hObject = LoadImage(NULL, "source.bmp", IMAGE_BITMAP, 500, 500, LR_LOADFROMFILE);
	CStatic *p = (CStatic *)GetDlgItem(Image_SOurce);
	p->SetBitmap((HBITMAP)sourceBitmap.Detach());

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CImageProcessDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CImageProcessDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CImageProcessDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	// 图片大小 500 * 500
	CString method(_T(""));
	comboboxmethod.GetLBText(comboboxmethod.GetCurSel(), method);

	BITMAP bm;
	CBitmap m_cBitmap;
	long width;
	long height;
	long widthBytes;

	// m_cBitmap.LoadBitmap(IDB_BITMAP_SOURCE);
	m_cBitmap.m_hObject = LoadImage(NULL, "source.bmp", IMAGE_BITMAP, 500, 500, LR_LOADFROMFILE);
	m_cBitmap.GetBitmap(&bm);
	height = bm.bmHeight;
	width = bm.bmWidth;
	widthBytes = bm.bmWidthBytes;

	/*
		char temp[1024];
		sprintf_s(temp, "%d", height);
		MessageBox(temp);
		*/

	if (method == "24位红色分离") {
		if (!m_cBitmap.m_hObject) {
			MessageBox("Error");
			return;
		}

		int size = height * widthBytes;
		BYTE *lpBits = new BYTE[size];
		m_cBitmap.GetBitmapBits(size, lpBits);
		BYTE *lpGray = new BYTE[height*widthBytes];
		int R, G, B;
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++)
			{
				B = lpBits[i*widthBytes + j * 4];
				G = lpBits[i*widthBytes + j * 4 + 1];
				R = lpBits[i*widthBytes + j * 4 + 2];
				lpGray[i*widthBytes + j * 4] = 0;
				lpGray[i*widthBytes + j * 4 + 1] = 0;
				lpGray[i*widthBytes + j * 4 + 2] = R;
			}
		}
		if (m_cBitmap.SetBitmapBits(height * widthBytes, lpGray) == 0) {
			MessageBox("Error");
		}

		imageresult.SetBitmap(m_cBitmap);
	}
	else if (method == "24位绿色分离") {
		if (!m_cBitmap.m_hObject) {
			MessageBox("Error");
			return;
		}

		int size = height * widthBytes;
		BYTE *lpBits = new BYTE[size];
		m_cBitmap.GetBitmapBits(size, lpBits);
		BYTE *lpGray = new BYTE[height*widthBytes];
		int R, G, B;
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++)
			{
				B = lpBits[i*widthBytes + j * 4];
				G = lpBits[i*widthBytes + j * 4 + 1];
				R = lpBits[i*widthBytes + j * 4 + 2];
				lpGray[i*widthBytes + j * 4] = 0;
				lpGray[i*widthBytes + j * 4 + 1] = G;
				lpGray[i*widthBytes + j * 4 + 2] = 0;
			}
		}
		if (m_cBitmap.SetBitmapBits(height * widthBytes, lpGray) == 0) {
			MessageBox("Error");
		}

		imageresult.SetBitmap(m_cBitmap);
	}
	else if (method == "24位蓝色分离") {
		if (!m_cBitmap.m_hObject) {
			MessageBox("Error");
			return;
		}

		int size = height * widthBytes;
		BYTE *lpBits = new BYTE[size];
		m_cBitmap.GetBitmapBits(size, lpBits);
		BYTE *lpGray = new BYTE[height*widthBytes];
		int R, G, B;
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++)
			{
				B = lpBits[i*widthBytes + j * 4];
				G = lpBits[i*widthBytes + j * 4 + 1];
				R = lpBits[i*widthBytes + j * 4 + 2];
				lpGray[i*widthBytes + j * 4] = B;
				lpGray[i*widthBytes + j * 4 + 1] = 0;
				lpGray[i*widthBytes + j * 4 + 2] = 0;
			}
		}
		if (m_cBitmap.SetBitmapBits(height * widthBytes, lpGray) == 0) {
			MessageBox("Error");
		}

		imageresult.SetBitmap(m_cBitmap);
	}
	else if (method == "24位彩色图灰度化") {
		if (!m_cBitmap.m_hObject) {
			MessageBox("Error");
			return;
		}

		int size = height * widthBytes;
		BYTE *lpBits = new BYTE[size];
		m_cBitmap.GetBitmapBits(size, lpBits);
		BYTE *lpGray = new BYTE[height*widthBytes];
		int R, G, B;
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++)
			{
				B = lpBits[i*widthBytes + j * 4];
				G = lpBits[i*widthBytes + j * 4 + 1];
				R = lpBits[i*widthBytes + j * 4 + 2];
				// lpGray[i*widthBytes + j * 4] = (B + G + R) / 3;
				// lpGray[i*widthBytes + j * 4 + 1] = (B + G + R) / 3;
				// lpGray[i*widthBytes + j * 4 + 2] = (B + G + R) / 3;
				lpGray[i*widthBytes + j * 4] = BYTE(R*0.299 + G*0.587 + B*0.114);
				lpGray[i*widthBytes + j * 4 + 1] = BYTE(R * 0.299 + G * 0.587 + B * 0.114);
				lpGray[i*widthBytes + j * 4 + 2] = BYTE(R * 0.299 + G * 0.587 + B * 0.114);
			}
		}
		if (m_cBitmap.SetBitmapBits(height * widthBytes, lpGray) == 0) {
			MessageBox("Error");
		}

		imageresult.SetBitmap(m_cBitmap);
	}
	else if (method == "8位灰度图反色") {
	if (!m_cBitmap.m_hObject) {
		MessageBox("Error");
		return;
	}

	int size = height * widthBytes;
	BYTE *lpBits = new BYTE[size];
	m_cBitmap.GetBitmapBits(size, lpBits);
	BYTE *lpGray = new BYTE[height*widthBytes];
	int R, G, B;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++)
		{
			B = lpBits[i*widthBytes + j * 4];
			G = lpBits[i*widthBytes + j * 4 + 1];
			R = lpBits[i*widthBytes + j * 4 + 2];
			lpGray[i*widthBytes + j * 4] = 255 - B;
			lpGray[i*widthBytes + j * 4 + 1] = 255 - G;
			lpGray[i*widthBytes + j * 4 + 2] = 255 - R;
		}
	}
	if (m_cBitmap.SetBitmapBits(height * widthBytes, lpGray) == 0) {
		MessageBox("Error");
	}

	imageresult.SetBitmap(m_cBitmap);
	}
	else if (method == "直方图统计") {
		int data[256];
		int size = height * widthBytes;
		BYTE *lpBits = new BYTE[size];
		m_cBitmap.GetBitmapBits(size, lpBits);
		
		for (int i = 0; i <= 255; i++) {
			data[i] = 0;
		}

		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				data[lpBits[i*widthBytes + j * 4]]++;
			}
		}
		ofstream saveFile("output.txt");
		for (int i = 0; i < 255; i++) {
			saveFile << (int)i;
			saveFile << ": ";
			saveFile << (int)data[i];
			saveFile << "; ";
			saveFile << 100*(float)data[i]/ (width*height);
			saveFile << "%";
			saveFile << endl;
		}
		saveFile.close();
		MessageBox("统计完成，已经保存到output.txt");
	}
	else if (method == "直方图均衡化") {
		BITMAPFILEHEADER bmpHead1;
		BITMAPINFOHEADER bmpInfo1;

		FILE * fp1 = fopen("source.bmp", "rb");

		fread(&bmpHead1, sizeof(BITMAPFILEHEADER), 1, fp1);
		fread(&bmpInfo1, sizeof(BITMAPINFOHEADER), 1, fp1);

		int height = bmpInfo1.biHeight, width = bmpInfo1.biWidth;
		if (bmpInfo1.biBitCount != 8)
		{
			MessageBox("这个图片不是8位图片");
		}
		else {
			int byteWidth = (width * bmpInfo1.biBitCount / 8 + 3) / 4 * 4;
			int size = byteWidth * height;

			RGBQUAD rgbquad[256];
			BYTE *source = new BYTE[size];
			BYTE *output = new BYTE[size];

			fread(rgbquad, sizeof(RGBQUAD), 256, fp1);
			fread(source, sizeof(BYTE), size, fp1);

			int data[256];
			int sumOfData = 0;
			for (int i = 0; i <= 255; i++) {
				data[i] = 0;
			}
			for (int i = 0; i < height; i++) {
				for (int j = 0; j < width; j++) {
					data[source[i*byteWidth + j * 4]]++;
				}
			}
			for (int i = 0; i <= 255; i++) {
				sumOfData += data[i];
			}
			float data_prob[256];
			data_prob[0] = (float)data[0] / sumOfData;
			for (int i = 1; i <= 255; i++) {
				data_prob[i] = data_prob[i - 1] + (float)data[i] / sumOfData;
			}
			float equal[256];
			for (int i = 0; i <= 255; i++) {
				equal[i] = (int)(data_prob[i] * 255 + 0.5);
			}

			int point = 0;
			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width; j++)
				{
					output[point] = equal[source[point]];
					point++;
				}
				while (point % 4 != 0)
				{
					output[point] = 0;
					point++;
				}
			}
			BYTE *lpBits = new BYTE[height*widthBytes];
			m_cBitmap.GetBitmapBits(height*widthBytes, lpBits);
			BYTE *lpGray = new BYTE[height*widthBytes];
			for (int i = 0; i < height; i++) {
				for (int j = 0; j < width; j++)
				{
					lpGray[i*widthBytes + j * 4] = equal[lpBits[i*widthBytes+j*4]];
					lpGray[i*widthBytes + j * 4 + 1] = equal[lpBits[i*widthBytes + j * 4 + 1]];
					lpGray[i*widthBytes + j * 4 + 2] = equal[lpBits[i*widthBytes + j * 4 + 2]];
				}
			}
			fclose(fp1);
			m_cBitmap.SetBitmapBits(height * widthBytes, lpGray);
			imageresult.SetBitmap(m_cBitmap);
		}
	}
	else {
		MessageBox("请使用合理的方法");
	}
}


void CImageProcessDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString method(_T(""));
	comboboxmethod.GetLBText(comboboxmethod.GetCurSel(), method);
	
	BITMAPFILEHEADER_MY bmpHead;
	BITMAPINFOHEADER_MY bmpInfo;
	RGBQUAD_MY bmpPla[256];

	BYTE_MY *imageDataInput = NULL;
	BYTE_MY *imageDataOutput = NULL;
	int width, height;

	FILE *fp  = fopen("source.bmp", "rb");
	if (fp == NULL) {
		MessageBox("读取文件错误");
		return;
	}
	
	fread(&bmpHead, 1, 14, fp);
	fread(&bmpInfo, 1, sizeof(tagBITMAPINFOHEADER_MY), fp);

	for (unsigned int nCounti = 0; nCounti < bmpInfo.biClrUsed; nCounti++)
	{
		fread((char *)&(bmpPla[nCounti].rgbBlue), 1, sizeof(BYTE_MY), fp);
		fread((char *)&(bmpPla[nCounti].rgbGreen), 1, sizeof(BYTE_MY), fp);
		fread((char *)&(bmpPla[nCounti].rgbRed), 1, sizeof(BYTE_MY), fp);
		fread((char *)&(bmpPla[nCounti].rgbReserved), 1, sizeof(BYTE_MY), fp);
	}

	width = bmpInfo.biWidth;
	height = bmpInfo.biHeight;
	width = (width * sizeof(IMAGEDATA_MY) + 3) / 4 * 4;
	imageDataInput = (BYTE_MY*)malloc(width * height * sizeof(IMAGEDATA_MY));
	imageDataOutput = (BYTE_MY*)malloc(width * height * sizeof(IMAGEDATA_MY));

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			(*(imageDataInput + i * width + j)) = 0;
			(*(imageDataOutput + i * width + j)) = 0;
		}
	}

	fread(imageDataInput, sizeof(struct tagIMAGEDATA_MY) * width, height, fp);
	fclose(fp);

	FILE * fpw;
	if ((fpw = fopen("output.bmp", "wb")) == NULL)
	{
		MessageBox("不能创建文件");
		return;
	}
	fwrite(&bmpHead, 1, 14, fpw);
	fwrite(&bmpInfo, 1, sizeof(tagBITMAPINFOHEADER_MY), fpw);
	for (unsigned int nCounti = 0; nCounti < bmpInfo.biClrUsed; nCounti++)
	{
		fwrite(&bmpPla[nCounti].rgbBlue, 1, sizeof(BYTE_MY), fpw);
		fwrite(&bmpPla[nCounti].rgbGreen, 1, sizeof(BYTE_MY), fpw);
		fwrite(&bmpPla[nCounti].rgbRed, 1, sizeof(BYTE_MY), fpw);
		fwrite(&bmpPla[nCounti].rgbReserved, 1, sizeof(BYTE_MY), fpw);
	}

	if (method == "24位红色分离") {
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				if (j % 3 == 0) {
					BYTE_MY temp = 0;
					fwrite(&temp, 1, sizeof(BYTE_MY), fpw);
				}
				if (j % 3 == 1) {
					BYTE_MY temp = 0;
					fwrite(&temp, 1, sizeof(BYTE_MY), fpw);
				}
				if (j % 3 == 2) {
					fwrite(&(*(imageDataInput + i * width+ j)), 1, sizeof(BYTE_MY), fpw);
				}
			}
		}
	}
	else if (method == "24位绿色分离") {
		for (int i = 0; i < height; ++i)
		{
			for (int j = 0; j < width; ++j)
			{
				if (j % 3 == 0) {
					BYTE_MY temp = 0;
					fwrite(&temp, 1, sizeof(BYTE_MY), fpw);
				}
				if (j % 3 == 1) {
					fwrite(&(*(imageDataInput + i * width + j)), 1, sizeof(BYTE_MY), fpw);
				}
				if (j % 3 == 2) {
					BYTE_MY temp = 0;
					fwrite(&temp, 1, sizeof(BYTE_MY), fpw);
				}
			}
		}
	}
	else if (method == "24位蓝色分离") {
		for (int i = 0; i < height; ++i)
		{
			for (int j = 0; j < width; ++j)
			{
				if (j % 3 == 0) {
					fwrite(&(*(imageDataInput + i * width + j)), 1, sizeof(BYTE_MY), fpw);					
				}
				if (j % 3 == 1) {
					BYTE_MY temp = 0;
					fwrite(&temp, 1, sizeof(BYTE_MY), fpw);
				}
				if (j % 3 == 2) {
					BYTE_MY temp = 0;
					fwrite(&temp, 1, sizeof(BYTE_MY), fpw);
				}
			}
		}
	}
	else if (method == "24位彩色图灰度化") {
		for (int i = 0; i < height; ++i)
		{
			for (int j = 0; j < width; ++j)
			{
				if (j % 3 == 0) {
					BYTE_MY temp = (BYTE_MY)((*(imageDataInput + i * width + j + 2)) * 0.299) + (BYTE_MY)((*(imageDataInput + i * width + j + 1))* 0.587) + (BYTE_MY)((*(imageDataInput + i * width + j)) * 0.114);
					fwrite(&temp, 1, sizeof(BYTE_MY), fpw);
				}
				if (j % 3 == 1) {
					BYTE_MY temp = (BYTE_MY)((*(imageDataInput + i * width + j + 1)) * 0.299) + (BYTE_MY)((*(imageDataInput + i * width + j))* 0.587) + (BYTE_MY)((*(imageDataInput + i * width + j - 1)) * 0.114);
					fwrite(&temp, 1, sizeof(BYTE_MY), fpw);
				}
				if (j % 3 == 2) {
					BYTE_MY temp = (BYTE_MY)((*(imageDataInput + i * width + j)) * 0.299) + (BYTE_MY)((*(imageDataInput + i * width + j - 1))* 0.587) + (BYTE_MY)((*(imageDataInput + i * width + j -2)) * 0.114);
					fwrite(&temp, 1, sizeof(BYTE_MY), fpw);
				}
			}
		}
	}
	else if (method == "24位彩色图反色") {
		for (int i = 0; i < height; ++i)
		{
			for (int j = 0; j < width; ++j)
			{
				BYTE_MY temp = 255 - (*(imageDataInput + i * width + j));
				fwrite(&temp, 1, sizeof(BYTE_MY), fpw);
			}
		}
	}
	else if (method == "8位灰度图反色") {
		BITMAPFILEHEADER bmpHead1;
		BITMAPINFOHEADER bmpInfo1;

		FILE * fp1 = fopen("source.bmp", "rb");

		fread(&bmpHead1, sizeof(BITMAPFILEHEADER), 1, fp1);
		fread(&bmpInfo1, sizeof(BITMAPINFOHEADER), 1, fp1);

		int height = bmpInfo1.biHeight, width = bmpInfo1.biWidth;
		if (bmpInfo1.biBitCount != 8)
		{
			MessageBox("这个图片不是8位图片");
		}
		else {
			int byteWidth = (width * bmpInfo1.biBitCount / 8 + 3) / 4 * 4;
			int size = byteWidth * height;

			RGBQUAD rgbquad[256];
			BYTE *source = new BYTE[size];
			BYTE *output = new BYTE[size];

			fread(rgbquad, sizeof(RGBQUAD), 256, fp1);
			fread(source, sizeof(BYTE), size, fp1);
			int point = 0;
			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width; j++)
				{
					output[point] = static_cast<byte>(255) - source[point];
					point++;
				}
				while (point % 4 != 0)
				{
					output[point] = 0;
					point++;
				}

			}

			FILE * fpw1 = fopen("output.bmp", "wb");

			fwrite(&bmpHead1, sizeof(BITMAPFILEHEADER), 1, fpw1);
			fwrite(&bmpInfo1, sizeof(BITMAPINFOHEADER), 1, fpw1);
			fwrite(&rgbquad, 4 * 256, 1, fpw1);
			fwrite(output, sizeof(BYTE), size, fpw1);

			fclose(fpw1);
			fclose(fp1);
		}
	}
	else if (method == "直方图统计") {
		BITMAP bm;
		CBitmap m_cBitmap;
		long width;
		long height;
		long widthBytes;

		m_cBitmap.m_hObject = LoadImage(NULL, "source.bmp", IMAGE_BITMAP, 500, 500, LR_LOADFROMFILE);
		m_cBitmap.GetBitmap(&bm);
		height = bm.bmHeight;
		width = bm.bmWidth;
		widthBytes = bm.bmWidthBytes;
		int data[256];
		int size = height * widthBytes;
		BYTE *lpBits = new BYTE[size];
		m_cBitmap.GetBitmapBits(size, lpBits);

		for (int i = 0; i <= 255; i++) {
			data[i] = 0;
		}

		int sum = 0;

		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				data[lpBits[i*widthBytes + j * 4]]++;
			}
		}

		for (int i = 0; i <= 255; i++) {
			sum += data[i];
		}

		BITMAPFILEHEADER bmpHead1;
		BITMAPINFOHEADER bmpInfo1;

		FILE * fp1 = fopen("info.bmp", "rb");

		fread(&bmpHead1, sizeof(BITMAPFILEHEADER), 1, fp1);
		fread(&bmpInfo1, sizeof(BITMAPINFOHEADER), 1, fp1);

		int height1 = bmpInfo1.biHeight, width1 = bmpInfo1.biWidth;
		if (bmpInfo1.biBitCount != 8)
		{
			MessageBox("这个图片不是8位图片");
		}
		else {
			int byteWidth1 = (width1 * bmpInfo1.biBitCount / 8 + 3) / 4 * 4;
			int size = byteWidth1 * height1;

			RGBQUAD rgbquad[256];
			BYTE *source = new BYTE[size];
			BYTE *output = new BYTE[size];

			fread(rgbquad, sizeof(RGBQUAD), 256, fp1);
			fread(source, sizeof(BYTE), size, fp1);
			fclose(fp1);

			int point = 0;
			for (int i = 0; i < height1; i++)
			{
				for (int j = 0; j < width1; j++)
				{
					if (j <= 100*data[i]/sum) {
						output[point] = 0;
					}
					else {
						output[point] = 255;
					}
					point++;
				}
				while (point % 4 != 0)
				{
					output[point] = 0;
					point++;
				}

			}

			FILE * fpw1 = fopen("info.bmp", "wb");

			fwrite(&bmpHead1, sizeof(BITMAPFILEHEADER), 1, fpw1);
			fwrite(&bmpInfo1, sizeof(BITMAPINFOHEADER), 1, fpw1);
			fwrite(&rgbquad, 4 * 256, 1, fpw1);
			fwrite(output, sizeof(BYTE), size, fpw1);

			fclose(fpw1);
		}

	}
	else if (method == "直方图均衡化") {
		BITMAPFILEHEADER bmpHead1;
		BITMAPINFOHEADER bmpInfo1;

		FILE * fp1 = fopen("source.bmp", "rb");

		fread(&bmpHead1, sizeof(BITMAPFILEHEADER), 1, fp1);
		fread(&bmpInfo1, sizeof(BITMAPINFOHEADER), 1, fp1);

		int height = bmpInfo1.biHeight, width = bmpInfo1.biWidth;
		if (bmpInfo1.biBitCount != 8)
		{
			MessageBox("这个图片不是8位图片");
		}
		else {
			int byteWidth = (width * bmpInfo1.biBitCount / 8 + 3) / 4 * 4;
			int size = byteWidth * height;

			RGBQUAD rgbquad[256];
			BYTE *source = new BYTE[size];
			BYTE *output = new BYTE[size];

			fread(rgbquad, sizeof(RGBQUAD), 256, fp1);
			fread(source, sizeof(BYTE), size, fp1);

			int data[256];
			int sumOfData = 0;
			for (int i = 0; i <= 255; i++) {
				data[i] = 0;
			}
			for (int i = 0; i < height; i++) {
				for (int j = 0; j < width; j++) {
					data[source[i*byteWidth + j * 4]]++;
				}
			}
			for (int i = 0; i <= 255; i++) {
				sumOfData += data[i];
			}
			float data_prob[256];
			data_prob[0] = (float)data[0] / sumOfData;
			for (int i = 1; i <= 255; i++) {
				data_prob[i] = data_prob[i-1]+(float)data[i] / sumOfData;
			}
			float equal[256];
			for (int i = 0; i <= 255; i++) {
				equal[i] = (int)(data_prob[i] * 255 + 0.5);
			}

			int point = 0;
			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width; j++)
				{
					output[point] = equal[source[point]];
					point++;
				}
				while (point % 4 != 0)
				{
					output[point] = 0;
					point++;
				}

			}

			FILE * fpw1 = fopen("output.bmp", "wb");

			fwrite(&bmpHead1, sizeof(BITMAPFILEHEADER), 1, fpw1);
			fwrite(&bmpInfo1, sizeof(BITMAPINFOHEADER), 1, fpw1);
			fwrite(&rgbquad, 4 * 256, 1, fpw1);
			fwrite(output, sizeof(BYTE), size, fpw1);

			fclose(fpw1);
			fclose(fp1);
		}
	}
	else {
		MessageBox("请使用合理的方法");
	}

	fclose(fpw);

	delete[] imageDataInput;
	delete[] imageDataOutput;

	
	MessageBox("文件保存成功");
}


