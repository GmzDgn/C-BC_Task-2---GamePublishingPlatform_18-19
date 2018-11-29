////C++ Boot Camp - Task 2 - 2018-19 
////Name: Gamze Dogan
////Student number: b8037358

#include "Users.h"
#include "DatabaseManager.h"

//------------UserBase------------
UserBase::UserBase(const Username& username, const string& password, const string& email)
	: m_username(username)
	, m_password(password)
	, m_email(email)
{}

UserBase::~UserBase() {}

const string UserBase::get_username() const { 
	return m_username; 
}

const string UserBase::get_password() const {
	return m_password; 
}

const string UserBase::get_email() const { 
	return m_email; 
}

void UserBase::set_password(const string& val) {
	m_password = val;
}

void UserBase::set_email(const string& val) { 
	m_email = val; 
}
//------------UserBase------------


//------------AdminUser------------
const UserTypeId AdminUser::get_user_type() const {
	return UserTypeId::kAdminUser;
}

void AdminUser::add_user() {
	string username, password, email, usertype, age = "0";
	cout << "Add a new user. Please enter an username: ";
	cin >> username;
	while (!(DatabaseManager::instance().find_user(username) == nullptr)) {
		cout << "This username is already taken. Please choose another one." << endl;
		cin >> username;
	}
	cout << "Enter a password: ";
	cin >> password;
	cout << "Enter an email address: ";
	cin >> email;
	while (DatabaseManager::instance().find_email(email)) {
		cout << "This email is already taken. Please choose another one: ";
		cin >> email;
	}
	cout << "Usertype: admin, player or gamestudio? (a/p/g): ";
	cin >> usertype;
	while (!(usertype != "a" || usertype != "admin" || usertype != "Admin"
		|| usertype != "p" || usertype != "player" || usertype != "Player" 
		|| usertype != "GameStudio" || usertype != "gamestudio")) {
		cout << "Please enter a valid usertype (a/p): ";
		cin >> usertype;
	}
	if (usertype == "a" || usertype == "admin" || usertype == "Admin") {
		usertype = "admin";
	} else if(usertype == "p" || usertype == "player" || usertype == "Player"){
		usertype = "player";
	} else {
		usertype = "gamestudio";
	}
	if (usertype != "gamestudio") {
		cout << "Please enter the age of the player: ";
		cin >> age;
	}
	DatabaseManager::instance().store_newUser(username, password, email, usertype, stoi(age));
	cout << endl << "You have added the user '" << username << "' successfully!" << endl << endl;
}

void AdminUser::add_game() {
	double price;
	string title, desc, ageLimit, gamestudio;

	cout << "Add a new game. Please enter a title: ";
	cin >> title;
	cout << "Enter the gamestudio: ";
	cin >> gamestudio;
	auto pGamestudio = DatabaseManager::instance().find_user(gamestudio);
	if (pGamestudio != nullptr) {
		cout << "Define a price for this game: ";
		cin >> price;
		cout << "Define an age limit for this game: ";
		cin >> ageLimit;
		cout << "Please enter a short description: ";
		cin.ignore();
		getline(cin, desc);
		DatabaseManager::instance().store_newGame(title, desc, price, stoi(ageLimit), pGamestudio->get_username());
		cout << endl << "You have added the game '" << title << "' successfully!" << endl << endl;
		pGamestudio = nullptr;
	} else {
		cout << endl << "This gamestudio doesn't exist!" << endl;
	}
}

void AdminUser::list_of_users() const {
	cout << endl << "All Users:" << endl;
	auto userVisitorLambda = [](const UserBase& rUser) {
		cout << rUser.get_username() << " " << rUser.get_email() << endl;
	};
	DatabaseManager::instance().visit_users(userVisitorLambda);
}

void AdminUser::modify_game(Game*& game, const int option, const Game::GameId gameId) {
	string tmp;
	if (option == 1) {
		cout << "Change the description: ";
		cin.ignore();
		getline(cin, tmp);
		game->set_desc(tmp);
		DatabaseManager::instance().modify_game(game, "", tmp, "");
	} else {
		cout << "Define a new price: ";
		cin >> tmp;
		game->set_price(stod(tmp));
		DatabaseManager::instance().modify_game(game, tmp, "", "");
	}
}

void AdminUser::remove_game() {
	string id;
	cout << "Please enter the ID of the game to remove: ";
	cin >> id;

	DatabaseManager::instance().delete_game(id);
	cout << "You removed the game successfully!" << endl << endl;
}

