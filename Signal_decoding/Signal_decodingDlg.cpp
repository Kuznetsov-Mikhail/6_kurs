
// Signal_decodingDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "Signal_decoding.h"
#include "Signal_decodingDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CSignaldecodingDlg



CSignaldecodingDlg::CSignaldecodingDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SIGNAL_DECODING_DIALOG, pParent)
	, bits_count(10)
	, sampling(100)
	, bitrate(5)
	, snr(20)
	, input_data("")
	, output_data("")
	, test_base(1)
	, test_study(1)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSignaldecodingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PIC1, viewer1);
	//DDX_Control(pDX, IDC_PIC2, viewer2);
	//DDX_Control(pDX, IDC_PIC3, viewer3);
	DDX_Text(pDX, IDC_EDIT1, bits_count);
	DDX_Text(pDX, IDC_EDIT2, sampling);
	DDX_Text(pDX, IDC_EDIT3, bitrate);
	DDX_Text(pDX, IDC_EDIT4, snr);
	DDX_Text(pDX, IDC_EDIT5, input_data);
	DDX_Text(pDX, IDC_EDIT6, output_data);
	DDX_Text(pDX, IDC_EDIT7, test_base);
	DDX_Text(pDX, IDC_EDIT8, test_study);
}

BEGIN_MESSAGE_MAP(CSignaldecodingDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCANCEL, &CSignaldecodingDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &CSignaldecodingDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CSignaldecodingDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CSignaldecodingDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CSignaldecodingDlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// Обработчики сообщений CSignaldecodingDlg

BOOL CSignaldecodingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CSignaldecodingDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CSignaldecodingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSignaldecodingDlg::OnBnClickedCancel()
{
	// TODO: добавьте свой код обработчика уведомлений
	CDialogEx::OnCancel();
}


void CSignaldecodingDlg::OnBnClickedButton1()
{
	UpdateData(1);
	decoder.Init(sampling,bitrate);
	draw.resize(decoder.Gold_filters.size());

	for (int i = 0; i < decoder.Gold_filters.size(); i++)
	{
		draw[i].resize(decoder.Gold_filters[i].size());
		for (int j=0;j< decoder.Gold_filters[i].size();j++)
		draw[i][j] = decoder.Gold_filters[i][j].real();
	}
	ViewerDraw(draw, 0, decoder.Gold_filters[0].size(), viewer1, "Gold_filter.png", false);
	UpdateData(0);
}

//code
void CSignaldecodingDlg::OnBnClickedButton2()
{
	UpdateData(1);
	decoder.input_generation(bits_count);
	decoder.signal_generation(sampling, bitrate,snr);
	draw.resize(2); 
	draw[0].resize(decoder.signal.size());
	draw[1].resize(decoder.signal.size());
	for (int i = 0; i < decoder.signal.size(); i++)
	{
		draw[0][i] = decoder.signal[i].real();
		draw[1][i] = decoder.signal[i].imag();
	}
	ViewerDraw(draw, 0, decoder.signal.size(), viewer1, "signal.png", false);
	string bufstr = decoder.get_input_data();
	input_data = bufstr.c_str();
	UpdateData(0);
}
//decode
void CSignaldecodingDlg::OnBnClickedButton3()
{
	UpdateData(1);
	string bufstr = decoder.Golds_convolution(draw,sampling,bitrate);
	output_data = bufstr.c_str();
	if (draw.empty()) return;
	if (draw[0].empty()) return;
	ViewerDraw(draw, 0, draw[0].size(), viewer1, "Golds_convolution.png", false);
	UpdateData(0);
}
void CSignaldecodingDlg::ViewerDraw(vector<vector<double>>& data, 
									double Xmin, double Xmax, CChartViewer& viewer_num,
									string PathPic, bool podpisi)
{
	if (data.empty())return;
	// In this example, we simply use random data for the 3 data series.
	vector<DoubleArray> Arr_dataReal; Arr_dataReal.resize(data.size());
	for (int i = 0; i < data.size(); i++)
	{
		Arr_dataReal[i] = vectorToArray(data[i]);
	}

	vector<double>Datatime;

	double OXstep = (Xmax - Xmin) / (data[0].size() - 1);
	for (double i = Xmin; i <= Xmax; i += OXstep)Datatime.push_back(i);
	DoubleArray timeStamps = vectorToArray(Datatime);

	// Create a XYChart object of size 600 x 400 pixels
	XYChart* c = new XYChart(850, 240);

	// Add a title box using grey (0x555555) 20pt Arial font
	//c->addTitle("", "arial.ttf", 20, 0x555555);

	// Set the plotarea at (70, 70) and of size 500 x 300 pixels, with transparent background and
	// border and light grey (0xcccccc) horizontal grid lines
	c->setPlotArea(70, 50, 700, 120, Chart::Transparent, -1, Chart::Transparent, 0xcccccc);

	// Add a legend box with horizontal layout above the plot area at (70, 35). Use 12pt Arial font,
	// transparent background and border, and line style legend icon.
	LegendBox* b = c->addLegend(20, 5, false, "arial.ttf", 12);
	b->setBackground(Chart::Transparent, Chart::Transparent);
	b->setLineStyleKey();

	// Set axis label font to 12pt Arial
	c->xAxis()->setLabelStyle("arial.ttf", 12);
	c->yAxis()->setLabelStyle("arial.ttf", 12);

	// Set the x and y axis stems to transparent, and the x-axis tick color to grey (0xaaaaaa)
	c->xAxis()->setColors(Chart::TextColor, Chart::TextColor, Chart::TextColor, 0xaaaaaa);
	c->yAxis()->setColors(Chart::TextColor);

	// Set the major/minor tick lengths for the x-axis to 10 and 0.
	c->xAxis()->setTickLength(10, 0);

	// For the automatic axis labels, set the minimum spacing to 80/40 pixels for the x/y axis.
	c->xAxis()->setTickDensity(80);
	c->yAxis()->setTickDensity(40);

	// Add a title to the y axis using dark grey (0x555555) 14pt Arial font
	c->yAxis()->setTitle("", "arial.ttf", 14, 0x555555);

	// Add a line layer to the chart with 3-pixel line width
	LineLayer* layer = c->addLineLayer();
	layer->setLineWidth(3);
	//
	if (podpisi) layer->setDataLabelFormat("{value|2} ");
	// Add 3 data series to the line layer
	for (int i = 0; i < Arr_dataReal.size(); i++)
	{
		stringstream ss;
		ss << "Data " << i;
		string name = ss.str();
		layer->addDataSet(Arr_dataReal[i], -1, name.c_str());
	}
	// The x-coordinates for the line layer
	layer->setXData(timeStamps);
	viewer_num.setChart(c);
	const char* chPathPic = PathPic.c_str();
	c->makeChart(chPathPic);
	delete c;
}

void CSignaldecodingDlg::OnBnClickedButton4()
{
	UpdateData(1);
	vector<complex<double>> base, study;
	base.resize(decoder.Gold_filters[test_base-1].size() * 3,0);
	for (int i = 0; i < decoder.Gold_filters[test_base-1].size(); i++)
		base[i + decoder.Gold_filters[test_base-1].size()] = decoder.Gold_filters[test_base-1][i];
	study = decoder.Gold_filters[test_study-1];
	vector<double> result;
	decoder.convolution(base, study, result);
	draw.resize(1);
	draw[0] = result;
	ViewerDraw(draw, 0, result.size(), viewer1, "test.png", false);
	UpdateData(0);
}
