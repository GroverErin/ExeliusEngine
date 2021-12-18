#pragma once
#include <new>

/// <summary>
/// Global new overload. Prefer macro EXELIUS_NEW.
/// </summary>
/// <param name="sizeToAllocate">- The size in bytes of memory to allocate.</param>
/// <param name="pFileName">- The name of the file this allocation was made in.</param>
/// <param name="lineNum">- The line number of this allocation call.</param>
/// <returns>The allocated memory.</returns>
void* operator new(size_t sizeToAllocate, const char* pFileName, int lineNum);

/// <summary>
/// Global new[] overload. Prefer macro EXELIUS_NEW_ARRAY.
/// </summary>
/// <param name="sizeToAllocate">- The size in bytes of memory to allocate.</param>
/// <param name="pFileName">- The name of the file this allocation was made in.</param>
/// <param name="lineNum">- The line number of this allocation call.</param>
/// <returns>The allocated memory.</returns>
void* operator new[](size_t sizeToAllocate, const char* pFileName, int lineNum);

/// <summary>
/// Global new overload, with alignment. Prefer macro EXELIUS_NEW.
/// </summary>
/// <param name="sizeToAllocate">- The size in bytes of memory to allocate.</param>
/// <param name="memoryAlignment">- The memory alignment for this allocation.</param>
/// <param name="pFileName">- The name of the file this allocation was made in.</param>
/// <param name="lineNum">- The line number of this allocation call.</param>
/// <returns>The allocated memory.</returns>
void* operator new(size_t sizeToAllocate, std::align_val_t memoryAlignment, const char* pFileName, int lineNum);

/// <summary>
/// Global new[] overload, with alignment. Prefer macro EXELIUS_NEW_ARRAY.
/// </summary>
/// <param name="sizeToAllocate">- The size in bytes of memory to allocate.</param>
/// <param name="memoryAlignment">- The memory alignment for this allocation.</param>
/// <param name="pFileName">- The name of the file this allocation was made in.</param>
/// <param name="lineNum">- The line number of this allocation call.</param>
/// <returns>The allocated memory.</returns>
void* operator new[](size_t sizeToAllocate, std::align_val_t memoryAlignment, const char* pFileName, int lineNum);

/// <summary>
/// Global new overload, with alignment. Prefer macro EXELIUS_NEW.
/// </summary>
/// <param name="sizeToAllocate">- The size in bytes of memory to allocate.</param>
/// <param name="memoryAlignment">- The memory alignment for this allocation.</param>
/// <param name="pFileName">- The name of the file this allocation was made in.</param>
/// <param name="lineNum">- The line number of this allocation call.</param>
/// <returns>The allocated memory.</returns>
void* operator new(size_t sizeToAllocate, size_t memoryAlignment, const char* pFileName, int lineNum);

/// <summary>
/// Global new[] overload, with alignment. Prefer macro EXELIUS_NEW_ARRAY.
/// </summary>
/// <param name="sizeToAllocate">- The size in bytes of memory to allocate.</param>
/// <param name="memoryAlignment">- The memory alignment for this allocation.</param>
/// <param name="pFileName">- The name of the file this allocation was made in.</param>
/// <param name="lineNum">- The line number of this allocation call.</param>
/// <returns>The allocated memory.</returns>
void* operator new[](size_t sizeToAllocate, size_t memoryAlignment, const char* pFileName, int lineNum);

/// <summary>
/// Global delete overload. Prefer macro EXELIUS_DELETE.
/// </summary>
/// <param name="pMemoryToFree">- The memory to free.</param>
void operator delete(void* pMemoryToFree);

/// <summary>
/// Global delete[] overload. Prefer macro EXELIUS_DELETE_ARRAY.
/// </summary>
/// <param name="pMemoryToFree">- The memory to free.</param>
void operator delete[](void* pMemoryToFree);

