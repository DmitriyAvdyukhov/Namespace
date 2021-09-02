#include "json.h"
#include "xml.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string_view>
#include <vector>
#include <fstream>

using namespace std;

struct Spending 
{
    string category;
    int amount;
};

int CalculateTotalSpendings(const vector<Spending>& spendings)
{
    int result = 0;
    for (const Spending& s : spendings) 
    {
        result += s.amount;
    }
    return result;
}

string FindMostExpensiveCategory(const vector<Spending>& spendings) 
{
    auto compare_by_amount = [](const Spending& lhs, const Spending& rhs)
    {
        return lhs.amount < rhs.amount;
    };
    return max_element(begin(spendings), end(spendings), compare_by_amount)->category;
}

vector<Spending> LoadFromJson(istream& input)
{
    using namespace json;
    vector<Spending> result;
    Document document = Load(input);
    Node node = document.GetRoot();   
       
    if (node.AsArray().size() != 0)
    {
        vector<Node>vNode = node.AsArray();
        for (const auto& n : vNode)
        {
            Spending spending;
            map<std::string, Node> as_map = n.AsMap();
            spending.amount = as_map.at("amount").AsInt();
             Node as_map_category = as_map.at("category");
            spending.category = as_map_category.AsString();
            result.push_back(spending);
        }
    }
    return result;
}

vector<Spending> LoadFromXml(istream& input)
{
    using namespace xml;
    vector<Spending> result;
    Document document = Load(input);
    Node node = document.GetRoot();  

    if (node.Children().size() != 0)
    {
        vector<Node>vNode = node.Children();
        for (const auto& n : vNode)
        {
            Node q = n;
            result.push_back({ q.AttributeValue<string>("category"s) , q.AttributeValue<int>("amount"s) });
        }
    }
    return result;
}

int main() {

    {
        using namespace json;
        ifstream in("TextFile2.txt");
        const vector<Spending> spendings = LoadFromJson(in);
        cout << "Total "sv << CalculateTotalSpendings(spendings) << '\n';
        cout << "Most expensive is "sv << FindMostExpensiveCategory(spendings) << '\n';
    }
    {
        using namespace xml;
        ifstream in1("TextFile3.txt");
        const vector<Spending> spendings1 = LoadFromXml(in1);
        cout << "Total "sv << CalculateTotalSpendings(spendings1) << '\n';
        cout << "Most expensive is "sv << FindMostExpensiveCategory(spendings1) << '\n';
    }
}