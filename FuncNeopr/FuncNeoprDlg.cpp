
// FuncNeoprDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "FuncNeopr.h"
#include "FuncNeoprDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CAboutDlg используется для описания сведений о приложении

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

// Реализация
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Диалоговое окно CFuncNeoprDlg



CFuncNeoprDlg::CFuncNeoprDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FUNCNEOPR_DIALOG, pParent)
	, bitrate(15e3)
	, chast_diskr(35e4)
	, f_0(35e3)
	, delta_t(5)
	, delta_w(5000)
	, noise(100)
	, delta_t_finded(0)
	, delta_w_finded(0)
	, bitsSize(10)
	, _pi(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFuncNeoprDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, bitrate);
	DDX_Text(pDX, IDC_EDIT2, chast_diskr);
	DDX_Text(pDX, IDC_EDIT4, bitsSize);
	DDX_Text(pDX, IDC_EDIT5, f_0);
	DDX_Text(pDX, IDC_EDIT7, delta_t);
	DDX_Text(pDX, IDC_EDIT9, delta_w);
	DDX_Text(pDX, IDC_EDIT10, noise);
	DDX_Text(pDX, IDC_EDIT6, delta_t_finded);
	DDX_Text(pDX, IDC_EDIT8, delta_w_finded);
	DDX_Control(pDX, IDC_MYSIGNAL1, Obj_Ris);
	DDX_Text(pDX, IDC_EDIT11, _pi);
}