void AdminUser::view_statistics() {
	auto allUsers = DatabaseManager::instance().get_all_users();
	auto allGames = DatabaseManager::instance().get_gameContainer();
	bool isRecords = false;

	cout << "(1) Statistic Of All Purchases\n";
	cout << "(2) History Of The Played Games\n";
	cout << "(3) Most Popular Game\n";
	cout << "(4) Average Game Price\n";
	cout << "(5) Average Player Age\n";
	cout << "(6) Show Statistics Of The Ages In Percent\n";
	char option;
	cin >> option;

	if (!allUsers.empty()) {
		string date, time, game, player, length;
		int tmp, allPrices = 0, totalSize = 0;
		list<Game> mostPurchasedGamesList, mostPlayedGamesList;
		list<int> agesOfThePlayers, agesSevenToEigtheen, agesNineteenToTwentyfive
				  , agesTwentySixToThirtyfive, agesThirtysixToFourtysix, agesOverFourtyseven;
		double percentageSevenToEigtheen, percentageNineteenToTwentyfive
			, percentageTwentysixToThirtyfive, percentageThirtysixToFourtysix, percentageOverFourtyseven
			, averagePrice = 0.0, averageAge = 0.0;

		switch (option) {
		case '1': 
			for (auto it : allUsers) {
				if (it.second->get_user_type() == UserTypeId::kPlayerUser) {
					PlayerUser* pPlayer = dynamic_cast<PlayerUser*>(it.second);
					if (pPlayer->get_myGames().size() > 0) {
						isRecords = true;
						for (auto it : pPlayer->get_myGames()) {
							cout << "Player '" << pPlayer->get_username() << "' has purchased '" 
								<< it.second->get_title() << "' on " 
								<< pPlayer->get_purchased_time() << endl;
						}
					}
					pPlayer = nullptr;
				}
			}
			if (!isRecords) {
				cout << "There is no records to show!" << endl << endl;
				isRecords = false;
			}
			break;
		case '2':
			for (auto it : allUsers) {
				if (it.second->get_user_type() == UserTypeId::kPlayerUser) {
					PlayerUser* pPlayer = dynamic_cast<PlayerUser*>(it.second);
					if (pPlayer != nullptr) {
						list<string> listRecords = pPlayer->get_records();
						if (listRecords.size() > 0) {
							isRecords = true;
							list<string>::const_iterator it = listRecords.begin();
							cout << "'" << pPlayer->get_username() << "' played '";
							while (it != listRecords.end()) {
								game = *it;
								cout << game << "' on ";
								date = *(++it);
								cout << date << " at ";
								time = *(++it);
								cout << time << ", ";
								length = *(++it);
								cout << length << " seconds long.\n";
								++it;
							}
						}
						pPlayer = nullptr;
					} else {
						cerr << "Player not found: view_statistics()";
					}
				}
			}
			if (!isRecords) {
				cout << "There is no records to show!" << endl << endl;
				isRecords = false;
			}
			break;
		case '3':
			cout << "(1) Most Purchased Game" << endl;
			cout << "(2) Most Played Game" << endl;
			char option;
			cin >> option;

			if (option == '1') {
				auto it = allGames.begin();
				tmp = it->second.get_counterPurchased();
				auto mostPurchasedGame = it;
				++it;
				while (it != allGames.end()) {
					if (tmp <= it->second.get_counterPurchased()) {
						if (tmp < it->second.get_counterPurchased()) {
							tmp = it->second.get_counterPurchased();
							mostPurchasedGamesList.clear();
							mostPurchasedGame = it;
						} else {
							mostPurchasedGamesList.push_back(it->second);
						}
					}
					++it;
				}
				if (mostPurchasedGamesList.empty()) {
					cout << endl << "The most purchased game is: " << mostPurchasedGame->second.get_title() << " ("
						<< mostPurchasedGame->second.get_counterPurchased() << " time(s))" << endl;
				} else {
					cout << endl << "The most purchased games are with " << mostPurchasedGame->second.get_counterPurchased() << " time(s):"
						<< endl << "- " << mostPurchasedGame->second.get_title() << endl;
					for (auto it : mostPurchasedGamesList) {
						cout << "- " << it.get_title() << endl;
					}
					cout << endl;
				}
			} else {
				auto it = allGames.begin();
				tmp = it->second.get_counterPlayed();
				auto mostPlayedGame = it;
				++it;
				while (it != allGames.end()) {
					if (tmp <= it->second.get_counterPlayed()) {
						if (tmp < it->second.get_counterPlayed()) {
							tmp = it->second.get_counterPlayed();
							mostPlayedGamesList.clear();
							mostPlayedGame = it;
						} else {
							mostPlayedGamesList.push_back(it->second);
						}
					}
					++it;
				}
				if (mostPlayedGamesList.empty()) {
					cout << endl << "The most played game is: " << mostPlayedGame->second.get_title() << " ("
						<< mostPlayedGame->second.get_counterPurchased() << " time(s))" << endl;
				} else {
					cout << endl << "The most played games are with " << mostPlayedGame->second.get_counterPlayed() << " time(s):"
						<< endl << "- " << mostPlayedGame->second.get_title() << endl;
					for (auto it : mostPlayedGamesList) {
						cout << "- " << it.get_title() << endl;
					}
					cout << endl;
				}
			}
			break;
		case '4':
			for (auto it : allGames) {
				allPrices += it.second.get_price();
			}
			averagePrice = allPrices / allGames.size();
			cout << endl << "The average game price is: " << averagePrice << "\x9C" << endl << endl;
			break;
		case '5':
			for (auto it : allUsers) {
				if (it.second->get_user_type() == UserTypeId::kPlayerUser) {
					auto pPlayer = dynamic_cast<PlayerUser*>(it.second);
					averageAge += pPlayer->get_age_of_player();
					pPlayer = nullptr;
				}
			}
			averageAge = averageAge / allUsers.size();
			cout << endl << "The average age is: " << averageAge << endl << endl;
			break;
		case '6':
			for (auto it : allUsers) {
				if (it.second->get_user_type() == UserTypeId::kPlayerUser) {
					PlayerUser* pPlayer = dynamic_cast<PlayerUser*>(it.second);
					if (pPlayer != nullptr) {
						if (pPlayer->get_age_of_player() >= 7 && pPlayer->get_age_of_player() <= 18) {
							agesSevenToEigtheen.push_back(pPlayer->get_age_of_player());
						}
						else if (pPlayer->get_age_of_player() >= 19 && pPlayer->get_age_of_player() <= 25) {
							agesNineteenToTwentyfive.push_back(pPlayer->get_age_of_player());
						}
						else if (pPlayer->get_age_of_player() >= 26 && pPlayer->get_age_of_player() <= 35) {
							agesTwentySixToThirtyfive.push_back(pPlayer->get_age_of_player());
						}
						else if (pPlayer->get_age_of_player() >= 36 && pPlayer->get_age_of_player() <= 46) {
							agesThirtysixToFourtysix.push_back(pPlayer->get_age_of_player());
						}
						else if (pPlayer->get_age_of_player() >= 47) {
							agesOverFourtyseven.push_back(pPlayer->get_age_of_player());
						}
						pPlayer = nullptr;
					} else {
						cerr << "Couldn't find player: view_statistics()";
					}					
				}
			}
			totalSize = agesSevenToEigtheen.size() + agesNineteenToTwentyfive.size() + agesTwentySixToThirtyfive.size()
				+ agesThirtysixToFourtysix.size() + agesOverFourtyseven.size();

			percentageSevenToEigtheen = (static_cast<double>(agesSevenToEigtheen.size()) / static_cast<double>(totalSize)) * 100;
			percentageNineteenToTwentyfive = (static_cast<double>(agesNineteenToTwentyfive.size()) / static_cast<double>(totalSize)) * 100;
			percentageTwentysixToThirtyfive = (static_cast<double>(agesTwentySixToThirtyfive.size()) / static_cast<double>(totalSize)) * 100;
			percentageThirtysixToFourtysix = (static_cast<double>(agesThirtysixToFourtysix.size()) / static_cast<double>(totalSize)) * 100;
			percentageOverFourtyseven = (static_cast<double>(agesOverFourtyseven.size()) / static_cast<double>(totalSize)) * 100;

			cout << "Age groups of players in percent: " << endl;
			cout << "[ Group 1:  7-18 years ]: " << percentageSevenToEigtheen << "%" << endl;
			cout << "[ Group 2: 19-25 years ]: " << percentageNineteenToTwentyfive << "%" << endl;
			cout << "[ Group 3: 26-35 years ]: " << percentageTwentysixToThirtyfive << "%" << endl;
			cout << "[ Group 1: 36-46 years ]: " << percentageThirtysixToFourtysix << "%" << endl;
			cout << "[ Group 1: 47+   years ]: " << percentageOverFourtyseven << "%" << endl << endl;
			break;
		default: cout << "INVALID OPTION!\n"; break;
		}
	} else {
		cout << endl << "There is no records to show!" << endl;
	}
}
//------------AdminUser------------


