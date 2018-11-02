//C++ Boot Camp - Task 2 - 2018-19 
//Name: 
//Student number: 

#include "DatabaseManager.h"

DatabaseManager::DatabaseManager()
{

}

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

void DatabaseManager::load_data()
{
	// For test purposes I shall populate the database manually here.
	// In your applications we want you to load data (and save) the contents of the database.

	// add some admin users.
	add_user(new AdminUser("davem", "12345", "d.r.moore@shu.ac.uk"));
	add_user(new AdminUser("pascalev", "54321", "p.vacher@shu.ac.uk"));

	// add some players
	add_user(new PlayerUser("frank", "frank12345", "frank@unknown.com"));
	add_user(new PlayerUser("jake", "jake12345", "jake@unknown.com"));
	add_user(new PlayerUser("andrew", "andrew12345", "andrew@unknown.com"));
	add_user(new PlayerUser("martin", "martin12345", "martin@unknown.com"));

	// add some games.
	add_game(Game(4789, "Bounceback", "A platform puzzle game for PSP"));
	add_game(Game(5246, "Piecefall", "A tetris like 3d puzzle game for PS4"));
}

void DatabaseManager::store_data()
{
	// You need a mechinsm for storing data here
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

UserBase* DatabaseManager::find_user(const UserBase::Username& username)
{
	auto it = m_users.find(username);
	if (it != m_users.end())
	{
		return it->second;
	}
	else
	{
		return nullptr;
	}
}

void DatabaseManager::add_game(Game& rGame)
{
	// Store the game indexed by game id.
	m_games.insert(std::make_pair(rGame.get_game_id(), rGame));
}

Game* DatabaseManager::find_game(const Game::GameId gameid)
{
	auto it = m_games.find(gameid);
	if (it != m_games.end())
	{
		return &it->second;
	}
	else
	{
		return nullptr;
	}
}
