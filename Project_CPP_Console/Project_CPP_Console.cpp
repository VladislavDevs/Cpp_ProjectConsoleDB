#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <limits>
#include <unordered_map>
#include <string>
#include <conio.h>
#include <windows.h>
#include<functional>

using namespace std;

class Person {
public:
	string _surname; //Фамилия.
	string _name; //Имя.
	string _middleName; //Отчество.
	string _street;
	unsigned short _house;
	unsigned short _apartment;
	string _phoneNumber;

	Person(string surname, string name, string middleName, string street, unsigned short house, unsigned short appartment, string phoneNumber) :
		_surname(surname), _name(name), _middleName(middleName), _street(street), _house(house), _apartment(appartment), _phoneNumber(phoneNumber) {
	}
};

//Отвечает за работу с БД.
class DatabaseManager {
	vector<Person> database;

public:
	//Загрузка данных из файла.
	void loadFromFile(const string& filename) {
		ifstream file(filename);

		if (!file) {
			cerr << "Ошибка открытия файла базы данных!" << endl;
			return;
		}

		string surname, name, middleName, street, phoneNumber;
		unsigned short house, apartment;

		while (file >> surname >> name >> middleName >> street >> house >> apartment >> phoneNumber) {
			database.emplace_back(surname, name, middleName, street, house, apartment, phoneNumber);
		}

		cout << "База данных успешно загружена. Загружено: " << database.size() << " записей\n";
	}

	//Проверка что введено число.
	int readInt(const string& prompt) {
		int value;

		cout << prompt;

		if (cin >> value) {
			return value;
		}
		else {
			cout << "Ошибка: введено не число. Попробуйте снова.\n";
			cin.clear();
			cin.ignore(32767, '\n');
		}
	}

	//Вывод данных из БД на экран в виде таблицы.
	void displayOnScreen() const {
		if (database.empty()) {
			cout << "База данных пустая!\n";
			return;
		}

		const int w_surname = 15;
		const int w_name = 15;
		const int w_middleName = 15;
		const int w_street = 20;
		const int w_house = 5;
		const int w_apartment = 8;
		const int w_phoneNumber = 15;

		//Вверхняя линия.
		cout << "||" << string(w_surname, '=') << "||"
			<< string(w_name, '=') << "||"
			<< string(w_middleName, '=') << "||"
			<< string(w_street, '=') << "||"
			<< string(w_house, '=') << "||"
			<< string(w_apartment, '=') << "||"
			<< string(w_phoneNumber, '=') << "||\n";

		//Заголовок.
		cout << "||" << left << setw(w_surname) << "Фамилия"
			<< "||" << setw(w_name) << "Имя"
			<< "||" << setw(w_middleName) << "Отчество"
			<< "||" << setw(w_street) << "Улица"
			<< "||" << setw(w_house) << "Дом"
			<< "||" << setw(w_apartment) << "Квартира"
			<< "||" << setw(w_phoneNumber) << "Телефон" << "||\n";

		//Промежуточная линия.
		cout << "||" << string(w_surname, '=') << "||"
			<< string(w_name, '=') << "||"
			<< string(w_middleName, '=') << "||"
			<< string(w_street, '=') << "||"
			<< string(w_house, '=') << "||"
			<< string(w_apartment, '=') << "||"
			<< string(w_phoneNumber, '=') << "||\n";

		//Выводим данные из БД.
		for (const auto& row : database) {
			cout << "||" << left << setw(w_surname) << row._surname
				<< "||" << setw(w_name) << row._name
				<< "||" << setw(w_middleName) << row._middleName
				<< "||" << setw(w_street) << row._street
				<< "||" << setw(w_house) << row._house
				<< "||" << setw(w_apartment) << row._apartment
				<< "||" << setw(w_phoneNumber) << row._phoneNumber << "||\n";
		}

		//Нижняя линия.
		cout << "||" << string(w_surname, '=') << "||"
			<< string(w_name, '=') << "||"
			<< string(w_middleName, '=') << "||"
			<< string(w_street, '=') << "||"
			<< string(w_house, '=') << "||"
			<< string(w_apartment, '=') << "||"
			<< string(w_phoneNumber, '=') << "||\n";
	}

