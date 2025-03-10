/*
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <https://unlicense.org>
 */

/* ----------------------------------------------------------------------- */
/* Keyed Binary Cipherage Utility: Shift, XOR, ROT128                      */
/*                fuzz.c -- data obfuscation implmentation                 */
/* Ver. 1.00                    24SEP2024                   CrateOfThunder */
/* ----------------------------------------------------------------------- */

/*
 * gcc -x c -ansi -Wall -Wextra -Wpedantic -Werror -Os -s
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int low = 0, rem = 0;

#define Q (1024*1024) /* 1 MiB */
#define sbr(b) ((b << low) | (b >> rem))
#define sbl(b) ((b >> low) | (b << rem))

int main(int argc, char *argv[])
{
  if (argc == 4) {
    FILE *ifile = fopen(argv[1], "rb"),
         *ofile = fopen(argv[2], "wb");
    char *key = argv[3], buf[Q], *p, k;
    size_t n = strlen(key), i = 0, j, r;

    while ((j = fread(buf, 1, Q, ifile)) != 0) {
      p = &buf[0];

      for (r = 0; r < j; r++) {
        k = key[i++];
        i %= n;
        low = k & 7;
        rem = 8 - low;
        k = (k & 1) ? sbr(k) : sbl(k);
        *p = ((*p ^ k) + 128) & 255;
        ++p;
      }

      fwrite(buf, 1, j, ofile);
    }

    fflush(ofile);
    fclose(ofile);
    fclose(ifile);
  }

  return 0;
}
