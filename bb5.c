#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define TAPE_SIZE 32768   /* tape has be infinite but for bb5 it's enough */
#define TAPE_OFFSET 16384 /* head starts in the middle */

/*
 * Busy Beaver 5-state champion (Marxen & Buntrock, 1989)
 * Produces 4098 ones in 47,176,870 steps.
 *
 * Transition table:  (state, read) -> (write, move, next_state)
 *   A,0 -> 1,R,B    A,1 -> 1,L,C
 *   B,0 -> 1,R,C    B,1 -> 1,R,B
 *   C,0 -> 1,R,D    C,1 -> 0,L,E
 *   D,0 -> 1,L,A    D,1 -> 1,L,D
 *   E,0 -> 1,R,H    E,1 -> 0,L,A
 */

enum
{
	A,
	B,
	C,
	D,
	E,
	HALT
};

/* transition table: [state][read] = {write, dir(+1=R,-1=L), next} */
static const int	g_table[5][2][3] = {
	/* A */ {{1, +1, B}, {1, -1, C}},
	/* B */ {{1, +1, C}, {1, +1, B}},
	/* C */ {{1, +1, D}, {0, -1, E}},
	/* D */ {{1, -1, A}, {1, -1, D}},
	/* E */ {{1, +1, HALT}, {0, -1, A}},
};

int	main(void)
{
	unsigned char	*tape;
	int				head;
	int				state;
	long			step;

	tape = calloc(TAPE_SIZE, 1);
	head = TAPE_OFFSET;
	state = A;
	step = 0;
}
