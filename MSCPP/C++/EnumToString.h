#pragma once

#undef ENUM_BEGIN
#undef ENUM
#undef ENUM_END

#define ENUM_BEGIN(typ)	const char* typ##_Names[] = {
#define ENUM(nam) #nam
#define ENUM_END(typ)\
			 };\
			inline const char* typ##_ToString(typ value) { return typ##_Names[value]; }\
			inline bool typ##_FromString(const char* value, typ &o_OutValue) {\
				for(int index=0; index < sizeof(typ##_Names)/ sizeof(typ##_Names[0]); ++index) {\
					if(strcmp(value,typ##_Names[index])==0) {\
						o_OutValue = (typ)index;\
						return true;\
					}\
				}\
				return false; /*Enum value not found*/\
			}