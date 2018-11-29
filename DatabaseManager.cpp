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

	// whether file is open or not.
	if (file.is_open()) {
		// whether file is empty or not.
		if (!(file.peek() == ifstream::traits_type::eof())) {	
			while (file >> username >> password >> email >> usertype >> funds >> age) {
				if (usertype == "admin") {
					add_user(new AdminUser(username, password, email));
				} else if(usertype == "player") {
					add_user(new PlayerUser(username, password, email, stod(age), stod(funds)));
				} else if (usertype == "gamestudio") {
					add_user(new GameStudio(username, password, email));
				} else if(usertype == "guest") {
					add_guest(new Guest(username, password, email));
				} 
			}
			file.close();
		}
	} else {
		cerr << "Couldn't open the file: load_list_users()";
	}
}

void DatabaseManager::load_list_games() {
	string readFile, title, desc, gameStudio;
	int ageLimit, version;
	double price;
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
					pStudio = nullptr;
				} else {
					cerr << "Couldn't add game to the list: load_list_games()." << endl;
				}
			}
			file.close();
		}
	} else {
		cerr << "Couldn't open the file: load_list_games().";
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
				if (pUser != nullptr) {
					while ((gameId = getVariable(readFile)) != "") {
						time = getVariable(readFile);
						pUser->set_purchased_time(time);
						int id = stoi(gameId);
						auto pGame = find_game(id);
						if (pGame != nullptr) {
							pUser->add_game_to_list(pGame);
							pGame->increment_counterPurchased();
							pGame = nullptr;
						} else {
							cerr << "Couldn't find the game in load_game_of_user()";
						}
					}
					pUser = nullptr;
				} else {
					cerr << "Couldn't find the user in load_game_of_user()";
				}
			}
		}
		file.close();
	} else {
		cerr << "Couldn't open the file: load_game_of_user()";
	}
}

void DatabaseManager::load_records() {
	string username, gameTitle, date, time, lenghtOfPlay, readFile;
	ifstream file("listOfRecords.txt");

	if (file.is_open()) {
		if (!(file.peek() == ifstream::traits_type::eof())) {
			while (getline(file, readFile)) {
				username = getVariable(readFile);
				gameTitle = getVariable(readFile);
				date = getVariable(readFile);
				time = getVariable(readFile);
				lenghtOfPlay = getVariable(readFile);
		
				auto pUser = dynamic_cast<PlayerUser*>(find_user(username));
				auto pGame = (find_game_by_title(gameTitle));
				if (pUser != nullptr) {
					pUser->push_records(gameTitle, date, time, lenghtOfPlay);
					pUser = nullptr;
				} else {
					cerr << endl << "Couldn't find the user in load_records()." << endl;;
				}
				if (pGame != nullptr) {
					pGame->increment_counterPlayed();
				}
				else {
					cerr << endl << "Couldn't find the game in load_records()." << endl;
				}
			}
			file.close();
		}
	} else {
		cerr << endl << "Couldn't open the file: load_records()" << endl;
	}
}

void DatabaseManager::store_newUser(const UserBase::Username& username, const string& password, const string& email, const string& usertype, const int age) {
	ofstream outFile("listOfUsers.txt", ios::app);
	string funds = "0.0";

	if (outFile.is_open()) {
		outFile << username << ' ' << password << ' ' << email << ' ' << usertype << ' ' << funds << ' ' << to_string(age) << endl;
		outFile.close();
		if (usertype == "admin") {
			add_user(new AdminUser(username, password, email));
		}
		else if (usertype == "player") {
			add_user(new PlayerUser(username, password, email, age, stod(funds)));
		}
		else if (usertype == "guest") {
			add_guest(new Guest(username, password, email));
		}
		else if (usertype == "gamestudio") {
			add_user(new GameStudio(username, password, email));
		}
	}
	else {
		cerr << "Textfile doesn't exist: store_newUser()";
	}
}

void DatabaseManager::store_newGame(const string& gameTitle, const string& description, const double gamePrice, const int ageLimit, const string& studio) {
	ofstream outFile("listOfGames.txt", ios::app);
	int id = ++(gameIdCounter);
	string gameId = to_string(id);
	string version = "1";

	if (outFile.is_open()) {
		outFile << gameId << ';' << gameTitle << ';' << description << ';' << to_string(gamePrice) << ';' << to_string(ageLimit) << ";" << studio << ";" << version << ";" << endl;
		outFile.close();

		Game game(gameIdCounter, gameTitle, description, gamePrice, ageLimit, studio, stoi(version));
		add_game(game);
		game.increment_counterPurchased();
		auto pStudio = dynamic_cast<GameStudio*>(find_user(studio));
		if (pStudio != nullptr) {
			pStudio->add_game_to_list(game);
			pStudio = nullptr;
		}
	}
	else {
		cerr << "Couldn't open the file: store_newGame()";
	}
	outFile.close();
}

