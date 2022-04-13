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

	bool is_player_on_left = m_player_base.first.first < m_ai_base.first.first,
		is_player_above = m_player_base.first.second > m_ai_base.first.second;

	if (is_player_above)
	{
		if (is_player_on_left) m_players_base_corner = { m_player_base.first.first, m_player_base.second.second };
		else m_players_base_corner = { m_player_base.second.first, m_player_base.second.second };
	}
	else
	{
		if (is_player_on_left) m_players_base_corner = { m_player_base.first.first, m_player_base.first.second };
		else m_players_base_corner = { m_player_base.second.first, m_player_base.first.second };
	}
}



bool AI::pawn_is_on_players_base(const Pawn* _pawn) const
{
	const i_pair& pawn_pos = _pawn->get_current_cell();
	return pawn_pos.first <= m_player_base.second.first && pawn_pos.first >= m_player_base.first.first &&
		pawn_pos.second <= m_player_base.second.second && pawn_pos.second >= m_player_base.first.second;
}

bool AI::pawn_can_move(const Pawn* _pawn) const
{
	int pawn_x = _pawn->get_current_cell().first,
		pawn_y = _pawn->get_current_cell().second;

	if (!out_of_bounds({ pawn_x + 1, pawn_y })) if (!at({ pawn_x + 1, pawn_y })) return true;
	if (!out_of_bounds({ pawn_x - 1, pawn_y })) if (!at({ pawn_x - 1, pawn_y })) return true;
	if (!out_of_bounds({ pawn_x, pawn_y + 1 })) if (!at({ pawn_x, pawn_y + 1 })) return true;
	if (!out_of_bounds({ pawn_x, pawn_y - 1 })) if (!at({ pawn_x, pawn_y - 1 })) return true;

	return false;
}

AI::i_pair AI::find_furthest_free_cell_on_base(const i_pair& _corner, const i_pair& _prev) const
{
	i_pair result = { -1, -1 };
    const unsigned int min_dist_after_this = _prev.first == -1 ? 0 : get_distance({ _prev.first, _prev.second }, { _corner.first, _corner.second });
	unsigned int min_dist = -1;
    if(_prev.first == -1 && !at({_corner.first, _corner.second})) return _corner;

	for (int i = m_player_base.first.first; i <= m_player_base.second.first; ++i)
	{
		for (int j = m_player_base.first.second; j <= m_player_base.second.second; ++j)
		{
			unsigned int cur_dist = get_distance({ _corner.first, _corner.second }, { i, j });
			if (cur_dist < min_dist && cur_dist > min_dist_after_this && !at({ i, j }) && ( _prev.first != i && _prev.second != j ))
			{
				result = { i, j };
				min_dist = cur_dist;
			}
		}
	}

	return result;
}

AI::i_pair AI::find_furthest_free_cell(const i_pair& _corner, const i_pair& _prev) const
{
    i_pair result = { -1, -1 };
    const unsigned int min_dist_after_this = _prev.first == -1 ? 0 : get_distance({ _prev.first, _prev.second }, { _corner.first, _corner.second });
    unsigned int min_dist = -1;

    for (int i = 0; i < get_field_size().first; ++i)
    {
        for (int j = 0; j < get_field_size().second; ++j)
        {
            unsigned int cur_dist = get_distance({ _corner.first, _corner.second }, { i, j });
            if (cur_dist < min_dist && cur_dist > min_dist_after_this && !at({ i, j }) && ( _prev.first != i && _prev.second != j ))
            {
                result = { i, j };
                min_dist = cur_dist;
            }
        }
    }

    return result;
}





void AI::build_path()
{
	LEti::EIPF::build_path();
}



