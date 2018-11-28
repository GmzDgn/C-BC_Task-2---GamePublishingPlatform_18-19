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
	load_records();
}

void DatabaseManager::load_list_users() {
	string readFile, username, password, email, usertype, funds, age;
	ifstream file("listOfUsers.txt");

	if (file.is_open()) {
		while (file >> username >> password >> email >> usertype >> funds >> age) {
			if (usertype == "admin") {
				add_user(new AdminUser(username, password, email));
			} else if(usertype == "player") {
				add_user(new PlayerUser(username, password, email, stod(age), stod(funds)));
			} else if(usertype == "guest") {
				add_guest(new Guest(username, password, email));
			} else if (usertype == "gamestudio") {
				add_user(new GameStudio(username, password, email));
			}
		}
		file.close();
	} else {
		cerr << "Couldn't open the file!";
	}
}

void DatabaseManager::load_list_games() {
	string readFile, title, desc, gameStudio;
	double price = 0.0;
	int ageLimit, version;
	ifstream file("listOfGames.txt");

	if (file.is_open()) {
		if (!(file.peek() == ifstream::traits_type::eof())) {
			while (getline(file, readFile)) {
				gameIdCounter = stoi(getVariable(readFile));
				title = getVariable(readFile);
				desc = getVariable(readFile);
				price = stod(getVariable(readFile));
				ageLimit = stoi(getVariable(readFile));
				gameStudio = getVariable(readFile);
				version = stoi(getVariable(readFile));
				Game rGame(gameIdCounter, title, desc, price, ageLimit, gameStudio, version);
				add_game(rGame);
				auto pStudio = dynamic_cast<GameStudio*>(find_user(gameStudio));
				if (pStudio != nullptr) {
					pStudio->add_game_to_list(rGame);
				}
				pStudio = nullptr;
			}
			file.close();
		}
	} else {
		cerr << "Couldn't open the file!";
	}
}

void DatabaseManager::load_game_of_user() {
	string readFile, username, gameId, time;
	ifstream file("listOfUserGames.txt", ios::app);

	if (file.is_open()) {
		if (!(file.peek() == ifstream::traits_type::eof())) {
			while (getline(file, readFile)) {
				username = getVariable(readFile);
				auto pUser = dynamic_cast<PlayerUser*>(find_user(username));
				while ((gameId = getVariable(readFile)) != "") {
					time = getVariable(readFile);
					pUser->set_purchased_time(time);
					Game::GameId id = stoi(gameId);
					auto pGame = find_game(id);
					pUser->add_game_to_list(pGame);
				}
			}
		}
		file.close();
	} else {
		cerr << "Couldn't open the file!";
	}
}

void DatabaseManager::load_records() {
	string username, gameTitle, date, time, lenghtOfPlay, readFile;
	ifstream file("listOfRecords.txt");
	int x = -1;

	if (file.is_open()) {
		if (!(file.peek() == ifstream::traits_type::eof())) {
			while (getline(file, readFile)) {
				username = getVariable(readFile);
				gameTitle = getVariable(readFile);
				date = getVariable(readFile);
				time = getVariable(readFile);
				lenghtOfPlay = getVariable(readFile);
				
				auto pUser = dynamic_cast<PlayerUser*>(find_user(username));
				if (pUser != nullptr) {
					pUser->push_records(gameTitle, date, time, lenghtOfPlay);
				}
			}
			file.close();
		}
	} else {
		cerr << "Couldn't open the file!";
	}
}

