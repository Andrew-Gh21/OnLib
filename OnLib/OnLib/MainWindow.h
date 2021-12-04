#pragma once

#include <QMainWindow>
#include "ui_MainWindow.h"
#include<unordered_map>
#include "../OnLib.DataTransfer/BookCategory.h"
#include "BookSection.h"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = Q_NULLPTR);
	~MainWindow();
private slots:
	void HandleLogOutButton();
	void HandleDeleteAccountButton();
	void HandleSearchIconButton();
	void HandleBackToMenuButton();
	void HandleSearchBooksButton(std::string s);
	void AddBooksToSection(const std::vector<data::Book>& books);

private:
	std::unordered_map<data::BookCategory, BookSection*> categories;
	Ui::MainWindow* ui;
};
