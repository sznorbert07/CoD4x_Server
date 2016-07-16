#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "../tomcrypt/tomcrypt.h"

static const unsigned char playerid_salt[] =
{
		0x83, 0xf3, 0x65, 0x90, 0x6c, 0x75, 0x33, 0xe1, 0x81, 0xaf, 0xc1, 0x44, 0xe1, 0x3e, 0xf0, 0x66,
		0x77, 0xec, 0x0e, 0x62, 0x6a, 0xe2, 0xf3, 0xba, 0x58, 0x1a, 0x72, 0x62, 0x2a, 0xc4, 0xcc, 0x61,
		0x49, 0x4b, 0xf5, 0xb9, 0x49, 0xea, 0x2b, 0xdf, 0x0f, 0x99, 0xe2, 0x8e, 0xb8, 0x7e, 0x50, 0x63,
		0xc6, 0xd5, 0x44, 0xdd, 0x0b, 0x56, 0x96, 0x19, 0x94, 0x76, 0x98, 0x90, 0x17, 0xb2, 0x66, 0xfa,
		0x23, 0x7c, 0xcd, 0x31, 0x49, 0x4b, 0x85, 0x31, 0xc2, 0xfe, 0x86, 0x3d, 0x80, 0xcd, 0xe5, 0x3a,
		0xe9, 0x43, 0xc3, 0x7c, 0x19, 0xc3, 0x9c, 0xbe, 0x19, 0x33, 0x5c, 0x22, 0x34, 0x16, 0xc9, 0xc2,
		0xce, 0xd2, 0x25, 0x2d, 0x5f, 0x2e, 0x32, 0x81, 0x97, 0xcf, 0x14, 0x96, 0x6b, 0x15, 0x59, 0xce,
		0xbd, 0x36, 0xdb, 0xbc, 0x23, 0x16, 0x74, 0x68, 0xa1, 0x4f, 0x0c, 0x46, 0xbc, 0x1e, 0x19, 0x12,
		0x8a, 0x86, 0x16, 0x2d, 0xe9, 0x3f, 0x22, 0x49, 0x0d, 0xaa, 0x6e, 0x15, 0x47, 0xe8, 0x19, 0x17,
		0xa2, 0xf1, 0xcc, 0xca, 0x49, 0x38, 0xa0, 0xa7, 0xb3, 0xcc, 0x92, 0xb5, 0x7c, 0x0c, 0xd4, 0x25,
		0x6a, 0x3e, 0x55, 0xc4, 0x72, 0x39, 0x81, 0x22, 0x0d, 0x1a, 0xc7, 0x1e, 0xf4, 0x96, 0xe4, 0xc6,
		0x6d, 0x6c, 0x43, 0x81, 0xcf, 0x64, 0x49, 0xa1, 0x10, 0x73, 0x46, 0x7a, 0x05, 0xdb, 0xdb, 0xd4,
		0x94, 0x3f, 0x04, 0x52, 0xd8, 0x23, 0x9d, 0x85, 0x59, 0x3a, 0x29, 0xfc, 0xfc, 0xd5, 0x06, 0xc9,
		0xdf, 0x58, 0xc3, 0x7e, 0x49, 0xe4, 0xbe, 0x79, 0x37, 0x97, 0x51, 0xc2, 0xf0, 0x0c, 0x38, 0x6f,
		0x96, 0x58, 0xaf, 0xb0, 0x43, 0x10, 0x41, 0x57, 0xef, 0xc6, 0xfa, 0x40, 0x6f, 0xa4, 0xc5, 0xc6,
		0x67, 0x08, 0xd3, 0x65, 0x0f, 0xb0, 0xa4, 0x4d, 0x54, 0xe4, 0x87, 0xc7, 0x0f, 0x52, 0xe2, 0xc6
};

uint64_t SV_SApiGUID2PlayerID(const char* guid)
{
	uint8_t diggest2[16];
	char digit2[3];
	int len = strlen(guid);
	int i;

	if(len != 32)
	{
		return 0;
	}

	for(i = 0; i < sizeof(diggest2); ++i)
	{
		digit2[i] = guid[2*i];
		digit2[i+1] = guid[2*i +1];
		digit2[i+2] = 0;
		diggest2[i] = strtol(digit2, NULL, 16);;
	}

	//Random account id
	uint32_t accountid;
	uint32_t universe = 32; //Custom universe 32 for HW auth players
	uint32_t accounttype = 1;
	uint32_t instance = 1;

	unsigned long outlen = sizeof(accountid);

	int hash_idx = find_hash("sha256");

	if(pkcs_5_alg2(diggest2, sizeof(diggest2), playerid_salt, sizeof(playerid_salt), 100, hash_idx, (unsigned char *)&accountid, &outlen) != CRYPT_OK) //This function is special. It sleeps
	{
		//Com_PrintError("Couldn't create hash for playerid. Player id is invalid\n");
		accountid = 0;
	}

	uint64_t steamid = ((uint64_t)universe << 56) | ((uint64_t)accounttype << 52) | ((uint64_t)instance << 32) | accountid;
	return steamid;
}
