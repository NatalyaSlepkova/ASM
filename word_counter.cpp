#include <bits/stdc++.h>
#include <stdint.h>

using namespace std;

const int BLOCK = 16;
int linearly(string s, size_t n)
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

void recount_flush(int &res, __m128i &a)
{
    for (int i = 0; i < BLOCK; i++)
    {
        res += (int)*((uint8_t*)&a + i);
    }
    a = _mm_setzero_si128();
}

int count(string& s)
{
    const __m128i MASK_SPACES = _mm_set_epi8(' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
                                             ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    const char* str = s.c_str();
    int res = 0;
    int shift = 0;
    bool was_space = true;

    for (; shift < s.size() && (size_t(str + shift) % BLOCK != 0 || shift == 0); shift++)
    {
        res += (s[shift] == ' ' && !was_space);
        was_space = (s[shift] == ' ');
    }

    __m128i ans = _mm_setzero_si128();
    int k = 0;
    int n = (int)s.size() - BLOCK;
    //cout << "size: " << (int)s.size() << ' ' << "n: " << n << "\n";
    for (; shift <= n; shift += BLOCK)
    {
        __m128i spaces = _mm_cmpeq_epi8(_mm_load_si128((__m128i *) (shift + str)), MASK_SPACES);
        __m128i shifted_spaces = _mm_cmpeq_epi8(_mm_loadu_si128((__m128i *) (shift + str - 1)), MASK_SPACES);
        __m128i count = _mm_andnot_si128(shifted_spaces, spaces);
        ans = _mm_sub_epi8(ans, count);
        ++k;
        if (k == 255)
        {
            recount_flush(res, ans);
            k = 0;
        }
    }
    recount_flush(res, ans);

    if (shift == 0)
    {
        was_space = true;
    }
    else
    {
        was_space = s[shift - 1] == ' ';
    }
    for (; shift < s.size(); shift++)
    {
        if (s[shift] == ' ')
        {
            res += !was_space;
            was_space = true;
        }
        else
        {
            was_space = false;
        }
    }

    return res + !was_space;
}

string test()
{
    std::string s = "";
    const int N = 1000;
    for (int i = 0; i < N; ++i)
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
        //cout << s << "\n";
        //cout << linearly(s, (int)s.size()) << "\n";
        //cout << count(s) << "\n";
        if (linearly(s, (int)s.size()) != count(s))
        {
            cout << ":(\n";
            return 0;
        }
        cout << "OK\n";
    }
    return 0;
}
