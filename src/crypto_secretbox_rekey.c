#include "crypto_secretbox_rekey.h"

#ifdef __cplusplus
extern "C" {
#endif

int
crypto_secretbox_rekey_detached(unsigned char *c, unsigned long long clen,
                                const unsigned char *mac_old,
                                unsigned char *mac_new,
                                const unsigned char *no, const unsigned char *ko,
                                const unsigned char *nn, const unsigned char *kn)
{
  crypto_onetimeauth_poly1305_state state;

  unsigned char      block0[64U];

  unsigned char      subkey_old[crypto_stream_salsa20_KEYBYTES];
  unsigned char      subkey_new[crypto_stream_salsa20_KEYBYTES];

  crypto_core_hsalsa20(subkey_old, no, ko, NULL);
  crypto_stream_salsa20(block0, crypto_stream_salsa20_KEYBYTES,
                        no + 16, subkey_old);

  if (crypto_onetimeauth_poly1305_verify(mac_old, c, clen, block0) != 0) {
    sodium_memzero(block0, sizeof block0);
    sodium_memzero(subkey_old, sizeof subkey_old);
    return -1;
  }

  crypto_core_hsalsa20(subkey_new, nn, kn, NULL);

  unsigned long long mlen0 = clen;
  if (mlen0 > 64U - crypto_secretbox_ZEROBYTES) {
    mlen0 = 64U - crypto_secretbox_ZEROBYTES;
  }

  unsigned long long i;
  for (i = 0U; i < mlen0; i++) {
    block0[crypto_secretbox_ZEROBYTES + i] = c[i];
  }

  crypto_stream_salsa20_xor(block0, block0,
                            crypto_secretbox_ZEROBYTES + mlen0,
                            nn + 16, subkey_new);
  crypto_stream_salsa20_xor(block0, block0,
                            crypto_secretbox_ZEROBYTES + mlen0,
                            no + 16, subkey_old);
  for (i = 0U; i < mlen0; i++) {
      c[i] = block0[i + crypto_secretbox_ZEROBYTES];
  }

  crypto_onetimeauth_poly1305_init(&state, block0);
  sodium_memzero(block0, sizeof block0);

  crypto_stream_salsa20_xor_ic(c + mlen0, c + mlen0, clen - mlen0,
                               nn + 16, 1U, subkey_new);
  sodium_memzero(subkey_new, sizeof subkey_new);

  crypto_stream_salsa20_xor_ic(c + mlen0, c + mlen0, clen - mlen0,
                               no + 16, 1U, subkey_old);
  sodium_memzero(subkey_old, sizeof subkey_old);

  crypto_onetimeauth_poly1305_update(&state, c, clen);

  crypto_onetimeauth_poly1305_final(&state, mac_new);

  sodium_memzero(&state, sizeof state);

  return 0;
}

int
crypto_secretbox_rekey_easy(unsigned char *c, unsigned long long clen,
                            const unsigned char *no, const unsigned char *ko,
                            const unsigned char *nn, const unsigned char *kn)
{
  return crypto_secretbox_rekey_detached(c + crypto_secretbox_MACBYTES, clen - crypto_secretbox_MACBYTES,
                                         c,
                                         c,
                                         no, ko, nn, kn);
}

#ifdef __cplusplus
}
#endif
