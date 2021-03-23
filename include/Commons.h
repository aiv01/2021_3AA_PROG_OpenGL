#pragma once
#include <stdexcept>

#define DIE_ON_ERROR(VALUE, MSG) if(VALUE == 0){printf(MSG); throw std::runtime_error(MSG);}
#define DIE_ON_NULL(VALUE, MSG) if(VALUE == NULL){printf(MSG); throw std::runtime_error(MSG);}
#define DIE(MSG)  { printf(MSG); throw std::runtime_error(MSG); }