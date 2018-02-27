#include <string.h>
#include <sodium.h>
#include "crypto_secretbox_rekey.h"

#define MESSAGE ((const unsigned char *) "test")
#define MESSAGE_LEN 4
#define CIPHERTEXT_LEN (crypto_secretbox_MACBYTES + MESSAGE_LEN)

unsigned char key_old[crypto_secretbox_KEYBYTES];
unsigned char nonce_old[crypto_secretbox_NONCEBYTES];

unsigned char key_new[crypto_secretbox_KEYBYTES];
unsigned char nonce_new[crypto_secretbox_NONCEBYTES];

unsigned char ciphertext_old[CIPHERTEXT_LEN];
unsigned char ciphertext_new[CIPHERTEXT_LEN];
unsigned char ciphertext_rekeyed[CIPHERTEXT_LEN];

void printchar (unsigned char *c, unsigned long long clen) {
  for (unsigned long long i = 0; i < clen; i++) {
    printf("%02x ", *(c + i));
  }
  printf("\n");
}

int main () {
  crypto_secretbox_keygen(key_new);
  crypto_secretbox_keygen(key_old);

  randombytes_buf(nonce_new, sizeof nonce_new);
  randombytes_buf(nonce_old, sizeof nonce_old);

  printf("message:        ");
  printchar(MESSAGE, MESSAGE_LEN);

  printf("key_new:        ");
  printchar(key_new, crypto_secretbox_KEYBYTES);

  printf("nonce_new:      ");
  printchar(nonce_new, crypto_secretbox_NONCEBYTES);

  printf("key_old:        ");
  printchar(key_old, crypto_secretbox_KEYBYTES);

  printf("nonce_old:      ");
  printchar(nonce_old, crypto_secretbox_NONCEBYTES);

  crypto_secretbox_easy(ciphertext_old, MESSAGE, MESSAGE_LEN, nonce_old, key_old);
  printf("ciphertext_old: ");
  printchar(ciphertext_old, CIPHERTEXT_LEN);

  crypto_secretbox_easy(ciphertext_new, MESSAGE, MESSAGE_LEN, nonce_new, key_new);
  printf("ciphertext_new: ");
  printchar(ciphertext_new, CIPHERTEXT_LEN);

  crypto_secretbox_easy(ciphertext_rekeyed, MESSAGE, MESSAGE_LEN, nonce_old, key_old);
  printf("ciphertext_rek: ");
  printchar(ciphertext_rekeyed, CIPHERTEXT_LEN);


  crypto_secretbox_rekey_easy(ciphertext_rekeyed, CIPHERTEXT_LEN,
                              nonce_old, key_old,
                              nonce_new, key_new);

  printf("ciphertext_reg: ");
  printchar(ciphertext_rekeyed, CIPHERTEXT_LEN);

  unsigned char decrypted[MESSAGE_LEN];
  if (crypto_secretbox_open_easy(decrypted, ciphertext_rekeyed, CIPHERTEXT_LEN, nonce_new, key_new) != 0) {
      return 1;
  }

  printf("decrypted:      ");
  printchar(decrypted, MESSAGE_LEN);

  return 0;
}
