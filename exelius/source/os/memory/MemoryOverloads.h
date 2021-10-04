#pragma once

/// <summary>
/// Global new overload. Prefer macro EXELIUS_NEW.
/// </summary>
/// <param name="sizeToAllocate">- The size in bytes of memory to allocate.</param>
/// <param name="pFileName">- The name of the file this allocation was made in.</param>
/// <param name="lineNum">- The line number of this allocation call.</param>
/// <returns>The allocated memory.</returns>
void* operator new(size_t sizeToAllocate, const char* pFileName, int lineNum);

/// <summary>
/// Global delete overload. Prefer macro EXELIUS_DELETE.
/// </summary>
/// <param name="pMemoryToFree">- The memory to free.</param>
void operator delete(void* pMemoryToFree);

/// <summary>
/// Global new[] overload. Prefer macro EXELIUS_NEW_ARRAY.
/// </summary>
/// <param name="sizeToAllocate">- The size in bytes of memory to allocate.</param>
/// <param name="pFileName">- The name of the file this allocation was made in.</param>
/// <param name="lineNum">- The line number of this allocation call.</param>
/// <returns>The allocated memory.</returns>
void* operator new[](size_t sizeToAllocate, const char* pFileName, int lineNum);

/// <summary>
/// Global delete[] overload. Prefer macro EXELIUS_DELETE_ARRAY.
/// </summary>
/// <param name="pMemoryToFree">- The memory to free.</param>
void operator delete[](void* pMemoryToFree);

#ifdef EXE_DEBUG
	#define EXELIUS_NEW(object) new(__FILE__, __LINE__) object
	#define EXELIUS_NEW_ARRAY(object, size) new(__FILE__, __LINE__) object[size]
	//#define EXELIUS_DELETE(objectPointer) operator delete(objectPointer, __FILE__, __LINE__)
	//#define EXELIUS_DELETE_ARRAY(objectPointer) operator delete[](objectPointer, __FILE__, __LINE__)
	#define EXELIUS_DELETE(objectPointer) delete objectPointer
	#define EXELIUS_DELETE_ARRAY(objectPointer) delete[] objectPointer
#else // !EXE_DEBUG
	#define EXELIUS_NEW(object) new object
	#define EXELIUS_NEW_ARRAY(object, size) new object[size]
	#define EXELIUS_DELETE(objectPointer) delete objectPointer
	#define EXELIUS_DELETE_ARRAY(objectPointer) delete[] objectPointer
#endif // EXE_DEBUG