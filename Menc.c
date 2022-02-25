#include "Menc.h"
#include <stdio.h>
#include <string.h>

/**
 * 加密字符串
 * 字符串每个字符须满足: 非空白, 单字节, ascii 码在 [0, 127] 中, 且不是 /|\<>:*"?
 * 加密算法特点: 不改变长度, 加密两次会得到原字符串, 加密后分布较为随机
 * 时间复杂度为 O(n**2), 空间复杂度为 O(1)
 * 使用方法: Menc [<字符串>...] [/ <文件名>...] 加密字符串并输出, 加密文件名并重命名
 * 注意: 该程序主要用于文件名加密, 不能进行文件内容加密
 * 算法为本人原创, 且无任何参考资料, 如有雷同纯属巧合
 */

// 对两个数字进行变化
void sh(char *a)
{
    int n = (int)*(a + 1) * 85 + *a;
    *(short *)a = *((short *)A + n);
}

// 对一组数字进行变化
void Msh(char *a, char *b)
{
    for (; a < b; a += 2)
    {
        sh(a);
    }
}

// 对字符串进行加密
char *Menc(char *s, char *end)
{
    int len = end - s;
    char *a0 = s, *a1 = s + 1, *b = end - 1;

    // 编码
    for (char *p = s; p < end; ++p)
    {
        *p = c2i[(unsigned char)*p];
        if (*p == -1)
        {
            return NULL;
        }
    }

    // 加密
    Msh(a0, b);
    for (int n = len + 1 >> 1; --n > 0;)
    {
        Msh(a1, b);
        Msh(a0, b);
    }

    // 解码
    for (char *p = s; p < end; ++p)
    {
        *p = i2c[*p];
    }

    return s;
}

// 对文件名进行加密, 并重命名
void ren(char *name)
{
    int len = (int)strlen(name);
    char name_0[256];
    char *end;

    if (len > 255)
    {
        return;
    }
    memcpy(name_0, name, len + 1);
    if ((end = strrchr(name, '.')) == NULL)
    {
        end = name + len;
    }
    if (end - name < 2 || !Menc(name, end))
    {
        return;
    }
    if (rename(name_0, name) == 0)
    {
        printf("%s -> %s\n", name_0, name);
    }
}

int main(int argc, char **argv)
{
    int isFile = 0;

    for (char **p = argv, **p_e = p + argc; ++p < p_e;)
    {
        if (**p == '/')
        {
            isFile = 1;
            if (*(*p + 1) != '\0')
            {
                ren(*p + 1);
            }
        }
        else if (isFile)
        {
            ren(*p);
        }
        else if (Menc(*p, *p + strlen(*p)))
        {
            puts(*p);
        }
    }
    return 0;
}
