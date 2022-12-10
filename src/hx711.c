
#include "hx711.h"
#include "critical.h"

// Локальные дефайны

// Количество бит в посылке
#define HX711_BITS_IN_PACKET 24
// Битовая маска для определения переполнения (1 в старшем бите)
#define HX711_NEGATIV_MASK 0x800000
// Все 24 бита
#define HX711_ALL_BITS_MASK 0xFFFFFF
// Делитель. Потом откалибровать, чтобы это были миллиметры ртутного столба
#define HX711_DIV 1
// Значение обрезки (три значащие десятичные цифры)
#define HX711_CROP_RES 999L
// Число раз, когда считанное значение == 0, после чего датчик определяется как отсутствующий
#define HX711_NA_CNT_MAX 10

// Прототипы приватных методов (для использования только внутри этого класса)

// Калибровка датчика
void HX711_Calibrate(hx711_t *object);
// Обмен данными с датчиком
int32_t HX711_ReadWriteAtomic(hx711_t *object);
// Считывание данных с датчика
int32_t HX711_GetData(hx711_t *object);
// Отправить число импульсов, соответствующее коэффициенту усиления
void HX711_WriteGain(hx711_t *object);
// Проверить доступность датчика
bool HX711_CheckIfAvailable(hx711_t *object, int32_t HX711_value);
// Занят ли датчик
bool HX711_IsBusy(hx711_t *object);
// Считать значение линии данных
bool HX711_ReadDt(hx711_t *object);
// Установить значение на линии тактирования
void HX711_WriteClk(hx711_t *object, bool set);
// Рассчитать значение
int16_t HX711_CalculateValue(hx711_t *object, int32_t data);
// Получить отрицательное значение из дополнительного кода
int32_t HX711_GetNegativ(uint32_t data);


// Реализации публичных методов

// Конструктор (вызывается в самом начале для инициализации значений полей класса)
void HX711_Constructor(hx711_t *object,	   // Ссылка на объект класса Sensor
					   GPIO_TypeDef *port, // Порт, куда подключён датчик
					   uint16_t clk_pin,   // Пин линии тактирования
					   uint16_t dt_pin,	   // Пин линии данных
					   gain_factor_t gain) // Значение коэффициента усиления
{
	// Инициализация полей начальными значениями
	object->m_port = port;
	object->m_clk_pin = clk_pin;
	object->m_dt_pin = dt_pin;
	object->m_gain = gain;

	object->m_value = 0;
	object->m_available = true;
	object->m_na_cnt = 0;

	HX711_Calibrate(object);
}

// Сканировать датчики
void HX711_Scan(hx711_t *object)
{
	// Пропускаем процедуру если датчик занят
	// При подтяжке вниз, отсутствие датчика или готовый к работе датчик = low
	if (HX711_IsBusy(object))
	{
		return;
	}

	// Промежуточное значение выходного результата
	int32_t raw;

	raw = HX711_ReadWriteAtomic(object);
	// raw = HX711_CropData(raw);

	object->m_available = HX711_CheckIfAvailable(object, raw);

	if (object->m_available == false)
	{
		return;
	}

	object->m_value = HX711_CalculateValue(object, raw);
}

// Получить показания датчика
int16_t HX711_GetValue(hx711_t *object)
{
	// Пользователь класса не должен иметь доступ напрямую к полям класса
	return object->m_value;
}

// Доступен ли датчик
bool HX711_IsAvailable(hx711_t *object)
{
	// Пользователь класса не должен иметь доступ напрямую к полям класса
	return object->m_available;
}

// Реализации приватных методов

void HX711_Calibrate(hx711_t *object)
{
	object->m_calibration = HX711_ReadWriteAtomic(object);
}

int32_t HX711_ReadWriteAtomic(hx711_t *object)
{
	// результат
	int32_t res;

	// ожидаем
	HX711_WriteClk(object, false);

	// Отключаем прерывания
	EnterCritical();

	res = HX711_GetData(object);

	HX711_WriteGain(object);

	// Включаем прерывания
	ExitCritical();

	return res;
}

int32_t HX711_GetData(hx711_t *object)
{
	// Результат
	int32_t res = 0;

	// Даем 24 импульса и собираем по битам значение на датчике
	// MSB first
	for (int i = 0; i < HX711_BITS_IN_PACKET; ++i)
	{
		HX711_WriteClk(object, true);
		res <<= 1;

		if (HX711_ReadDt(object))
		{
			++res;
		}

		HX711_WriteClk(object, false);
	}

	// Отрицательное значение
	res = HX711_GetNegativ(res);

	return res;
}

void HX711_WriteGain(hx711_t *object)
{
	// Пишем GAIN в датчик
	for (int i = 0; i < object->m_gain; ++i)
	{
		HX711_WriteClk(object, true);
		HX711_WriteClk(object, false);
	}
}

bool HX711_CheckIfAvailable(hx711_t *object, int32_t HX711_value)
{
	bool isZero = (HX711_value == 0);
	bool isAvailable = (object->m_available);

	if (isZero)
	{
		object->m_na_cnt++;
		if (object->m_na_cnt >= HX711_NA_CNT_MAX)
		{
			if (isAvailable)
			{
				return false;
			}
		}
	}
	else
	{
		object->m_na_cnt--;
		if (object->m_na_cnt <= 0)
		{
			object->m_na_cnt = 0;
			if (!isAvailable)
			{
				HX711_Calibrate(object);
				return true;
			}
		}
	}

	return object->m_available;
}

bool HX711_IsBusy(hx711_t *object)
{
	return (HX711_ReadDt(object) == true);
}

bool HX711_ReadDt(hx711_t *object)
{
	return HAL_GPIO_ReadPin(object->m_port, object->m_dt_pin);
}

void HX711_WriteClk(hx711_t *object, bool set)
{
	if (set)
	{
		HAL_GPIO_WritePin(object->m_port, object->m_clk_pin, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(object->m_port, object->m_clk_pin, GPIO_PIN_RESET);
	}
}

int16_t HX711_CalculateValue(hx711_t *object, int32_t data)
{
	// Результат
	int32_t res;

	// Вычисляем значение с коэффициентами
	res = (data - object->m_calibration) / HX711_DIV;

	// Обрезаем до 3 цифр
	if (res > HX711_CROP_RES)
	{
		res = HX711_CROP_RES;
	}
	if (res < -HX711_CROP_RES)
	{
		res = -HX711_CROP_RES;
	}

	return (int16_t)res;
}

int32_t HX711_GetNegativ(uint32_t data)
{
	if ((data & HX711_NEGATIV_MASK) == 0)
	{
		return data;
	}

	data = ~data;
	data &= HX711_ALL_BITS_MASK;
	int32_t res = data + 1;
	res = -res;

	return res;
}