void DatabaseManager::store_purchased_game(PlayerUser* player, const Game* game) {
	ifstream file("listOfUserGames.txt", ios::app);
	ofstream newFile("listOfTheUserGames.txt", ios::app);
	string readFile, tmp, username, gameId, timeString;
	bool isStoredGame = false;

	if (player != nullptr || game != nullptr) {
		if (file.is_open() && newFile.is_open()) {
			if (!(file.peek() == ifstream::traits_type::eof())) {
				while (getline(file, readFile)) {
					username = getVariable(readFile);
					tmp = username + ";";
					while ((gameId = getVariable(readFile)) != "") {
						tmp += gameId + ";";
						timeString = getVariable(readFile);
						tmp += timeString + ";";
					}
					if (player->get_username() == username) {
						timeString = getDate();
						string id = to_string(game->get_game_id()) + ";";
						tmp += id;
						tmp += timeString + ";";
						isStoredGame = true;
						player->set_purchased_time(timeString);
					}
					tmp += '\n';
					newFile << tmp;
				}
				if (!isStoredGame) {
					tmp = player->get_username() + ";" + to_string(game->get_game_id()) + ";" + getDate() + ";";
					player->set_purchased_time(timeString);
					newFile << tmp;
				}
			} else {
				username = player->get_username();
				gameId = to_string(game->get_game_id());
				tmp = username + ";" + gameId + ";" + getDate() + ";";
				player->set_purchased_time(timeString);
				newFile << tmp;
			}
			file.close();
			newFile.close();
			remove("listOfUserGames.txt");
			rename("listOfTheUserGames.txt", "listOfUserGames.txt");
		} else {
			cerr << "Couldn't open the file: store_purchased_game()";
		}
	} else {
		cerr << "There is no game or player to store the purchased game: store_purchased_game()";
	}
}

void DatabaseManager::store_playedGame_records(PlayerUser* player, Game* game, const string& date, const double length) {
	ofstream outFile("listOfRecords.txt", ios::app);
	string username = player->get_username();
	string gameTitle = game->get_title();
	string time = getTime();

	if (outFile.is_open()) {
		outFile << username << ';' << gameTitle << ';' << date << ';' << time << ";" << to_string(length) << ';' << endl;
		outFile.close();
		player->push_records(game->get_title(), date, time, to_string(length));
		game->increment_counterPlayed();
	}
	else {
		cerr << "Couldn't open the file: store_playedGame_records()";
	}
	outFile.close();
}

void DatabaseManager::modify_game(Game*& pGame, const string& newPrice, const string& newDescription, const string& newVersion) {
	string readFile, id, price;
	ifstream file("listOfGames.txt", ios::app);
	ofstream newFile("listOfTheGames.txt");

	if (file.is_open() && newFile.is_open()) {
		if (!(file.peek() == ifstream::traits_type::eof())) {
			while (getline(file, readFile)) {
				int pos = readFile.find(';');
				id = readFile.substr(0, pos);
				if (pGame != nullptr) {
					if (pGame->get_game_id() != stoi(id)) {
						newFile << readFile << endl;
					}
					else {
						if (!newPrice.empty()) {
							readFile = id + ";" + pGame->get_title() + ";" + pGame->get_desc() + ";" + newPrice + ";"
								+ to_string(pGame->get_ageLimit()) + ";" + pGame->get_studio() + ";"
								+ to_string(pGame->get_version()) + ";\n";
							newFile << readFile;
						}
						else if (!newDescription.empty()) {
							readFile = id + ";" + pGame->get_title() + ";" + newDescription + ";"
								+ to_string(pGame->get_price()) + ";" + to_string(pGame->get_ageLimit()) + ";"
								+ pGame->get_studio() + ";" + to_string(pGame->get_version()) + ";\n";
							newFile << readFile;
						}
						else if (!newVersion.empty()) {
							readFile = id + ";" + pGame->get_title() + ";" + pGame->get_desc() + ";"
								+ to_string(pGame->get_price()) + ";" + to_string(pGame->get_ageLimit()) + ";"
								+ pGame->get_studio() + ";" + newVersion + ";\n";
							newFile << readFile;
						}
					}
				}
				else {
					cerr << "Game is null. Couldn't modify the game.";
				}
			}
		}
		else {
			cerr << "There is no game to modify: modify_game().";
		}
		file.close();
		newFile.close();
		remove("listOfGames.txt");
		rename("listOfTheGames.txt", "listOfGames.txt");
	}
	else {
		cerr << "Couldn't open the file: modify_game().";
	}
}

