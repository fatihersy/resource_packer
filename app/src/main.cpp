#include "defines.h"
#include "raylib.h"
#include "core/fmemory.h"

#include <vector>

#define MAX_RESOURCE_FILES 256

#define MAX_FILENAME_LENGTH 64
#define MAX_FILENAME_EXT_LENGTH 5

#define HEADER_SYMBOL_BEGIN "__BEGIN__"
#define HEADER_SYMBOL_END "__END__"

#define MAP_FILE_EXTENSION ".txt"
#define MAP_PAK_FOLDER_PATH "map_files/"
#define ASSET_PAK_FOLDER_PATH "asset_files/"


typedef struct main_state {
  i32 stage_count;
	std::vector<std::string> file_buffer;
  std::vector<std::string> file_names;
  std::vector<std::string> asset_file_names;

  std::string asset_pak_data;
  std::string map_pak_data;
  std::string read_buffer;
  std::string header_start;
  std::string header_end;
	size_t asset_pak_data_size;
	size_t map_pak_data_size;
	size_t read_buffer_size;
	size_t header_start_size;
	size_t header_end_size;

  main_state(void) {
    this->stage_count = 0;
    this->file_buffer = std::vector<std::string>();
    this->file_names = std::vector<std::string>();
    this->asset_file_names = std::vector<std::string>();
    this->asset_pak_data = std::string("");
    this->map_pak_data = std::string("");
    this->read_buffer = std::string("");
		this->asset_pak_data_size = 0u;
		this->map_pak_data_size = 0u;
		this->read_buffer_size = 0u;
		this->header_start_size = 0u;
		this->header_end_size = 0u;
  }
} main_state;

static main_state * state = nullptr;

filename_offset_data filename_offset(std::string str);
const char* get_header(file_data* curr_file);
std::string stage_index_to_filename(worldmap_stage_file_content wsf_type, i32 index, i32 layer = 0);

void write_asset_pak(void);
void write_map_pak(void);
const char * append_map_pak_data(worldmap_stage_file_content content_type, i32 location, i32 layer);

const char* file_ext_enum_to_str(file_extension ext);

i32 read_file(const char * path) {
  if (!FileExists(path)) {
    TraceLog(LOG_INFO, "main::write_map_pak()::file '%s' doesn't exist", path);
    return -1;
  }
  i32 loaded_data = 1;
  u8* data = LoadFileData(path, &loaded_data);
  if (loaded_data <= 1 ) {
    TraceLog(LOG_INFO, "main::write_map_pak()::file '%s' doesn't exist", path);
    return -1;
  }
  state->read_buffer.append(data, data + loaded_data);
  UnloadFileData(data);
	return loaded_data;
}

int main(void) 
{
  memory_system_initialize();

  state = (main_state*)allocate_memory_linear(sizeof(main_state), true);
  if (not state or state == nullptr) {
    return EXIT_FAILURE;
  }
  *state = main_state();

  state->asset_file_names.push_back(std::string("aaa_game_start_loading_screen.png"));
  state->asset_file_names.push_back(std::string("abracadabra.ttf"));
  state->asset_file_names.push_back(std::string("miosevka_light.ttf"));
  state->asset_file_names.push_back(std::string("button_click1.wav"));
  state->asset_file_names.push_back(std::string("button_click2.wav"));
  state->asset_file_names.push_back(std::string("button_click3.wav"));
  state->asset_file_names.push_back(std::string("deny.wav"));
  state->asset_file_names.push_back(std::string("main_menu_theme.wav"));
  state->asset_file_names.push_back(std::string("worldmap_wo_clouds.png"));
  state->asset_file_names.push_back(std::string("atlas.png"));

  state->header_start = std::string(HEADER_SYMBOL_BEGIN);
  state->header_end = std::string(HEADER_SYMBOL_END);
	state->header_start_size = TextLength(state->header_start.c_str());
	state->header_end_size = TextLength(state->header_end.c_str());

  write_asset_pak();
  write_map_pak();

  return EXIT_SUCCESS;
}

