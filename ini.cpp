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
		if (query.find('[') != query.npos)
		{
			query.resize(query.find(']'));
			auto pos = query.find_first_not_of(' ', query.find('[') + 1);
			auto pos_end = query.find_last_not_of(' ') + 1;
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
				auto pos_key_end = 0;
				auto pos_ignor = query.find('=', pos_key_start + 1);
				
				char ch_left = query[pos_ignor - 1];	

				ch_left == ' ' ? pos_key_end = query.find_last_not_of(' ', pos_ignor - 1) + 1 : pos_key_end = pos_ignor;	

				auto pos_value_start = query.find_first_not_of(' ', pos_ignor + 1);
				auto pos_value_end = 0;

				query.back() == ' ' ? pos_value_end = query.find_last_not_of(' ') + 1 : pos_value_end = query.back();		
							
				result.AddSection(name)[query.substr(pos_key_start, pos_key_end - pos_key_start)] =
					query.substr(pos_value_start, pos_value_end - pos_value_start);
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

 