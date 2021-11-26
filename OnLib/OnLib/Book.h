#pragma once

#include <string>

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
	std::string m_ISBN;
	std::string m_description;
	TypeOfBook type;

public:
	Book(std::string, std::string, std::string, std::string, TypeOfBook);
	Book() = default;

	std::string GetName() const;
	std::string GetAuthor() const;
	std::string GetISBN() const;
	std::string GetDescription() const;
	TypeOfBook GetType() const;

	void SetName(const std::string& name);
	void SetAuthor(const std::string &author);
	void SetISBN(const std::string &ISBN);
	void SetDescription(const std::string &description);
	void SetType(const TypeOfBook &type);
	
	~Book() = default;
};