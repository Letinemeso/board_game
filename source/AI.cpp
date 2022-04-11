#include "../include/AI.h"


void AI::set_pawns(const Pawn* _ai_pawns, unsigned int _ai_pawns_count, const Pawn* _player_pawns, unsigned int _player_pawns_count)
{
	m_ai_pawns = _ai_pawns;
	m_ai_pawns_count = _ai_pawns_count;
	m_player_pawns = _player_pawns;
	m_player_pawns_count = _player_pawns_count;
}

void AI::set_bases(std::pair<i_pair, i_pair> _ai_base, std::pair<i_pair, i_pair> _player_base)
{
	m_ai_base = _ai_base;
	m_ai_base_width = abs((int)m_ai_base.first.first - (int)m_ai_base.second.first);
	m_ai_base_height = abs((int)m_ai_base.first.second - (int)m_ai_base.second.second);

	m_player_base = _player_base;
	m_player_base_width = abs((int)m_player_base.first.first - (int)m_player_base.second.first);
	m_player_base_height = abs((int)m_player_base.first.second - (int)m_player_base.second.second);
}



bool AI::pawn_is_on_players_base(const Pawn* _pawn) const
{
	auto pawn_pos = _pawn->get_current_cell();
	return pawn_pos.first <= m_player_base.second.first && pawn_pos.first >= m_player_base.first.first &&
		pawn_pos.second <= m_player_base.second.second && pawn_pos.second >= m_player_base.first.second;
}

bool AI::pawn_can_move(const Pawn* _pawn) const
{
	return true;
}



void AI::set_pawn_and_destination()
{
	ASSERT(!m_ai_pawns || !m_player_pawns);

	int dx = m_ai_base.first.first - m_player_base.first.first,
		dy = m_ai_base.first.second - m_player_base.first.second;

	bool is_player_on_left = m_player_base.first.first < m_ai_base.first.first,
		is_player_above = m_player_base.first.second > m_ai_base.first.second;

	i_pair where_to = { -1, -1 };
	const Pawn* ptmn = /*&m_ai_pawns[0]*/nullptr;
	unsigned int prev_dist =  get_distance({ 999, 999 }, 
			{ m_player_base.first.first, m_player_base.first.second });


	for (unsigned int i = 0; i < m_ai_pawns_count; ++i)
	{
		if (pawn_is_on_players_base(&m_ai_pawns[i]))
			continue;
		auto pawn_pos = m_ai_pawns[i].get_current_cell();
		for (unsigned int ii = m_player_base.first.first; ii <= m_player_base.second.first; ++ii)
		{
			for (unsigned int jj = m_player_base.first.second; jj <= m_player_base.second.second; ++jj)
			{
				if (at({ (int)ii, (int)jj })) continue;
				unsigned int dist = get_distance({ pawn_pos.first, pawn_pos.second }, { (int)ii, (int)jj });
				if (dist < prev_dist)
				{
					ptmn = &m_ai_pawns[i];
					prev_dist = dist;
					where_to = { ii, jj };
				}
			}
		}
	}
	if (where_to == i_pair(-1, -1))
	{
		for (unsigned int i = 0; i < m_ai_pawns_count; ++i)
		{
			if (pawn_is_on_players_base(&m_ai_pawns[i])) 
				continue;
			auto pawn_pos = m_ai_pawns[i].get_current_cell();
			for (unsigned int ii = m_player_base.first.first; ii <= m_player_base.second.first; ++ii)
			{
				for (unsigned int jj = m_player_base.first.second; jj <= m_player_base.second.second; ++jj)
				{
					unsigned int dist = get_distance({ pawn_pos.first, pawn_pos.second }, { (int)ii, (int)jj });
					if (dist < prev_dist)
					{
						ptmn = &m_ai_pawns[i];
						prev_dist = dist;
						where_to = { ii, jj };
					}
				}
			}
		}
	}


	set_destination(where_to.first, where_to.second);
	m_pawn_to_move_next = ptmn;
}


const Pawn& AI::get_pawn_to_move_next() const
{
	ASSERT(!m_pawn_to_move_next);
	return *m_pawn_to_move_next;
}