void AI::set_pawn_and_destination()
{
	ASSERT(!m_ai_pawns || !m_player_pawns);

    unsigned int min_steps = -1;
	i_pair min_steps_dest = { -1, -1 };
	const Pawn* min_steps_pawn = nullptr;

    i_pair where_to = find_furthest_free_cell_on_base(m_players_base_corner, {-1, -1});

    while (where_to.first != -1)
    {
        for (unsigned int p = 0; p < m_ai_pawns_count; ++p)
        {
            const Pawn& cur_pawn = m_ai_pawns[p];
            if (!pawn_can_move(&cur_pawn) || pawn_is_on_players_base(&cur_pawn)) continue;

            set_start_pos(cur_pawn.get_current_cell().first, cur_pawn.get_current_cell().second);
            set_destination(where_to.first, where_to.second);
            build_path();

            if (is_path_found() && get_steps_count() < min_steps)
            {
                min_steps = get_steps_count();
                min_steps_dest = where_to;
                min_steps_pawn = &cur_pawn;
            }
        }

        where_to = find_furthest_free_cell_on_base(m_players_base_corner, where_to);
    }
    if (min_steps == (unsigned int)-1)
    {
        i_pair where_to = find_furthest_free_cell_on_base(m_players_base_corner, { -1, -1 });
        while (where_to.first != -1)
        {
            for (unsigned int p = 0; p < m_ai_pawns_count; ++p)
            {
                const Pawn& cur_pawn = m_ai_pawns[p];
                if (!pawn_can_move(&cur_pawn) || !pawn_is_on_players_base(&cur_pawn)) continue;

                unsigned int dist_to_edge = get_distance({m_players_base_corner.first, m_players_base_corner.second},
                    {cur_pawn.get_current_cell().first, cur_pawn.get_current_cell().second});
                unsigned int destination_to_edge_dist = get_distance({m_players_base_corner.first, m_players_base_corner.second},
                    {where_to.first, where_to.second});
                if(dist_to_edge <= destination_to_edge_dist) continue;

                set_start_pos(cur_pawn.get_current_cell().first, cur_pawn.get_current_cell().second);
                set_destination(where_to.first, where_to.second);
                build_path();

                if (is_path_found() && get_steps_count() < min_steps)
                {
                    min_steps = get_steps_count();
                    min_steps_dest = where_to;
                    min_steps_pawn = &cur_pawn;
                }
            }

            where_to = find_furthest_free_cell_on_base(m_players_base_corner, where_to);
        }
    }
    if (min_steps == (unsigned int)-1)
    {
        i_pair where_to = find_furthest_free_cell(m_players_base_corner, { -1, -1 });
        min_steps = 0;
//        while (where_to.first != -1)
        while (min_steps == 0 && where_to.first != -1)
        {
            for (unsigned int p = 0; p < m_ai_pawns_count; ++p)
            {
                const Pawn& cur_pawn = m_ai_pawns[p];
                if (!pawn_can_move(&cur_pawn) || pawn_is_on_players_base(&cur_pawn)) continue;

                set_start_pos(cur_pawn.get_current_cell().first, cur_pawn.get_current_cell().second);
                set_destination(where_to.first, where_to.second);
                build_path();

                if (is_path_found() && get_steps_count() > min_steps)
                {
                    min_steps = get_steps_count();
                    min_steps_dest = where_to;
                    min_steps_pawn = &cur_pawn;
                }
            }

            where_to = find_furthest_free_cell(m_players_base_corner, where_to);
        }



//        for (unsigned int i = 0; i < m_ai_pawns_count; ++i)
//        {
//            const Pawn& cur_pawn = m_ai_pawns[i];
//            if (!pawn_can_move(&cur_pawn) || pawn_is_on_players_base(&cur_pawn)) continue;

//            set_start_pos(cur_pawn.get_current_cell().first, cur_pawn.get_current_cell().second);
//            set_destination(min_steps_dest.first, min_steps_dest.second);
//            build_path();

////            if (is_path_found() && get_steps_count() > min_steps)
////            {
////                min_steps = get_steps_count();
////                min_steps_pawn = &cur_pawn;
////            }
//            min_steps = 0;
//            min_steps_pawn = &cur_pawn;
//        }
    }
    if (min_steps == (unsigned int)-1 || min_steps == 0)
    {
        min_steps_dest = m_players_base_corner;

        for (unsigned int i = 0; i < m_ai_pawns_count; ++i)
        {
            if (pawn_can_move(&m_ai_pawns[i]))
            {
                min_steps_pawn = &m_ai_pawns[i];
                break;
            }
        }
    }


    /*while (where_to.first != -1)
    {
        for (unsigned int p = 0; p < m_ai_pawns_count; ++p)
        {
            const Pawn& cur_pawn = m_ai_pawns[p];
            if (!pawn_can_move(&cur_pawn) || pawn_is_on_players_base(&cur_pawn)) continue;

            set_start_pos(cur_pawn.get_current_cell().first, cur_pawn.get_current_cell().second);
            set_destination(where_to.first, where_to.second);
            build_path();

            if (is_path_found() && get_steps_count() < min_steps)
            {
                min_steps = get_steps_count();
                min_steps_dest = where_to;
                min_steps_pawn = &cur_pawn;
            }
        }

        where_to = find_furthest_free_cell_on_base(m_players_base_corner, where_to);
    }
    if (min_steps == (unsigned int)-1)
    {
        i_pair where_to = find_furthest_free_cell_on_base(m_players_base_corner, { -1, -1 });
        while (where_to.first != -1)
        {
            for (unsigned int p = 0; p < m_ai_pawns_count; ++p)
            {
                const Pawn& cur_pawn = m_ai_pawns[p];
                if (!pawn_can_move(&cur_pawn) || !pawn_is_on_players_base(&cur_pawn)) continue;

                set_start_pos(cur_pawn.get_current_cell().first, cur_pawn.get_current_cell().second);
                set_destination(where_to.first, where_to.second);
                build_path();

                if (is_path_found() && get_steps_count() < min_steps)
                {
                    min_steps = get_steps_count();
                    min_steps_dest = where_to;
                    min_steps_pawn = &cur_pawn;
                }
            }

            where_to = find_furthest_free_cell_on_base(m_players_base_corner, where_to);
        }
    }
    if (min_steps == (unsigned int)-1)
    {
        min_steps_dest = m_players_base_corner;

        for (unsigned int i = 0; i < m_ai_pawns_count; ++i)
        {
            if (pawn_can_move(&m_ai_pawns[i]) && !pawn_is_on_players_base(&m_ai_pawns[i]))
            {
                min_steps_pawn = &m_ai_pawns[i];
                break;
            }
        }
    }*/

	set_start_pos(min_steps_pawn->get_current_cell().first, min_steps_pawn->get_current_cell().second);
	set_destination(min_steps_dest.first, min_steps_dest.second);
	build_path();
	m_pawn_to_move_next = min_steps_pawn;


	/*unsigned int max_steps = 0;
	i_pair max_steps_dest = { -1, -1 };
	const Pawn* max_steps_pawn = nullptr;

	for (int i = m_player_base.first.first; i <= m_player_base.second.first; ++i)
	{
		for (int j = m_player_base.first.second; j <= m_player_base.second.second; ++j)
		{
			if (!at({ i, j }))
			{
				for (unsigned int p = 0; p < m_ai_pawns_count; ++p)
				{
					const Pawn& cur_pawn = m_ai_pawns[p];
					if (!pawn_can_move(&cur_pawn)) continue;

					set_start_pos(cur_pawn.get_current_cell().first, cur_pawn.get_current_cell().second);
					set_destination(i, j);
					build_path();

					if (is_path_found() && get_steps_count() > max_steps)
					{
						max_steps = get_steps_count();
						max_steps_dest = { i, j };
						max_steps_pawn = &cur_pawn;
					}
				}
			}
		}
	}

	if (max_steps == 0)
	{
		max_steps_dest = m_players_base_corner;

		for (unsigned int i = 0; i < m_ai_pawns_count; ++i)
		{
			if (pawn_can_move(&m_ai_pawns[i]))
			{
				max_steps_pawn = &m_ai_pawns[i];
				break;
			}
		}
	}

	set_start_pos(max_steps_pawn->get_current_cell().first, max_steps_pawn->get_current_cell().second);
	set_destination(max_steps_dest.first, max_steps_dest.second);
	build_path();
	m_pawn_to_move_next = max_steps_pawn;*/

    /*i_pair where_to = find_furthest_free_cell_on_base(m_players_base_corner);
	if (where_to.first == -1) where_to = m_players_base_corner;
	set_destination(where_to.first, where_to.second);

	const Pawn* ptmn = nullptr;
	unsigned int prev_dist = -1;

	for (unsigned int i = 0; i < m_ai_pawns_count; ++i)
	{
		const Pawn& cur_pawn = m_ai_pawns[i];
		if (!pawn_can_move(&cur_pawn)) continue;

		const i_pair& ppos = cur_pawn.get_current_cell();

		unsigned int cur_dist_from_corner = get_distance({ ppos.first, ppos.second }, { m_players_base_corner.first, m_players_base_corner.second });
		unsigned int dist_to_destination = get_distance({ ppos.first, ppos.second }, { where_to.first, where_to.second });

		if (cur_dist_from_corner <= dist_to_destination) continue;

		if (prev_dist > dist_to_destination)
		{
			ptmn = &cur_pawn;
			prev_dist = dist_to_destination;
		}
	}*/


	/*for (unsigned int i = 0; i < m_ai_pawns_count; ++i)
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
	}*/


	//set_destination(where_to.first, where_to.second);
	//m_pawn_to_move_next = ptmn;
}


const Pawn& AI::get_pawn_to_move_next() const
{
	ASSERT(!m_pawn_to_move_next);
	return *m_pawn_to_move_next;
}
