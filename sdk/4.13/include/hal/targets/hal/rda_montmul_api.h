#ifndef _RDA_MONTMUL_API_H_
#define _RDA_MONTMUL_API_H_

/**
 * \brief  RDA MPI structure
 */
typedef struct
{
 //   int s;                    /*!<  integer sign      */
    unsigned int n;           /*!<  total # of limbs  */
    unsigned int *p;          /*!<  pointer to limbs  */
} rda_mpi;

extern int rda_hw_montmul( rda_mpi *A, const rda_mpi *B, const rda_mpi *N, uint32_t mm );

#endif
