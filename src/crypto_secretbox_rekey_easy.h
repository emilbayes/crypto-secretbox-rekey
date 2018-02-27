#ifndef CRYPTO_SECRETBOX_REKEY_H
#define CRYPTO_SECRETBOX_REKEY_H 1

#include <sodium.h>

#ifdef __cplusplus
extern "C" {
#endif

int
crypto_secretbox_rekey_detached(unsigned char *c, unsigned long long clen,
                                const unsigned char *mac_old,
                                unsigned char *mac_new,
                                const unsigned char *no, const unsigned char *ko,
                                const unsigned char *nn, const unsigned char *kn);

int
crypto_secretbox_rekey_easy(unsigned char *c, unsigned long long clen,
                            const unsigned char *no, const unsigned char *ko,
                            const unsigned char *nn, const unsigned char *kn);

#ifdef __cplusplus
}
#endif

#endif
