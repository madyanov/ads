#include "rk.h"

size_t *rk_search(const char *text, const char *patt) {
    if (!text || !patt) {
        return NULL;
    }

    size_t patt_l = strlen(patt);
    size_t text_l = strlen(text);

    if (text_l < patt_l || !text_l || !patt_l) {
        return NULL;
    }

    long char_max = UCHAR_MAX + 1;
    long prime = 103;
    long patt_h = 0;
    long text_h = 0;
    size_t i, j;
 
    for (i = 0; i < patt_l; i++) {
        patt_h = (char_max * patt_h + patt[i]) % prime;
        text_h = (char_max * text_h + text[i]) % prime;
    }

    long pow = 1;
 
    for (i = 0; i < patt_l - 1; i++) {
        pow = (char_max * pow) % prime;
    }

    size_t *occs = NULL;
 
    for (i = 0; i <= text_l - patt_l; i++) {
        if (patt_h == text_h) {
            for (j = 0; j < patt_l; j++) {
                if (text[i + j] != patt[j]) {
                    break;
                }
            }
 
            if (j == patt_l) {
                vec_push(occs, i);
            }
        }
 
        if (i < text_l - patt_l) {
            text_h = (char_max * (text_h - text[i] * pow) + text[i + patt_l]) % prime;

            if (text_h < 0) {
                text_h += prime;
            }
        }
    }

    return occs;
}
