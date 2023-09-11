#ifndef STRINGS_H
#define STRINGS_H

#include "defines.h"

DLL_EXPORT char* stringFormat(const char* str, ...);
DLL_EXPORT char* stringReplace(const char* str,const char* old_chunck, const char* new_chunck);
DLL_EXPORT void stringCopy(char* dest, const char* src);
DLL_EXPORT char* stringDuplicate(const char* str);
DLL_EXPORT u32 stringLength(const char* str);
DLL_EXPORT char* stringJoin(const char* str1, const char* str2);
DLL_EXPORT bool stringCompare(const char* a,const char* b);
DLL_EXPORT s32 stringHasChar(const char* str,const char c);
DLL_EXPORT char* stringRemove(char* str, const char c);
DLL_EXPORT u32 stringSplit(char* str, char splitter, char** list);
DLL_EXPORT u32 stringWords(const char* str, char*** out);

DLL_EXPORT bool stringFreeCleanDynamicList(char** list, u32 length);
DLL_EXPORT bool stringFree(char** str);

#endif /* STRINGS_H */