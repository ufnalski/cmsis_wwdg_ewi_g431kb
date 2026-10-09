// By: ufnalski

#pragma once

#define USE_WINDOW
// #define RELOAD_IN_EWI

#if (defined(USE_WINDOW) + defined(RELOAD_IN_EWI) != 1)
#error "Choose exactly one option!"
#endif

// Why reloading in EWI resets the system if a window is introduced?

void WWDG_Init(void);
void WWDG_Refresh(void);
