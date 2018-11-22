//C++ Boot Camp - Task 2 - 2018-19 
//Name: Gamze Dogan
//Student number: b8037358

#include "DatabaseManager.h"

DatabaseManager::DatabaseManager() {}

DatabaseManager::~DatabaseManager() {
	// We must delete users since we have take ownership.
	for (auto it : m_users) {
		if (it.second) delete it.second;
	}
}

DatabaseManager& DatabaseManager::instance() {
	static DatabaseManager s_instance;
	return s_instance;
}

void DatabaseManager::load_data() {
	load_list_users();
	load_list_games();
	load_game_of_user();
}

void DatabaseManager::load_list_users() {
	string readFile, username, password, email, usertype, funds;
	ifstream file("listOfUsers.txt");

	if (file.is_open()) {
		while (file >> username >> password >> email >> usertype >> funds) {
			if (usertype == "admin") {
				add_user(new AdminUser(username, password, email));
			} else if(usertype == "player") {
				add_user(new PlayerUser(username, password, email, stod(funds)));
			} else {
				add_guest(new Guest(username, password, email));
			}
		}
		file.close();
	} else {
		cerr << "Couldn't open the file!";
	}
}

void DatabaseManager::load_list_games() {
	string readFile, title, desc;
	double price = 0.0;
	ifstream file("listOfGames.txt");

	if (file.is_open()) {
		while (getline(file, readFile)) {
			gameIdCounter = stoi(getVariable(readFile));
			title = getVariable(readFile);
			desc = getVariable(readFile);
			price = stod(getVariable(readFile));

			add_game(Game(gameIdCounter, title, desc, price));
		}
		file.close();
	} else {
		cerr << "Couldn't open the file!";
	}
}

void DatabaseManager::load_game_of_user() {
	string readFile, username, gameId;
	ifstream file("listOfUserGames.txt", ios::app);

	if (file.is_open()) {
		while (getline(file, readFile)) {
			username = getVariable(readFile);
			auto pUser = dynamic_cast<PlayerUser*>(find_user(username));
			while ((gameId = getVariable(readFile)) != "") {
				Game::GameId id = stoi(gameId);
				pUser->add_game_to_list(id);
			}
		}
		file.close();
	} else {
		cerr << "Couldn't open the file!";
	}
}

void DatabaseManager::modify_user(const string& username, const double newFund) {
	string readFile, id, fund, password, email, usertype;
	ifstream file("listOfUsers.txt", ios::app);
	ofstream newFile("listOfTheUsers.txt", ios::app);

	if (file.is_open()) {
		while (getline(file, readFile)) {
			istringstream stream(readFile);
			stream >> id >> password >> email >> usertype >> fund;
			if (newFile.is_open()) {
				if (id != username) {
					newFile << readFile << endl;
				} else {
					readFile = id + ' ' + password + ' ' + email + ' ' + usertype + ' ' + to_string(newFund);
					newFile << readFile << endl;
				}
			} else {
				cerr << "Couldn't open the file!";
			}
		}
		file.close();
		newFile.close();
		remove("listOfUsers.txt");
		rename("listOfTheUsers.txt", "listOfUsers.txt");
	} else {
		cerr << "Couldn't open the file!";
	}
}

void DatabaseManager::modify_game(Game*& rGame, const string& newPrice, const string& newDescription) {
	string readFile, id, price;
	ifstream file("listOfGames.txt", ios::app);
	ofstream newFile("listOfTheGames.txt");

	if (file.is_open()) {
		while (getline(file, readFile)) {
			int pos = readFile.find(';');
			id = readFile.substr(0, pos);
			if (newFile.is_open()) {
				if (rGame->get_game_id() != stoi(id)) {
					newFile << readFile << endl;
				}
				else {
					if (!newPrice.empty()) {
						readFile = id + ";" + rGame->get_title() + ";" + rGame->get_desc() + ";" + newPrice + ";\n";
						newFile << readFile;
					}
					else if (!newDescription.empty()) {
						readFile = id + ";" + rGame->get_title() + ";" + newDescription + ";" + to_string(rGame->get_price()) + ";\n";
						newFile << readFile;
					}
				}
			} else {
				cerr << "Couldn't open the file!";
			}
		}
		file.close();
		newFile.close();
		remove("listOfGames.txt");
		rename("listOfTheGames.txt", "listOfGames.txt");
	} else {
		cerr << "Couldn't open the file!";
	}
}

void DatabaseManager::delete_game(const string& gameId) {
	string readFile, id;
	ifstream file("listOfGames.txt", ios::app);
	ofstream newFile("listOfTheGames.txt");
	
	if (file.is_open()) {
		while (getline(file, readFile)) {
			int pos = readFile.find(';');
			id = readFile.substr(0, pos);
			if (newFile.is_open()) {
				if (gameId != id) {
					newFile << readFile;
				}
				else {
					m_games.erase(stoi(gameId));
				}
			} else {
				cerr << "Couldn't open the file!";
			}
		}
		file.close();
		newFile.close();
		remove("listOfGames.txt");
		rename("listOfTheGames.txt", "listOfGames.txt");
	} else {
		cerr << "Couldn't open the file!";
	}
}

