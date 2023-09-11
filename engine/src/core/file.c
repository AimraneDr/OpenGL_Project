#include "core/files.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "core/logger.h"
#include "core/CDS/strings.h"

#include "core/CDS/dynamic_array.h"

bool file_sys_init(){
    return true;
}
bool file_sys_shutdown(){
    return true;
}

bool file_grab(FileHandler* out_handler, const char* _file_name, ProcessOps ops_flags){
    out_handler->content = dynamic_array_create(char*);

    //process path
    char* fileName = stringReplace(_file_name,"\\","/");
    FILE* f;
    char* read_m = "rb";
    f = fopen(
        fileName,
        read_m
    );
    if (f == NULL) {
        ERR("Failed to open file [%s]", fileName);
        return false;
    }

    // Determine the file size
    fseek(f, 0, SEEK_END);
    long file_size = ftell(f);
    rewind(f);

    // Allocate a buffer to hold the file content

    if((ops_flags == 0) || (ops_flags & PROCESS_OP_READ_MODE_CHAR)){

        char* buff = (char*)malloc(file_size + 1);
        if (buff == NULL) {
            ERR("Memory allocation error.");
            fclose(f);
            return false;
        }
        // Read the entire file into the buffer
        u64 bytes_read = fread(buff, 1, file_size, f);
        if (bytes_read != file_size) {
            ERR("Error reading file.");
            free(buff);
            fclose(f);
            return false;
        }
        buff[file_size] = '\0';
        dynamic_array_push(out_handler->content,buff);

    }else if (ops_flags & PROCESS_OP_READ_MODE_CHAR_LINES) {
        // Read the file line by line
        char* buff = (char*)malloc(file_size + 1);
        u64 bytes_read = fread(buff, 1, file_size, f);

        u32 offset = 0;
        u32 chunk_size = 0;
        for(u32 i=0; i<file_size / sizeof(char); i++){
            if(buff[i] == '\n' || buff[i] == '\0' || i == file_size / sizeof(char) - 1){
                //if it is the last char include it
                if(i == file_size / sizeof(char) - 1 && buff[i] != '\n' && buff[i] != '\0') chunk_size++;

                char* chunk_buf = (char*)malloc(chunk_size * sizeof(char));
                memcpy(chunk_buf, buff + offset, chunk_size);
                chunk_buf[chunk_size] = '\0';
                dynamic_array_push(out_handler->content, chunk_buf);
                if(buff[i] == '\0' || i == file_size / sizeof(char) - 1) break;
                offset+=chunk_size+1;
                chunk_size = 0;
            }else{
                chunk_size++;
            }
        }
        // char* line = NULL;
        // u32 line_len = 0;

        // while (fgets(line, line_len, f) != NULL) {
        //     DEBUG("line detected")
        //     line_len = 255;
        //     line = (char*)malloc(line_len * sizeof(char));
        //     dynamic_array_push(out_handler->content, line);
        // }

        // free(line);
    }

    // Null-terminate the buffer

    out_handler->hndl = f;
    out_handler->size = file_size;

    fclose(f);
    return true;
}

bool file_ditch(FileHandler* out_handler){
    dynamic_array_destroy(out_handler->content)
    out_handler->content = (void*)0;
    out_handler->hndl = (void*)0;
    out_handler->size = 0;
    return true;
}

char* file_join_path(char* base, char* subpath){
    char* result;
    char* _base = stringReplace(base, "\\", "/");
    char* _subpath = stringReplace(subpath,"\\","/");
    if(_base[stringLength(_base) - 1] != '/') _base = stringJoin(_base,"/");
    result = stringJoin(_base,_subpath);
    return result;
}

char* file_get_whole_content(FileHandler* h){
    u32 len = dynamic_array_length(h->content);
    char* splitter = " ";
    if(len > 1){
        u64 buffer_size = 0;
        for(u32 i=0; i < len; i++){
            buffer_size += stringLength(h->content[i]);
        }
        DEBUG("Buffer Size : %d", buffer_size);
        char* buffer = (char*)malloc(buffer_size+len);
        u32 offset = 0;
        for(u32 i=0; i<len; i++){
            u32 str_len = stringLength(h->content[i]);
            memcpy(buffer + offset, h->content[i],str_len);
            offset+=str_len;
            memcpy(buffer + offset, splitter,1);
            offset+=1;
        }
        return buffer;
    }else if(len == 1){
        return h->content[0];
    }else{
        return NULL;
    }
}