	//Добавление новой строки в базу данных.
	void addRow() {
		string surname, name, middleName, street, phoneNumber;
		unsigned short house, apartment;

		cout << "Создание новой строки в базе данных. Введите новые данные:\n";

		cout << "Фамилия: ";
		cin >> surname;

		cout << "Имя: ";
		cin >> name;

		cout << "Отчество: ";
		cin >> middleName;

		cout << "Улица: ";
		cin >> street;

		house = readInt("Дом: "); //Проверяем что введено число.

		apartment = readInt("Квартира: "); //Проверяем что введено число.

		cout << "Телефон: ";
		cin >> phoneNumber;


		database.emplace_back(surname, name, middleName, street, house, apartment, phoneNumber);

		cout << "Запись успешно добавлена!\n";
	}

	//Удаление строки из БД по ID.
	void deleteRow(int id) {
		if (id < 1 || id > database.size()) {
			cerr << "Передан не корректный ID!\n";
			return;
		}

		database.erase(database.begin() + id - 1);

		cout << "Запись успешно удалена!\n";
	}

	//Сохранение БД в файл.
	void saveToFile(const string& filename) const {
		ofstream file(filename);

		if (!file) {
			cerr << "Ошибка сохранения в файл!\n";
			return;
		}

		for (const auto& row : database) {
			file << row._surname << "\t" << row._name << "\t" 
				<< row._middleName << "\t" << row._street << "\t" 
				<< row._house << "\t" << row._apartment << "\t" << row._phoneNumber << "\n";
		}

		cout << "Данные сохранены в " << filename << endl;
	}

	//Сортировка по фамилии. (*)
	void sortBySurname() {
		sort(database.begin(), database.end(), [](const Person& a, const Person& b) { return a._surname < b._surname; });

		cout << "База данных отсортирована по фамилии!\n";
	}

	//Поиск по фамилии. (*)
	void searchBySurname(const string& surname) const {
		bool found = false;
		for (const auto& row : database) {
			if (row._surname == surname) {
				if (!found) {
					cout << "Найденные записи:\n";
					found = true;
				}

				cout << row._surname << "\t" << row._name << "\t" << row._middleName << "\t" << row._phoneNumber << endl;
			}
		}

		if (!found)
			cout << "Записей не найдено!\n";
	}

	//Выборка по диапазону улиц. (**)
	void filterByStreet(const string& start, const string& end) const {
		cout << "Улицы от " << start << " до " << end << ":\n";

		for (const auto& row : database) {
			if (row._street >= start && row._street <= end) {
				cout << row._surname << "\t" << row._street << endl;
			}
		}
	}

	//Количество людей в домах. (Индивидуальная)
	void countPeoplePerHouse() const {
		unordered_map<unsigned short, int> counter;

		for (const auto& row : database) {
			counter[row._house]++;
		}

		cout << "Количество людей в домах:\n";

		for (const auto& [house, count] : counter) {
			cout << "Дом " << house << ": " << count << "человек.\n";
		}
	}

	// 1. Сформировать список уникальных адресов с количеством жильцов
	void showUniqueAddresses() const {
		struct Address {
			string street;
			unsigned short house;
			unsigned short apartment;
		};

		// Вектор пар: Address + count
		vector<pair<Address, int>> address_counts;

		for (const auto& p : database) {
			Address addr{ p._street, p._house, p._apartment };

			// Поиск адреса в векторе
			bool found = false;
			for (auto& entry : address_counts) {
				if (entry.first.street == addr.street &&
					entry.first.house == addr.house &&
					entry.first.apartment == addr.apartment) {
					entry.second++;
					found = true;
					break;
				}
			}

			if (!found) {
				address_counts.push_back({ addr, 1 });
			}
		}

		cout << "\nУникальные адреса и количество жильцов:\n";

		const int w_street = 20;
		const int w_house = 6;
		const int w_apartment = 10;
		const int w_count = 10;

		//Вверхняя линия.
		cout << "||" << string(w_street, '=') << "||"
			<< string(w_house, '=') << "||"
			<< string(w_apartment, '=') << "||"
			<< string(w_count, '=') << "||\n";

		cout << "||" << left << setw(w_street) << "Улица"
			<< "||" << setw(w_house) << "Дом"
			<< "||" << setw(w_apartment) << "Квартира"
			<< "||" << setw(w_count) << "Жильцов" << "||\n";

		//Промежуточная линия.
		cout << "||" << string(w_street, '=') << "||"
			<< string(w_house, '=') << "||"
			<< string(w_apartment, '=') << "||"
			<< string(w_count, '=') << "||\n";

		for (const auto& entry : address_counts) {
			cout << "||" << left << setw(w_street) << entry.first.street
				<< "||" << setw(w_house) << entry.first.house
				<< "||" << setw(w_apartment) << entry.first.apartment
				<< "||" << setw(w_count) << entry.second << "||\n";
		}

		//Последняя линия.
		cout << "||" << string(w_street, '=') << "||"
			<< string(w_house, '=') << "||"
			<< string(w_apartment, '=') << "||"
			<< string(w_count, '=') << "||\n";
	}

