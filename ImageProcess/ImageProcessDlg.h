
// ImageProcessDlg.h: 头文件
//

#pragma once


// CImageProcessDlg 对话框
class CImageProcessDlg : public CDialogEx
{
// 构造
public:
	CImageProcessDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IMAGEPROCESS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString MethodToDo;
	afx_msg void OnBnClickedButton1();
	CComboBox comboboxmethod;
	CStatic imageresult;
	CStatic imagesource;
	afx_msg void OnBnClickedButton2();

	CEdit sizeChange;
	afx_msg void OnCbnSelchangeCombomethod();
	CEdit leftMove;
	CEdit upMove;
	CEdit rotateSize;
};
