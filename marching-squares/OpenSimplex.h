// FROM https://github.com/smcameron/open-simplex-noise-in-c

#ifndef OPEN_SIMPLEX_NOISE_H__
#define OPEN_SIMPLEX_NOISE_H__


#if ((__GNUC_STDC_INLINE__) || (__STDC_VERSION__ >= 199901L))
#include <stdint.h>
#define INLINE inline
#elif (defined (_MSC_VER) || defined (__GNUC_GNU_INLINE__))
#include <stdint.h>
#define INLINE __inline
#else 
	/* ANSI C doesn't have inline or stdint.h. */
#define INLINE
#endif

#ifdef __cplusplus
extern "C" {
#endif

	struct osn_context;

	int open_simplex_noise(int64_t seed, struct osn_context** ctx);
	void open_simplex_noise_free(struct osn_context* ctx);
	int open_simplex_noise_init_perm(struct osn_context* ctx, int16_t p[], int nelements);
	double open_simplex_noise2(const struct osn_context* ctx, double x, double y);
	double open_simplex_noise3(const struct osn_context* ctx, double x, double y, double z);
	double open_simplex_noise4(const struct osn_context* ctx, double x, double y, double z, double w);

#ifdef __cplusplus
}
#endif

#endif