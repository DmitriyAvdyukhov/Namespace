#include "ini.h"

using namespace std;

// место для реализаций методов и функций библиотеки ini
// не забудьте, что они должны быть помещены в namespace ini

ini::Document ini::Load(std::istream& input)
{
	Document result;	
	string name;
	while (input)
	{
		string query;	
		string key;
		string value;
		getline(input, query);
		if (query[0] == '[')
		{
			auto pos = 1;
			auto pos_end = query.find(']');
			name = query.substr(pos, pos_end - pos);	
		}
		else
		{
			if (query.find('=') == query.npos)
			{
				if (name.length() > 0)
				{
					result.AddSection(name);
				}
			}
			else
			{
				auto pos = query.find_first_not_of(' ');
				auto pos_end = query.find('=', pos + 1);
				key = query.substr(pos, pos_end - pos);
				CutString(key);				
				pos = pos_end + 1;
				pos_end = query.find('\n', pos + 1);
				value = query.substr(pos, pos_end - pos);
				CutString(value);				
				result.AddSection(name).insert({ key, value });
			}
		}
	}
	return result;
}

ini::Section& ini::Document::AddSection(std::string name)
{
	Section& result = sections_[name];
	return result;
}

const ini::Section& ini::Document::GetSection(const std::string& name) const
{	
	if (sections_.count(name) > 0)
	{
		return sections_.at(name);
	}	
	static Section q{};
	return q;
}

std::size_t ini::Document::GetSectionCount() const
{
	return  sections_.size();
}

 void CutString(std::string& str)
{
	auto pos = str.find_first_not_of(' ');
	auto pos_end = str.find_last_not_of(' ');	
	str = str.substr(pos, pos_end - pos + 1);	
}