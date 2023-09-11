#ifndef FILES_H
#define FILES_H

#include "defines.h"

#define ENGINE_RESOURCES_PATH "/home/aimrane-dr/Dev/C/OpenGL_Project/engine/resources/"


typedef struct FileHandler{
    void* hndl;
    /// @brief dynamic array of strings (char*)
    char** content;
    u64 size;
}FileHandler;

typedef enum ProcessOps {
    PROCESS_OP_NO_OP_SPECIFIED,
    PROCESS_OP_CREATE_NOT_EXISTES ,
    PROCESS_OP_READ_MODE_BINARY,
    PROCESS_OP_READ_MODE_CHAR,
    PROCESS_OP_READ_MODE_CHAR_LINES,
    PROCESS_OP_KEEP_FILE_HNDL_OPEN
}ProcessOps;

/// @brief initialize file system, create or getting access to needed directories and folders... 
bool file_sys_init();
bool file_sys_shutdown();

/// @brief Open a file then copy its content then close the file (if no ops is spicified)
/// @param out_handler pointer to the file handler
/// @param file_name path to file
/// @param ops_flags opperations flags desired to be performed
/// @return 
DLL_EXPORT bool file_grab(FileHandler* out_handler, const char* _file_name, ProcessOps ops_flags);

/// @brief forget the file data (lose grip of the file) and return the file handler to its default values
/// @param out_handler pointer to the handler
/// @return 
DLL_EXPORT bool file_ditch(FileHandler* out_handler);

DLL_EXPORT char* file_join_path(char* base, char* subpath);

DLL_EXPORT char* file_get_whole_content(FileHandler* h);
#endif /* FILES_H */