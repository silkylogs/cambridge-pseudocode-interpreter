int printf(const char *, ...);
void exit(int);
#define CHAR_BIT 8

#if defined PLATFORM_APPLE
 void *memcpy(void *dst, const void *src, unsigned long sz);
#elif defined PLATFORM_WINDOWS
 void *memcpy(void *dst, const void *src, unsigned long long sz);
#else 
 void *memcpy(void *dst, const void *src, unsigned long long sz);
#endif

typedef unsigned int word;
typedef unsigned char byte;
#define assert_platform_sizes \
	do { if (sizeof(word) != 4*sizeof(byte)) { \
		printf("\nCritical: Program is only defined for sizeof word == 4 * sizeof byte.\n"); \
		exit(0); \
	}} while(0);
