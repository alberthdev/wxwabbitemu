#define USE_GIF_SIZES

#define GIF_FRAME_MAX (120 * 64 * 4)
#define SCRXSIZE 96
#define SCRYSIZE 64

#define GIF_IDLE 0
#define GIF_START 1
#define GIF_FRAME 2
#define GIF_END 3


#include "gui_wx.h"

extern int gif_write_state;
extern int gif_file_size;
extern char gif_file_name[256];
extern bool gif_autosave;
extern bool gif_use_increasing;
extern WORD gif_base_delay;
extern int gif_xs;
extern int gif_ys;
extern BYTE gif_frame[GIF_FRAME_MAX];
extern int gif_time;
extern int gif_newframe;
extern int gif_colors;
extern int gif_base_delay_start;
extern int gif_file_num;
extern int gif_size;
extern bool gif_bw;

void gif_writer();