//------------PlayerUser------------
PlayerUser::PlayerUser(const Username& username, const string& password, const string& email, const int age, const double fund) 
	: UserBase(username, password, email), m_age(age), m_accountFunds(fund) {}

const UserTypeId PlayerUser::get_user_type() const {
	return UserTypeId::kPlayerUser;
}

 const PlayerUser::MyGameList& PlayerUser::get_game_list() const {
	return m_myGames;
}

const double PlayerUser::get_available_funds() const {
	 return m_accountFunds;
}

const string PlayerUser::get_purchased_time() const {
	return time;
}

const map<Game::GameId, Game*> PlayerUser::get_myGames() const {
	return m_myGames;
}

const list<string> PlayerUser::get_records() const {
	return l_records;
}

const int PlayerUser::get_age_of_player() const {
	return m_age;
}

 void PlayerUser::set_purchased_time(const string& timestemp) {
	 time = timestemp;
}

void PlayerUser::output_game_list() {
	cout << endl << "Your bag:" << endl;
	 if (!m_myGames.empty()) {
		 for (auto it : m_myGames) {
			 auto pGame = DatabaseManager::instance().find_game(it.first);
			 cout << "ID: " << pGame->get_game_id() << " / Title: " << pGame->get_title() << " / Description: " << pGame->get_desc() << endl;
		 }
		 cout << endl;
	 } else {
		 cout << "The list is empty." << endl << endl;
	 }
}

