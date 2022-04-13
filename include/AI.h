#ifndef __AI
#define __AI

#include "EIPF.h"

#include "../include/Pawn.h"


class AI : public LEti::EIPF
{
private:
	const Pawn* m_ai_pawns = nullptr;
	unsigned int m_ai_pawns_count = 0;
	const Pawn* m_player_pawns = nullptr;
	unsigned int m_player_pawns_count = 0;

	const Pawn* m_pawn_to_move_next = nullptr;

	using i_pair = std::pair<int, int>;
	using ipair_pair = std::pair<i_pair, i_pair>;

	ipair_pair m_ai_base;
	unsigned int m_ai_base_width = 0, m_ai_base_height = 0;
	ipair_pair m_player_base;
	unsigned int m_player_base_width = 0, m_player_base_height = 0;
	i_pair m_players_base_corner;

public:
	void set_pawns(const Pawn* _ai_pawns, unsigned int _ai_pawns_count, const Pawn* _player_pawns, unsigned int _player_pawns_count);
	void set_bases(std::pair<i_pair, i_pair> _ai_base, std::pair<i_pair, i_pair> _player_base);
	void set_pawn_and_destination();

private:
	bool pawn_is_on_players_base(const Pawn* _pawn) const;
	bool pawn_can_move(const Pawn* _pawn) const;
    i_pair find_furthest_free_cell_on_base(const i_pair& _corner, const i_pair& _prev) const;
    i_pair find_furthest_free_cell(const i_pair& _corner, const i_pair& _prev) const;

private:
	void build_path() override;

public:
	const Pawn& get_pawn_to_move_next() const;

};

#endif // __AI
