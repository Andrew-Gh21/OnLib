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

std::string Book::GetName()
{
	return m_name;
}

std::string Book::GetAuthor()
{
	return m_author;
}

std::string Book::GetDescription()
{
	return m_description;
}

Book::Book(std::string name, std::string author, std::string description):m_name(name),m_author(author),m_description(description)
{
}