void PlayerUser::add_funds() {
	cout << "Add funds: ";
	string fund;
	cin >> fund;
	this->m_accountFunds += stod(fund);
	double get = get_available_funds();
	cout << "FUNDS: " << get << endl;
	DatabaseManager::instance().modify_user(get_username(), m_accountFunds);
	cout << "You added " << fund << " in your wallet successfully! Current wallet: " << m_accountFunds << endl << endl;
}

void PlayerUser::add_game_to_list(Game* pGame) {
	m_myGames.insert(make_pair(pGame->get_game_id(), pGame));
}

void PlayerUser::withdraw_funds(const double val) {
	m_accountFunds -= val;
}

void PlayerUser::search_game_by_title() {
	cout << "Enter the title of the game: ";
	string title;
	cin.ignore();
	getline(cin, title);
	
	auto pGame = DatabaseManager::instance().find_game_by_title(title);
	if (pGame == nullptr) {
		cout << "The title which you entered is not existing." << endl << endl;
	}
	else {
		cout << "Title: " << pGame->get_title() << " / Description: " << pGame->get_desc() << " / Price: " << pGame->get_price() << endl << endl;
		pGame = nullptr;
	}
}

void PlayerUser::search_game_by_ageLimit() {
	cout << "Enter the age: ";
	string age;
	cin >> age;
	DatabaseManager::instance().find_game_by_ageLimit(stoi(age));
}

void PlayerUser::buy_game() {
	Game* pGame = nullptr;
	double gamePrice = 0;
	double userFunds = 0;

	cout << "Enter the ID of the game to buy it: ";
	string id;
	cin >> id;
	if (id != "") {
		pGame = DatabaseManager::instance().find_game(stoi(id));
		if (this->get_age_of_player() >= pGame->get_ageLimit()) {
			if (pGame != nullptr) {
				gamePrice = pGame->get_price();
			}
			userFunds = this->get_available_funds();
			if (userFunds >= gamePrice) {
				this->withdraw_funds(gamePrice);
				DatabaseManager::instance().modify_user(this->get_username(), this->get_available_funds());
				add_game_to_list(pGame);
				DatabaseManager::instance().store_purchased_game(this, pGame);
				cout << "The game '" << pGame->get_title() << "' is yours now!" << endl << endl;
			} else {
				cout << "Sorry you cannot purchase this game. You don't have enough money!" << endl << endl;
			}
			pGame = nullptr;
		} else {
			cin.ignore();
			cout << "Sorry you cannot purchase this game. You are too young for this game." << endl << endl;

		}
	}
}

