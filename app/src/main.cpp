#include <stdexcept>

#include "defines.h"
#include "raylib.h"
#include "core/fmemory.h"

#define MAX_RESOURCE_FILES 256

#define MAX_FILENAME_LENGTH 64
#define MAX_FILENAME_EXT_LENGTH 5

#define HEADER_SYMBOL_BEGIN "__BEGIN__"
#define HEADER_SYMBOL_END "__END__"

#define MAP_FILE_EXTENSION ".txt"
#define MAP_PAK_FOLDER_PATH "map_layers/"
#define ASSET_PAK_FOLDER_PATH "asset_files/"

typedef struct write_pak_result {
  std::string file_name;
  u64 size;
  bool is_success;
  write_pak_result() {
    this->file_name = std::string();
    this->size = 0u;
    this->is_success = false;
  }
  write_pak_result(bool _success, const char * _name = "", u64 _size = 0u) : write_pak_result() {
    this->is_success = _success;
    this->file_name = _name;
    this->size = _size;
  }
} write_pak_result;

typedef struct main_state {
  i32 stage_count;
	std::vector<std::string> file_buffer;
  std::vector<asset_pak_folder> folder_to_pak;
  
  std::string map_pak_data;
  std::string read_buffer;
  std::string header_start;
  std::string header_end;
	size_t map_pak_data_size;
	size_t read_buffer_size;
	size_t header_start_size;
	size_t header_end_size;

  main_state(void) {
    this->stage_count = 0;
    this->file_buffer = std::vector<std::string>();
    this->folder_to_pak = std::vector<asset_pak_folder>();
    this->map_pak_data = std::string("");
    this->read_buffer = std::string("");
    this->header_start = std::string("");
    this->header_end = std::string("");
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

write_pak_result write_asset_pak(asset_pak_folder* asset_folder);
write_pak_result write_map_pak(void);
const char * append_map_pak_data(worldmap_stage_file_content content_type, i32 location, i32 layer);

const char* file_ext_enum_to_str(file_extension ext);

i32 read_file(const char * path) {
  if (not FileExists(path)) {
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

  state->folder_to_pak.push_back(asset_pak_folder("asset1.pak", "asset1_files/",
    std::vector<std::string>({
      "aaa_game_start_loading_screen.png",
      "worldmap_wo_clouds.png",
      "black_background_1.png",
      "black_background_2.png",
      "zap.png",
      "mood.ttf",
      "miosevka_italic.ttf",
      "miosevka_light.ttf",
      "miosevka_regular.ttf",
      "miosevka_bold.ttf",
      "coin_pickup.mp3",
      "health_pickup.mp3",
      "level_up.mp3",
      "button_click1.wav",
      "button_click2.wav",
      "button_click3.wav",
      "deny1.wav",
      "deny2.wav",
      "exp_pickup.wav",
      "game_over.wav",
      "zap1.wav",
      "zap2.wav",
      "zap3.wav",
      "zap4.wav",
      "zombie_die1.wav",
      "zombie_die2.wav",
      "zombie_die3.wav",
      "mainmenu_theme1.wav",
      "mainmenu_theme2.wav",
      "mainmenu_theme3.wav",
      "map_selection1.wav",
      "map_selection2.wav",
      "ingame_play_theme1.ogg",
      "ingame_play_theme2.ogg",
      "ingame_play_theme3.ogg",
      "ingame_play_theme4.ogg",
      "spin_result_star_105x105.png",
      "spin_sfx.wav",
      "spin_result.wav",
      "entrance1_160x280.png",
      "entrance2_160x280.png",
      "entrance3_160x280.png",
      "entrance4_160x280.png",
      "entrance5_160x280.png",
      "entrance6_160x224.png",
      "entrance7_160x280.png",
      "entrance8_160x280.png",
      "entrance9_160x280.png",
      "sigils/ability_cd_clear-modified.png",
      "sigils/aoe_clear-modified.png",
      "sigils/basic_attack_damage_clear-modified.png",
      "sigils/basic_attack_speed_clear-modified.png",
      "sigils/condition_duration_clear-modified.png",
      "sigils/critical_chance_clear-modified.png",
      "sigils/critical_damage_clear-modified.png",
      "sigils/damage_deferral_clear-modified.png",
      "sigils/damage_over_time_clear-modified.png",
      "sigils/damage_reduction_clear-modified.png",
      "sigils/drop_rate_clear-modified.png",
      "sigils/exp_gain_clear-modified.png",
      "sigils/health_sigil_clear-modified.png",
      "sigils/hp_regen_clear-modified.png",
      "sigils/letal_sigil_effectiveness_clear-modified.png",
      "sigils/mana_clear-modified.png",
      "sigils/mana_regen_clear-modified.png",
      "sigils/move_speed_clear-modified.png",
      "sigils/overall_damage_clear-modified.png",
      "sigils/overall_luck_clear-modified.png",
      "sigils/projectile_amouth_clear-modified.png",
      "sigils/reward_modifier_clear-modified.png",
      "sigils/sigil_effectiveness_clear-modified.png",
      "sigils/stamina_regen_clear-modified.png",
      "sigils/stamina_sigil_clear-modified.png",
      "sigils/total_trait_points_clear-modified.png",
      "sigils/vital_sigil_effectiveness_clear-modified.png"
    })
  ));

  state->folder_to_pak.push_back(asset_pak_folder("asset2.pak", "asset2_files/",
    std::vector<std::string>({
      "0._loc_data",
      "1._loc_data",
      "atlas.png",
      "fade_transition.fs",
      "font_outline.fs",
      "map_choice_image.fs",
      "post_process.fs",
      "prg_bar_mask.fs",
      "sdr_spawn.fs",
      "sdf_text.fs",
      "sdr_spin.fs"
    })
  ));

  state->header_start = std::string(HEADER_SYMBOL_BEGIN);
  state->header_end = std::string(HEADER_SYMBOL_END);
	state->header_start_size = TextLength(state->header_start.c_str());
	state->header_end_size = TextLength(state->header_end.c_str());

  write_pak_result pak1_result = write_asset_pak(&state->folder_to_pak.at(0));
  if (not pak1_result.is_success) {
    throw std::runtime_error("write_asset_pak for asset1 return with failure");
  }
  write_pak_result pak2_result = write_asset_pak(&state->folder_to_pak.at(1));
  if (not pak2_result.is_success) {
    throw std::runtime_error("write_asset_pak for asset2 return with failure");
  }

  write_pak_result map_result = write_map_pak();

  
  TraceLog(LOG_INFO, "%s is written with %llu bytes", pak1_result.file_name.c_str(), pak1_result.size);
  TraceLog(LOG_INFO, "%s is written with %llu bytes", pak2_result.file_name.c_str(), pak2_result.size);
  TraceLog(LOG_INFO, "%s is written with %llu bytes", map_result.file_name.c_str(),  map_result.size);

  return EXIT_SUCCESS;
}

write_pak_result write_asset_pak(asset_pak_folder* asset_folder) {
  if (not asset_folder or asset_folder == nullptr) {
    return write_pak_result(false);
  }

  for (size_t itr_000 = 0u; itr_000 < asset_folder->file_names.size(); ++itr_000) {
		std::string _path = asset_folder->file_names.at(itr_000);
    const char * path = TextFormat("%s%s%s", RESOURCE_PATH, asset_folder->path_to_resource.c_str(), _path.c_str());
    if (not FileExists(path)) {
      TraceLog(LOG_INFO, "main::write_asset_pak()::file '%s' doesn't exist", _path.c_str());
      continue;
    }
    int loaded_data = 1;
    u8* data = LoadFileData(path, &loaded_data);
		if (loaded_data <= 1) {
      TraceLog(LOG_INFO, "main::write_asset_pak()::file '%s' cannot loaded successfully", _path.c_str());
			write_pak_result(false);
		}
    size_t size_cache = asset_folder->pak_data.size();

    asset_folder->pak_data.append(state->header_start);
    asset_folder->pak_data.append(data, data + loaded_data);
    asset_folder->pak_data.append(state->header_end);

    std::string data_begin = asset_folder->pak_data.substr(size_cache, 16);
    std::string data_end = asset_folder->pak_data.substr(asset_folder->pak_data.size() - 16, 16);

    UnloadFileData(data);
  }

  SaveFileData(asset_folder->pak_file_name.c_str(), asset_folder->pak_data.data(), asset_folder->pak_data.size());
  return write_pak_result(true, asset_folder->pak_file_name.c_str(), asset_folder->pak_data.size());
}
write_pak_result write_map_pak(void) {
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
  return write_pak_result(true, MAP_PAK_FILE_NAME, state->map_pak_data.size());
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
	return state->file_buffer.back().c_str();
}

const char* file_ext_enum_to_str(file_extension ext) {
  if (ext >= FILE_EXT_MAX or ext <= FILE_EXT_UNDEFINED) {
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
    case FILE_EXT_LOC_DATA: { 
      return "._loc_data";
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

