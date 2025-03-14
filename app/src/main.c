#include "defines.h"
#include "core/fmemory.h"

#define FILE_SYMBOL_START_SYMBOL 0x21
#define FILE_SYMBOL_SYMBOL_MOD 127
#define FILE_SYMBOL_SYMBOL_MAX 127

#define FILE_SYMBOL_HEADER_SYMBOL_NEW_ENTRY_SPEC_START "start"
#define FILE_SYMBOL_HEADER_SYMBOL_SIZE_START "filesize"
#define FILE_SYMBOL_HEADER_SYMBOL_FILE_EXT_START "fileext"
#define FILE_SYMBOL_HEADER_SYMBOL_FILENAME_START "filename"
#define FILE_SYMBOL_HEADER_SYMBOL_NEW_ENTRY_SPEC_END "end"
#define FILE_SYMBOL_DATA_START ""
#define FILE_SYMBOL_DATA_END ""

typedef struct main_state {
  file_data file_names[FILE_EXT_MAX][MAX_RESOURCE_FILES];

  u8 pak_data[TOTAL_PAK_FILE_SIZE];
  u32 pak_data_offset;
  u16 total_file_count;
  u64 total_file_size;
} main_state;

static main_state *restrict state;

filename_offset_data filename_offset(const char*);
const char* get_header(file_data* curr_file);
void compress_resource_folder(void);
const char* file_ext_enum_to_str(file_extension ext);
i32 math_pow(i32 x,i32 n);

int main(void) 
{
  memory_system_initialize();
  state = (main_state*)allocate_memory_linear(sizeof(main_state), true);

  compress_resource_folder();

  return 0;
}

void compress_resource_folder(void) {
  for (u16 i=0; i<FILE_EXT_MAX; ++i) {
    const char * curr_ext = file_ext_enum_to_str(i);
    FilePathList pathlist = LoadDirectoryFilesEx(RESOURCE_PATH, curr_ext, false);

    for (u16 j=0; j<pathlist.count; ++j) {
      file_data* curr_file = &state->file_names[i][j];
      filename_offset_data offdata = filename_offset(pathlist.paths[j]);
      copy_memory(curr_file->file_name, pathlist.paths[j] + offdata.filename_offset, offdata.path_length);
      copy_memory(curr_file->file_extension, curr_ext, TextLength(curr_ext));
      for (u16 k=0; k<MAX_FILE_SYMBOL_LENGTH; ++k) {
        u16 symbol = (FILE_SYMBOL_START_SYMBOL + (state->total_file_count%math_pow(FILE_SYMBOL_SYMBOL_MOD, k))) % FILE_SYMBOL_SYMBOL_MAX;
        copy_memory(&curr_file->symbol[k], TextFormat("%c", symbol), TextLength(curr_ext));
      }
      if(MAX_FILE_SYMBOL_LENGTH_GAP) curr_file->symbol[MAX_FILE_SYMBOL_LENGTH_GAP] = '\0';
      const char * path = TextFormat("%s%s", RESOURCE_PATH, curr_file->file_name);
      if (!FileExists(path)) {
        TraceLog(LOG_INFO, "main::compress_resource_folder()::file '%s' doesn't exist", curr_file->file_name);
        continue;
      }
      int loaded_data = 1;
      u8* data = LoadFileData(path, &loaded_data);
      curr_file->size = loaded_data;

      const char* header = TextFormat("%s%s%s%llu%s%s%s%s%s",
        FILE_SYMBOL_HEADER_SYMBOL_NEW_ENTRY_SPEC_START,
        curr_file->symbol,
        FILE_SYMBOL_HEADER_SYMBOL_SIZE_START,
        curr_file->size,
        FILE_SYMBOL_HEADER_SYMBOL_FILE_EXT_START,
        curr_file->file_extension,
        FILE_SYMBOL_HEADER_SYMBOL_FILENAME_START,
        curr_file->file_name,
        FILE_SYMBOL_HEADER_SYMBOL_NEW_ENTRY_SPEC_END
      );
      u32 new_offset = TextLength(header);
      
      copy_memory(state->pak_data + state->pak_data_offset, header, new_offset);
      state->pak_data_offset += new_offset;
      copy_memory(state->pak_data + state->pak_data_offset, data, loaded_data);
      state->pak_data_offset += loaded_data;

      state->total_file_count++;
      state->total_file_size = loaded_data + new_offset;
    }
  }



}

filename_offset_data filename_offset(const char* str) {
  filename_offset_data data = {0};
  u16 counter = 0;
  while (str[counter] != '\0' && counter+1 < MAX_FILENAME_LENGTH) {
    if (str[counter] == '\\') {
      data.filename_offset = counter+1;
    }

    counter++;
  }
  data.path_length = counter;

  if (counter < MAX_FILENAME_LENGTH) {
    data.is_success = true;
  }

  return data;
}
const char* file_ext_enum_to_str(file_extension ext) {
  if (ext >= FILE_EXT_MAX || ext <= FILE_EXT_UNDEFINED) {
    TraceLog(LOG_WARNING, "main::file_ext_enum_to_str()::File ext is out of bound");
    return "";
  }

  switch (ext) {
    case FILE_EXT_PNG: { 
      return ".png";
      break; 
    }
    case FILE_EXT_TXT: { 
      return ".txt";
      break; 
    }
    case FILE_EXT_TTF: { 
      return ".ttf";
      break; 
    }
    default: {
      TraceLog(LOG_ERROR, "main::file_ext_enum_to_str()::Unsupported File Extension");
      return "";
    }
  }
  return "";
}
i32 math_pow(i32 x,i32 n) {
    int i; /* Variable used in loop counter */
    int number = 1;

    for (i = 0; i < n; ++i)
        number *= x;

    return(number);
}
