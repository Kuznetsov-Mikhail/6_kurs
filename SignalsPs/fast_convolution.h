#ifdef FROMDLL
	#define FCLIBAPI extern "C" __declspec(dllexport)
#else
	#define FCLIBAPI extern "C" __declspec(dllimport)
#endif

// Filtration algorithms
enum Filtering_Algorithm
{
	CPU_TD,
	CPU_FD,
	GPU_TD,
	GPU_FD
};

// Alias for vector < complex < float > > 
typedef std::vector < std::complex < float > > signal_buf;
// Alias for vector < vector < complex < float > > >
typedef std::vector < signal_buf > bank_buf;

/** Fast_convolustion function
 *  @param signal	Input signal
 *  @param filters	Bank of filters
 *  @param outputs  Output signals
 *  @param alg		Algorithm
 */
FCLIBAPI void __stdcall fast_convolution(	const signal_buf &signal,
											const bank_buf &filters,
											bank_buf &outputs,
											Filtering_Algorithm alg);

 /**
* Загрузить фильтр из файла
* @param file_name Имя файла
* @param filter    Вектор, в который будут записаны коэффициенты фильтра
* @return          Признак успеха операции
*/
FCLIBAPI bool __stdcall LoadFilter(    const std::string &file_name, std::vector <std::complex <float> > &filter);

/** Сгенерировать полосовой КИХ-фильтр
  * freq1				Нижняя частота среза (единицы частоты - Гц, кГц, МГц и т.д. Важно, чтобы все частоты были в одних единицах)
  * freq2				Верхняя частота среза
  * trans_zone_width	Ширина переходной зоны
  * sampling_rate		Частота дискретизации
  * fir					Импульсная характеристика КИХ-фильтра
*/
FCLIBAPI void __stdcall CreateFirFilter(		double freq1, 
												double freq2, 
												double trans_zone_width, 
												double sampling_rate, 
												std::vector <std::complex <float> > &fir);