void DatabaseManager::modify_user(const string& username, const double newFund) {
	string readFile, id, fund, password, email, usertype, age;
	ifstream file("listOfUsers.txt", ios::app);
	ofstream newFile("listOfTheUsers.txt", ios::app);

	if (file.is_open()) {
		while (getline(file, readFile)) {
			istringstream stream(readFile);
			stream >> id >> password >> email >> usertype >> fund >> age;
			if (newFile.is_open()) {
				if (id != username) {
					newFile << readFile << endl;
				} else {
					readFile = id + ' ' + password + ' ' + email + ' ' + usertype + ' ' + to_string(newFund) + ' ' + age;
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

void DatabaseManager::modify_game(Game*& rGame, const string& newPrice, const string& newDescription, const string& newVersion) {
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
				} else {
					if (!newPrice.empty()) {
						readFile = id + ";" + rGame->get_title() + ";" + rGame->get_desc() + ";" + newPrice + ";" + to_string(rGame->get_ageLimit()) + ";" + rGame->get_studio() + ";" + to_string(rGame->get_version()) + ";\n";
						newFile << readFile;
					} else if (!newDescription.empty()) {
						readFile = id + ";" + rGame->get_title() + ";" + newDescription + ";" + to_string(rGame->get_price()) + ";" + to_string(rGame->get_ageLimit()) + ";" + rGame->get_studio() + ";" + to_string(rGame->get_version()) + ";\n";
						newFile << readFile;
					} else if (!newVersion.empty()) {
						readFile = id + ";" + rGame->get_title() + ";" + rGame->get_desc() + ";" + to_string(rGame->get_price()) + ";" + to_string(rGame->get_ageLimit()) + ";" + rGame->get_studio() + ";" + newVersion + ";\n";
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
					newFile << readFile << endl;
				}
				else {
					m_games.erase(stoi(gameId));
					delete_game_from_listOfUserGames(gameId);

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

void DatabaseManager::delete_game_from_listOfUserGames(const string& gameId) {
	string readFile, username, id, tmp, purchaseDate;
	ifstream file("listOfUserGames.txt", ios::app);
	ofstream newFile("listOfTheUserGames.txt");
	bool isErased = false;

	if (file.is_open()) {
		while (getline(file, readFile)) {
			username = getVariable(readFile);
			tmp = username + ";";
			while ((id = getVariable(readFile)) != "") {
				purchaseDate = getVariable(readFile);
				if (newFile.is_open()) {
					if (gameId != id) {
						tmp += id + ";" + purchaseDate + ";";
						newFile << tmp;
					} else {
						auto findPlayer = find_user(username);
						if (findPlayer != nullptr) {
							auto pPlayer = dynamic_cast<PlayerUser*>(findPlayer);
							auto myGames = pPlayer->get_game_list();
							myGames.erase(stoi(gameId));
							if (myGames.size() == 0) {
								isErased = true;
							}
						}
					}
				}
				else {
					cerr << "Couldn't open the file!";
				}
			}
			if (!isErased) {
				newFile << endl;
			}
			else {
				isErased = false;
			}
		}
		file.close();
		newFile.close();
		remove("listOfUserGames.txt");
		rename("listOfTheUserGames.txt", "listOfUserGames.txt");
	}
	else {
		cerr << "Couldn't open the file!";
	}
}

void DatabaseManager::store_newGame(const string& gameTitle, const string& description, const double gamePrice, const int rAgeLimit, const string& studio) {
	ofstream outFile("listOfGames.txt", ios::app);
	double price = gamePrice;
	int id = ++(gameIdCounter);
	string gameId = to_string(id);
	string title = gameTitle;
	string desc = description;
	int ageLimit = rAgeLimit;
	string gameStudio = studio;
	string version = "1";

	if (outFile.is_open()) {
		outFile << id << ';' << title << ';' << desc << ';' << to_string(price) << ';' << to_string(ageLimit) << ";" << gameStudio << ";" << version << ";" << endl;
		outFile.close();

		Game rGame(gameIdCounter, title, desc, price, ageLimit, gameStudio, stoi(version));
		add_game(rGame);
		auto pStudio = dynamic_cast<GameStudio*>(find_user(gameStudio));
		if (pStudio != nullptr) {
			pStudio->add_game_to_list(rGame);
		}
		pStudio = nullptr;
	} else {
		cerr << "Couldn't open the file!";
	}
	outFile.close();
}

void DatabaseManager::store_purchased_game(PlayerUser* rPlayer, Game* rGame) {
	ifstream file("listOfUserGames.txt", ios::app);
	ofstream newFile("listOfTheUserGames.txt", ios::app);
	string readFile, tmp, username, gameId, timeString;
	bool isStoredGame = false;

	if (file.is_open()) {
		if (!(file.peek() == ifstream::traits_type::eof())) {
			while (getline(file, readFile)) {
				username = getVariable(readFile);
				tmp = username + ";";
				while ((gameId = getVariable(readFile)) != "") {
					tmp += gameId;
					tmp += ";";
					timeString = getVariable(readFile);
					tmp += timeString;
					tmp += ";";
				}
				if (rPlayer->get_username() == username) {
					timeString = getDate();
					string id = to_string(rGame->get_game_id()) + ";";
					tmp += id;
					tmp += timeString;
					tmp += ";";
					isStoredGame = true;
					rPlayer->set_purchased_time(timeString);
				} 
				tmp += '\n';
				if (newFile.is_open()) {
					newFile << tmp;
				} else {
					cerr << "Couldn't open the file!";
				}
			}
			if (!isStoredGame) {
				tmp = rPlayer->get_username() + ";" + to_string(rGame->get_game_id()) + ";" + getDate() + ";";
				rPlayer->set_purchased_time(timeString);
				if (newFile.is_open()) {
					newFile << tmp;
				}
				else {
					cerr << "Couldn't open the file!";
				}
			}
		} else {
			username = rPlayer->get_username();
			gameId = to_string(rGame->get_game_id());
			tmp = username + ";" + gameId + ";" + getDate() + ";";
			rPlayer->set_purchased_time(timeString);
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

string DatabaseManager::getDate() {
	string timeString;
	time_t rawtime = time(0);
	struct tm timeinfo;
	errno_t result;
	result = localtime_s(&timeinfo, &rawtime);
	timeString = to_string(timeinfo.tm_mday) + "/" + to_string(timeinfo.tm_mon + 1) + "/" + to_string(timeinfo.tm_year + 1900);
	return timeString;
}

const string DatabaseManager::getTime() const{
	SYSTEMTIME time;
	GetLocalTime(&time);
	return (to_string(time.wHour) + ":" + to_string(time.wMinute) + ":" + to_string(time.wSecond));
}

const map<UserBase::Username, UserBase*> DatabaseManager::get_all_users() const {
	return m_users;
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

void DatabaseManager::store_newUser(const UserBase::Username& user, const string& pw, const string& mail, const string& type, const int rAge) {
	ofstream outFile("listOfUsers.txt", ios::app);
	string username = user;
	string password = pw;
	string email = mail;
	string usertype = type;
	string funds = "0.0";
	int age = rAge;

	if (outFile.is_open()) {
		outFile << username << ' ' << password << ' ' << email << ' ' << usertype << ' ' << funds << ' ' << to_string(age) << endl;
		outFile.close();
		if (usertype == "admin") {
			add_user(new AdminUser(username, password, email));
		} else if (usertype == "player") {
			add_user(new PlayerUser(username, password, email, age, stod(funds)));
		} else if (usertype == "guest") {
			add_user(new Guest(username, password, email));
		} else if (usertype == "gamestudio") {
			add_user(new GameStudio(username, password, email));
		}
	} else {
		cerr << "Textfile doesn't exist!";
	}
}

void DatabaseManager::store_playedGame_records(PlayerUser* rPlayer, const Game* rGame, const string& tDate, const double tLength) {
	ofstream outFile("listOfRecords.txt", ios::app);
	string username = rPlayer->get_username();
	string gameTitle = rGame->get_title();
	string date = tDate;
	string time = getTime();
	string length = to_string(tLength);

	if (outFile.is_open()) {
		outFile << username << ';' << gameTitle << ';' << date << ';' << time << ";" << length << ';' << endl;
		outFile.close();
		rPlayer->push_records(rGame->get_title(), date, time, length);
	}
	else {
		cerr << "Couldn't open the file!";
	}
	outFile.close();
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
	m_games.insert(make_pair(rGame.get_game_id(), rGame));
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

void DatabaseManager::find_game_by_ageLimit(const int age) {
	int ageLimit;
	bool found = false;
	auto it = m_games.begin();
	while (it != m_games.end()) {
		ageLimit = it->second.get_ageLimit();
		if (ageLimit >= age) {
			cout << endl << "Title: " << it->second.get_title() << " / Age limit: " << it->second.get_ageLimit() << endl << endl;
			found = true;
		}
		++it;
	}
	if (!found) {
		cout << endl << "No game defined with this age limit." << endl << endl;
	}
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
