#ifdef NDEBUG
	#define assert(cond) ((void)0)
#else
	void assertImpl(const char*, const char*, long);
	#define assert(cond) \
	((cond) ? (void)0) : assertImpl(???))
#endif
