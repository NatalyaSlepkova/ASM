#include <bits/stdc++.h>
#include <stdint.h>
#include <xmmintrin.h>
#include <tmmintrin.h>

using namespace std;

const int BLOCK = 16;
size_t linearly(string s, size_t n)
{
    int ans = 0;
    for (size_t i = 0; i < n ; i++)
    {
        if (s[i] != ' ' && (s[i - 1] == ' ' || i == 0))
        {
            ++ans;
        }
    }
    return ans;
}

void recount_flush(size_t &res, __m128i &a)
{
    for (int i = 0; i < BLOCK; i++)
    {
        res += (int)*((uint8_t*)&a + i);
    }
    a = _mm_setzero_si128();
}

size_t count(const char* str, size_t size)
{
    const __m128i MASK_SPACES = _mm_set_epi8(' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
                                             ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    const __m128i MASK_ONES = _mm_set_epi8(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
    size_t res = 0, shift = 0;
    if (*str != ' ')
    {
        ++res;
    }
    bool was_space = false;
    for (;(size_t) (str + shift) % BLOCK != 0; shift++)
    {
        res += (was_space && str[shift] != ' ');
        was_space = (str[shift] == ' ');
    }

    if (was_space && shift != 0 && *(str + shift) != ' ')
    {
        ++res;
    }

    __m128i ans = _mm_setzero_si128();
    size_t n = size - (size - shift) % BLOCK - BLOCK;
    __m128i last, now = _mm_cmpeq_epi8(_mm_load_si128((__m128i *) (str + shift)), MASK_SPACES);
    int k = 0;
    for (; shift < n; shift += BLOCK)
    {
        last = now;
        now = _mm_cmpeq_epi8(_mm_load_si128((__m128i *) (str + shift + BLOCK)), MASK_SPACES);
        __m128i shifted_spaces = _mm_alignr_epi8(now, last, 1);
        __m128i count = _mm_and_si128(_mm_andnot_si128(shifted_spaces, last), MASK_ONES);
        ans = _mm_add_epi8(ans, count);
        ++k;
        if (k == 255)
        {
            recount_flush(res, ans);
            k = 0;
        }
    }

    recount_flush(res, ans);

    //cout << "res: " << res << "\n" << "n: " << n << "\n";
    shift = n;

    if(*(str + shift - 1) == ' ' && *(str + shift) != ' '){
        //cout << "hear\n";
        --res;
    }

    was_space = *(str + shift - 1) == ' ';
    //cout << "was_space: " << was_space << "\n";
    //cout << "shift: " << shift << "\n" << "size: " << size << "\n";
    for(size_t i = shift; i < size; i++){
        char cur = *(str + i);
        //cout << "cur: " << cur << ' ';
        res += (was_space && cur != ' ');
        was_space = (cur == ' ');
    }

    return res;
}

string test()
{
    std::string s = "";
    int len = 1000;
    for (int i = 0; i < len; ++i)
    {
        int k = rand() % 2;
        if (k)
        {
            s += "A";
        } else
        {
            s += " ";
        }
    }
    return s;
}

int main() {
    srand(time(0));
    for (int i = 0; i < 1000; i++)
    {
        std::string s = test();
        const char* st = s.c_str();
        cout << s << "\n";
        size_t right_ans = linearly(s, (int)s.size());
        size_t my_ans = count(st, s.size());
        cout << "right: " << right_ans << "\n";
        cout << "my: " << my_ans << "\n";
        if (right_ans != my_ans)
        {
            cout << ":(\n";
            return 0;
        }
        cout << "OK\n";
    }
    return 0;
}
