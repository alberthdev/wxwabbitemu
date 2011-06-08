#include "guidebug_wx.h"

/* TODO:
 * [21:07:24] <Buckeye> alberthro at this point you need to do 4 things
 * [21:07:30] <Buckeye> in this order
 * [21:07:35] <Buckeye> 1. get disassembly up and working
 * [21:07:54] <Buckeye> 2. make a registers view
 * [21:08:03] <Buckeye> 3. allow you to step
 * [21:08:25] <Buckeye> 4. make a memory view
 * [21:08:49] <Buckeye> at that point your debugger is adequate enough to solve 70% of problems
 * 
 * We'll get there... eventually! :)
 */

wxRect db_rect(-1, -1, -1, -1);
