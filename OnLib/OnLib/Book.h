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

	void SetName(std::string name);
	void SetAuthor(std::string author);
	void SetISBN(std::string ISBN);
	void SetDescription(std::string description);
	void SetType(TypeOfBook type);
	
	~Book() = default;
};