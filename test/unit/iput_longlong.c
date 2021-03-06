/*
 * adaptation of example from SGI man page for shmem_iput.
 */

#include <stdio.h>
#include <shmem.h>
#define _IPUT(a) shmem_##a##_iput

//#define IPUT _IPUT(long)
#define IPUT shmem_longlong_iput
#define DataType long long

static DataType source[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
static DataType target[10];

int main(int argc, char **argv)
{
	int me, nProcs, rc=0;

	shmem_init();
	me = shmem_my_pe();
	nProcs = shmem_n_pes();

	if (me == 0) {
		int j;
		/* put 5 words into target on PE's [1 to (nProcs-1)] */
		for(j=1; j < nProcs; j++)
			IPUT (target, source, 1, 2, 5, j);
	}

	shmem_barrier_all(); /* sync sender and receiver */

	if (me != 0) {
		if (target[0] != 1 ||
		    target[1] != 3 ||
		    target[2] != 5 ||
		    target[3] != 7 ||
		    target[4] != 9)
		{
			printf("ERR: target on PE %d is %lld %lld %lld %lld %lld\n"
				"  Expected 1,3,5,7,9?\n",
				me, target[0], target[1], target[2],
				target[3], target[4] );
			rc = 1;
		}
	}

	shmem_finalize();

	return rc;
}
