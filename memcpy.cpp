#include <ctime>
#include <cstdlib>
#include <iostream>
#include <emmintrin.h>

using namespace std;

const int N = (1 << 30);

void copy_asm(char *dst, char const *src, size_t size)
{
    const size_t BLOCK = 16;
    size_t k;

    for (k = 0; k < size && (size_t)(dst + k) % BLOCK != 0; k++)
    {
        *dst = *src;
        src++;
        dst++;
    }

    ssize_t back = (size - k) % BLOCK;

    for (size_t i = k; i < size - back; i += BLOCK)
    {
        __m128i reg;

        __asm__ volatile ("movdqu\t (%1), %0\n"
                "movntdq\t %0, (%2)\n"
        : "=x"(reg)
        : "r"(src), "r"(dst)
        :"memory");

        src += BLOCK;
        dst += BLOCK;
    }

    for (int i = (size - back); i < size; i++)
    {
        *dst = *src;
        src++;
        dst++;
    }
    _mm_sfence();

}

void copy_asm(void* dst, void const* src, size_t size) {
    copy_asm(static_cast<char*>(dst), static_cast<char const*>(src), size);
}

int main()
{
    srand(time(NULL));

    for (int i = 0; i < 1000; i++)
    {
        int n = rand() % 10000 + 1;
        int a[n];
        int b[n];
        for (size_t j = 0; j < n; j++)
        {
            a[j] = rand();
        }
        copy_asm(b, a, sizeof(a));
        for (int j = 0; j < n; j++)
        {
            if (a[j] != b[j])
            {
                cout << ":(\n";
                return 0;
            }
        }
        cout << "OK\n";
    }
    return 0;
}
