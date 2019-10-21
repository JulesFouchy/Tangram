#pragma once

#include <functional>

class GUIwidget_FilepathPicker {
public:
	GUIwidget_FilepathPicker(const char* fileFilter);
	~GUIwidget_FilepathPicker() = default;

	void Show();
	inline const std::string& getFilepath() const { return m_filepath; }
private:
	std::string m_filepath;
	const char* m_fileFilter;
};