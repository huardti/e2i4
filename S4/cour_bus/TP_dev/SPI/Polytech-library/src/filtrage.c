#include "filtrage.h"
#include <stdint.h>

static int16_t BUFFER[BUFFER_SIZE];
static int16_t COEFFS [] ={-8,-8,-8,-9,-9,-9,-9,-10,-10,-10,-10,-11,-11,-11,-11,-12,-12,-12,-12,-13,-13,-13,-13,-13,-13,-14,-14,-14,-14,-14,-14,-14,-14,-14,-15,-15,-15,-15,-15,-15,985,-15,-15,-15,-15,-15,-15,-14,-14,-14,-14,-14,-14,-14,-14,-14,-13,-13,-13,-13,-13,-13,-12,-12,-12,-12,-11,-11,-11,-11,-10,-10,-10,-10,-9,-9,-9,-9,-8,-8,-8};


void init_filtre()
{
    raz_tamp(BUFFER);
}

void raz_tamp()
{
    for (int i = 0; i < BUFFER_SIZE; i++) {
        BUFFER[i] = 0;
    }
}

void inst_buff(int16_t new_ech)
{
    for (int i = 0; i < BUFFER_SIZE - 1; i++) {
        BUFFER[i] = BUFFER[i+1];
    }
    BUFFER[BUFFER_SIZE - 1] = new_ech;
}

static int32_t calc_conv()
{
    int32_t y = 0;

    for (int k = 0; k < BUFFER_SIZE; k++) {
            y += BUFFER[k] * COEFFS[k];
    }
    return y;
}

int32_t filtre()
{
    int32_t y;
    y = calc_conv();

    return y/1000 + 2047;
}
