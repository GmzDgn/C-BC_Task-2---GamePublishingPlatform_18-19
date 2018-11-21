//C++ Boot Camp - Task 2 - 2018-19 
//Name: Gamze Dogan
//Student number: b8037358

#include "DatabaseManager.h"

DatabaseManager::DatabaseManager(){}

DatabaseManager::~DatabaseManager()
{
	// We must delete users since we have take ownership.
	for (auto it : m_users)
	{
		if (it.second) delete it.second;
	}
}

DatabaseManager& DatabaseManager::instance()
{
	static DatabaseManager s_instance;
	return s_instance;
}

void DatabaseManager::load_data() {
	load_data_ListUser();
	load_data_ListGame();
}

void DatabaseManager::load_data_ListUser() {
	ifstream fileList;
	string readFile, username, password, email, usertype, funds;
	fileList.open("listOfUsers.txt");

	if (fileList.is_open()) {
		while (fileList >> username >> password >> email >> usertype >> funds) {
			if (usertype == "admin") {
				add_user(new AdminUser(username, password, email));
			}
			else {
				add_user(new PlayerUser(username, password, email, stod(funds)));
			}
		}
		fileList.close();
	} else {
		cerr << "Couldn't open the file!";
	}
}

void DatabaseManager::load_data_ListGame() {
	ifstream fileList;
	int gameId = 0;
	double price;
	string readFile, title, desc;
	fileList.open("listOfGames.txt");

	if (fileList.is_open()) {
		while (getline(fileList, readFile)) {
			gameId = stoi(getVariable(readFile));
			title = getVariable(readFile);
			desc = getVariable(readFile);
			price = stod(getVariable(readFile));

			add_game(Game(gameId, title, desc, price));
		}
		this->gameIdCounter = gameId;
		fileList.close();
	} else {
		cerr << "Couldn't open the file!";
	}
}

void DatabaseManager::modify_user(const string& username, const double newFund) {
	ifstream fileList;
	ofstream outFile("listOfTheUsers.txt");
	string readFile, id, fund, password, email, usertype;
	fileList.open("listOfUsers.txt", ios::app);

	if (fileList.is_open()) {
		while (getline(fileList, readFile)) {
			istringstream stream(readFile);
			stream >> id >> password >> email >> usertype >> fund;
			if (id != username) {
				outFile << readFile << endl;
			} else {
				readFile = id + ' ' + password + ' ' + email + ' ' + usertype + ' ' + to_string(newFund);
				outFile << readFile << endl;
			}
		}
		fileList.close();
		outFile.close();

		remove("listOfUsers.txt");
		rename("listOfTheUsers.txt", "listOfUsers.txt");
	} else {
		cerr << "Couldn't open the file!";
	}
}

void DatabaseManager::modify_game(Game*& rGame, const string& newPrice, const string& newDescription) {
	ifstream fileList;
	ofstream outFile("listOfTheGames.txt");
	string readFile, id, price;
	fileList.open("listOfGames.txt", ios::app);

	if (fileList.is_open()) {
		while (getline(fileList, readFile)) {
			int pos = readFile.find(';');
			id = readFile.substr(0, pos);
			if (rGame->get_game_id() != stoi(id)) {
				outFile << readFile << endl;
			} else {
				if (!newPrice.empty()) {
					readFile = id + ";" + rGame->get_title() + ";" + rGame->get_desc() + ";" + newPrice + ";\n";
					outFile << readFile;
				} else if (!newDescription.empty()) {
					readFile = id + ";" + rGame->get_title() + ";" + newDescription + ";" + to_string(rGame->get_price()) + ";\n";
					outFile << readFile;
				}

			}
		}
	}
	fileList.close();
	outFile.close();

	remove("listOfGames.txt");
	rename("listOfTheGames.txt", "listOfGames.txt");
}