	// 2. Удалить абонентов без телефона (Индивидуальная)
	void removeEntriesWithoutPhone() {
		size_t before = database.size();

		database.erase(
			remove_if(database.begin(), database.end(),
				[](const Person& p) {
					return p._phoneNumber.empty() || p._phoneNumber == "-";
				}),
			database.end()
		);

		size_t after = database.size();
		cout << "Удалено записей без телефона: " << (before - after) << "\n";
	}
};

class Menu {
public:
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	void SetColor(int textColor, int bgColor) {
		SetConsoleTextAttribute(hConsole, (bgColor << 4) | textColor);
	}

	void showMenu(const string menuItems[], int itemCount, int selected) {
		system("cls");

		for (int i = 0; i < itemCount; i++) {
			if (i == selected) {
				SetColor(0, 15);
				cout << "> " << menuItems[i] << endl;
				SetColor(15, 0);
			}
			else {
				cout << "  " << menuItems[i] << endl;
			}
		}
	}
};

int main() {

	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "Russian");

	DatabaseManager db;

	string filename = "database.txt";

	//Кнопки меню.
	string menuItems[] = { 
		"1. Загрузить данные из файла", 
		"2. Посмотреть данные из БД", 
		"3. Добавить запись в БД", 
		"4. Удалить запись из БД", 
		"5. Сохранить данные в файл", 
		"6. Сортировать по фамилии", 
		"7. Поиск по фамилии",
		"8. Выборка по улицам", 
		"9. Статистика по домам", 
		"10. Сформировать список уникальных адресов с количеством жильцов", 
		"11. Удалить абонентов без телефона", 
		"0. Выход из программы" };

	const int menuSize = sizeof(menuItems) / sizeof(menuItems[0]);
	int selected = 0;
	int key;

	Menu menu;

	//Выводим меню.
	menu.showMenu(menuItems, menuSize, selected);

	//Подсказка к меню.
	cout << "\nИспользуйте стрелки (вверх) и (вниз) для перемещения по меню.\n";
	cout << "Нажмите Enter для выбора пункта меню.\n";

	while (true) {
		key = _getch();

		if (key == 224) {
			key = _getch();

			if (key == 72) {
				selected--;

				if (selected < 0)
					selected = menuSize - 1;
			}
			else if (key == 80) {
				selected++;

				if (selected >= menuSize)
					selected = 0;
			}

			system("cls");

			//Выводим меню.
			menu.showMenu(menuItems, menuSize, selected);

			//Подсказка к меню.
			cout << "\nИспользуйте стрелки (вверх) и (вниз) для перемещения по меню.\n";
			cout << "Нажмите Enter для выбора пункта меню.\n";
		}
		else if (key == 13) {
			system("cls");

			switch (selected) {
			case 0:
				db.loadFromFile(filename);
				break;

			case 1:
				db.displayOnScreen();
				break;

			case 2:
				db.addRow();
				break;

			case 3: {
				int id;

				id = db.readInt("Введите номер записи: ");

				db.deleteRow(id);
				break;
			}

			case 4:
				db.saveToFile(filename);
				break;

			case 5:
				db.sortBySurname();
				break;

			case 6: {
				string surname;
				cout << "Введите фамилию: ";
				cin >> surname;

				db.searchBySurname(surname);
				break;
			}

			case 7: {
				string start, end;

				cout << "Введите начало диапазона: ";
				cin >> start;

				cout << "Введите конец диапазона: ";
				cin >> end;

				db.filterByStreet(start, end);
				break;
			}

			case 8:
				db.countPeoplePerHouse();
				break;

			case 9:
				db.showUniqueAddresses();
				break;

			case 10:
				db.removeEntriesWithoutPhone();
				break;
			}

			if (selected == menuSize - 1)
				break;

			system("pause");
			system("cls");

			//Выводим меню.
			menu.showMenu(menuItems, menuSize, selected);

			//Подсказка к меню.
			cout << "\nИспользуйте стрелки (вверх) и (вниз) для перемещения по меню.\n";
			cout << "Нажмите Enter для выбора пункта меню.\n";
		}
	}

	return 0;
}