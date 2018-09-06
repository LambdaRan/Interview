
#include <stdio.h>
#include <stdlib.h>

#include <string.h>

/*
 * sizeof(word) MUST BE A POWER OF TWO
 * SO THAT wmask BELOW IS ALL ONES
 */
typedef	long word;		/* "word" used for optimal copy speed */

#define	wsize	sizeof(word)
#define	wmask	(wsize - 1)

// 简化版 假设内存对齐
void* simpMemcpy(void *dst0, const void *src0, size_t length)
{
    char* dst = dst0;
    const char* src = src0;
    size_t t;
    if (length == 0 || dst == src) return dst;
    if ((unsigned long)dst < (unsigned long)src)
    {
        // 从前往后拷贝
        t = length / wsize;
        do { // 按字拷贝
            *(word*)dst = *(word*)src;
            dst += wsize;
            src += wsize;
        } while (--t);
        // 剩余的按字节拷贝
        t = length & wmask;
        do {
            *dst++ = *src++;
        } while (--t);
    }
    else  // 重叠 (dst > src && dst < src + length)
    {   // 从后向前拷贝
        src += length;
        dst += length;
        t = length / wsize;
        do { // 按字拷贝
            dst -= wsize;
            src -= wsize;
            *(word*)dst = *(word*)src;
        } while (--t);
        // 剩余的按字节拷贝
        t = length & wmask;
        do {
            *--dst = *--src;
        } while (--t);
    }
    return dst0;
}

void* Memcpy(void *dst0, const void *src0, size_t length)
{
    char* dst = dst0;
    const char* src = src0;
    size_t t;
    if (length == 0 || dst == src)
        goto done;
	/*
	 * Macros: loop-t-times; and loop-t-times, t>0
	 */
#define	TLOOP(s) if (t) TLOOP1(s)
#define	TLOOP1(s) do { s; } while (--t)    

    if ((unsigned long)dst < (unsigned long)src)
    {
        // Copy forward.
        t = (long)src; // only need low bits
        if ((t | (long)dst) & wmask) // 如果不对齐
        {
            // Try to align operands. This cannot be done unless the low bits match
            // 尝试对齐操作数。 除非低位匹配，否则无法完成此操作
            if ((t ^ (long)dst) & wmask || length < wsize)
                t = length;
            else  
                t = wsize - (t & wmask);
            length -= t;
            TLOOP1(*dst++ = *src++); // 将不对齐的按字节拷贝
        }
        // Copy whole words, then mop up any trailing bytes.
        // 复制整个单词，然后擦除任何尾随字节。
        t = length / wsize; // 对齐按字拷贝
        TLOOP(*(word *)dst = *(word *)src; src += wsize; dst += wsize);
        t = length & wmask; // 剩余的按字节拷贝
        TLOOP(*dst++ = *src++);
    }
    else  // 重叠， 从后往前拷贝
    {
		//  * Copy backwards.  Otherwise essentially the same.
		//  * Alignment works as before, except that it takes
		//  * (t&wmask) bytes to align, not wsize-(t&wmask).
        src += length;
        dst += length;
        t = (long)src;
        if ((t | (long)dst) & wmask)
        {
            if ((t ^ (long)dst) & wmask || length <= wsize)
                t = length;
            else  
                t &= wmask;
            length -= t;
            TLOOP1(*--dst = *--src);
        }
        t = length / wsize;
        TLOOP(src -= wsize; dst -= wsize; *(word*)dst = *(word*)src);
        t = length & wmask;
        TLOOP(*--dst = *--src);
    }
done: 
    return (dst0);
}

int main()
{
    char src[] = "01234567890123456789";
    char dst[64];

    simpMemcpy(dst, src, sizeof(src)/sizeof(char));
    printf("%s\n", dst);

    simpMemcpy(dst+17, dst, sizeof(src)/sizeof(char));
    printf("%s\n", dst);
    return 0;
}

