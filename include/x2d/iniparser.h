#ifndef X2D_INI_PARSER_H
#define X2D_INI_PARSER_H

#include "engine.h"

class XDAPI XIniFile
{
public:
	XIniFile(string path);

	string getValue(const string &section, const string &key) const;
	void setValue(const string &section, const string &key, const string &value);

	bool isSection(const string &section) const;

	void save();

private:

	struct Key
	{
		string comment;
		string value;
	};

	struct Section
	{
		string comment;
		map<string, Key> keys;
	};

	map<string, Section> m_sections;
	string m_path;
};

#endif // X2D_INI_PARSER_H