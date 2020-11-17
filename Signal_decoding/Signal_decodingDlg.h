
// Signal_decodingDlg.h: файл заголовка
//

#pragma once
#include "decoding_helper.h"
using namespace std;

// Диалоговое окно CSignaldecodingDlg
class CSignaldecodingDlg : public CDialogEx
{
// Создание
public:
	CSignaldecodingDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SIGNAL_DECODING_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	decoding_helper decoder;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButton1();
	// кол-во информационных символов
	int bits_count;
	// частота дискретизации
	int fd;
	int bitrate;
	// ОСШ (в дБ)
	double snr;
};
