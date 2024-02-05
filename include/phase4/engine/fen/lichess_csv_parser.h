#ifndef PHASE4_ENGINE_FEN_LICHESS_CSV_PARSER_H
#define PHASE4_ENGINE_FEN_LICHESS_CSV_PARSER_H

#include <phase4/engine/moves/move.h>

#include <cstdint>
#include <fstream>
#include <optional>
#include <string>
#include <vector>

namespace phase4::engine::fen {

struct LichessPuzzle {
	std::string puzzleId;
	std::string fen;
	moves::Move badMove;
	moves::Moves expectedMoves;
	uint16_t rating;
	uint16_t ratingDeviation;
	uint16_t popularity;
	uint16_t nbPlays;
	std::vector<std::string> themes;
	std::string gameUrl;
	std::vector<std::string> openingTags;
};

class LichessCsvParser {
public:
	static constexpr size_t SPACE = 1;
	static constexpr size_t NUM_COLUMNS = 9;
	static constexpr size_t NUM_ROWS = 8;
	static constexpr size_t NUM_PIECES = NUM_COLUMNS * NUM_ROWS + SPACE;
	static constexpr size_t COLOR_TO_MOVE = 1 + SPACE;
	static constexpr size_t EN_PASSANT = 2 + SPACE;
	static constexpr size_t CASTLING_RIGHTS = 4 + SPACE;
	static constexpr size_t MAX_HALF_MOVES = 4 + SPACE;
	static constexpr size_t MAX_MOVES = 4;
	static constexpr size_t NULL_TERMINATOR = 1;
	static constexpr size_t EXPECTED_FEN_SIZE = NUM_PIECES + COLOR_TO_MOVE + EN_PASSANT + CASTLING_RIGHTS + MAX_HALF_MOVES + MAX_MOVES + NULL_TERMINATOR;

#define INVALIDATE_RETURN(cond) \
	if (cond) {                 \
		m_isValid = false;      \
		return std::nullopt;    \
	}

	LichessCsvParser(const std::string &filename) :
			file(filename) {
		if (!file.is_open()) {
			std::cerr << "Error opening the file." << std::endl;
			m_isValid = false;
			return;
		}

		std::string header;
		if (!std::getline(file, header) || header != "PuzzleId,FEN,Moves,Rating,RatingDeviation,Popularity,NbPlays,Themes,GameUrl,OpeningTags") {
			m_isValid = false;
			return;
		}
	}

	~LichessCsvParser() {
		if (file.is_open()) {
			file.close();
		}
	}

	std::optional<LichessPuzzle> nextPuzzle() {
		if (!m_isValid) {
			return std::nullopt;
		}

		std::string line;
		line.reserve(100);
		INVALIDATE_RETURN(!std::getline(file, line));

		std::istringstream lineStream(line);

		LichessPuzzle puzzle;

		puzzle.puzzleId.reserve(10);
		INVALIDATE_RETURN(!std::getline(lineStream, puzzle.puzzleId, ','));

		puzzle.fen.reserve(EXPECTED_FEN_SIZE);
		INVALIDATE_RETURN(!std::getline(lineStream, puzzle.fen, ','));

		std::string moves;
		INVALIDATE_RETURN(!std::getline(lineStream, moves, ','));

		std::istringstream moveStream(moves);

		std::string token;
		token.reserve(6); // expect move notation of 2, 2 character squares and a possible promotion + null terminator
		moveStream >> token;
		puzzle.badMove = moves::Move(token);
		while (moveStream >> token) {
			puzzle.expectedMoves.emplace_back(token);
		}

		std::string rating;
		rating.reserve(5); // expect a 4 character number + null terminator
		INVALIDATE_RETURN(!std::getline(lineStream, rating, ','));
		const auto [_, ec] = std::from_chars(rating.data(), rating.data() + rating.size(), puzzle.rating);
		if (ec != std::errc()) {
			return std::nullopt;
		}

		return puzzle;
	}

private:
	std::ifstream file;
	bool m_isValid = true;
};

} //namespace phase4::engine::fen

#endif
