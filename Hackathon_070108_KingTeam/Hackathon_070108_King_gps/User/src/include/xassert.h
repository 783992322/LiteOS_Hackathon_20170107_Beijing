/* �Ǳ�׼���Ժ�xassert�����׼assert�и��������������з���ֵ������������ڱ��ʽ��ʹ�� */

#undef xassert

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef NDEBUG
/*
 * If not debugging, xassert does nothing.
 */
#define xassert(x)	((void)0)

#else /* debugging enabled */

/*
 * "debug/xassert.c" nicely supplies a function which does the actual output and
 * call to abort.
 */
#ifndef ___XASSERT_H_
#define ___XASSERT_H_
extern int _xassert(const char*, const char*, int);
#endif

/*
 * Definition of the xassert macro.
 */
#define xassert(e)       ((e) ? (int)0 : _xassert(#e, __FILE__, __LINE__))


#endif	/* NDEBUG */

#ifdef	__cplusplus
}
#endif

