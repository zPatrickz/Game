#ifndef __GAME_H__
#define __GAME_H__

#include "types.h"
#include "const.h"
#include "x86/x86.h"
#include "irq.h"
#include "video.h"

void init_100hz_timer(void);
void init_game(void);
void start_frame(void);
void key_stroke(int);

#endif
