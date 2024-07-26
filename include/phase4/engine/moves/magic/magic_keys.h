#ifndef PHASE4_ENGINE_MOVES_MAGIC_MAGIC_KEYS_H
#define PHASE4_ENGINE_MOVES_MAGIC_MAGIC_KEYS_H

#include <phase4/engine/common/random.h>

#include <array>
#include <type_traits>

namespace phase4::engine::moves::magic {

class MagicKeys {
public:
	using Array = std::array<uint64_t, 64>;

	static inline constexpr Array ROOK_KEYS{
		360305701962137858ULL,
		90107182288211968ULL,
		72084258017058816ULL,
		1188967893946106004ULL,
		612501094328763392ULL,
		7061645478438637576ULL,
		36109062047269376ULL,
		4683744711986921506ULL,
		9223512913930240000ULL,
		18155686290538496ULL,
		281754172653616ULL,
		586031180986458376ULL,
		23925446505136384ULL,
		1156862197230997796ULL,
		1297318176249413772ULL,
		14060378778434093184ULL,
		4611726150673105032ULL,
		5809652866230542467ULL,
		72344567657005073ULL,
		738662907462156320ULL,
		13979455817913991184ULL,
		23222235367998464ULL,
		41729769104212480ULL,
		288272157930176641ULL,
		108227130694909952ULL,
		614917273148194944ULL,
		72356663356563456ULL,
		17596483111168ULL,
		9223591943475626288ULL,
		293297140489521168ULL,
		292786769529800705ULL,
		9223653795299885316ULL,
		18016323200090256ULL,
		297309181105410048ULL,
		17598636892160ULL,
		9261725203099028208ULL,
		144256011471947776ULL,
		563517056897048ULL,
		1152932534418411585ULL,
		18015775080055811ULL,
		306315144052375552ULL,
		96264626760712256ULL,
		1407512326766608ULL,
		2342733823483084928ULL,
		616827231207429ULL,
		562969553534984ULL,
		10097079165661741136ULL,
		36294642237452ULL,
		4619286392575369472ULL,
		4629701104153657408ULL,
		4925880985976960ULL,
		288802122466754688ULL,
		36055187378503808ULL,
		70506251323393ULL,
		291045160345731584ULL,
		281477405352192ULL,
		634557812445706ULL,
		2452492022409462850ULL,
		4629735605607088169ULL,
		4621823657369931777ULL,
		9570286717372682ULL,
		9288682955866113ULL,
		580973152355483780ULL,
		4789480217151594ULL
	};

	static inline constexpr Array BISHOP_KEYS{
		571780541481002ULL,
		1169899716219396ULL,
		2306977716336861568ULL,
		2256812042616964ULL,
		4653270782181697ULL,
		434636964176601096ULL,
		9228439754852270083ULL,
		1724176035487744ULL,
		18049660359278848ULL,
		38291634932286976ULL,
		2377940288892108816ULL,
		144141595968012360ULL,
		1127069220143168ULL,
		4822466862252224ULL,
		73192848603750401ULL,
		576566863850373634ULL,
		40815007105092608ULL,
		2379097225298445072ULL,
		1155175520758927410ULL,
		1126346629597189ULL,
		2318227918936866816ULL,
		19703252681558276ULL,
		5633899766481408ULL,
		4794970754516096ULL,
		10525774346417375232ULL,
		5154515343132803ULL,
		89060576592640ULL,
		23723064500027456ULL,
		4521484223528960ULL,
		9656281105221288208ULL,
		1297600192396658753ULL,
		4902769077665677570ULL,
		598271774032384ULL,
		21409793496450561ULL,
		4611722920833057000ULL,
		9183224194466564ULL,
		180709151252283526ULL,
		2738332171377902336ULL,
		1445814402770338048ULL,
		72220326238372097ULL,
		180726864233628674ULL,
		4611757540103168000ULL,
		913224123876909152ULL,
		586318978457043457ULL,
		324338488398840080ULL,
		4504767860703488ULL,
		297281560172822808ULL,
		42925657684230272ULL,
		565217763540992ULL,
		9224955919191253120ULL,
		4902168796898787361ULL,
		36046944364265648ULL,
		17609450061834ULL,
		13844103333750638624ULL,
		153299151089582212ULL,
		721706547608092804ULL,
		2311195440441328133ULL,
		585507206070784ULL,
		575664162819ULL,
		635668138000ULL,
		181833112318150144ULL,
		2305926778525335745ULL,
		2330999148511312ULL,
		81073658256180248ULL
	};
};

} //namespace phase4::engine::moves::magic

#endif
