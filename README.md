# `crypto-secretbox-rekey`

> Experimental extension to `libsodium` `crypto_secretbox` that allows for rekeying without decrypting the ciphertext

:warning: Work in progress! This is an experiment :construction:

## Usage

```c
#define MESSAGE ((const unsigned char *) "test")
#define MESSAGE_LEN 4
#define CIPHERTEXT_LEN (crypto_secretbox_MACBYTES + MESSAGE_LEN)

unsigned char key_old[crypto_secretbox_KEYBYTES];
unsigned char nonce_old[crypto_secretbox_NONCEBYTES];

unsigned char key_new[crypto_secretbox_KEYBYTES];
unsigned char nonce_new[crypto_secretbox_NONCEBYTES];

unsigned char ciphertext[CIPHERTEXT_LEN];

crypto_secretbox_keygen(key_new);
crypto_secretbox_keygen(key_old);

randombytes_buf(nonce_new, sizeof nonce_new);
randombytes_buf(nonce_old, sizeof nonce_old);

crypto_secretbox_easy(ciphertext_rekeyed, MESSAGE, MESSAGE_LEN, nonce_old, key_old);

crypto_secretbox_rekey_easy(ciphertext_rekeyed, CIPHERTEXT_LEN,
                            nonce_old, key_old,
                            nonce_new, key_new);

unsigned char decrypted[MESSAGE_LEN];
if (crypto_secretbox_open_easy(decrypted, ciphertext_rekeyed, CIPHERTEXT_LEN, nonce_new, key_new) != 0) {
  /* message forged! */
}
```

## API

```c
int crypto_secretbox_rekey_detached(unsigned char *c, unsigned long long clen,
                                    const unsigned char *mac_old,
                                    unsigned char *mac_new,
                                    const unsigned char *no, const unsigned char *ko,
                                    const unsigned char *nn, const unsigned char *kn);
```

```c
int crypto_secretbox_rekey_easy(unsigned char *c, unsigned long long clen,
                                const unsigned char *no, const unsigned char *ko,
                                const unsigned char *nn, const unsigned char *kn);
```

## Math notes

$$K_{derived} = \mathrm{hsalsa}_{20}(n_{1\dots16}, K_{in}), |K_{derived}| = 32$$

$$\mathrm{PRF} = \mathrm{salsa}_{20}(n_{17\dots32},K_{derived})$$

$$K_{\mathrm{MAC}} = PRF_{1\dots32}, |K_{\mathrm{MAC}}| = 32$$

$$C = M \oplus PRF_{33\dots}$$

Note that the above two equations look slightly different in the implementation
as they're called `block0`:

$$B_0 = [K_{\mathrm{MAC}},c_1,\dots,c_{32}], |B| = 64$$

$$\mathrm{MAC} = \mathrm{Poly1305}(C, K_{\mathrm{MAC}})$$

## License

[ISC](LICENSE)
