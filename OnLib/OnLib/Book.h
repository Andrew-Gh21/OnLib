#pragma once
#include<string>
class Book
{
private:
	std::string m_name;
	std::string m_author;
	std::string m_description;
public:
	void SetName(std::string name);
	void SetDescription(std::string description);
	void SetAuthor(std::string author);
	std::string GetName();
	std::string GetAuthor();
	std::string GetDescription();
	Book() = default;
	Book(std::string, std::string, std::string);
	~Book() = default;
};

