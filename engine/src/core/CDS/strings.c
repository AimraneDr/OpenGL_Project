#include "core/CDS/strings.h"

#include "core/logger.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include <string.h>

#include "core/CDS/dynamic_array.h"

char* stringFormat(const char* format, ...)
{
    __builtin_va_list args;
    __builtin_va_start(args, format);

    // Determine the size of the formatted string
    __builtin_va_list args_copy;
    __builtin_va_copy(args_copy, args);
    u32 size = vsnprintf(NULL, 0, format, args_copy);
    __builtin_va_end(args_copy);

    // Allocate memory for the formatted string
    char* buffer = (char*)malloc((size + 1) * sizeof(char));

    // Format the string
    vsprintf(buffer, format, args);

    __builtin_va_end(args);

    return buffer;
}





char* stringReplace(const char* str, const char* old_chunk, const char* new_chunk) {
    // Calculate the lengths of the input strings
    u32 old_len = strlen(old_chunk);
    u32 new_len = strlen(new_chunk);
    u32 str_len = strlen(str);

    // Count the number of occurrences of the old_chunk in the string
    u32 count = 0;
    const char* ptr = str;
    while ((ptr = strstr(ptr, old_chunk)) != NULL) {
        count++;
        ptr += old_len;
    }

    // Calculate the length of the final string after replacement
    u32 result_len = str_len + (new_len - old_len) * count;

    // Allocate memory for the result string
    char* result = (char*)malloc((result_len + 1) * sizeof(char));
    if (result == NULL) {
        ERR("string.c -> stringReplace() : Memory allocation failed.");
        return NULL;
    }

    // Iterate through the input string, replacing old_chunk with new_chunk
    const char* src = str;
    char* dest = result;
    while (count > 0) {
        ptr = strstr(src, old_chunk);
        if (ptr == NULL) {
            break;
        }

        // Copy characters before the old_chunk
        size_t len = ptr - src;
        strncpy(dest, src, len);
        dest += len;

        // Copy the new_chunk
        strcpy(dest, new_chunk);
        dest += new_len;

        // Move the source pointer after the old_chunk
        src = ptr + old_len;

        count--;
    }

    // Copy any remaining characters after the last occurrence of old_chunk
    strcpy(dest, src);

    return result;
}


void stringCopy(char* dest, const char* src){
    char* destPtr = dest;
    const char* srcPtr = src;

    while (*srcPtr)
    {
        *destPtr = *srcPtr;
        destPtr++;
        srcPtr++;
    }

    *destPtr = '\0';
}

char* stringDuplicate(const char* str) {
    size_t length = strlen(str);
    char* duplicate = malloc(length + 1);
    if (duplicate != NULL) {
        memcpy(duplicate, str, length);
        duplicate[length] = '\0';
    }
    return duplicate;
}

u32 stringLength(const char* str)
{
    const char* charPtr = str;
    while (*charPtr)
    {
        charPtr++;
    }
    return charPtr - str;
}

char* stringJoin(const char* a, const char* b){
    u32 alen = stringLength(a);
    u32 blen = stringLength(b);
    char* result = (char*)malloc((alen + blen + 1) * sizeof(char));

    char* a_offset = (char*)a;
    char* b_offset = (char*)b;
    char* r_offset = result;
    if (result == NULL) ERR("Memory allocation failed.\n");
    for(u32 i=0; i < alen+blen; i++){
        if(i<alen){
            *r_offset = *a_offset;
            r_offset++;
            a_offset++;
        }else{
            *r_offset = *b_offset;
            r_offset++;
            b_offset++;
        }
    }
    *r_offset = '\0';
    return result;
}

bool stringCompare(const char* a,const char* b){
    u32 alen = stringLength(a);
    u32 blen = stringLength(b);

    if(alen != blen) return false;

    char* a_offset = (char*)a;
    char* b_offset = (char*)b;
    for(u32 i=0; i<alen;i++){
        if(*a_offset != *b_offset) return false;

        a_offset++;
        b_offset++;
    }
    return true;
}

