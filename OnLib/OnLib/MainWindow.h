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

public slots:
	void AddBooksToSection(const std::vector<data::Book>& books);

private slots:
	void HandleLogOutButton();
	void HandleDeleteAccountButton();
	void HandleSearchIconButton();
	void HandleHomeButton();
	void HandleMyListButton();
	void HandleSearchBooksButton(std::string s);
	void StyleSheets();
	
private:
	std::unordered_map<data::BookCategory, BookSection*> categories;
	Ui::MainWindow* ui;
	QAction* searchButtonWidgetAction;
	QAction* searchLineEditWidgetAction;
	QLineEdit* searchLineEdit;
};
