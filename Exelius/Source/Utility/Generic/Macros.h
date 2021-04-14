#pragma once
#include <assert.h>

#define BIT(x) (1 << x)

#define SAFE_DELETE(_ptr_) delete _ptr_; _ptr_ = nullptr
#define SAFE_DELETE_ARRAY(_ptr_) delete[] _ptr_; _ptr_ = nullptr

#define EXE_ASSERT(_arg_) assert(_arg_)