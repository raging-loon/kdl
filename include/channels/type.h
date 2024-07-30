#ifndef CHANNELS_TYPE_H_
#define CHANNELS_TYPE_H_
#include <string>
namespace kdl
{



// TODO: CLEAN THIS CLASS UP
struct Variable
{
	constexpr static uint8_t RMOD_WIDE = (1 << 0);
	constexpr static uint8_t RMOD_ASCII = (1 << 1);
	constexpr static uint8_t RMOD_NOCASE = (1 << 2);
	
	constexpr static uint8_t REF_TYPE_LOCAL_VAR = 1;
	constexpr static uint8_t REF_TYPE_RULE = 2;
	constexpr static uint8_t REF_TYPE_EXTERNAL_VAR = 3;

	enum Type
	{
		NONE,
		STRING,
		REGEX,
		BYTE_SEQUENCE
	};

	std::string searchstr;

	Type type {NONE};

	uint8_t searchMods;

	uint8_t referenceType;

};



}

#endif // CHANNELS_TYPE_H_