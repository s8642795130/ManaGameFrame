#pragma once
#include <map>
#include <utility>
#include <string>

enum class GameMsg : int
{
	NONE = 1000,
	PLAYER_INFO,
	GAME_STATUS,
	MATCH,
	ENTER_ROOM,
	GAME_PROC
};

class NetMsgDefine
{
private:
	static std::map<int, std::string> m_map_msg;
public:
	static void SetNetMsg()
	{
		m_map_msg.emplace(std::pair<int, std::string>(static_cast<int>(GameMsg::PLAYER_INFO), "hallPlugin"));
		m_map_msg.emplace(std::pair<int, std::string>(static_cast<int>(GameMsg::GAME_STATUS), "hallPlugin"));
		m_map_msg.emplace(std::pair<int, std::string>(static_cast<int>(GameMsg::MATCH), "matchPlugin"));
		m_map_msg.emplace(std::pair<int, std::string>(static_cast<int>(GameMsg::ENTER_ROOM), "gamePlugin"));
		m_map_msg.emplace(std::pair<int, std::string>(static_cast<int>(GameMsg::GAME_PROC), "gamePlugin"));
	}

	static const std::map<int, std::string> GetNetMsg()
	{
		return m_map_msg;
	}
};