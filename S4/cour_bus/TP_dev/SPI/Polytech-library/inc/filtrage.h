#ifndef _FILTRAGE_H_
#define _FILTRAGE_H_

#include <stdint.h>

#define BUFFER_SIZE 81

void init_filtre();

void raz_tamp();
void inst_buff(int16_t new_ech);

int32_t filtre();


#endif // !_FILTRAGE_H_