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
				auto pos_key_start = query.find_first_not_of(' ');
				auto pos_ignor = query.find('=', pos_key_start + 1);
				key = query.substr(pos_key_start, pos_ignor - pos_key_start);
				if (key.back() == ' ')
				{
					key = key.substr(0, key.find_last_not_of(' ') + 1 );
				}
				auto pos_value_start = query.find_first_not_of(' ', pos_ignor + 1);
				value = query.substr(pos_value_start);
				if (value.back() == ' ')
				{
					value = value.substr(0, value.find_last_not_of(' ') + 1);
				}
							
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

 