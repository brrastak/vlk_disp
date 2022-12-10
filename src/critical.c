// user
#include "critical.h"
// cstd
#include <stdint.h>


uint32_t primask_bit;


// Начало критической секции
void EnterCritical(void)
{
    primask_bit = __get_PRIMASK();
    __disable_irq();
}
// Конец критической секции
void ExitCritical(void)
{
    __set_PRIMASK(primask_bit);
}
