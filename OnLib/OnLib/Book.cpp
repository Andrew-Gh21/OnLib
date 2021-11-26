#include "Book.h"

Book::Book(std::string name, std::string author, std::string ISBN, std::string description, TypeOfBook type)
	: m_name(name), m_author(author), m_ISBN(ISBN), m_description(description), type(type)
{
}

TypeOfBook Book::GetType() const
{
	return type;
}

std::string Book::GetName() const
{
	return m_name;
}

std::string Book::GetAuthor() const
{
	return m_author;
}

std::string Book::GetISBN() const
{
	return m_ISBN;
}

std::string Book::GetDescription() const
{
	return m_description;
}

void Book::SetName(const std::string &name)
{
	this->m_name = name;
}

void Book::SetDescription(const std::string &description)
{
	this->m_description = description;
}

void Book::SetAuthor(const std::string &author)
{
	this->m_author = author;
}

void Book::SetISBN(const std::string &ISBN)
{
	this->m_ISBN = ISBN;
}

void Book::SetType(const TypeOfBook &type)
{
	this->type = type;
}
