#ifndef TEC_SHA256_H
#define TEC_SHA256_H

/*
	This is basicly the implementation of sha256 by Brad Conte
	http://bradconte.com/sha256_c
	I only made formatting changes
	GI
*/

#define uchar unsigned char // 8-bit byte
#define uint unsigned int // 32-bit word

typedef struct _T_SHA256_CTX T_SHA256_CTX;
struct _T_SHA256_CTX{

	uchar data[64];
	uint datalen;
	uint bitlen[2];
	uint state[8];

};

// DBL_INT_ADD treats two unsigned ints a and b as one 64-bit integer and adds c to it
#define DBL_INT_ADD(a,b,c) if (a > 0xffffffff - (c)) ++b; a += c;
#define ROTLEFT(a,b) (((a) << (b)) | ((a) >> (32-(b))))
#define ROTRIGHT(a,b) (((a) >> (b)) | ((a) << (32-(b))))

#define CH(x,y,z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define EP0(x) (ROTRIGHT(x,2) ^ ROTRIGHT(x,13) ^ ROTRIGHT(x,22))
#define EP1(x) (ROTRIGHT(x,6) ^ ROTRIGHT(x,11) ^ ROTRIGHT(x,25))
#define SIG0(x) (ROTRIGHT(x,7) ^ ROTRIGHT(x,18) ^ ((x) >> 3))
#define SIG1(x) (ROTRIGHT(x,17) ^ ROTRIGHT(x,19) ^ ((x) >> 10))

void t_sha256_transform(T_SHA256_CTX *ctx, uchar data[]);
void t_sha256_init(T_SHA256_CTX *ctx);
void t_sha256_update(T_SHA256_CTX *ctx, uchar data[], uint len);
void t_sha256_final(T_SHA256_CTX *ctx, uchar hash[]);

#endif