void DatabaseManager::modify_user(const UserBase::Username& username, const double newFund) {
	string readFile, id, fund, password, email, usertype, age;
	ifstream file("listOfUsers.txt", ios::app);
	ofstream newFile("listOfTheUsers.txt", ios::app);

	if (file.is_open() && newFile.is_open()) {
		if (!(file.peek() == ifstream::traits_type::eof())) {
			while (getline(file, readFile)) {
				istringstream stream(readFile);
				stream >> id >> password >> email >> usertype >> fund >> age;
				if (id != username) {
					newFile << readFile << endl;
				} else {
					readFile = id + ' ' + password + ' ' + email + ' ' + usertype + ' ' + to_string(newFund) + ' ' + age;
					newFile << readFile << endl;
				}
			}
		} else {
			cerr << "There is no user to modify: modify_user().";
		}
		file.close();
		newFile.close();
		remove("listOfUsers.txt");
		rename("listOfTheUsers.txt", "listOfUsers.txt");
	} else {
		cerr << "Couldn't open the file: modify_user().";
	}
}

void DatabaseManager::delete_game_from_listOfUserGames(const string& gameId) {
	string readFile, username, id, tmp, purchaseDate;
	ifstream file("listOfUserGames.txt", ios::app);
	ofstream newFile("listOfTheUserGames.txt");
	bool isErased = false;

	if (file.is_open() && newFile.is_open()) {
		while (getline(file, readFile)) {
			username = getVariable(readFile);
			tmp = username + ";";
			while ((id = getVariable(readFile)) != "") {
				purchaseDate = getVariable(readFile);
				if (gameId != id) {
					tmp += id + ";" + purchaseDate + ";";
					newFile << tmp;
				}
				else {
					auto pPlayerFound = dynamic_cast<PlayerUser*>(find_user(username));
					if (pPlayerFound != nullptr) {
						auto myGames = pPlayerFound->get_game_list();
						myGames.erase(stoi(gameId));
						if (myGames.size() == 0) {
							isErased = true;
						}
						pPlayerFound = nullptr;
					}
					else {
						cout << "Couldn't find the player!";
					}
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
		cerr << "Couldn't open the file: delete_game_from_listOfUserGames()";
	}
}

void DatabaseManager::delete_game(const string& gameId) {
	string readFile, id;
	ifstream file("listOfGames.txt", ios::app);
	ofstream newFile("listOfTheGames.txt");
	
	if (file.is_open() && newFile.is_open()) {
		while (getline(file, readFile)) {
			int pos = readFile.find(';');
			id = readFile.substr(0, pos);
			if (gameId != id) {
				newFile << readFile << endl;
			} else {
				m_games.erase(stoi(gameId));
				delete_game_from_listOfUserGames(gameId);
			}
		}
		file.close();
		newFile.close();
		remove("listOfGames.txt");
		rename("listOfTheGames.txt", "listOfGames.txt");
	} else {
		cerr << "Couldn't open the file: delete_game()";
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

void DatabaseManager::add_game(const Game& game) {
	// Store the game indexed by game id.
	m_games.insert(make_pair(game.get_game_id(), game));
}

const map<UserBase::Username, UserBase*> DatabaseManager::get_all_users() const {
	return m_users;
}

Game* DatabaseManager::find_game(const Game::GameId gameid) {
	auto it = m_games.find(gameid);
	if (it != m_games.end()) {
		return &it->second;
	} else {
		return nullptr;
	}
}

Game* DatabaseManager::find_game_by_title(const string& title) {
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
	bool found = false;
	auto it = m_games.begin();
	while (it != m_games.end()) {
		int ageLimit = it->second.get_ageLimit();
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

bool DatabaseManager::find_email(const string& email) {
	for (auto it = m_users.begin(); it != m_users.end(); ++it) {
		if ((*it).second->get_email() == email) {
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

string DatabaseManager::to_lower_string(const string& lowerString) {
	string searchLowerTitle;
	for (int i = 0; i <= lowerString.length(); ++i) {
		searchLowerTitle += tolower(lowerString[i]);
	}
	return searchLowerTitle;
}

string DatabaseManager::getVariable(string& readFile) {
	int pos = readFile.find(';');
	string tmp = readFile.substr(0, pos);
	readFile = readFile.substr((pos + 1), readFile.length() - (pos + 1));
	return tmp;
}

const string DatabaseManager::getDate() const {
	string timeString;
	time_t rawtime = time(0);
	struct tm timeinfo;
	errno_t result;
	result = localtime_s(&timeinfo, &rawtime);
	timeString = to_string(timeinfo.tm_mday) + "/" + to_string(timeinfo.tm_mon + 1) + "/" + to_string(timeinfo.tm_year + 1900);
	return timeString;
}

const string DatabaseManager::getTime() const {
	SYSTEMTIME time;
	GetLocalTime(&time);
	return (to_string(time.wHour) + ":" + to_string(time.wMinute) + ":" + to_string(time.wSecond));
}

const map<Game::GameId, Game> DatabaseManager::get_gameContainer() const {
	return m_games;
}
