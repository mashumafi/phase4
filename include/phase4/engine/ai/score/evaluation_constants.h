#ifndef PHASE4_ENGINE_AI_SCORE_EVALUATION_CONSTANTS_H
#define PHASE4_ENGINE_AI_SCORE_EVALUATION_CONSTANTS_H

#include <cstdint>

namespace phase4::engine::ai::score {

struct EvaluationConstants {
	static const int Pieces[6];

	static const int Checkmate;
	static const int ThreefoldRepetition;
	static const int InsufficientMaterial;

	static const int DoubledPawns[2];
	static const int IsolatedPawns[2];
	static const int ChainedPawns[2];
	static const int PassingPawns[2];

	static const int CenterMobilityModifier;
	static const int OutsideMobilityModifier;

	static const int KingInDanger;
	static const int PawnShield;

	static const int DoubledRooks;
	static const int RookOnOpenFile;
	static const int PairOfBishops;

	static const int Fianchetto;
	static const int FianchettoWithoutBishop;
	static const int OpenFileNextToKing;

	static const int OpeningEndgameEdge;

	static const uint64_t Center;
	static const uint64_t ExtendedCenter;
	static const uint64_t ExtendedCenterRing;
	static const uint64_t Outside;
};

inline constexpr int32_t EvaluationConstants::Pieces[] = { 100, 350, 370, 570, 1190, 20000 };

inline constexpr int32_t EvaluationConstants::Checkmate = 32000;
inline constexpr int EvaluationConstants::ThreefoldRepetition = 0;
inline constexpr int EvaluationConstants::InsufficientMaterial = 0;

inline constexpr int EvaluationConstants::DoubledPawns[] = { -10, -30 };
inline constexpr int EvaluationConstants::IsolatedPawns[] = { -25, -5 };
inline constexpr int EvaluationConstants::ChainedPawns[] = { 4, 4 };
inline constexpr int EvaluationConstants::PassingPawns[] = { 0, 30 };

inline constexpr int EvaluationConstants::CenterMobilityModifier = 7;
inline constexpr int EvaluationConstants::OutsideMobilityModifier = 6;

inline constexpr int EvaluationConstants::KingInDanger = -20;
inline constexpr int EvaluationConstants::PawnShield = 20;

inline constexpr int EvaluationConstants::DoubledRooks = 40;
inline constexpr int EvaluationConstants::RookOnOpenFile = 50;
inline constexpr int EvaluationConstants::PairOfBishops = 50;

inline constexpr int EvaluationConstants::Fianchetto = 25;
inline constexpr int EvaluationConstants::FianchettoWithoutBishop = -25;
inline constexpr int EvaluationConstants::OpenFileNextToKing = -30;
 
inline constexpr int EvaluationConstants::OpeningEndgameEdge = 20500;
 
inline constexpr uint64_t EvaluationConstants::Center = 0x1818000000;
inline constexpr uint64_t EvaluationConstants::ExtendedCenter = 0x3c3c3c3c0000;
inline constexpr uint64_t EvaluationConstants::ExtendedCenterRing = EvaluationConstants::ExtendedCenter & ~EvaluationConstants::Center;
inline constexpr uint64_t EvaluationConstants::Outside = 0xffffc3c3c3c3ffff;

} //namespace phase4::engine::ai::score
#endif
