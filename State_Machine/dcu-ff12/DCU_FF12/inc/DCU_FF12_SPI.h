/**
 * @author Christopher Maneth (christopher.maneth@fast-forest.de)
 * @brief
 * @version 0.1
 * @date 2020-01-03
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef DCU_FF12_SPI_H
#define DCU_FF12_SPI_H

#include "DCU_FF12.h"

result_t   InitSPI1     (DCU_spi_options options);
short      TransferSPI1 (short data);
short      ReadSPI1      (void);

result_t   InitSPI2     (DCU_spi_options options);
short      TransferSPI2 (short data);
short      ReadSPI2      (void);


#endif //DCU_FF12_SPI_H
