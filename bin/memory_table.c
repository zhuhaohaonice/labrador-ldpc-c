#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "ldpc_codes.h"
#include "ldpc_encoder.h"
#include "ldpc_decoder.h"

int main()
{
    enum ldpc_code codes[] = {
        LDPC_CODE_N128_K64,
        LDPC_CODE_N256_K128,
        LDPC_CODE_N512_K256,
        LDPC_CODE_N1280_K1024,
        LDPC_CODE_N1536_K1024,
        LDPC_CODE_N2048_K1024
    };

    size_t i, g, ci, cs, vi, vs, bf_wa, mp_wa, llrs, mp_out;
    int n, k;

    (void)ci; (void)cs; (void)vi; (void)vs; (void)bf_wa; (void)mp_wa;
    (void)llrs; (void)mp_out;

    printf("\nRequired RAM size, in bytes:\n\n");
    printf("| Code        | TX Small | TX Fast  | RX BF    | RX MP    |\n");
    printf("|-------------|----------|----------|----------|----------|\n");

    for(i=0; i<sizeof(codes)/sizeof(codes[0]); i++) {
        enum ldpc_code code = codes[i];
        printf("| ");

        /* (n, k) */
        ldpc_codes_get_params(code, &n, &k, NULL, NULL, NULL, NULL);
        printf("(%4d,%4d) |", n, k);

        /* TX Small */
        printf(" %8d |", n/8);

        /* TX Fast */
        g = ldpc_codes_size_generator(code);
        printf(" %8zu |", n/8 + g);

        /* RX BF */
        ldpc_codes_size_sparse_paritycheck(code, &ci, &cs, &vi, &vs);
        bf_wa = ldpc_decode_size_bf_wa(code);
        printf(" %8zu |", ci+cs+bf_wa+n/8+k/8);

        /* RX MP */
        llrs = ldpc_decode_size_llrs(code);
        mp_wa = ldpc_decode_size_mp_wa(code);
        mp_out = ldpc_decode_size_mp_out(code);
        printf(" %8zu |", ci+cs+vi+vs+llrs+mp_wa+mp_out+n/8);

        printf("\n");
    }

    printf("\n");

    return 0;
}
