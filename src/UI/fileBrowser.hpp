#pragma once
//Big thanks to https://www.daniweb.com/programming/software-development/threads/159753/opening-a-filedialog

#include <windows.h>
#include <string.h>
#include <iostream>
#include <Commdlg.h>

std::string openfilename(const char* filter = "All Files (*.*)\0*.*\0", HWND owner = NULL);