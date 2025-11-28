#include <minicrt.h>

/* * 基础字符输出封装
 */
int fputc(int c, FILE *stream) {
  // 写入 1 个字节
  if (fwrite(&c, 1, 1, stream) != 1)
    return EOF;
  return c;
}

int fputs(const char *str, FILE *stream) {
  int len = strlen(str);
  // 写入字符串长度
  if (fwrite(str, 1, len, stream) != len)
    return EOF;
  return len;
}

/* * 核心格式化函数 vfprintf
 * 支持 %d (十进制), %x (十六进制), %s (字符串), %c (字符)
 */
int vfprintf(FILE *stream, const char *format, va_list arglist) {
  int translating = 0; // 状态机：0=普通模式, 1=正在解析%
  int ret = 0;         // 已输出字符计数
  const char *p = format;
  char buf[32]; // 用于数字转换的缓冲区 (ARM64 long long 可能很长，给32比较安全)

  for (; *p != '\0'; ++p) {
    switch (*p) {
    case '%':
      if (!translating) {
        translating = 1; // 进入格式化模式
      } else {
        // "%%" -> 输出一个 '%'
        if (fputc('%', stream) < 0)
          return EOF;
        ret++;
        translating = 0;
      }
      break;

    default:
      if (!translating) {
        // 普通字符，直接输出
        if (fputc(*p, stream) < 0)
          return EOF;
        ret++;
      } else {
        // 正在解析格式控制符 (例如 'd', 's')
        switch (*p) {
        case 'd': {
          int val = va_arg(arglist, int);
          itoa(val, buf, 10);
          if (fputs(buf, stream) < 0)
            return EOF;
          ret += strlen(buf);
          break;
        }
        case 'x': { // 补充一个十六进制，很有用
          int val = va_arg(arglist, int);
          itoa(val, buf, 16);
          if (fputs(buf, stream) < 0)
            return EOF;
          ret += strlen(buf);
          break;
        }
        case 's': {
          const char *str = va_arg(arglist, const char *);
          if (!str)
            str = "(null)";
          if (fputs(str, stream) < 0)
            return EOF;
          ret += strlen(str);
          break;
        }
        case 'c': {
          // 注意：va_arg读取字符时要用 int，因为入栈会对齐
          int ch = va_arg(arglist, int);
          if (fputc(ch, stream) < 0)
            return EOF;
          ret++;
          break;
        }
        default:
          // 不认识的格式符，原样输出 % 和当前字符
          fputc('%', stream);
          fputc(*p, stream);
          ret += 2;
          break;
        }
        translating = 0; // 处理完毕，回到普通模式
      }
      break;
    }
  }
  return ret;
}

/* * printf 实现
 */
int printf(const char *format, ...) {
  va_list(arglist);
  va_start(arglist, format);
  int ret = vfprintf(stdout, format, arglist);
  va_end(arglist);
  return ret;
}

/* * fprintf 实现
 */
int fprintf(FILE *stream, const char *format, ...) {
  va_list(arglist);
  va_start(arglist, format);
  int ret = vfprintf(stream, format, arglist);
  va_end(arglist);
  return ret;
}