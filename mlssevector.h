#ifndef MLSSEVECTOR_H
#define MLSSEVECTOR_H

#include <stdint.h>
#include <emmintrin.h>
#include <cstdlib>
#include <new>

#define ML_ALIGN_16BYTE \
public: \
	static void *operator new(size_t size) { \
		return mlAllocateAlignedMemory(size, 16); \
	} \
	static void operator delete(void *p) { \
		mlFreeAlignedMemory(p); \
	} \
	static void *operator new(size_t size, void *buf) { \
		Q_UNUSED(size); \
		return buf; \
	} \
	static void operator delete(void *p, void *buf) { \
		Q_UNUSED(p); \
		Q_UNUSED(buf); \
	} \
	static void *operator new[](size_t size) { \
		return mlAllocateAlignedMemory(size, 16); \
	} \
	static void operator delete[](void *p) { \
		mlFreeAlignedMemory(p); \
	} \
	static void *operator new[](size_t size, void *buf) { \
		Q_UNUSED(size); \
		return buf; \
	} \
	static void operator delete[](void *p, void *buf) { \
		Q_UNUSED(p); \
		Q_UNUSED(buf); \
	}

// Vector classes to use SSE/SSE2 instructions

struct MLSseV4f32 {
	union {
		float f[4];
		__v4sf v;
	};
	
	ML_ALIGN_16BYTE
};

struct MLSseV4i32 {
	union {
		int32_t i[4];
		__v4si v;
	};
	
	ML_ALIGN_16BYTE
};


inline __v4sf mlFloatToVector(float f)
{
	return (__v4sf){f,f,f,f};
}


#endif // MLSSEVECTOR_H