void DatabaseManager::delete_game(const string& gameId) {
	ifstream fileList;
	ofstream outFile("listOfTheGames.txt");
	string readFile, id;
	fileList.open("listOfGames.txt", ios::app);
	
	if (fileList.is_open()) {
		while (getline(fileList, readFile)) {
			int pos = readFile.find(';');
			id = readFile.substr(0, pos);
			if (gameId != id) {
				outFile << readFile;
			}
			else {
				//TODO FUNKTIONIRTS?
				m_games.erase(stoi(gameId));
			}
		}
	}
	fileList.close();
	outFile.close();

	remove("listOfGames.txt");
	rename("listOfTheGames.txt", "listOfGames.txt");
}

void DatabaseManager::store_newGame(string& gameTitle, string& description, double gamePrice) {
	ofstream outFile;
	outFile.open("listOfGames.txt", ios::app);
	int id = ++(this->gameIdCounter);

	string title = gameTitle;
	string desc = description;
	string gameId = to_string(id);
	double price = gamePrice;

	if (!outFile) {
		cerr << "Textfile doesn't exist!";
	} else {
		outFile << id << ';' << title << ';' << desc << ';' << to_string(price) << ';' << endl;
		outFile.close();
		add_game(Game(id, title, desc, price));
		cout << "You have added the game " << title << " successfully!" << endl;
	}
}

void DatabaseManager::store_newUser(UserBase::Username& user, string& pw, string& mail, string& type)
{
	ofstream outFile;
	outFile.open("listOfUsers.txt", ios::app);
	string username = user;
	string password = pw;
	string email = mail;
	string usertype = type;
	string funds = "0.0";

	if (!outFile) {
		cerr << "Textfile doesn't exist!";
	} else {
		outFile << username << ' ' << password << ' ' << email << ' ' << usertype << ' ' << funds << endl;
		outFile.close();
		if (usertype == "admin") {
			add_user(new AdminUser(username, password, email));
		} else {
			add_user(new PlayerUser(username, password, email, stod(funds)));
			cout << "You have added the user " << username << " successfully!" << endl;
		}
	}
}

void DatabaseManager::add_user(UserBase* pUser)
{
	// Store the user instance in the user map, indexed by username.
	// We are taking ownership of the memory pointer to.
	if (pUser)
	{
		m_users.insert(std::make_pair(pUser->get_username(), pUser));
	}
}

void DatabaseManager::add_game(Game& rGame)
{
	// Store the game indexed by game id.
	m_games.insert(std::make_pair(rGame.get_game_id(), rGame));
}

Game* DatabaseManager::find_game(const Game::GameId gameid) {
	auto it = m_games.find(gameid);
	if (it != m_games.end()) {
		return &it->second;
	} else {
		return nullptr;
	}
}

Game* DatabaseManager::find_game_by_title(string& title) {
	string searchTitle = to_lower_string(title);
	string iteratorTitle;

	auto it = m_games.begin();
	while (it != m_games.end()) {
		iteratorTitle = it->second.get_title();
		iteratorTitle = to_lower_string(iteratorTitle);
		if (iteratorTitle == searchTitle) {
			return &it->second;
		}
		++it;
	}
	return nullptr;
}

string DatabaseManager::to_lower_string(string& lowerString) {
	string searchLowerTitle;
	for (int i = 0; i <= lowerString.length(); ++i) {
		searchLowerTitle += tolower(lowerString[i]);
	}
	return searchLowerTitle;
}


bool DatabaseManager::find_email(const string& mail) {
	for (auto it = m_users.begin(); it != m_users.end(); ++it) {
		if ((*it).second->get_email() == mail) {
			return true;
		}
	}
	return false;
}

UserBase* DatabaseManager::find_user(const UserBase::Username& username)
{
	auto it = m_users.find(username);
	if (it != m_users.end())
	{
		return it->second;
	} else {
		return nullptr;
	}
}

string DatabaseManager::getVariable(string& readFile) {
	int pos = readFile.find(';');
	string tmp = readFile.substr(0, pos);
	readFile = readFile.substr((pos + 1), readFile.length() - (pos + 1));
	return tmp;
}
