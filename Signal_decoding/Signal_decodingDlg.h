
// Signal_decodingDlg.h: файл заголовка
//

#pragma once
#include "decoding_helper.h"
#include "ChartViewer.h"
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
	/*Объект отрисовки*/
	CChartViewer viewer1;
	/*Объект отрисовки*/
	CChartViewer viewer2;
	/*Объект отрисовки*/
	CChartViewer viewer3;
	void ViewerDraw(vector<vector<double>>& data, double Xmin, double Xmax, CChartViewer& viewer_num, string PathPic, bool podpisi);
	/*Преобразование вектора double к вектору DoubleArray*/
	DoubleArray vectorToArray(vector<double>& v)
	{
		return (v.size() == 0) ? DoubleArray() : DoubleArray(v.data(), (int)v.size());
	}
	vector<vector<double>> draw;
	afx_msg void OnBnClickedCancel();
	// кол-во информационных символов
	int bits_count;
	// частота дискретизации
	double sampling;
	int bitrate;
	// ОСШ (в дБ)
	double snr;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	//input
	CString input_data;
	//output
	CString output_data;
	vector<complex<double>> _s;
	vector<vector<double>> _ccfWithGC;
};
