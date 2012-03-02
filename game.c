#include "game.h"

/* Since you must rewrite every part of this file,
   there is no detailed comments. */

#define MAXN   1000
#define G      0.003
#define LENGTH 15
#define BLACK  0

/* So many pointers~ */
struct Firework {
	int x, y;
	float vx, vy;
	int color;
	int timestamp;
	struct Firework *prev, *next;
} pool[MAXN], *top, *active;

/* This is actually not random.
   Sequence is the same for every startup. */
inline int
rand() {
	static int seed = 0;
	seed = 0x015A4E35 * seed + 1;
	return (seed >> 16) & 0x7FFF;
}

void
init_game(void) {
	int i;
	for (i = 0; i < MAXN - 1; i ++) {
		pool[i].next = pool + (i + 1);
	}
	pool[MAXN - 1].next = NULL;
	top = pool;
	active = NULL;
}

static inline int
get_x(struct Firework *ptr, float timestamp) {
	return (int)(ptr->x + ptr->vx * timestamp + G * timestamp * timestamp);
}
static inline int
get_y(struct Firework *ptr, float timestamp) {
	return (int)(ptr->y + ptr->vy * timestamp);
}

void
start_frame(void) {
	static int count = 0;
	struct Firework *ptr;
	int i;

	count = (count + 1) % 50;
	if (count == 0) {
		key_stroke(0);
	}

	/* erase the trace */
	for (ptr = active; ptr != NULL; ptr = ptr->next) {
		for (i = ptr->timestamp; i >= 0 && i >= ptr->timestamp - (LENGTH - 1); i --) {
			draw_pixel( get_x(ptr, i), get_y(ptr, i), BLACK );
		}
	}

	/* draw the current frame */
	ptr = active;
	while (ptr != NULL) {
		struct Firework *prev = ptr->prev, *next = ptr->next;
		ptr->timestamp ++;

		if (ptr->timestamp > 200) {
			if (prev != NULL) prev->next = next;
			if (next != NULL) next->prev = prev;

			if (prev == NULL && next == NULL) active = NULL;
			ptr->next = top;
			ptr->prev = NULL;
			top = ptr;
		} else {
			for (i = ptr->timestamp; i >= 0 && i >= ptr->timestamp - (LENGTH - 1); i --) {
				draw_pixel( get_x(ptr, i), get_y(ptr, i), ptr->color );
			}
		}
		ptr = next;
	}
}

/* cos(k/10 * 2pi), sin(k/10 * 2pi), k = 0, 1, ... 9 */
float vx [] = {1.00000, 0.80902, 0.30902, -0.30902, -0.80902, -1.00000, -0.80902, -0.30902, 0.30902, 0.80902};
float vy [] = {0.00000, 0.58779, 0.95106, 0.95106, 0.58779, 0.00000, -0.58779, -0.95106, -0.95106, -0.58779};

/* Add a new firework */
void
key_stroke(int code) {
	struct Firework *ptr;
	int i;
	int x = rand() % 100 + 50;
	int y = rand() % 160 + 80;
	int color = rand() % 256 & 0x6c;

	if (code > 128) return;

	for (i = 0; i < 10; i ++) {
		if (top == NULL) {
			return;
		}
		ptr = top;
		top = top->next;

		ptr->x = x;
		ptr->y = y;
		ptr->vx = vx[i];
		ptr->vy = vy[i];
		ptr->color = color;
		ptr->timestamp = 0;

		ptr->next = active;
		ptr->prev = NULL;
		if (active != NULL) active->prev = ptr;
		active = ptr;
	}
}