void write_asset_pak(void) {
    for (size_t itr_000 = 0u; itr_000 < state->asset_file_names.size(); ++itr_000) {
			std::string _path = state->asset_file_names.at(itr_000);

      const char * path = TextFormat("%s%s", RESOURCE_PATH ASSET_PAK_FOLDER_PATH, _path.c_str());
      if (not FileExists(path)) {
        TraceLog(LOG_INFO, "main::write_asset_pak()::file '%s' doesn't exist", _path.c_str());
        continue;
      }
      int loaded_data = 1;
      u8* data = LoadFileData(path, &loaded_data);
			if (loaded_data <= 1) {
        TraceLog(LOG_INFO, "main::write_asset_pak()::file '%s' cannot loaded successfully", _path.c_str());
				return;
			}

      state->asset_pak_data.append(state->header_start);
      state->asset_pak_data.append(data, data + loaded_data);
      state->asset_pak_data.append(state->header_end);

      UnloadFileData(data);
    }

  SaveFileData(ASSET_PAK_FILE_NAME, state->asset_pak_data.data(), state->asset_pak_data.size());
}
void write_map_pak(void) {

  for (size_t itr_000 = 1u; itr_000 <= MAX_WORLDMAP_LOCATIONS; ++itr_000) 
  {
		append_map_pak_data(WSF_CONTENT_COLLISION, itr_000, 0);

		append_map_pak_data(WSF_CONTENT_LAYER, itr_000, 0);
		append_map_pak_data(WSF_CONTENT_LAYER, itr_000, 1);
		append_map_pak_data(WSF_CONTENT_LAYER, itr_000, 2);
		append_map_pak_data(WSF_CONTENT_LAYER, itr_000, 3);
		append_map_pak_data(WSF_CONTENT_LAYER, itr_000, 4);
    
		append_map_pak_data(WSF_CONTENT_PROP, itr_000, 0);
  }

  SaveFileData(MAP_PAK_FILE_NAME, state->map_pak_data.data(), state->map_pak_data.size());
}
const char * append_map_pak_data(worldmap_stage_file_content content_type, i32 location, i32 layer) {
  std::string wsf_filename = stage_index_to_filename(content_type, location, layer);
  const char * path = TextFormat("%s%s", RESOURCE_PATH MAP_PAK_FOLDER_PATH, wsf_filename.c_str());
  read_file(path);
  state->map_pak_data.append(state->header_start);
  state->map_pak_data.append(state->read_buffer);
  state->map_pak_data.append(state->header_end);

	state->file_buffer.push_back(state->read_buffer);
	state->read_buffer.clear();
  SaveFileData("eager.txt", state->map_pak_data.data(), state->map_pak_data.size());
	return state->file_buffer.back().c_str();
}

const char* file_ext_enum_to_str(file_extension ext) {
  if (ext >= FILE_EXT_MAX || ext <= FILE_EXT_UNDEFINED) {
    TraceLog(LOG_WARNING, "main::file_ext_enum_to_str()::File ext is out of bound");
    return nullptr;
  }
  switch (ext) {
    case FILE_EXT_PNG: { 
      return ".png";
    }
    case FILE_EXT_WAV: { 
      return ".wav";
    }
    case FILE_EXT_TTF: { 
      return ".ttf";
    }
    case FILE_EXT_TXT: { 
      return ".txt";
    }
    default: {
      TraceLog(LOG_ERROR, "main::file_ext_enum_to_str()::Unsupported File Extension");
      return nullptr;
    }
  }
  return nullptr;
}

std::string stage_index_to_filename(worldmap_stage_file_content wsf_type, i32 index, i32 layer) {
  switch (wsf_type) {
  case WSF_CONTENT_COLLISION: {
    return std::string(TextFormat("stage%d_collision%s", index, MAP_FILE_EXTENSION));
  }
  case WSF_CONTENT_LAYER: {
    return std::string(TextFormat("stage%d_layer%d%s", index, layer, MAP_FILE_EXTENSION));
  }
  case WSF_CONTENT_PROP: {
    return std::string(TextFormat("stage%d_prop%s", index, MAP_FILE_EXTENSION));
  }
  default: {
    return std::string("");
  }
  }

  return std::string("");
}