BEGIN_MESSAGE_MAP(CFuncNeoprDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON3, &CFuncNeoprDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &CFuncNeoprDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CFuncNeoprDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON4, &CFuncNeoprDlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// Обработчики сообщений CFuncNeoprDlg

BOOL CFuncNeoprDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Добавление пункта "О программе..." в системное меню.

	// IDM_ABOUTBOX должен быть в пределах системной команды.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

void CFuncNeoprDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CFuncNeoprDlg::OnPaint()
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
HCURSOR CFuncNeoprDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CFuncNeoprDlg::OnBnClickedButton1() // ГЕНЕРАЦИЯ СИГНАЛОВ
{
	UpdateData(1);
	double delay = 0.1;
	//GetSignals_MSK(Signal_1, Signal_2, chast_diskr, f_0, bitrate, bitsSize, delay);
	GetSignals_FM(Signal_1, Signal_2, chast_diskr, f_0, bitrate, bitsSize, delay);
	delta_t = Signal_1.size() * delay;

	Dopler_shift(Signal_2, chast_diskr, delta_w);
	addNoize_Complex(Signal_1, noise);
	addNoize_Complex(Signal_2, noise);

	vector<vector<double>> draw_vector;
	draw_vector.resize(2);
	draw_vector[0].resize(Signal_2.size());
	draw_vector[1].resize(Signal_2.size());
	for (int i = 0; i < Signal_2.size(); i++)
	{
		draw_vector[0][i] = Signal_2[i].imag();
		draw_vector[1][i] = Signal_2[i].real();
	}
	MyViewerDraw("Data", draw_vector, 0, Signal_2.size(), Obj_Ris, "Signal.png", false);
	UpdateData(0);
}

void CFuncNeoprDlg::OnBnClickedButton2() // ПОСТРОИТЬ ФН
{
	if (Signal_1.empty())return;
	UpdateData(1);
	vector<double> neopr_real;
	Uncertainty(neopr_real, Signal_1, Signal_2);
	t_f_detected(neopr_real, Signal_1.size(), step2(Signal_1.size()), chast_diskr, delta_t_finded, delta_w_finded);
	MyViewerDraw3D(neopr_real, Obj_Ris, "neopr_real.png", false);
	UpdateData(0);
}

void CFuncNeoprDlg::OnBnClickedButton3() // ИССЛЕДОВАНИЕ
{
	UpdateData(TRUE);
	vector<vector<double>> study; study.resize(1);
	int dopler_f_min = 0;
	int dopler_f_max = delta_w;
	int shag = 200;
	int try_size = 2;
	for (double dopler = dopler_f_min; dopler <= dopler_f_max; dopler += shag)
	{
		double pi = 0;
		for (int j = 0; j < try_size; j++)
		{
			double delay = 0.1 + 0.8 * rand() / RAND_MAX;
			//GetSignals_MSK(Signal_1, Signal_2, chast_diskr, f_0, bitrate, bitsSize, delay);
			GetSignals_FM(Signal_1, Signal_2, chast_diskr, f_0, bitrate, bitsSize, delay);
			Dopler_shift(Signal_2, chast_diskr, dopler);
			addNoize_Complex(Signal_1, noise);
			addNoize_Complex(Signal_2, noise);
			vector<double> corr;
			Correlation_omp(corr, Signal_1, Signal_2);
			pi += peak_intensity(corr);
		}
		pi /= try_size;
		study[0].push_back(pi);
	}
	MyViewerDraw("Study", study, dopler_f_min, dopler_f_max, Obj_Ris, "Study.png", true);
	SetCursor(LoadCursor(nullptr, IDC_ARROW));
	UpdateData(0);
}

DoubleArray CFuncNeoprDlg::vectorToArray(std::vector<double>& v)
{
	return (v.size() == 0) ? DoubleArray() : DoubleArray(v.data(), (int)v.size());
}

void CFuncNeoprDlg::MyViewerDraw(string Data, vector<vector<double>>& data, double Xmin, double Xmax, CChartViewer& viewer_num, string PathPic, bool podpisi)
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
	XYChart* c = new XYChart(1100, 650);

	// Add a title box using grey (0x555555) 20pt Arial font
	//c->addTitle("", "arial.ttf", 20, 0x555555);

	// Set the plotarea at (70, 70) and of size 500 x 300 pixels, with transparent background and
	// border and light grey (0xcccccc) horizontal grid lines
	c->setPlotArea(70, 70, 1000, 500, Chart::Transparent, -1, Chart::Transparent, 0xcccccc);

	// Add a legend box with horizontal layout above the plot area at (70, 35). Use 12pt Arial font,
	// transparent background and border, and line style legend icon.
	LegendBox* b = c->addLegend(70, 35, false, "arial.ttf", 12);
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
	if (podpisi) layer->setDataLabelFormat("{value|4}");
	// Add 3 data series to the line layer
	for (int i = 0; i < Arr_dataReal.size(); i++)
	{
		stringstream ss;
		ss << Data << i;
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
void CFuncNeoprDlg::MyViewerDraw3D(vector<double>& data,
	CChartViewer& viewer_num, string PathPic, bool podpisi)
{
	if (data.empty())return;
	// In this example, we simply use random data for the 3 data series.
	//vector<DoubleArray> Arr_dataReal; Arr_dataReal.resize(data.size());
	//for (int i = 0; i < data.size(); i++)
	//{
	//	Arr_dataReal[i] = vectorToArray(data[i]);
	//}
	int my_size = sqrt(data.size());
	vector<double> dataX; dataX.resize(data.size());
	vector<double> dataY; dataY.resize(data.size());
	for (int i = 0; i < my_size; i++)
		for (int j = 0; j < my_size; j++)
		{
			dataX[i * my_size + j] = i;
			dataY[i * my_size + j] = ((double)j/my_size) * chast_diskr;
		}
	SurfaceChart* c = new SurfaceChart(750, 650);
	c->addTitle("FN", "timesi.ttf", 20);
	c->setPlotRegion(380, 260, 360, 360, 270);
	c->setViewAngle(30, 210);
	c->setPerspective(60);

	c->setData(vectorToArray(dataX), vectorToArray(dataY), vectorToArray(data));
    // Spline interpolate data to a 80 x 80 grid for a smooth surface
    c->setInterpolation(80, 80);

    // Use semi-transparent black (c0000000) for x and y major surface grid lines. Use dotted style
    // for x and y minor surface grid lines.
    int majorGridColor = 0xc0000000;
    int minorGridColor = c->dashLineColor(majorGridColor, Chart::DotLine);
    c->setSurfaceAxisGrid(majorGridColor, majorGridColor, minorGridColor, minorGridColor);

    // Set contour lines to semi-transparent white (80ffffff)
    c->setContourColor(0x80ffffff);

    // Add a color axis (the legend) in which the left center is anchored at (665, 280). Set the
    // length to 200 pixels and the labels on the right side.
    c->setColorAxis(665, 280, Chart::Left, 200, Chart::Right);

    // Set the x, y and z axis titles using 12 points Arial Bold font
    c->xAxis()->setTitle("t", "arialbd.ttf", 12);
    c->yAxis()->setTitle("f [Hz]", "arialbd.ttf", 12);
    c->zAxis()->setTitle("", "arialbd.ttf", 12);
	viewer_num.setChart(c);
    // Output the chart
    c->makeChart("surface3.jpg");
    //free up resources
    delete c;
}


void CFuncNeoprDlg::fur(vector <complex <double>>& data, int is)
{

	int i, j, istep, n;
	n = data.size();
	int m, mmax;
	double r, r1, theta, w_r, w_i, temp_r, temp_i;
	double pi = 3.1415926f;

	r = pi * is;
	j = 0;
	for (i = 0; i < n; i++)
	{
		if (i < j)
		{
			temp_r = data[j].real();
			temp_i = data[j].imag();
			data[j] = data[i];
			data[i] = temp_r + complex <double>(0, 1) * temp_i;

		}
		m = n >> 1;
		while (j >= m) { j -= m; m = (m + 1) / 2; }
		j += m;
	}
	mmax = 1;
	while (mmax < n)
	{
		istep = mmax << 1;
		r1 = r / (double)mmax;
		for (m = 0; m < mmax; m++)
		{
			theta = r1 * m;
			w_r = (double)cos((double)theta);
			w_i = (double)sin((double)theta);
			for (i = m; i < n; i += istep)
			{
				j = i + mmax;
				temp_r = w_r * data[j].real() - w_i * data[j].imag();
				temp_i = w_r * data[j].imag() + w_i * data[j].real();
				data[j] = (data[i].real() - temp_r) + complex <double>(0, 1) * (data[i].imag() - temp_i);
				data[i] += (temp_r)+complex <double>(0, 1) * (temp_i);
			}
		}
		mmax = istep;
	}
	if (is > 0)
		for (i = 0; i < n; i++)
		{
			data[i] /= (double)n;
		}
}

int CFuncNeoprDlg::step2(int sizein)
{
	int i = 0;
	double S = sizein;
	for (;;)
	{
		if (S > 1)
		{
			i++;
			S /= 2;
		}
		else break;
	}
	return pow(2, i);
}

void CFuncNeoprDlg::addNoize_Complex(vector<complex<double>>& mass, double Noize)
{
	vector<double> shum_ampl;
	shum_ampl.resize(mass.size());
	for (int i = 0; i < shum_ampl.size(); i++)
	{
		shum_ampl[i] = 0;
	}
	double sum_signal = 0;
	double sum_shum = 0;
	for (int i = 0; i < mass.size(); i++)
	{
		sum_signal += mass[i].real() * mass[i].real() + mass[i].imag() * mass[i].imag();
	}
	for (int i = 0; i < mass.size(); i++)
	{
		double M, ksi;
		M = rand() % 9 + 12;
		ksi = 0;
		for (int k = 1; k <= M; k++)
		{
			ksi += (double)((rand() % 21 - 10) / 10.);
		}
		shum_ampl[i] = ksi / M;
	}
	vector<complex<double>> shum_c(shum_ampl.size());
	for (int i = 0; i < shum_c.size(); i++)
	{
		double r_phi = (rand() / RAND_MAX) * 2 * M_PI;
		shum_c[i] = shum_ampl[i] * cos(r_phi) + complex<double>(0, 1) * sin(r_phi);
	}
	for (int i = 0; i < mass.size(); i++)
	{
		sum_shum += shum_c[i].real() * shum_c[i].real() + \
			shum_c[i].imag() * shum_c[i].imag();
	}
	sum_signal = sqrt(sum_signal);
	sum_shum = sqrt(sum_shum);

	double alfa = sum_signal / (sum_shum * (pow(10, Noize / 20.)));
	for (int i = 0; i < mass.size(); i++)
	{
		mass[i] += alfa * shum_c[i] + complex<double>(0, 1) * alfa * shum_c[i];
	}
}

void CFuncNeoprDlg::NormalPhaza(double& phaza)
{
	while (1)
	{
		if (phaza > 0)
		{
			if (phaza > (2 * M_PI))  phaza -= 2 * M_PI;
			else break;
		}
		else
		{
			if (phaza < (-2 * M_PI)) phaza += 2 * M_PI;
			else break;
		}
	}
}

void CFuncNeoprDlg::Dopler_shift(vector<complex<double>>& mass, double sampling, double PhiDopler)
{
	for (int i = 0; i < mass.size(); i++)
	{
		mass[i] *= exp(complex<double>(0, 1) * 2. * M_PI * PhiDopler * (double)i / sampling);
	}
}

void CFuncNeoprDlg::OnBnClickedButton4()
{
	if (Signal_1.empty())return;
	UpdateData(1);
	vector<double> neopr_real;
	Uncertainty_omp(neopr_real, Signal_1, Signal_2,1);
	_pi = peak_intensity(neopr_real);
	vector<vector<double>> draw_vector;
	draw_vector.resize(1);
	draw_vector[0] = neopr_real;
	MyViewerDraw("Data", draw_vector, 0, draw_vector[0].size(), Obj_Ris, "2DFN.png", false);
	UpdateData(0);
}
