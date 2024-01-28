#ifndef PHASE4_ENGINE_COMMON_EVALUATION_CONSTANTS_H
#define PHASE4_ENGINE_COMMON_EVALUATION_CONSTANTS_H

#include <array>
#include <cstdint>

namespace phase4::engine::common {

struct EvaluationConstants {
	static const std::array<int32_t, 6> PIECE_VALUES;

	static const int32_t CHECKMATE;
	static const int32_t THREEFOLD_REPETITION;
	static const int32_t INSUFFICIENT_MATERIAL;

	static const std::array<int32_t, 2> DoubledPawns;
	static const std::array<int32_t, 2> IsolatedPawns;
	static const std::array<int32_t, 2> ChainedPawns;
	static const std::array<int32_t, 2> PassingPawns;

	static const int32_t CENTER_MOBILITY_MODIFIER;
	static const int32_t OUTSIDE_MOBILITY_MODIFIER;

	static const int32_t KingInDanger;
	static const int32_t PawnShield;

	static const int32_t DoubledRooks;
	static const int32_t RookOnOpenFile;
	static const int32_t PairOfBishops;

	static const int32_t Fianchetto;
	static const int32_t FianchettoWithoutBishop;
	static const int32_t OpenFileNextToKing;

	static const int32_t OpeningEndgameEdge;

	static const uint64_t Center;
	static const uint64_t ExtendedCenter;
	static const uint64_t ExtendedCenterRing;
	static const uint64_t Outside;
};

inline constexpr std::array<int32_t, 6> EvaluationConstants::PIECE_VALUES = { 100, 350, 370, 570, 1190, 20000 };

inline constexpr int32_t EvaluationConstants::CHECKMATE = 32000;
inline constexpr int32_t EvaluationConstants::THREEFOLD_REPETITION = 0;
inline constexpr int32_t EvaluationConstants::INSUFFICIENT_MATERIAL = 0;

inline constexpr std::array<int32_t, 2> EvaluationConstants::DoubledPawns = { -10, -30 };
inline constexpr std::array<int32_t, 2> EvaluationConstants::IsolatedPawns = { -25, -5 };
inline constexpr std::array<int32_t, 2> EvaluationConstants::ChainedPawns = { 4, 4 };
inline constexpr std::array<int32_t, 2> EvaluationConstants::PassingPawns = { 0, 30 };

inline constexpr int32_t EvaluationConstants::CENTER_MOBILITY_MODIFIER = 7;
inline constexpr int32_t EvaluationConstants::OUTSIDE_MOBILITY_MODIFIER = 6;

inline constexpr int32_t EvaluationConstants::KingInDanger = -20;
inline constexpr int32_t EvaluationConstants::PawnShield = 20;

inline constexpr int32_t EvaluationConstants::DoubledRooks = 40;
inline constexpr int32_t EvaluationConstants::RookOnOpenFile = 50;
inline constexpr int32_t EvaluationConstants::PairOfBishops = 50;

inline constexpr int32_t EvaluationConstants::Fianchetto = 25;
inline constexpr int32_t EvaluationConstants::FianchettoWithoutBishop = -25;
inline constexpr int32_t EvaluationConstants::OpenFileNextToKing = -30;

inline constexpr int32_t EvaluationConstants::OpeningEndgameEdge = 20500;

inline constexpr uint64_t EvaluationConstants::Center = 0x1818000000;
inline constexpr uint64_t EvaluationConstants::ExtendedCenter = 0x3c3c3c3c0000;
inline constexpr uint64_t EvaluationConstants::ExtendedCenterRing = EvaluationConstants::ExtendedCenter & ~EvaluationConstants::Center;
inline constexpr uint64_t EvaluationConstants::Outside = 0xffffc3c3c3c3ffff;

} //namespace phase4::engine::common
#endif
