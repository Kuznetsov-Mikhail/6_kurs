
// SignalsPsDlg.h: файл заголовка
//

#pragma once


// Диалоговое окно CSignalsPsDlg
class CSignalsPsDlg : public CDialogEx
{
// Создание
public:
	CSignalsPsDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SIGNALSPS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	// Частота дискретизации
	int sampling;
	// Несущая
	int f_0;
	// Частота модуляции
	int bitrate;
	// Число бит данных
	int bits_size;
	// ОСШ для несущего сигнала
	double SNR_1;
	// ОСШ для полученного сигнала
	double SNR_2;
	// Тип модуляции (1-АМ, 2-ФМ2, 3-MSK)
	int mod_type;
	// Задержка (отсчёты)
	int delay;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
};
