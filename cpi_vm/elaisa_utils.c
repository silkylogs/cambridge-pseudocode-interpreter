int write_byte(byte *adr, byte what) {
 *adr = what;
 return 1;
}

int write_word(byte *adr, word what) {
 adr[0] = (what >> (0 * 4)) & 0xFF;
 adr[1] = (what >> (1 * 4)) & 0xFF;
 adr[2] = (what >> (2 * 4)) & 0xFF;
 adr[3] = (what >> (3 * 4)) & 0xFF;
 return 4;
}

int read_byte(byte *adr, byte *out) {
 *out = *adr;
 return 1;
}

int read_word(byte *adr, word *out) {
 *out = 0;
 *out |= ((word)adr[0]) << (word)(0 * 4);
 *out |= ((word)adr[1]) << (word)(1 * 4);
 *out |= ((word)adr[2]) << (word)(2 * 4);
 *out |= ((word)adr[3]) << (word)(3 * 4);
 return 4;
}

void print_byte(byte b) {
 printf("%2.2x ", b);
}

void print_word_bytes(word i) {
 byte tmp[4] = { 0 };
 memcpy(tmp, &i, sizeof i);
 for (i = 0; i < sizeof i; ++i) {
  print_byte(tmp[i]);
 }
}
