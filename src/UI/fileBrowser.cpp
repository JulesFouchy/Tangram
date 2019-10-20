#include "fileBrowser.hpp"

#include "UI/log.hpp"

// Returns an empty string if dialog is canceled
std::string FileBrowser::openfilename(const char* filter, HWND owner ) {
	tagOFNA ofn;
	char fileName[MAX_PATH] = "";
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = owner;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = "";
	std::string fileNameStr;
	if (GetOpenFileNameA(&ofn))
		fileNameStr = fileName;
	return fileNameStr;
}

// Returns an empty string if dialog is canceled
std::tuple<std::string, int> FileBrowser::savefilename(const char* filter, HWND owner) {
	OPENFILENAME ofn;
	char fileName[MAX_PATH] = "";
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = owner;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = "";
	std::string fileNameStr;
	if (GetSaveFileNameA(&ofn))
		fileNameStr = fileName;
	return std::tuple<std::string, int>(fileNameStr, ofn.nFilterIndex);
}