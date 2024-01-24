#ifndef PHASE4_ENGINE_AI_SCORE_EVALUATION_CONSTANTS_H
#define PHASE4_ENGINE_AI_SCORE_EVALUATION_CONSTANTS_H

#include <cstdint>

namespace phase4::engine::ai::score {

struct EvaluationConstants {
	static const int32_t Pieces[6];

	static const int32_t Checkmate;
	static const int32_t ThreefoldRepetition;
	static const int32_t InsufficientMaterial;

	static const int32_t DoubledPawns[2];
	static const int32_t IsolatedPawns[2];
	static const int32_t ChainedPawns[2];
	static const int32_t PassingPawns[2];

	static const int32_t CenterMobilityModifier;
	static const int32_t OutsideMobilityModifier;

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

inline constexpr int32_t EvaluationConstants::Pieces[] = { 100, 350, 370, 570, 1190, 20000 };

inline constexpr int32_t EvaluationConstants::Checkmate = 32000;
inline constexpr int32_t EvaluationConstants::ThreefoldRepetition = 0;
inline constexpr int32_t EvaluationConstants::InsufficientMaterial = 0;

inline constexpr int32_t EvaluationConstants::DoubledPawns[] = { -10, -30 };
inline constexpr int32_t EvaluationConstants::IsolatedPawns[] = { -25, -5 };
inline constexpr int32_t EvaluationConstants::ChainedPawns[] = { 4, 4 };
inline constexpr int32_t EvaluationConstants::PassingPawns[] = { 0, 30 };

inline constexpr int32_t EvaluationConstants::CenterMobilityModifier = 7;
inline constexpr int32_t EvaluationConstants::OutsideMobilityModifier = 6;

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

} //namespace phase4::engine::ai::score
#endif
