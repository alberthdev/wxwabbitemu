#include "stdafx.h"

typedef struct {
	TCHAR name[32];
	int bit;
} bits_t;

typedef struct {
	TCHAR name[32];
	int flag;
	bits_t bits[8];
} flag_t;


flag_t flags83p[] = {
 {_T("trigFlags") ,0,{ { _T("") , -1 } , { _T("") , -1 } , { _T("trigDeg") , 2 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } } },
 {_T("kbdFlags"),0,{ { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("kbdSCR") , 3 } , { _T("kbdKeyPress") , 4 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } } },
 {_T("doneFlags"),0,{ { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , {_T("") , -1 } , { _T("donePrgm") , 5 } , { _T("") , -1 } , { _T("") , -1 } } },
 {_T("ioDelFlag"),0,{ { _T("inDelete") , 0 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } } },
 {_T("editFlags"),1,{ { _T("") , -1 } , { _T("") , -1 } , { _T("editOpen") , 2 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } } },
 {_T("monFlags"),1,{ { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("monAbandon") , 4 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } } },
 {_T("plotFlags"),2,{ { _T("") , -1 } , { _T("plotLoc") , 1 } , { _T("plotDisp") , 2 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } } },
 {_T("grfModeFlags"),2,{ { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("grfFuncM") , 4 } , { _T("grfPolarM") , 5 } , { _T("grfParamM") , 6 } , { _T("grfRecurM") , 7 } } },
 {_T("graphFlags"),3,{ { _T("graphDraw") , 0 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } } },
 {_T("grfDBFlags"),4,{ { _T("grfDot") , 0 } , { _T("grfSimul") , 1 } , { _T("grfGrid") , 2 } , { _T("grfPolar") , 3 } , { _T("grfNoCoord") , 4 } , { _T("grfNoAxis") , 5 } , { _T("grfLabel") , 6 } , { _T("") , -1 } } },
 {_T("textFlags"),5,{ { _T("") , -1 } , { _T("textEraseBelow") , 1 } , { _T("textScrolled") , 2 } , { _T("textInverse") , 3 } , { _T("textInsMode") , 4 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } } },
 {_T("ParsFlag2"),7,{ { _T("numOP1") , 0 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } } },
 {_T("newDispF"),8,{ { _T("preClrForMode") , 0 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } } },
 {_T("apdFlags"),8,{ { _T("") , -1 } , { _T("") , -1 } , { _T("apdAble") , 2 } , { _T("apdRunning") , 3 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } } },
 {_T("onFlags"),9,{ { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("onRunning") , 3 } , { _T("onInterrupt") , 4 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } } },
 {_T("statFlags"),9,{ { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("statsValid") , 6 } , { _T("statANSDISP") , 7 } } },
 {_T("fmtFlags"),10,{ { _T("fmtExponent") , 0 } , { _T("fmtEng") , 1 } , { _T("fmtHex") , 2 } , { _T("fmtOct") , 3 } , { _T("fmtBin") , 4 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } } },
 {_T("numMode"),10,{ { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("fmtReal") , 5 } , { _T("fmtRect") , 6 } , { _T("fmtPolar") , 7 } } },
 {_T("fmtOverride"),11,{ { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } } },
 {_T("fmtEditFlags"),12,{ { _T("fmtEdit") , 0 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } } },
 {_T("curFlags"),12,{ { _T("") , -1 } , { _T("") , -1 } , { _T("curAble") , 2 } , { _T("curOn") , 3 } , { _T("curLock") , 4 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } } },
 {_T("appFlags"),13,{ { _T("appWantIntrpt") , 0 } , { _T("appTextSave") , 1 } , { _T("appAutoScroll") , 2 } , { _T("appMenus") , 3 } , { _T("appLockMenus") , 4 } , { _T("appCurGraphic") , 5 } , { _T("appCurWord") , 6 } , { _T("appExit") , 7 } } },
 {_T("seqFlags"),15,{ { _T("webMode") , 0 } , { _T("webVert") , 1 } , { _T("sequv") , 2 } , { _T("seqvw") , 3 } , { _T("sequw") , 4 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } } },
 {_T("promptFlags"),17,{ { _T("promptEdit") , 0 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } } },
 {_T("indicFlags"),18,{ { _T("indicRun") , 0 } , { _T("indicInUse") , 1 } , { _T("indicOnly") , 2 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } } },
 {_T("shiftFlags"),18,{ { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("shift2nd") , 3 } , { _T("shiftAlpha") , 4 } , { _T("shiftLwrAlph") , 5 } , { _T("shiftALock") , 6 } , { _T("shiftKeepAlph") , 7 } } },
 {_T("tblFlags"),19,{ { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("autoFill") , 4 } , { _T("autoCalc") , 5 } , { _T("reTable") , 6 } , { _T("") , -1 } } },
 {_T("sGrFlags"),20,{ { _T("grfSplit") , 0 } , { _T("vertSplit") , 1 } , { _T("grfSChanged") , 2 } , { _T("grfSplitOverride") , 3 } , { _T("write_on_graph") , 4 } , { _T("g_style_active") , 5 } , { _T("cmp_mod_box") , 6 } , { _T("textWrite") , 7 } } },
 {_T("newIndicFlags"),21,{ { _T("extraIndic") , 0 } , { _T("saIndic") , 1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } } },
 {_T("newFlags2"),22,{ { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("noRestores") , 5 } , { _T("") , -1 } , { _T("") , -1 } } },
 {_T("smartFlags"),23,{ { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } } },
 {_T("more_Flags"),26,{ { _T("") , -1 } , { _T("") , -1 } , { _T("No_Del_Stat") , 2 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } } },
 {_T("linkFlags"),27,{ { _T("") , -1 } , { _T("IDis95h") , 1 } , { _T("IDis82h") , 2 } , { _T("IDis83h") , 3 } , { _T("IDis03h") , 4 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } } },
 {_T("asm_Flag1"),33,{ { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } } },
 {_T("asm_Flag2"),34,{ { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } } },
 {_T("asm_Flag3"),35,{ { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } } },
 {_T("getSendFlg"),36,{ { _T("") , -1 } , {_T( "comFailed") , 1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } } },
 {_T("appLwrCaseFlag"),36,{ { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("lwrCaseActive") , 3 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } } },
 {_T("apiFlag"),40,{ { _T("AppAllowContext") , 0 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("appRunning") , 4 } , { _T("") , -1 } , { _T("") , -1 } , { _T("appRetOffKey") , 7 } } },
 {_T("apiFlg3"),42,{ { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } } },
 {_T("apiFlg4"),43,{ { _T("") , -1 } , { _T("") , -1 } , { _T("fullScrnDraw") , 2 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } } },
 {_T("groupFlags"),38,{ { _T("") , -1 } , { _T("inGroup") , 1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } } },
 {_T("xapFlag0"),46,{ { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } } },
 {_T("xapFlag1"),47,{ { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } } },
 {_T("xapFlag2"),48,{ { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } } },
 {_T("xapFlag3"),49,{ { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } } },
 {_T("fontFlags"),50,{ { _T("") , -1 } , { _T("") , -1 } , { _T("fracDrawLFont") , 2 } , { _T("fracTallLFont") , 3 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("customFont") , 7 } } },
 {_T("plotFlag3"),60,{ { _T("bufferOnly") , 0 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("useFastCirc") , 4 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } } },
 {_T("scriptFlag"),51,{ { _T("alt_On") , 0 } , { _T("alt_Off") , 1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } } },
 {_T("rclFlag2"),51,{ { _T("") , -1 } , { _T("") , -1 } , { _T("useRclQueueEnd") , 2 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } } },
 {_T("backGroundLink"),51,{ { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("ignoreBPLink") , 3 } , { _T("bPLinkOn") , 4 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } } },
 {_T("sysHookFlg"),52,{ { _T("getCSCHookActive") , 0 } , { _T("libraryHookActive") , 1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("homescreenHookActive") , 4 } , { _T("rawKeyHookActive") , 5 } , { _T("catalog2HookActive") , 6 } , { _T("cursorHookActive") , 7 } } },
 {_T("sysHookFlg1"),53,{ { _T("tokenHookActive") , 0 } , { _T("localizeHookActive") , 1 } , { _T("windowHookActive") , 2 } , { _T("graphHookActive") , 3 } , { _T("yEquHookActive") , 4 } , { _T("fontHookActive") , 5 } , { _T("regraphHookActive") , 6 } , { _T("drawingHookActive") , 7 } } },
 {_T("sysHookFlag2"),54,{ { _T("traceHookActive") , 0 } , { _T("parserHookActive") , 1 } , { _T("appChangeHookActive") , 2 } , { _T("catalog1HookActive") , 3 } , { _T("helpHookActive") , 4 } , { _T("cxRedispHookActive") , 5 } , { _T("menuHookActive") , 6 } , { _T("silentLinkHookActive") , 7 } } },
 {_T("dBKeyFlags"),61,{ { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("keyDefaultsF") , 6 } , { _T("") , -1 } } },
 {_T(""),-1,{ { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } , { _T("") , -1 } } }
};
