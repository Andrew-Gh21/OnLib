#include "Book.h"

void Book::SetName(std::string name)
{
	this->m_name = name;
}

void Book::SetDescription(std::string description)
{
	this->m_description = description;
}

void Book::SetAuthor(std::string author)
{
	this->m_author = author;
}

void Book::SetType(TypeOfBook type)
{
	this->type = type;
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

std::string Book::GetDescription() const
{
	return m_description;
}

Book::Book(std::string name, std::string author, std::string description,TypeOfBook type):m_name(name),m_author(author),m_description(description),type(type)
{

}
