#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define TAPE_SIZE 32768   /* tape has be infinite but for bb5 it's enough */
#define TAPE_OFFSET 16384 /* head starts in the middle */
#define TOTAL_STEPS 47176870L
#ifndef INTERVAL
# define INTERVAL 1
#endif
#ifndef SLEEP_US
# define SLEEP_US 10000
#endif

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

enum { A, B, C, D, E, HALT };

/* transition table: [state][read] = {write, dir(+1=R,-1=L), next} */
static const int	g_table[5][2][3] = {
	/* A */ {{1, +1, B}, {1, -1, C}},
	/* B */ {{1, +1, C}, {1, +1, B}},
	/* C */ {{1, +1, D}, {0, -1, E}},
	/* D */ {{1, -1, A}, {1, -1, D}},
	/* E */ {{1, +1, HALT}, {0, -1, A}},
};

static const char	*g_states = "ABCDE*";

static void			render(const unsigned char *tape, int head, int state, long step, int tape_min, int tape_max);

int	main(void)
{
	unsigned char	*tape;
	int				head;
	int				state;
	long			step;
	int				tape_min;
	int				tape_max;

	tape = calloc(TAPE_SIZE, 1);
	head = TAPE_OFFSET;
	state = A;
	step = 0;
	tape_min = head;
	tape_max = head;
	while (state != HALT)
	{
		const int *t = g_table[state][tape[head]];
		tape[head] = t[0];
		head += t[1];
		state = t[2];
		if (head < tape_min)
			tape_min = head;
		if (head > tape_max)
			tape_max = head;
		step++;
		if (step % INTERVAL == 0)
		{
			render(tape, head, state, step, tape_min, tape_max);
			usleep(SLEEP_US);
		}
	}
	render(tape, head, state, step, tape_min, tape_max);
	free(tape);
	return (0);
}

static void	render(const unsigned char *tape, int head, int state, long step, int tape_min, int tape_max)
{
	long	eta;

	eta = ((TOTAL_STEPS - step) / INTERVAL) * SLEEP_US / 1000000;
	printf("\033[H\033[J");
	printf("Step %10ld / %ld (%5.1f%%) | State %c | Width %5d | Head @%-5d | ETA %03ld:%02ld:%02ld\n\n",
		step, TOTAL_STEPS, 100.0 * step / TOTAL_STEPS,
		g_states[state], tape_max - tape_min + 1, head - tape_min,
		eta / 3600, (eta / 60) % 60, eta % 60);
	for (int i = tape_min; i <= tape_max; i++)
	{
		if (i == head)
			printf("\033[7m%c\033[0m", tape[i] + '0');
		else
			printf("%c", tape[i] + '0');
	}
	printf("\n");
	fflush(stdout);
}