void PlayerUser::play_game() {
	int result = 0;
	cout << endl << "Which game do you want to play?" << endl;
	if (!m_myGames.empty()) {
		output_game_list();
		cout << "Please enter the ID to launch the game: ";
		string id;
		cin >> id;
		auto pGame = DatabaseManager::instance().find_game(stoi(id));

		CStopWatch stopwatch;
		stopwatch.startTimer();
		string date = DatabaseManager::instance().getDate();

		cout << pGame->get_title() << " IS LAUNCHING..." << endl << endl;
		while (result == 0) {
			cout << "(q) Quit game\n";
			char option;
			cin >> option;
			switch (option)	{
			case 'q':
			{
				stopwatch.stopTimer();
				double length = stopwatch.getElapsedTime();
				DatabaseManager::instance().store_playedGame_records(this, pGame, date, length);

				result = -1;
				break;
			}
			default:  cout << "INAVLID OPTION\n"; break;
			}
		}
		pGame = nullptr;
	} else {
		cout << "Your bag is empty" << endl << endl;
	}
}

void PlayerUser::gift_another_player() {
	cout << "Which player do you want to gift?" << endl;
	string playerToGift, gameToGift;
	cin >> playerToGift;
	PlayerUser* userToGift = dynamic_cast<PlayerUser*>(DatabaseManager::instance().find_user(playerToGift));
	if (userToGift != nullptr) {
		auto listOfUserToGift = userToGift->get_game_list();
		cout << "Which game do you want to gift?" << endl;
		cin >> gameToGift;
		auto pGame = DatabaseManager::instance().find_game_by_title(gameToGift);
		if (pGame != nullptr) {
			if (userToGift->get_age_of_player() >= pGame->get_ageLimit()) {
				auto myGames = get_game_list();
				for (map<Game::GameId, Game*> ::const_iterator it = myGames.begin(); it != myGames.end(); ++it) {
					if (it->first == pGame->get_game_id()) {
						listOfUserToGift.insert(make_pair(pGame->get_game_id(), pGame));
						DatabaseManager::instance().store_purchased_game(userToGift, pGame);
						myGames.erase(it);
						DatabaseManager::instance().delete_game_from_listOfUserGames(to_string(pGame->get_game_id()));
						break;
					}
				}
				cout << "You gave " + userToGift->get_username() + " the game: " + pGame->get_title() + ".";
			} else {
				cout << endl << "You cannot gift this game. The age limit for this game is " << pGame->get_ageLimit() << " and '" << userToGift->get_username() << "' is under this age." << endl << endl;
			}
			pGame = nullptr;
		} else {
			cout << "Couldn't find game";
		}
		userToGift = nullptr;
	} else {
		cout << "This user doesn't exist!" << endl;
	}
}

void PlayerUser::push_records(const string & game, const string & date, const string & time, const string & length) {
	l_records.push_back(game);
	l_records.push_back(date);
	l_records.push_back(time);
	l_records.push_back(length);
}
//------------PlayerUser------------


//------------Guest------------
const UserTypeId Guest::get_user_type() const {
	return UserTypeId();
}
//------------Guest------------


//------------GameStudio------------
const UserTypeId GameStudio::get_user_type() const {
	return UserTypeId::kGameStudioUser;
}

float const GameStudio::get_version(const string& gameId) const {
	auto pGame = DatabaseManager::instance().find_game(stoi(gameId));
	return pGame->get_version();
}

const list<Game> GameStudio::get_gameLIst() const {
	return l_gameList;
}

void GameStudio::set_version() {
	output_gameList();
	cout << "Please enter the ID of the game to change the version: ";
	string id;
	cin >> id;
	auto pGame = DatabaseManager::instance().find_game(stoi(id));
	if (pGame != nullptr) {
		cout << "Enter a new version: ";
		string version;
		cin >> version;
		int gameVer = stoi(version);
		pGame->set_new_version(gameVer);
		DatabaseManager::instance().modify_game(pGame, "", "", version);
	}
	else {
		cout << "Couldn't find the game.";
	}
}

void GameStudio::add_game_to_list(const Game& rGame) {
	l_gameList.push_back(rGame);
}

const void GameStudio::output_gameList() const {
	cout << endl << "Your games: " << endl;
	for (list<Game>::const_iterator it = l_gameList.begin(); it != l_gameList.end(); ++it) {
		cout << "ID: " << it->get_game_id() << " / Title: " << it->get_title() << " / Description: " << it->get_desc() << " / Version: " << it->get_version() << endl;
	}
	cout << endl;
}
//------------GameStudio------------