void DatabaseManager::store_newGame(const string& gameTitle, const string& description, const double gamePrice) {
	ofstream outFile("listOfGames.txt", ios::app);
	double price = gamePrice;
	int id = ++(gameIdCounter);
	string gameId = to_string(id);
	string title = gameTitle;
	string desc = description;

	if (outFile.is_open()) {
		outFile << id << ';' << title << ';' << desc << ';' << to_string(price) << ';' << endl;
		outFile.close();
		add_game(Game(id, title, desc, price));
	} else {
		cerr << "Couldn't open the file!";
	}
	outFile.close();
}

void DatabaseManager::store_purchased_game(PlayerUser* rPlayer, Game* rGame) {
	ifstream file("listOfUserGames.txt", ios::app);
	ofstream newFile("listOfTheUserGames.txt", ios::app);
	string readFile, tmp, username, gameId;

	if (file.is_open()) {
		while (getline(file, readFile)) {
			username = getVariable(readFile);
			tmp = username + ";";
			while ((gameId = getVariable(readFile))!= "") {
				tmp += gameId;
				tmp += ";";
			}
			if (rPlayer->get_username() == username) {
				string id = to_string(rGame->get_game_id()) + ";";
				tmp += id;
			}
			tmp += '\n';
			if (newFile.is_open()) {
				newFile << tmp;
			} else {
				cerr << "Couldn't open the file!";
			}
		}
		file.close();
		newFile.close();
		remove("listOfUserGames.txt");
		rename("listOfTheUserGames.txt", "listOfUserGames.txt");
	} else {
		cerr << "Couldn't open the file!";
	}
}

void DatabaseManager::delete_game_of_user(PlayerUser* rPlayer, Game*& rGame) {
	string readFile, username, tmp, gameId;
	ifstream file("listOfUserGames.txt", ios::app);
	ofstream newFile("listOfTheUserGames.txt");

	if (file.is_open()) {
		while (getline(file, readFile)) {
			int pos = readFile.find(';');
			username = readFile.substr(0, pos);
			if (newFile.is_open()) {
				if (rPlayer->get_username() != username) {
					newFile << readFile << endl;
				} else {
					username = getVariable(readFile) + ";";
					tmp += username;
					while ((gameId = getVariable(readFile)) != "") {
						if (gameId != to_string(rGame->get_game_id())) {
							tmp += gameId;
							tmp += ";";
						}
					}
					tmp += "\n";
					newFile << tmp;
				}
			} else {
				cerr << "Couldn't open the file!";
			}
		}
		file.close();
		newFile.close();
		remove("listOfUserGames.txt");
		rename("listOfTheUserGames.txt", "listOfUserGames.txt");
	} else {
		cerr << "Couldn't open the file!";
	}
}

void DatabaseManager::store_newUser(const UserBase::Username& user, const string& pw, const string& mail, const string& type) {
	ofstream outFile("listOfUsers.txt", ios::app);
	string username = user;
	string password = pw;
	string email = mail;
	string usertype = type;
	string funds = "0.0";

	if (outFile.is_open()) {
		outFile << username << ' ' << password << ' ' << email << ' ' << usertype << ' ' << funds << endl;
		outFile.close();
		if (usertype == "admin") {
			add_user(new AdminUser(username, password, email));
		}
		else {
			add_user(new PlayerUser(username, password, email, stod(funds)));
		}
	} else {
		cerr << "Textfile doesn't exist!";
	}
}

void DatabaseManager::add_user(UserBase* pUser) {
	// Store the user instance in the user map, indexed by username.
	// We are taking ownership of the memory pointer to.
	if (pUser) {
		m_users.insert(std::make_pair(pUser->get_username(), pUser));
	}
}

void DatabaseManager::add_guest(UserBase* pUser) {
	if (pUser) {
		l_guest.push_back(pUser);
	}
}

void DatabaseManager::add_game(Game& rGame) {
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

UserBase* DatabaseManager::find_user(const UserBase::Username& username) {
	auto it = m_users.find(username);
	if (it != m_users.end()) {
		return it->second;
	} else {
		return nullptr;
	}
}

UserBase* DatabaseManager::find_guest(const string& email) {
	list<UserBase*>::iterator it = l_guest.begin();
	while (it != l_guest.end()) {
		if ((*it)->get_email() == email) {
			return *it;
		}
		++it;
	}
	return nullptr;
}

string DatabaseManager::getVariable(string& readFile) {
	int pos = readFile.find(';');
	string tmp = readFile.substr(0, pos);
	readFile = readFile.substr((pos + 1), readFile.length() - (pos + 1));
	return tmp;
}