/// <summary>
/// Placement Delete Override. Called by the comiler if stack unwiding on exception
/// is enabled and a constructor throws during the matching operator new call.
/// </summary>
/// <param name="pMemoryToFree">- The memory to free.</param>
/// <param name="pFileName">- The name of the file this allocation was made in.</param>
/// <param name="lineNum">- The line number of this allocation call.</param>
void operator delete(void* pMemoryToFree, const char* pFileName, int lineNum);

/// <summary>
/// Placement Delete Override. Called by the comiler if stack unwiding on exception
/// is enabled and a constructor throws during the matching operator new call.
/// </summary>
/// <param name="pMemoryToFree">- The memory to free.</param>
/// <param name="pFileName">- The name of the file this allocation was made in.</param>
/// <param name="lineNum">- The line number of this allocation call.</param>
void operator delete[](void* pMemoryToFree, const char* pFileName, int lineNum);

/// <summary>
/// Placement Delete Override. Called by the comiler if stack unwiding on exception
/// is enabled and a constructor throws during the matching operator new call.
/// </summary>
/// <param name="pMemoryToFree">- The memory to free.</param>
/// <param name="memoryAlignment">- The memory alignment for this allocation.</param>
/// <param name="pFileName">- The name of the file this allocation was made in.</param>
/// <param name="lineNum">- The line number of this allocation call.</param>
void operator delete(void* pMemoryToFree, std::align_val_t memoryAlignment, const char* pFileName, int lineNum);

/// <summary>
/// Placement Delete Override. Called by the comiler if stack unwiding on exception
/// is enabled and a constructor throws during the matching operator new call.
/// </summary>
/// <param name="pMemoryToFree">- The memory to free.</param>
/// <param name="memoryAlignment">- The memory alignment for this allocation.</param>
/// <param name="pFileName">- The name of the file this allocation was made in.</param>
/// <param name="lineNum">- The line number of this allocation call.</param>
void operator delete[](void* pMemoryToFree, std::align_val_t memoryAlignment, const char* pFileName, int lineNum);

/// <summary>
/// Placement Delete Override. Called by the comiler if stack unwiding on exception
/// is enabled and a constructor throws during the matching operator new call.
/// </summary>
/// <param name="pMemoryToFree">- The memory to free.</param>
/// <param name="memoryAlignment">- The memory alignment for this allocation.</param>
/// <param name="pFileName">- The name of the file this allocation was made in.</param>
/// <param name="lineNum">- The line number of this allocation call.</param>
void operator delete(void* pMemoryToFree, size_t memoryAlignment, const char* pFileName, int lineNum);

/// <summary>
/// Placement Delete Override. Called by the comiler if stack unwiding on exception
/// is enabled and a constructor throws during the matching operator new call.
/// </summary>
/// <param name="pMemoryToFree">- The memory to free.</param>
/// <param name="memoryAlignment">- The memory alignment for this allocation.</param>
/// <param name="pFileName">- The name of the file this allocation was made in.</param>
/// <param name="lineNum">- The line number of this allocation call.</param>
void operator delete[](void* pMemoryToFree, size_t memoryAlignment, const char* pFileName, int lineNum);

#ifdef EXE_DEBUG
	#define EXELIUS_NEW(object) new(__FILE__, __LINE__) object
	#define EXELIUS_NEW_ARRAY(object, size) new(__FILE__, __LINE__) object[size]
	#define EXELIUS_DELETE(objectPointer) delete objectPointer; objectPointer = nullptr
	#define EXELIUS_DELETE_ARRAY(objectPointer) delete[] objectPointer; objectPointer = nullptr
#else // !EXE_DEBUG
	#define EXELIUS_NEW(object) new object
	#define EXELIUS_NEW_ARRAY(object, size) new object[size]
	#define EXELIUS_DELETE(objectPointer) delete objectPointer; objectPointer = nullptr
	#define EXELIUS_DELETE_ARRAY(objectPointer) delete[] objectPointer; objectPointer = nullptr
#endif // EXE_DEBUG