#define PICTUS_VER_MAJOR		1
#define PICTUS_VER_MINOR		1
#define PICTUS_VER_REVISION		6
#define PICTUS_VER_BUILD		0

#ifdef UNICODE
#define LOC_TX_(t) L#t
#else
#define LOC_TX_(t) #t
#endif

#define LOC_TX(t) LOC_TX_(t)

#define RC_FILE_VERSION			PICTUS_VER_MAJOR,PICTUS_VER_MINOR,PICTUS_VER_REVISION,PICTUS_VER_BUILD
#define STR_RC_FILE_VERSION		LOC_TX(RC_FILE_VERSION)