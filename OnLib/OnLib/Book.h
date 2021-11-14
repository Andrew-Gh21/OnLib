#pragma once
#include<string>

enum class TypeOfBook {
	SF,
	Action,
	Comedy,
	Romance,
	Drama
};
class Book
{
private:
	std::string m_name;
	std::string m_author;
	std::string m_description;
	TypeOfBook type;
public:
	void SetName(std::string name);
	void SetDescription(std::string description);
	void SetAuthor(std::string author);
	void SetType(TypeOfBook type);
	TypeOfBook GetType();
	std::string GetName();
	std::string GetAuthor();
	std::string GetDescription();
	Book() = default;
	Book(std::string, std::string, std::string,TypeOfBook);
	~Book() = default;
};

