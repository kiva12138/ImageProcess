
// ImageProcessDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "ImageProcess.h"
#include "ImageProcessDlg.h"
#include "afxdialogex.h"
#include "bmphead.h"
#include <algorithm>
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
	DDX_Control(pDX, IDC_EDIT1, sizeChange);
	DDX_Control(pDX, IDC_EDIT2, leftMove);
	DDX_Control(pDX, IDC_EDIT3, upMove);
	DDX_Control(pDX, IDC_EDIT4, rotateSize);
	DDX_Control(pDX, Given_Divid, given_divide);
}

BEGIN_MESSAGE_MAP(CImageProcessDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CImageProcessDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CImageProcessDlg::OnBnClickedButton2)
	ON_CBN_SELCHANGE(IDC_COMBOMETHOD, &CImageProcessDlg::OnCbnSelchangeCombomethod)
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
	else if (method == "平均处理") {
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
					int n1, n2, n3, n4, n5, n6, n7, n8;
					if (j == 0 && i == 0) {
						n1 = source[point];
						n2 = source[point];
						n3 = source[point + 1];
						n4 = source[point];
						n5 = source[point + 1];
						n6 = source[point + width];
						n7 = source[point + width];
						n8 = source[point + width + 1];
					}
					else if (i == 0 && j > 0 && j < width - 1) {
						n1 = source[point - 1];
						n2 = source[point];
						n3 = source[point + 1];
						n4 = source[point - 1];
						n5 = source[point + 1];
						n6 = source[point + width - 1];
						n7 = source[point + width];
						n8 = source[point + width + 1];
					}
					else if (i == 0 && j == width - 1) {
						n1 = source[point - 1];
						n2 = source[point];
						n3 = source[point];
						n4 = source[point - 1];
						n5 = source[point];
						n6 = source[point + width - 1];
						n7 = source[point + width];
						n8 = source[point + width];
					}
					else if (j == 0 && i > 0 && i < height - 1) {
						n1 = source[point - width];
						n2 = source[point - width];
						n3 = source[point - width + 1];
						n4 = source[point];
						n5 = source[point + 1];
						n6 = source[point + width];
						n7 = source[point + width];
						n8 = source[point + width + 1];
					}
					else if (j == width - 1 && i > 0 && i < height - 1) {
						n1 = source[point - width - 1];
						n2 = source[point - width];
						n3 = source[point - width];
						n4 = source[point - 1];
						n5 = source[point];
						n6 = source[point + width - 1];
						n7 = source[point + width];
						n8 = source[point + width];
					}
					else if (j == 0 && i == height - 1) {
						n1 = source[point - width];
						n2 = source[point - width];
						n3 = source[point - width + 1];
						n4 = source[point];
						n5 = source[point + 1];
						n6 = source[point];
						n7 = source[point];
						n8 = source[point + 1];
					}
					else if (j == width - 1 && i == height - 1) {
						n1 = source[point - width - 1];
						n2 = source[point - width];
						n3 = source[point - width];
						n4 = source[point - 1];
						n5 = source[point];
						n6 = source[point - 1];
						n7 = source[point];
						n8 = source[point];
					}
					else if (j > 0 && j < width - 1 && i == height - 1) {
						n1 = source[point - width - 1];
						n2 = source[point - width];
						n3 = source[point - width + 1];
						n4 = source[point - 1];
						n5 = source[point + 1];
						n6 = source[point - 1];
						n7 = source[point];
						n8 = source[point + 1];
					}
					else {
						n1 = source[point - width - 1];
						n2 = source[point - width];
						n3 = source[point - width + 1];
						n4 = source[point - 1];
						n5 = source[point + 1];
						n6 = source[point + width - 1];
						n7 = source[point + width];
						n8 = source[point + width + 1];
					}
					output[point] = (int)(source[point] + n1 + n2 + n3 + n4 + n5 + n6 + n7 + n8) / 9;
					point++;
				}
				while (point % 4 != 0)
				{
					output[point] = 0;
					point++;
				}

			}

			fclose(fp1);

			
			BYTE *lpGray = new BYTE[height*widthBytes];
			for (int i = 0; i < height; i++) {
				for (int j = 0; j < width; j++)
				{
					lpGray[(height-i-1)*widthBytes + j * 4] = output[i*width + j ];
					lpGray[(height - i-1)*widthBytes + j * 4 + 1] = output[i*width + j];
					lpGray[(height - i-1)*widthBytes + j * 4 + 2] = output[i*width + j];
				}
			}
			m_cBitmap.SetBitmapBits(height * widthBytes, lpGray);
			imageresult.SetBitmap(m_cBitmap);
			delete[] lpGray;
		}
	}
	else if (method == "中值滤波") {
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
					int n[9];
					if (j == 0 && i == 0) {
						n[1] = source[point];
						n[2] = source[point];
						n[3] = source[point + 1];
						n[4] = source[point];
						n[5] = source[point + 1];
						n[6] = source[point + width];
						n[7] = source[point + width];
						n[8] = source[point + width + 1];
					}
					else if (i == 0 && j > 0 && j < width - 1) {
						n[1] = source[point - 1];
						n[2] = source[point];
						n[3] = source[point + 1];
						n[4] = source[point - 1];
						n[5] = source[point + 1];
						n[6] = source[point + width - 1];
						n[7] = source[point + width];
						n[8] = source[point + width + 1];
					}
					else if (i == 0 && j == width - 1) {
						n[1] = source[point - 1];
						n[2] = source[point];
						n[3] = source[point];
						n[4] = source[point - 1];
						n[5] = source[point];
						n[6] = source[point + width - 1];
						n[7] = source[point + width];
						n[8] = source[point + width];
					}
					else if (j == 0 && i > 0 && i < height - 1) {
						n[1] = source[point - width];
						n[2] = source[point - width];
						n[3] = source[point - width + 1];
						n[4] = source[point];
						n[5] = source[point + 1];
						n[6] = source[point + width];
						n[7] = source[point + width];
						n[8] = source[point + width + 1];
					}
					else if (j == width - 1 && i > 0 && i < height - 1) {
						n[1] = source[point - width - 1];
						n[2] = source[point - width];
						n[3] = source[point - width];
						n[4] = source[point - 1];
						n[5] = source[point];
						n[6] = source[point + width - 1];
						n[7] = source[point + width];
						n[8] = source[point + width];
					}
					else if (j == 0 && i == height - 1) {
						n[1] = source[point - width];
						n[2] = source[point - width];
						n[3] = source[point - width + 1];
						n[4] = source[point];
						n[5] = source[point + 1];
						n[6] = source[point];
						n[7] = source[point];
						n[8] = source[point + 1];
					}
					else if (j == width - 1 && i == height - 1) {
						n[1] = source[point - width - 1];
						n[2] = source[point - width];
						n[3] = source[point - width];
						n[4] = source[point - 1];
						n[5] = source[point];
						n[6] = source[point - 1];
						n[7] = source[point];
						n[8] = source[point];
					}
					else if (j > 0 && j < width - 1 && i == height - 1) {
						n[1] = source[point - width - 1];
						n[2] = source[point - width];
						n[3] = source[point - width + 1];
						n[4] = source[point - 1];
						n[5] = source[point + 1];
						n[6] = source[point - 1];
						n[7] = source[point];
						n[8] = source[point + 1];
					}
					else {
						n[1] = source[point - width - 1];
						n[2] = source[point - width];
						n[3] = source[point - width + 1];
						n[4] = source[point - 1];
						n[5] = source[point + 1];
						n[6] = source[point + width - 1];
						n[7] = source[point + width];
						n[8] = source[point + width + 1];
					}
					n[0] = source[point];
					sort(n, n + 8);
					output[point] = n[4];
					point++;
				}
				while (point % 4 != 0)
				{
					output[point] = 0;
					point++;
				}

			}

			fclose(fp1);


			BYTE *lpGray = new BYTE[height*widthBytes];
			for (int i = 0; i < height; i++) {
				for (int j = 0; j < width; j++)
				{
					lpGray[(height - i - 1)*widthBytes + j * 4] = output[i*width + j];
					lpGray[(height - i - 1)*widthBytes + j * 4 + 1] = output[i*width + j];
					lpGray[(height - i - 1)*widthBytes + j * 4 + 2] = output[i*width + j];
				}
			}
			m_cBitmap.SetBitmapBits(height * widthBytes, lpGray);
			imageresult.SetBitmap(m_cBitmap);
			delete[] lpGray;
		}
	}
	else if (method == "图像缩放") {
		CString str = "该操作请使用保存文件查看";
		MessageBox(str);
	}
	else if (method == "图像平移") {
		CString str;
		int sizeToMoveLeft;
		int sizeToMoveUp;
		GetDlgItemText(IDC_EDIT2, str);
		sizeToMoveLeft = _ttoi(str);
		GetDlgItemText(IDC_EDIT3, str);
		sizeToMoveUp = _ttoi(str);
		BITMAPFILEHEADER bmpHeadSize;
		BITMAPINFOHEADER bmpInfoSize;

		FILE * fp1 = fopen("source.bmp", "rb");

		fread(&bmpHeadSize, sizeof(BITMAPFILEHEADER), 1, fp1);
		fread(&bmpInfoSize, sizeof(BITMAPINFOHEADER), 1, fp1);

		int heightOfSize = bmpInfoSize.biHeight, widthOfSize = bmpInfoSize.biWidth;
		if (bmpInfoSize.biBitCount != 8)
		{
			MessageBox("这个图片不是8位图片");
		}
		else {
			int byteWidthOfSize = (widthOfSize * bmpInfoSize.biBitCount / 8 + 3) / 4 * 4;
			int sizeOfSize = byteWidthOfSize * heightOfSize;

			RGBQUAD rgbquad[256];
			BYTE *source = new BYTE[sizeOfSize];
			BYTE *output = new BYTE[sizeOfSize];

			fread(rgbquad, sizeof(RGBQUAD), 256, fp1);
			fread(source, sizeof(BYTE), sizeOfSize, fp1);

			int point = 0;
			for (int i = 0; i < bmpInfoSize.biHeight; i++)
			{
				for (int j = 0; j < bmpInfoSize.biWidth; j++)
				{
					if (j - sizeToMoveLeft >= 0 && j - sizeToMoveLeft <= bmpInfoSize.biWidth
						&& i - sizeToMoveUp >= 0 && i - sizeToMoveUp <= bmpInfoSize.biHeight) {
						output[point] = source[(i - sizeToMoveUp)*byteWidthOfSize + j - sizeToMoveLeft];
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

			BYTE *lpGray = new BYTE[height*widthBytes];
			for (int i = 0; i < height; i++) {
				for (int j = 0; j < width; j++)
				{
					lpGray[(height - i - 1)*widthBytes + j * 4] = output[i*width + j];
					lpGray[(height - i - 1)*widthBytes + j * 4 + 1] = output[i*width + j];
					lpGray[(height - i - 1)*widthBytes + j * 4 + 2] = output[i*width + j];
				}
			}
			m_cBitmap.SetBitmapBits(height * widthBytes, lpGray);
			imageresult.SetBitmap(m_cBitmap);
			delete[] lpGray;

			fclose(fp1);
		}
	}
	else if (method == "水平镜像") {
	BITMAPFILEHEADER bmpHeadSize;
	BITMAPINFOHEADER bmpInfoSize;

	FILE * fp1 = fopen("source.bmp", "rb");

	fread(&bmpHeadSize, sizeof(BITMAPFILEHEADER), 1, fp1);
	fread(&bmpInfoSize, sizeof(BITMAPINFOHEADER), 1, fp1);

	int heightOfSize = bmpInfoSize.biHeight, widthOfSize = bmpInfoSize.biWidth;
	if (bmpInfoSize.biBitCount != 8)
	{
		MessageBox("这个图片不是8位图片");
	}
	else {
		int byteWidthOfSize = (widthOfSize * bmpInfoSize.biBitCount / 8 + 3) / 4 * 4;
		int sizeOfSize = byteWidthOfSize * heightOfSize;

		RGBQUAD rgbquad[256];
		BYTE *source = new BYTE[sizeOfSize];
		BYTE *output = new BYTE[sizeOfSize];

		fread(rgbquad, sizeof(RGBQUAD), 256, fp1);
		fread(source, sizeof(BYTE), sizeOfSize, fp1);

		int point = 0;
		for (int i = 0; i < bmpInfoSize.biHeight; i++)
		{
			for (int j = 0; j < bmpInfoSize.biWidth; j++)
			{
				output[point] = source[i*byteWidthOfSize + bmpInfoSize.biWidth - j];
				point++;
			}
			while (point % 4 != 0)
			{
				output[point] = 0;
				point++;
			}

		}

		BYTE *lpGray = new BYTE[height*widthBytes];
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++)
			{
				lpGray[(height - i - 1)*widthBytes + j * 4] = output[i*width + j];
				lpGray[(height - i - 1)*widthBytes + j * 4 + 1] = output[i*width + j];
				lpGray[(height - i - 1)*widthBytes + j * 4 + 2] = output[i*width + j];
			}
		}
		m_cBitmap.SetBitmapBits(height * widthBytes, lpGray);
		imageresult.SetBitmap(m_cBitmap);
		delete[] lpGray;
		fclose(fp1);
	}
	}
	else if (method == "垂直镜像") {
	BITMAPFILEHEADER bmpHeadSize;
	BITMAPINFOHEADER bmpInfoSize;

	FILE * fp1 = fopen("source.bmp", "rb");

	fread(&bmpHeadSize, sizeof(BITMAPFILEHEADER), 1, fp1);
	fread(&bmpInfoSize, sizeof(BITMAPINFOHEADER), 1, fp1);

	int heightOfSize = bmpInfoSize.biHeight, widthOfSize = bmpInfoSize.biWidth;
	if (bmpInfoSize.biBitCount != 8)
	{
		MessageBox("这个图片不是8位图片");
	}
	else {
		int byteWidthOfSize = (widthOfSize * bmpInfoSize.biBitCount / 8 + 3) / 4 * 4;
		int sizeOfSize = byteWidthOfSize * heightOfSize;

		RGBQUAD rgbquad[256];
		BYTE *source = new BYTE[sizeOfSize];
		BYTE *output = new BYTE[sizeOfSize];

		fread(rgbquad, sizeof(RGBQUAD), 256, fp1);
		fread(source, sizeof(BYTE), sizeOfSize, fp1);

		int point = 0;
		for (int i = 0; i < bmpInfoSize.biHeight; i++)
		{
			for (int j = 0; j < bmpInfoSize.biWidth; j++)
			{
				output[point] = source[(bmpInfoSize.biHeight - i - 1)*byteWidthOfSize + j];
				point++;
			}
			while (point % 4 != 0)
			{
				output[point] = 0;
				point++;
			}

		}

		BYTE *lpGray = new BYTE[height*widthBytes];
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++)
			{
				lpGray[(height - i - 1)*widthBytes + j * 4] = output[i*width + j];
				lpGray[(height - i - 1)*widthBytes + j * 4 + 1] = output[i*width + j];
				lpGray[(height - i - 1)*widthBytes + j * 4 + 2] = output[i*width + j];
			}
		}
		m_cBitmap.SetBitmapBits(height * widthBytes, lpGray);
		imageresult.SetBitmap(m_cBitmap);
		delete[] lpGray;
		fclose(fp1);
	}
	}
	else if (method == "图像旋转") {
		CString str = "该操作请使用保存文件查看";
		MessageBox(str);
	}
	else if (method == "Prewitt处理") {
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
				int n1, n2, n3, n4, n5, n6, n7, n8;
				if (j == 0 && i == 0) {
					n1 = source[point];
					n2 = source[point];
					n3 = source[point + 1];
					n4 = source[point];
					n5 = source[point + 1];
					n6 = source[point + width];
					n7 = source[point + width];
					n8 = source[point + width + 1];
				}
				else if (i == 0 && j > 0 && j < width - 1) {
					n1 = source[point - 1];
					n2 = source[point];
					n3 = source[point + 1];
					n4 = source[point - 1];
					n5 = source[point + 1];
					n6 = source[point + width - 1];
					n7 = source[point + width];
					n8 = source[point + width + 1];
				}
				else if (i == 0 && j == width - 1) {
					n1 = source[point - 1];
					n2 = source[point];
					n3 = source[point];
					n4 = source[point - 1];
					n5 = source[point];
					n6 = source[point + width - 1];
					n7 = source[point + width];
					n8 = source[point + width];
				}
				else if (j == 0 && i > 0 && i < height - 1) {
					n1 = source[point - width];
					n2 = source[point - width];
					n3 = source[point - width + 1];
					n4 = source[point];
					n5 = source[point + 1];
					n6 = source[point + width];
					n7 = source[point + width];
					n8 = source[point + width + 1];
				}
				else if (j == width - 1 && i > 0 && i < height - 1) {
					n1 = source[point - width - 1];
					n2 = source[point - width];
					n3 = source[point - width];
					n4 = source[point - 1];
					n5 = source[point];
					n6 = source[point + width - 1];
					n7 = source[point + width];
					n8 = source[point + width];
				}
				else if (j == 0 && i == height - 1) {
					n1 = source[point - width];
					n2 = source[point - width];
					n3 = source[point - width + 1];
					n4 = source[point];
					n5 = source[point + 1];
					n6 = source[point];
					n7 = source[point];
					n8 = source[point + 1];
				}
				else if (j == width - 1 && i == height - 1) {
					n1 = source[point - width - 1];
					n2 = source[point - width];
					n3 = source[point - width];
					n4 = source[point - 1];
					n5 = source[point];
					n6 = source[point - 1];
					n7 = source[point];
					n8 = source[point];
				}
				else if (j > 0 && j < width - 1 && i == height - 1) {
					n1 = source[point - width - 1];
					n2 = source[point - width];
					n3 = source[point - width + 1];
					n4 = source[point - 1];
					n5 = source[point + 1];
					n6 = source[point - 1];
					n7 = source[point];
					n8 = source[point + 1];
				}
				else {
					n1 = source[point - width - 1];
					n2 = source[point - width];
					n3 = source[point - width + 1];
					n4 = source[point - 1];
					n5 = source[point + 1];
					n6 = source[point + width - 1];
					n7 = source[point + width];
					n8 = source[point + width + 1];
				}
				output[point] = (int)(source[point] - n1 + n3 - n4 + n5 - n6 + n8) / 9;
				point++;
			}
			while (point % 4 != 0)
			{
				output[point] = 0;
				point++;
			}

		}

		BYTE *lpGray = new BYTE[height*widthBytes];
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++)
			{
				lpGray[(height - i - 1)*widthBytes + j * 4] = output[i*width + j];
				lpGray[(height - i - 1)*widthBytes + j * 4 + 1] = output[i*width + j];
				lpGray[(height - i - 1)*widthBytes + j * 4 + 2] = output[i*width + j];
			}
		}
		m_cBitmap.SetBitmapBits(height * widthBytes, lpGray);
		imageresult.SetBitmap(m_cBitmap);
		delete[] lpGray;
		fclose(fp1);
	}
	}
	else if (method == "Sobel处理") {
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
				int n1, n2, n3, n4, n5, n6, n7, n8;
				if (j == 0 && i == 0) {
					n1 = source[point];
					n2 = source[point];
					n3 = source[point + 1];
					n4 = source[point];
					n5 = source[point + 1];
					n6 = source[point + width];
					n7 = source[point + width];
					n8 = source[point + width + 1];
				}
				else if (i == 0 && j > 0 && j < width - 1) {
					n1 = source[point - 1];
					n2 = source[point];
					n3 = source[point + 1];
					n4 = source[point - 1];
					n5 = source[point + 1];
					n6 = source[point + width - 1];
					n7 = source[point + width];
					n8 = source[point + width + 1];
				}
				else if (i == 0 && j == width - 1) {
					n1 = source[point - 1];
					n2 = source[point];
					n3 = source[point];
					n4 = source[point - 1];
					n5 = source[point];
					n6 = source[point + width - 1];
					n7 = source[point + width];
					n8 = source[point + width];
				}
				else if (j == 0 && i > 0 && i < height - 1) {
					n1 = source[point - width];
					n2 = source[point - width];
					n3 = source[point - width + 1];
					n4 = source[point];
					n5 = source[point + 1];
					n6 = source[point + width];
					n7 = source[point + width];
					n8 = source[point + width + 1];
				}
				else if (j == width - 1 && i > 0 && i < height - 1) {
					n1 = source[point - width - 1];
					n2 = source[point - width];
					n3 = source[point - width];
					n4 = source[point - 1];
					n5 = source[point];
					n6 = source[point + width - 1];
					n7 = source[point + width];
					n8 = source[point + width];
				}
				else if (j == 0 && i == height - 1) {
					n1 = source[point - width];
					n2 = source[point - width];
					n3 = source[point - width + 1];
					n4 = source[point];
					n5 = source[point + 1];
					n6 = source[point];
					n7 = source[point];
					n8 = source[point + 1];
				}
				else if (j == width - 1 && i == height - 1) {
					n1 = source[point - width - 1];
					n2 = source[point - width];
					n3 = source[point - width];
					n4 = source[point - 1];
					n5 = source[point];
					n6 = source[point - 1];
					n7 = source[point];
					n8 = source[point];
				}
				else if (j > 0 && j < width - 1 && i == height - 1) {
					n1 = source[point - width - 1];
					n2 = source[point - width];
					n3 = source[point - width + 1];
					n4 = source[point - 1];
					n5 = source[point + 1];
					n6 = source[point - 1];
					n7 = source[point];
					n8 = source[point + 1];
				}
				else {
					n1 = source[point - width - 1];
					n2 = source[point - width];
					n3 = source[point - width + 1];
					n4 = source[point - 1];
					n5 = source[point + 1];
					n6 = source[point + width - 1];
					n7 = source[point + width];
					n8 = source[point + width + 1];
				}
				output[point] = (int)(source[point] - n1 + n3 - 2 * n4 + 2 * n5 - n6 + n8) / 9;
				point++;
			}
			while (point % 4 != 0)
			{
				output[point] = 0;
				point++;
			}

		}

		BYTE *lpGray = new BYTE[height*widthBytes];
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++)
			{
				lpGray[(height - i - 1)*widthBytes + j * 4] = output[i*width + j];
				lpGray[(height - i - 1)*widthBytes + j * 4 + 1] = output[i*width + j];
				lpGray[(height - i - 1)*widthBytes + j * 4 + 2] = output[i*width + j];
			}
		}
		m_cBitmap.SetBitmapBits(height * widthBytes, lpGray);
		imageresult.SetBitmap(m_cBitmap);
		delete[] lpGray;
		fclose(fp1);
	}
	}
	else if (method == "给定阈值分割") {
	CString str;
	int given_number;
	GetDlgItemText(Given_Divid, str);
	given_number = _ttoi(str);

	BITMAPFILEHEADER bmpHeadSize;
	BITMAPINFOHEADER bmpInfoSize;

	FILE * fp2 = fopen("source.bmp", "rb");

	fread(&bmpHeadSize, sizeof(BITMAPFILEHEADER), 1, fp2);
	fread(&bmpInfoSize, sizeof(BITMAPINFOHEADER), 1, fp2);

	int heightOfSize = bmpInfoSize.biHeight, widthOfSize = bmpInfoSize.biWidth;
	if (bmpInfoSize.biBitCount != 8)
	{
		MessageBox("这个图片不是8位图片");
	}
	else {
		int byteWidthOfSize = (widthOfSize * bmpInfoSize.biBitCount / 8 + 3) / 4 * 4;
		int sizeOfSize = byteWidthOfSize * heightOfSize;

		RGBQUAD rgbquad[256];
		BYTE *source = new BYTE[sizeOfSize];
		BYTE *output = new BYTE[sizeOfSize];

		fread(rgbquad, sizeof(RGBQUAD), 256, fp2);
		fread(source, sizeof(BYTE), sizeOfSize, fp2);

		int point = 0;
		for (int i = 0; i < bmpInfoSize.biHeight; i++)
		{
			for (int j = 0; j < bmpInfoSize.biWidth; j++)
			{
				if (source[point] <= given_number) {
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
		fclose(fp2);
		BYTE *lpGray = new BYTE[height*widthBytes];
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++)
			{
				lpGray[(height - i - 1)*widthBytes + j * 4] = output[i*width + j];
				lpGray[(height - i - 1)*widthBytes + j * 4 + 1] = output[i*width + j];
				lpGray[(height - i - 1)*widthBytes + j * 4 + 2] = output[i*width + j];
			}
		}
		m_cBitmap.SetBitmapBits(height * widthBytes, lpGray);
		imageresult.SetBitmap(m_cBitmap);
		delete[] lpGray;
	}
	}
	else if (method == "迭代阈值分割") {
	int given_number = 128;
	long width;
	long height;
	long widthBytes;
	BITMAP bm;
	CBitmap m_cBitmap;

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

	int cal_divede = 128;
	long sum1 = 0, sum2 = 0, sum3 = 0, sum4 = 0;
	float divide1, divide2;
	do {
		given_number = cal_divede;
		for (int i = 0; i < given_number; i++) {
			sum1 += i * data[i];
			sum2 += data[i];
		}
		for (int i = given_number; i < 255; i++) {
			sum3 += i * data[i];
			sum4 += data[i];
		}
		divide1 = ((float)sum1 / sum2);
		divide2 = ((float)sum3 / sum4);
		cal_divede = (int)(divide1 + divide2) / 2;
	} while (cal_divede != given_number);

	char temp[100];
	sprintf(temp, "阈值为%d", given_number);
	MessageBox(temp);

	BITMAPFILEHEADER bmpHeadSize;
	BITMAPINFOHEADER bmpInfoSize;

	FILE * fp2 = fopen("source.bmp", "rb");

	fread(&bmpHeadSize, sizeof(BITMAPFILEHEADER), 1, fp2);
	fread(&bmpInfoSize, sizeof(BITMAPINFOHEADER), 1, fp2);

	int heightOfSize = bmpInfoSize.biHeight, widthOfSize = bmpInfoSize.biWidth;
	if (bmpInfoSize.biBitCount != 8)
	{
		MessageBox("这个图片不是8位图片");
	}
	else {
		int byteWidthOfSize = (widthOfSize * bmpInfoSize.biBitCount / 8 + 3) / 4 * 4;
		int sizeOfSize = byteWidthOfSize * heightOfSize;

		RGBQUAD rgbquad[256];
		BYTE *source = new BYTE[sizeOfSize];
		BYTE *output = new BYTE[sizeOfSize];

		fread(rgbquad, sizeof(RGBQUAD), 256, fp2);
		fread(source, sizeof(BYTE), sizeOfSize, fp2);

		int point = 0;
		for (int i = 0; i < bmpInfoSize.biHeight; i++)
		{
			for (int j = 0; j < bmpInfoSize.biWidth; j++)
			{
				if (source[point] <= given_number) {
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

		fclose(fp2);
		BYTE *lpGray = new BYTE[height*widthBytes];
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++)
			{
				lpGray[(height - i - 1)*widthBytes + j * 4] = output[i*width + j];
				lpGray[(height - i - 1)*widthBytes + j * 4 + 1] = output[i*width + j];
				lpGray[(height - i - 1)*widthBytes + j * 4 + 2] = output[i*width + j];
			}
		}
		m_cBitmap.SetBitmapBits(height * widthBytes, lpGray);
		imageresult.SetBitmap(m_cBitmap);
		delete[] lpGray;
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
	
	

	if (method == "24位红色分离") {
		BITMAPFILEHEADER_MY bmpHead;
		BITMAPINFOHEADER_MY bmpInfo;
		RGBQUAD_MY bmpPla[256];

		BYTE_MY *imageDataInput = NULL;
		BYTE_MY *imageDataOutput = NULL;
		int width, height;

		FILE *fp = fopen("source.bmp", "rb");
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
		fclose(fpw);

		delete[] imageDataInput;
		delete[] imageDataOutput;
	}
	else if (method == "24位绿色分离") {
		BITMAPFILEHEADER_MY bmpHead;
		BITMAPINFOHEADER_MY bmpInfo;
		RGBQUAD_MY bmpPla[256];

		BYTE_MY *imageDataInput = NULL;
		BYTE_MY *imageDataOutput = NULL;
		int width, height;

		FILE *fp = fopen("source.bmp", "rb");
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
		fclose(fpw);

		delete[] imageDataInput;
		delete[] imageDataOutput;
	}
	else if (method == "24位蓝色分离") {
	BITMAPFILEHEADER_MY bmpHead;
	BITMAPINFOHEADER_MY bmpInfo;
	RGBQUAD_MY bmpPla[256];

	BYTE_MY *imageDataInput = NULL;
	BYTE_MY *imageDataOutput = NULL;
	int width, height;

	FILE *fp = fopen("source.bmp", "rb");
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
		fclose(fpw);

		delete[] imageDataInput;
		delete[] imageDataOutput;
	}
	else if (method == "24位彩色图灰度化") {
	BITMAPFILEHEADER_MY bmpHead;
	BITMAPINFOHEADER_MY bmpInfo;
	RGBQUAD_MY bmpPla[256];

	BYTE_MY *imageDataInput = NULL;
	BYTE_MY *imageDataOutput = NULL;
	int width, height;

	FILE *fp = fopen("source.bmp", "rb");
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
		fclose(fpw);

		delete[] imageDataInput;
		delete[] imageDataOutput;
	}
	else if (method == "24位彩色图反色") {
	BITMAPFILEHEADER_MY bmpHead;
	BITMAPINFOHEADER_MY bmpInfo;
	RGBQUAD_MY bmpPla[256];

	BYTE_MY *imageDataInput = NULL;
	BYTE_MY *imageDataOutput = NULL;
	int width, height;

	FILE *fp = fopen("source.bmp", "rb");
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
		for (int i = 0; i < height; ++i)
		{
			for (int j = 0; j < width; ++j)
			{
				BYTE_MY temp = 255 - (*(imageDataInput + i * width + j));
				fwrite(&temp, 1, sizeof(BYTE_MY), fpw);
			}
		}
		fclose(fpw);

		delete[] imageDataInput;
		delete[] imageDataOutput;
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
				data[lpBits[i*widthBytes + j *4]]++;
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
					if (j <= 2000*data[i]/sum) {
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
	else if (method == "平均处理") {
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
					int n1, n2, n3, n4, n5, n6, n7, n8;
					if (j==0 && i==0) {
						n1 = source[point];
						n2 = source[point];
						n3 = source[point + 1];
						n4 = source[point];
						n5 = source[point + 1];
						n6 = source[point + width];
						n7 = source[point + width];
						n8 = source[point + width + 1];
					} else if (i == 0 && j > 0 && j < width-1) {
						n1 = source[point-1];
						n2 = source[point];
						n3 = source[point + 1];
						n4 = source[point-1];
						n5 = source[point + 1];
						n6 = source[point + width-1];
						n7 = source[point + width];
						n8 = source[point + width + 1];
					} else if (i == 0 && j == width-1) {
						n1 = source[point - 1];
						n2 = source[point];
						n3 = source[point];
						n4 = source[point - 1];
						n5 = source[point];
						n6 = source[point + width - 1];
						n7 = source[point + width];
						n8 = source[point + width];
					}
					else if (j == 0 && i>0 && i< height -1) {
						n1 = source[point - width];
						n2 = source[point -width];
						n3 = source[point-width+1];
						n4 = source[point];
						n5 = source[point+1];
						n6 = source[point + width];
						n7 = source[point + width];
						n8 = source[point + width+1];
					}
					else if (j ==width-1  && i > 0 && i < height - 1) {
						n1 = source[point - width-1];
						n2 = source[point - width];
						n3 = source[point - width];
						n4 = source[point-1];
						n5 = source[point];
						n6 = source[point + width-1];
						n7 = source[point + width];
						n8 = source[point + width];
					}
					else if (j == 0 && i ==height-1) {
						n1 = source[point - width];
						n2 = source[point - width];
						n3 = source[point - width+1];
						n4 = source[point];
						n5 = source[point+1];
						n6 = source[point];
						n7 = source[point];
						n8 = source[point+1];
					}
					else if (j == width-1 && i == height - 1) {
						n1 = source[point - width-1];
						n2 = source[point - width];
						n3 = source[point - width];
						n4 = source[point-1];
						n5 = source[point];
						n6 = source[point-1];
						n7 = source[point];
						n8 = source[point];
					}
					else if (j >0 && j<width-1 && i == height - 1) {
						n1 = source[point - width-1];
						n2 = source[point - width];
						n3 = source[point - width + 1];
						n4 = source[point-1];
						n5 = source[point + 1];
						n6 = source[point-1];
						n7 = source[point];
						n8 = source[point + 1];
					}
					else {
						n1 = source[point - width - 1];
						n2 = source[point - width];
						n3 = source[point - width + 1];
						n4 = source[point - 1];
						n5 = source[point + 1];
						n6 = source[point +width- 1];
						n7 = source[point +width];
						n8 = source[point +width+ 1];
					}
					output[point] = (int)(source[point]+n1+n2+n3+n4+n5+n6+n7+n8)/9;
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
	else if (method == "中值滤波") {
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
					int n[9];
					if (j == 0 && i == 0) {
						n[1] = source[point];
						n[2] = source[point];
						n[3] = source[point + 1];
						n[4] = source[point];
						n[5] = source[point + 1];
						n[6] = source[point + width];
						n[7] = source[point + width];
						n[8] = source[point + width + 1];
					}
					else if (i == 0 && j > 0 && j < width - 1) {
						n[1] = source[point - 1];
						n[2] = source[point];
						n[3] = source[point + 1];
						n[4] = source[point - 1];
						n[5] = source[point + 1];
						n[6] = source[point + width - 1];
						n[7] = source[point + width];
						n[8] = source[point + width + 1];
					}
					else if (i == 0 && j == width - 1) {
						n[1] = source[point - 1];
						n[2] = source[point];
						n[3] = source[point];
						n[4] = source[point - 1];
						n[5] = source[point];
						n[6] = source[point + width - 1];
						n[7] = source[point + width];
						n[8] = source[point + width];
					}
					else if (j == 0 && i > 0 && i < height - 1) {
						n[1] = source[point - width];
						n[2] = source[point - width];
						n[3] = source[point - width + 1];
						n[4] = source[point];
						n[5] = source[point + 1];
						n[6] = source[point + width];
						n[7] = source[point + width];
						n[8] = source[point + width + 1];
					}
					else if (j == width - 1 && i > 0 && i < height - 1) {
						n[1] = source[point - width - 1];
						n[2] = source[point - width];
						n[3] = source[point - width];
						n[4] = source[point - 1];
						n[5] = source[point];
						n[6] = source[point + width - 1];
						n[7] = source[point + width];
						n[8] = source[point + width];
					}
					else if (j == 0 && i == height - 1) {
						n[1] = source[point - width];
						n[2] = source[point - width];
						n[3] = source[point - width + 1];
						n[4] = source[point];
						n[5] = source[point + 1];
						n[6] = source[point];
						n[7] = source[point];
						n[8] = source[point + 1];
					}
					else if (j == width - 1 && i == height - 1) {
						n[1] = source[point - width - 1];
						n[2] = source[point - width];
						n[3] = source[point - width];
						n[4] = source[point - 1];
						n[5] = source[point];
						n[6] = source[point - 1];
						n[7] = source[point];
						n[8] = source[point];
					}
					else if (j > 0 && j < width - 1 && i == height - 1) {
						n[1] = source[point - width - 1];
						n[2] = source[point - width];
						n[3] = source[point - width + 1];
						n[4] = source[point - 1];
						n[5] = source[point + 1];
						n[6] = source[point - 1];
						n[7] = source[point];
						n[8] = source[point + 1];
					}
					else {
						n[1] = source[point - width - 1];
						n[2] = source[point - width];
						n[3] = source[point - width + 1];
						n[4] = source[point - 1];
						n[5] = source[point + 1];
						n[6] = source[point + width - 1];
						n[7] = source[point + width];
						n[8] = source[point + width + 1];
					}
					n[0] = source[point];
					sort(n, n+8);
					output[point] = n[4];
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
	else if (method == "图像缩放") {
		CString str;
		float sizeOfChange;
		GetDlgItemText(IDC_EDIT1, str);
		sizeOfChange = _ttof(str);

		BITMAPFILEHEADER bmpHeadSize;
		BITMAPINFOHEADER bmpInfoSize;

		FILE * fp1 = fopen("source.bmp", "rb");

		fread(&bmpHeadSize, sizeof(BITMAPFILEHEADER), 1, fp1);
		fread(&bmpInfoSize, sizeof(BITMAPINFOHEADER), 1, fp1);

		int heightOfSize = bmpInfoSize.biHeight, widthOfSize = bmpInfoSize.biWidth;
		if (bmpInfoSize.biBitCount != 8)
		{
			MessageBox("这个图片不是8位图片");
		}
		else {
			int byteWidthOfSize = (widthOfSize * bmpInfoSize.biBitCount / 8 + 3) / 4 * 4;
			int sizeOfSize = byteWidthOfSize * heightOfSize;

			RGBQUAD rgbquad[256];
			BYTE *source = new BYTE[sizeOfSize];

			fread(rgbquad, sizeof(RGBQUAD), 256, fp1);
			fread(source, sizeof(BYTE), sizeOfSize, fp1);

			bmpInfoSize.biHeight = (int)(heightOfSize*sizeOfChange);
			bmpInfoSize.biWidth = (int)(widthOfSize*sizeOfChange);
			bmpInfoSize.biClrUsed = 256;
			int byteWidthOfSize2 = (bmpInfoSize.biWidth * bmpInfoSize.biBitCount / 8 + 3) / 4 * 4;
			sizeOfSize = byteWidthOfSize2 * bmpInfoSize.biHeight;
			BYTE *output = new BYTE[sizeOfSize];

			int point = 0;
			for (int i = 0; i < bmpInfoSize.biHeight; i++)
			{
				for (int j = 0; j < bmpInfoSize.biWidth; j++)
				{
					output[point] = source[(int)(i/ sizeOfChange)*byteWidthOfSize + (int)(j/ sizeOfChange)];
					point++;
				}
				while (point % 4 != 0)
				{
					output[point] = 0;
					point++;
				}

			}

			FILE * fpw1 = fopen("output.bmp", "wb");

			fwrite(&bmpHeadSize, sizeof(BITMAPFILEHEADER), 1, fpw1);
			fwrite(&bmpInfoSize, sizeof(BITMAPINFOHEADER), 1, fpw1);
			fwrite(&rgbquad, 4 * 256, 1, fpw1);
			fwrite(output, sizeof(BYTE), sizeOfSize, fpw1);

			fclose(fpw1);
			fclose(fp1);
		}
	}
	else if (method == "图像平移") {
		CString str;
		int sizeToMoveLeft;
		int sizeToMoveUp;
		GetDlgItemText(IDC_EDIT2, str);
		sizeToMoveLeft = _ttoi(str);
		GetDlgItemText(IDC_EDIT3, str);
		sizeToMoveUp = _ttoi(str);
		BITMAPFILEHEADER bmpHeadSize;
		BITMAPINFOHEADER bmpInfoSize;

		FILE * fp1 = fopen("source.bmp", "rb");

		fread(&bmpHeadSize, sizeof(BITMAPFILEHEADER), 1, fp1);
		fread(&bmpInfoSize, sizeof(BITMAPINFOHEADER), 1, fp1);

		int heightOfSize = bmpInfoSize.biHeight, widthOfSize = bmpInfoSize.biWidth;
		if (bmpInfoSize.biBitCount != 8)
		{
			MessageBox("这个图片不是8位图片");
		}
		else {
			int byteWidthOfSize = (widthOfSize * bmpInfoSize.biBitCount / 8 + 3) / 4 * 4;
			int sizeOfSize = byteWidthOfSize * heightOfSize;

			RGBQUAD rgbquad[256];
			BYTE *source = new BYTE[sizeOfSize];
			BYTE *output = new BYTE[sizeOfSize];

			fread(rgbquad, sizeof(RGBQUAD), 256, fp1);
			fread(source, sizeof(BYTE), sizeOfSize, fp1);

			int point = 0;
			for (int i = 0; i < bmpInfoSize.biHeight; i++)
			{
				for (int j = 0; j < bmpInfoSize.biWidth; j++)
				{
					if (j - sizeToMoveLeft >= 0 && j - sizeToMoveLeft <= bmpInfoSize.biWidth
							&& i-sizeToMoveUp >= 0 && i - sizeToMoveUp <= bmpInfoSize.biHeight) {
						output[point] = source[(i-sizeToMoveUp)*byteWidthOfSize + j-sizeToMoveLeft];
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

			FILE * fpw1 = fopen("output.bmp", "wb");

			fwrite(&bmpHeadSize, sizeof(BITMAPFILEHEADER), 1, fpw1);
			fwrite(&bmpInfoSize, sizeof(BITMAPINFOHEADER), 1, fpw1);
			fwrite(&rgbquad, 4 * 256, 1, fpw1);
			fwrite(output, sizeof(BYTE), sizeOfSize, fpw1);

			fclose(fpw1);
			fclose(fp1);
		}
		
	}
	else if (method == "水平镜像") {
		BITMAPFILEHEADER bmpHeadSize;
		BITMAPINFOHEADER bmpInfoSize;

		FILE * fp1 = fopen("source.bmp", "rb");

		fread(&bmpHeadSize, sizeof(BITMAPFILEHEADER), 1, fp1);
		fread(&bmpInfoSize, sizeof(BITMAPINFOHEADER), 1, fp1);

		int heightOfSize = bmpInfoSize.biHeight, widthOfSize = bmpInfoSize.biWidth;
		if (bmpInfoSize.biBitCount != 8)
		{
			MessageBox("这个图片不是8位图片");
		}
		else {
			int byteWidthOfSize = (widthOfSize * bmpInfoSize.biBitCount / 8 + 3) / 4 * 4;
			int sizeOfSize = byteWidthOfSize * heightOfSize;

			RGBQUAD rgbquad[256];
			BYTE *source = new BYTE[sizeOfSize];
			BYTE *output = new BYTE[sizeOfSize];

			fread(rgbquad, sizeof(RGBQUAD), 256, fp1);
			fread(source, sizeof(BYTE), sizeOfSize, fp1);

			int point = 0;
			for (int i = 0; i < bmpInfoSize.biHeight; i++)
			{
				for (int j = 0; j < bmpInfoSize.biWidth; j++)
				{
					output[point] = source[i*byteWidthOfSize + bmpInfoSize.biWidth - j];
					point++;
				}
				while (point % 4 != 0)
				{
					output[point] = 0;
					point++;
				}

			}

			FILE * fpw1 = fopen("output.bmp", "wb");

			fwrite(&bmpHeadSize, sizeof(BITMAPFILEHEADER), 1, fpw1);
			fwrite(&bmpInfoSize, sizeof(BITMAPINFOHEADER), 1, fpw1);
			fwrite(&rgbquad, 4 * 256, 1, fpw1);
			fwrite(output, sizeof(BYTE), sizeOfSize, fpw1);

			fclose(fpw1);
			fclose(fp1);
		}
	}
	else if (method == "垂直镜像") {
		BITMAPFILEHEADER bmpHeadSize;
		BITMAPINFOHEADER bmpInfoSize;

		FILE * fp1 = fopen("source.bmp", "rb");

		fread(&bmpHeadSize, sizeof(BITMAPFILEHEADER), 1, fp1);
		fread(&bmpInfoSize, sizeof(BITMAPINFOHEADER), 1, fp1);

		int heightOfSize = bmpInfoSize.biHeight, widthOfSize = bmpInfoSize.biWidth;
		if (bmpInfoSize.biBitCount != 8)
		{
			MessageBox("这个图片不是8位图片");
		}
		else {
			int byteWidthOfSize = (widthOfSize * bmpInfoSize.biBitCount / 8 + 3) / 4 * 4;
			int sizeOfSize = byteWidthOfSize * heightOfSize;

			RGBQUAD rgbquad[256];
			BYTE *source = new BYTE[sizeOfSize];
			BYTE *output = new BYTE[sizeOfSize];

			fread(rgbquad, sizeof(RGBQUAD), 256, fp1);
			fread(source, sizeof(BYTE), sizeOfSize, fp1);

			int point = 0;
			for (int i = 0; i < bmpInfoSize.biHeight; i++)
			{
				for (int j = 0; j < bmpInfoSize.biWidth; j++)
				{
					output[point] = source[(bmpInfoSize.biHeight-i -1)*byteWidthOfSize + j];
					point++;
				}
				while (point % 4 != 0)
				{
					output[point] = 0;
					point++;
				}

			}

			FILE * fpw1 = fopen("output.bmp", "wb");

			fwrite(&bmpHeadSize, sizeof(BITMAPFILEHEADER), 1, fpw1);
			fwrite(&bmpInfoSize, sizeof(BITMAPINFOHEADER), 1, fpw1);
			fwrite(&rgbquad, 4 * 256, 1, fpw1);
			fwrite(output, sizeof(BYTE), sizeOfSize, fpw1);

			fclose(fpw1);
			fclose(fp1);
		}
	}
	else if (method == "图像旋转") {
		CString str;
		float rotate;
		GetDlgItemText(IDC_EDIT4, str);
		rotate = _ttof(str);
		rotate = rotate * 3.1415926 / 180;

		BITMAPFILEHEADER bmpHead;
		BITMAPINFOHEADER bmpInfo;

		FILE * fp1 = fopen("source.bmp", "rb");

		fread(&bmpHead, sizeof(BITMAPFILEHEADER), 1, fp1);
		fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, fp1);

		int height = bmpInfo.biHeight, width = bmpInfo.biWidth;
		if (bmpInfo.biBitCount != 8)
		{
			MessageBox("这个图片不是8位图片");
		}
		else {
			bmpInfo.biWidth *= 2;
			bmpInfo.biHeight *= 2;
			int byteWidth = (width * bmpInfo.biBitCount / 8 + 3) / 4 * 4;
			int size = byteWidth * height;

			RGBQUAD rgbquad[256];
			BYTE *source = new BYTE[size];
			BYTE *output = new BYTE[size * 4];
			for (int i = 0; i < size*4; i++) {
				output[i] = 255;
			}

			fread(rgbquad, sizeof(RGBQUAD), 256, fp1);
			fread(source, sizeof(BYTE), size, fp1);

			int midX_pre, midY_pre, midX_aft, midY_aft;
			midX_pre = width / 2;
			midY_pre = height / 2;
			midX_aft = width;
			midY_aft = height;
			int pre_i, pre_j, after_i, after_j;

			int point = 0;
			for (int i = 0; i < height*2; i++)
			{
				for (int j = 0; j < width*2; j++)
				{
					after_i = i - midX_aft;
					after_j = j - midY_aft;
					pre_i = (int)(cos((double)rotate) * after_i - sin((double)rotate) * after_j) + midX_pre;
					pre_j = (int)(sin((double)rotate) * after_i + cos((double)rotate) * after_j) + midY_pre;
					if (pre_i >= 0 && pre_i < height && pre_j >= 0 && pre_j < width) {
						output[i * 2 * byteWidth + j] = source[pre_i * byteWidth + pre_j];
					}
					point++;
				}
				while (point % 4 != 0)
				{
					output[point] = 0;
					point++;
				}
			}

			FILE * fpw1 = fopen("output.bmp", "wb");

			fwrite(&bmpHead, sizeof(BITMAPFILEHEADER), 1, fpw1);
			fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, fpw1);
			fwrite(&rgbquad, 4 * 256, 1, fpw1);
			fwrite(output, sizeof(BYTE), size*4, fpw1);

			fclose(fpw1);
			fclose(fp1);
		}
	}
	else if (method == "Prewitt处理") {
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
					int n1, n2, n3, n4, n5, n6, n7, n8;
					if (j == 0 && i == 0) {
						n1 = source[point];
						n2 = source[point];
						n3 = source[point + 1];
						n4 = source[point];
						n5 = source[point + 1];
						n6 = source[point + width];
						n7 = source[point + width];
						n8 = source[point + width + 1];
					}
					else if (i == 0 && j > 0 && j < width - 1) {
						n1 = source[point - 1];
						n2 = source[point];
						n3 = source[point + 1];
						n4 = source[point - 1];
						n5 = source[point + 1];
						n6 = source[point + width - 1];
						n7 = source[point + width];
						n8 = source[point + width + 1];
					}
					else if (i == 0 && j == width - 1) {
						n1 = source[point - 1];
						n2 = source[point];
						n3 = source[point];
						n4 = source[point - 1];
						n5 = source[point];
						n6 = source[point + width - 1];
						n7 = source[point + width];
						n8 = source[point + width];
					}
					else if (j == 0 && i > 0 && i < height - 1) {
						n1 = source[point - width];
						n2 = source[point - width];
						n3 = source[point - width + 1];
						n4 = source[point];
						n5 = source[point + 1];
						n6 = source[point + width];
						n7 = source[point + width];
						n8 = source[point + width + 1];
					}
					else if (j == width - 1 && i > 0 && i < height - 1) {
						n1 = source[point - width - 1];
						n2 = source[point - width];
						n3 = source[point - width];
						n4 = source[point - 1];
						n5 = source[point];
						n6 = source[point + width - 1];
						n7 = source[point + width];
						n8 = source[point + width];
					}
					else if (j == 0 && i == height - 1) {
						n1 = source[point - width];
						n2 = source[point - width];
						n3 = source[point - width + 1];
						n4 = source[point];
						n5 = source[point + 1];
						n6 = source[point];
						n7 = source[point];
						n8 = source[point + 1];
					}
					else if (j == width - 1 && i == height - 1) {
						n1 = source[point - width - 1];
						n2 = source[point - width];
						n3 = source[point - width];
						n4 = source[point - 1];
						n5 = source[point];
						n6 = source[point - 1];
						n7 = source[point];
						n8 = source[point];
					}
					else if (j > 0 && j < width - 1 && i == height - 1) {
						n1 = source[point - width - 1];
						n2 = source[point - width];
						n3 = source[point - width + 1];
						n4 = source[point - 1];
						n5 = source[point + 1];
						n6 = source[point - 1];
						n7 = source[point];
						n8 = source[point + 1];
					}
					else {
						n1 = source[point - width - 1];
						n2 = source[point - width];
						n3 = source[point - width + 1];
						n4 = source[point - 1];
						n5 = source[point + 1];
						n6 = source[point + width - 1];
						n7 = source[point + width];
						n8 = source[point + width + 1];
					}
					output[point] = (int)(source[point] - n1 + n3 - n4 + n5 - n6 + n8) / 9;
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
	else if (method == "LOG处理") {
		MessageBox(method);
	}
	else if (method == "Sobel处理") {
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
					int n1, n2, n3, n4, n5, n6, n7, n8;
					if (j == 0 && i == 0) {
						n1 = source[point];
						n2 = source[point];
						n3 = source[point + 1];
						n4 = source[point];
						n5 = source[point + 1];
						n6 = source[point + width];
						n7 = source[point + width];
						n8 = source[point + width + 1];
					}
					else if (i == 0 && j > 0 && j < width - 1) {
						n1 = source[point - 1];
						n2 = source[point];
						n3 = source[point + 1];
						n4 = source[point - 1];
						n5 = source[point + 1];
						n6 = source[point + width - 1];
						n7 = source[point + width];
						n8 = source[point + width + 1];
					}
					else if (i == 0 && j == width - 1) {
						n1 = source[point - 1];
						n2 = source[point];
						n3 = source[point];
						n4 = source[point - 1];
						n5 = source[point];
						n6 = source[point + width - 1];
						n7 = source[point + width];
						n8 = source[point + width];
					}
					else if (j == 0 && i > 0 && i < height - 1) {
						n1 = source[point - width];
						n2 = source[point - width];
						n3 = source[point - width + 1];
						n4 = source[point];
						n5 = source[point + 1];
						n6 = source[point + width];
						n7 = source[point + width];
						n8 = source[point + width + 1];
					}
					else if (j == width - 1 && i > 0 && i < height - 1) {
						n1 = source[point - width - 1];
						n2 = source[point - width];
						n3 = source[point - width];
						n4 = source[point - 1];
						n5 = source[point];
						n6 = source[point + width - 1];
						n7 = source[point + width];
						n8 = source[point + width];
					}
					else if (j == 0 && i == height - 1) {
						n1 = source[point - width];
						n2 = source[point - width];
						n3 = source[point - width + 1];
						n4 = source[point];
						n5 = source[point + 1];
						n6 = source[point];
						n7 = source[point];
						n8 = source[point + 1];
					}
					else if (j == width - 1 && i == height - 1) {
						n1 = source[point - width - 1];
						n2 = source[point - width];
						n3 = source[point - width];
						n4 = source[point - 1];
						n5 = source[point];
						n6 = source[point - 1];
						n7 = source[point];
						n8 = source[point];
					}
					else if (j > 0 && j < width - 1 && i == height - 1) {
						n1 = source[point - width - 1];
						n2 = source[point - width];
						n3 = source[point - width + 1];
						n4 = source[point - 1];
						n5 = source[point + 1];
						n6 = source[point - 1];
						n7 = source[point];
						n8 = source[point + 1];
					}
					else {
						n1 = source[point - width - 1];
						n2 = source[point - width];
						n3 = source[point - width + 1];
						n4 = source[point - 1];
						n5 = source[point + 1];
						n6 = source[point + width - 1];
						n7 = source[point + width];
						n8 = source[point + width + 1];
					}
					output[point] = (int)(source[point] - n1 + n3 - 2 * n4 + 2 * n5 - n6 + n8) / 9;
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
	else if (method == "给定阈值分割") {
		CString str;
		int given_number;
		GetDlgItemText(Given_Divid, str);
		given_number = _ttoi(str);

		long width;
		long height;
		long widthBytes;
		BITMAP bm;
		CBitmap m_cBitmap;

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
					if (j <= 6000 * data[i] / sum) {
						output[point] = 0;
					}
					else {
						output[point] = 255;
					}
					if (i == given_number) {
						output[point] = 155;
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

		BITMAPFILEHEADER bmpHeadSize;
		BITMAPINFOHEADER bmpInfoSize;

		FILE * fp2 = fopen("source.bmp", "rb");

		fread(&bmpHeadSize, sizeof(BITMAPFILEHEADER), 1, fp2);
		fread(&bmpInfoSize, sizeof(BITMAPINFOHEADER), 1, fp2);

		int heightOfSize = bmpInfoSize.biHeight, widthOfSize = bmpInfoSize.biWidth;
		if (bmpInfoSize.biBitCount != 8)
		{
			MessageBox("这个图片不是8位图片");
		}
		else {
			int byteWidthOfSize = (widthOfSize * bmpInfoSize.biBitCount / 8 + 3) / 4 * 4;
			int sizeOfSize = byteWidthOfSize * heightOfSize;

			RGBQUAD rgbquad[256];
			BYTE *source = new BYTE[sizeOfSize];
			BYTE *output = new BYTE[sizeOfSize];

			fread(rgbquad, sizeof(RGBQUAD), 256, fp2);
			fread(source, sizeof(BYTE), sizeOfSize, fp2);

			int point = 0;
			for (int i = 0; i < bmpInfoSize.biHeight; i++)
			{
				for (int j = 0; j < bmpInfoSize.biWidth; j++)
				{
					if (source[point] <= given_number) {
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

			FILE * fpw2 = fopen("output.bmp", "wb");

			fwrite(&bmpHeadSize, sizeof(BITMAPFILEHEADER), 1, fpw2);
			fwrite(&bmpInfoSize, sizeof(BITMAPINFOHEADER), 1, fpw2);
			fwrite(&rgbquad, 4 * 256, 1, fpw2);
			fwrite(output, sizeof(BYTE), sizeOfSize, fpw2);

			fclose(fpw2);
			fclose(fp2);
		}
	}
	else if (method == "迭代阈值分割") {
		int given_number = 128;
		long width;
		long height;
		long widthBytes;
		BITMAP bm;
		CBitmap m_cBitmap;

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

		int cal_divede = 128;
		long sum1 = 0, sum2 = 0, sum3 = 0, sum4 = 0;
		float divide1, divide2;
		do {
			given_number = cal_divede;
			for (int i = 0; i < given_number; i++) {
				sum1 += i * data[i];
				sum2 += data[i];
			}
			for (int i = given_number; i < 255; i++) {
				sum3 += i * data[i];
				sum4 += data[i];
			}
			divide1 = ((float)sum1/sum2);
			divide2 = ((float)sum3 / sum4);
			cal_divede = (int)(divide1 + divide2) / 2;
		} while (cal_divede != given_number);

		char temp[100];
		sprintf(temp, "阈值为%d", given_number);
		MessageBox(temp);

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
					if (j <= 6000 * data[i] / sum) {
						output[point] = 0;
					}
					else {
						output[point] = 255;
					}
					if (i == given_number) {
						output[point] = 155;
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

		BITMAPFILEHEADER bmpHeadSize;
		BITMAPINFOHEADER bmpInfoSize;

		FILE * fp2 = fopen("source.bmp", "rb");

		fread(&bmpHeadSize, sizeof(BITMAPFILEHEADER), 1, fp2);
		fread(&bmpInfoSize, sizeof(BITMAPINFOHEADER), 1, fp2);

		int heightOfSize = bmpInfoSize.biHeight, widthOfSize = bmpInfoSize.biWidth;
		if (bmpInfoSize.biBitCount != 8)
		{
			MessageBox("这个图片不是8位图片");
		}
		else {
			int byteWidthOfSize = (widthOfSize * bmpInfoSize.biBitCount / 8 + 3) / 4 * 4;
			int sizeOfSize = byteWidthOfSize * heightOfSize;

			RGBQUAD rgbquad[256];
			BYTE *source = new BYTE[sizeOfSize];
			BYTE *output = new BYTE[sizeOfSize];

			fread(rgbquad, sizeof(RGBQUAD), 256, fp2);
			fread(source, sizeof(BYTE), sizeOfSize, fp2);

			int point = 0;
			for (int i = 0; i < bmpInfoSize.biHeight; i++)
			{
				for (int j = 0; j < bmpInfoSize.biWidth; j++)
				{
					if (source[point] <= given_number) {
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

			FILE * fpw2 = fopen("output.bmp", "wb");

			fwrite(&bmpHeadSize, sizeof(BITMAPFILEHEADER), 1, fpw2);
			fwrite(&bmpInfoSize, sizeof(BITMAPINFOHEADER), 1, fpw2);
			fwrite(&rgbquad, 4 * 256, 1, fpw2);
			fwrite(output, sizeof(BYTE), sizeOfSize, fpw2);

			fclose(fpw2);
			fclose(fp2);
		}
	}
	else {
		MessageBox("请使用合理的方法");
	}

	
	MessageBox("文件保存成功");
}


void CImageProcessDlg::OnCbnSelchangeCombomethod()
{
	// TODO: 在此添加控件通知处理程序代码
	CString method(_T(""));
	comboboxmethod.GetLBText(comboboxmethod.GetCurSel(), method);

	sizeChange.ShowWindow(SW_HIDE);
	upMove.ShowWindow(SW_HIDE);
	leftMove.ShowWindow(SW_HIDE);
	rotateSize.ShowWindow(SW_HIDE);
	given_divide.ShowWindow(SW_HIDE);

	if (method == "图像缩放") {
		sizeChange.ShowWindow(SW_SHOW);
	}
	else if (method =="图像平移"){
		upMove.ShowWindow(SW_SHOW);
		upMove.SetWindowTextA("向上平移");
		leftMove.ShowWindow(SW_SHOW);
		leftMove.SetWindowTextA("向右平移");
	}
	else if (method == "图像旋转") {
		rotateSize.ShowWindow(SW_SHOW);
		rotateSize.SetWindowTextA("旋转角度");
	}
	else if (method == "给定阈值分割") {
		given_divide.ShowWindow(SW_SHOW);
	}
}
