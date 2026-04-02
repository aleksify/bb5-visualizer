#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define TAPE_SIZE 32768   /* tape has be infinite but for bb5 it's enough */
#define TAPE_OFFSET 16384 /* head starts in the middle */
#define INTERVAL 1
#define SLEEP_US 10000
#define TOTAL_STEPS 47176870L

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

static void			render(const unsigned char *tape, int head, int state, long step);

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
	while (state != HALT)
	{
		const int *t = g_table[state][tape[head]];
		tape[head] = t[0];
		head += t[1];
		state = t[2];
		step++;
		if (step % INTERVAL == 0)
		{
			render(tape, head, state, step);
			usleep(SLEEP_US);
		}
	}
	render(tape, head, state, step);
	free(tape);
	return (0);
}

static void	render(const unsigned char *tape, int head, int state, long step)
{
	int		tape_start;
	int		tape_end;
	double	eta;

	tape_start = head;
	tape_end = head;
	for (int i = 0; i < TAPE_SIZE; i++)
	{
		if (tape[i] || i == head)
		{
			if (i < tape_start)
				tape_start = i;
			if (i > tape_end)
				tape_end = i;
		}
	}
	eta = ((TOTAL_STEPS - step) / INTERVAL) * (SLEEP_US / 1e6);
	printf("\033[H\033[J");
	printf("Step %10ld / %ld (%5.1f%%) | State %c | Width %5d | Head @%-5d | ETA %03d:%02d:%02d\n\n",
		step, TOTAL_STEPS, 100.0 * step / TOTAL_STEPS,
		g_states[state], tape_end - tape_start + 1, head - tape_start,
		(int)(eta / 3600), (int)(eta / 60) % 60, (int)eta % 60);
	for (int i = tape_start; i <= tape_end; i++)
	{
		if (i == head)
			printf("\033[7m%c\033[0m", tape[i] ? '1' : '0');
		else
			putchar(tape[i] ? '1' : '0');
	}
	printf("\n");
	fflush(stdout);
}