s32 stringHasChar(const char* str,const char c){
    const char* charPtr = str;
    while (*charPtr)
    {
        if(*charPtr == c)
            return charPtr - str;
        charPtr++;
    }
    return -1;
}

char* stringRemove(char* str, const char c) {
    if (str == NULL)
        return NULL;

    char* read_ptr = str;
    char* write_ptr = str;

    while (*read_ptr) {
        if (*read_ptr != c) {
            *write_ptr = *read_ptr;
            write_ptr++;
        }
        read_ptr++;
    }

    *write_ptr = '\0';  // Add null terminator at the end

    return str;
}

/// @brief this function do nothing for the time being
/// @param str string strem to be splitted
/// @param splitter 
/// @return synamic array containing all chuncks
u32 stringSplit(char* str, char splitter, char** list) {
    // Determine the length of the input string
    u32 strLength = stringLength(str);
    if (strLength == 0)
        return 0;

    // Count the number of tokens in the string
    u32 tokenCount = 0;
    char* token = strtok(str, &splitter);
    while (token != NULL) {
        tokenCount++;
        token = strtok(NULL, &splitter);
    }

    // Allocate memory for the token array
    list = (char**)malloc((tokenCount + 1) * sizeof(char*));
    if (list == NULL) {
        ERR("Memory allocation failed.")
        fprintf(stderr, "Memory allocation failed.\n");
        return 0;
    }

    // Split the string into tokens
    tokenCount = 0;
    token = strtok(str, &splitter);
    while (token != NULL) {
        u32 tokenLen = stringLength(token);
        char* t = (char*)malloc((tokenLen + 1) * sizeof(char));
        if (t == NULL) {
            ERR("Memory allocation failed.")
            fprintf(stderr, "Memory allocation failed.\n");
            // Free previously allocated memory
            for (u32 i = 0; i < tokenCount; i++) {
                free(list[i]);
            }
            free(list);
            return 0;
        }
        strcpy(t, token);
        list[tokenCount] = t;
        tokenCount++;
        token = strtok(NULL, &splitter);
    }

    list[tokenCount] = NULL;  // Null-terminate the token array

    return tokenCount;
}

u32 stringWords(const char* str, char*** out) {
    // DEBUG("string passed : %s", str)
    if (str == NULL)
        return 0;

    int length = strlen(str);

    // Count the number of words in the string
    int wordCount = 0;
    bool inWord = false;

    for (int i = 0; i < length; i++) {
        if ((str[i] == ' ' || str[i] == '\t' || str[i] == '\n') && inWord) {
            inWord = false;
        }
        else if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n') {
            if (!inWord) {
                wordCount++;
                inWord = true;
            }
        }
    }

    if (wordCount == 0)
        return 0;

    // Allocate memory for the word array
    char** words = (char**)malloc((wordCount + 1) * sizeof(char*));
    if (words == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return 0;
    }

    // Extract the words from the string
    int wordIndex = 0;
    const char* wordStart = NULL;
    bool inWord2 = false;

    for (int i = 0; i <= length; i++) {
        if ((str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || str[i] == '\0') && inWord2) {
            int wordLength = &str[i] - wordStart;
            char* word = (char*)malloc((wordLength + 1) * sizeof(char));
            if (word == NULL) {
                ERR("failed to alloc memory | string.c/stringWords/word")
                // Free previously allocated memory
                for (int j = 0; j < wordIndex; j++) {
                    free(words[j]);
                }
                free(words);
                return 0;
            }
            strncpy(word, wordStart, wordLength);
            word[wordLength] = '\0';
            words[wordIndex] = word;
            wordIndex++;
            inWord2 = false;
        }
        else if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n') {
            if (!inWord2) {
                wordStart = &str[i];
                inWord2 = true;
            }
        }
    }

    words[wordCount] = NULL;  // Null-terminate the word array

    // Assign the word array to the output parameter
    *out = words;

    return wordCount;
}


bool stringFreeCleanDynamicList(char** list, u32 length) {
    DEBUG("freeing...")
    for (u32 i = 0; i < length; i++) {
        free(list[i]);
    }
    DEBUG("freed")
    return true;
}

bool stringFree(char** str){
    free(*str);
    return true;
}