// #include "cube.hpp"
#include "renderer.h"

extern "C" void request_animation_frame(void *);
extern "C" size_t getCheckboxValue(JsString);

const int num_civs = 45;

bool assetsLoaded = false;

const char *civilization_names[num_civs] = {
    "Armenians", "Azteks",      "Bengalis",    "Berbers",    "Bohemians",
    "Britons",   "Bulgarians",  "Burgundiars", "Burmese",    "Byzantines",
    "Celts",     "Chinese",     "Cumans",      "Dravidians", "Ethiopians",
    "Franks",    "Georgians",   "Goths",       "Gurjaras",   "Hindustanis",
    "Huns",      "Incas",       "Italians",    "Japanese",   "Khmer",
    "Koreans",   "Lithuanians", "Magyars",     "Malay",      "Malians",
    "Mayans",    "Mongols",     "Persians",    "Poles",      "Portuguese",
    "Romans",    "Saracens",    "Sicilians",   "Slavs",      "Spanish",
    "Tatars",    "Teutons",     "Turks",       "Vietnamese", "Vikings"};

unsigned char ids[num_civs] = {5,  9,  10, 11, 15, 17, 23, 31, 32, 36, 41, 42,
                               44, 1,  20, 25, 30, 39, 21, 22, 27, 38, 3,  14,
                               29, 34, 8,  24, 28, 43, 6,  12, 26, 40, 7,  37,
                               4,  33, 2,  13, 18, 19, 35, 0,  16};

enum class Region : uint8_t {
  AMERICA = 0,
  AFRICA,
  EUROPE,
  ASIA,
};

enum class CivFlags : uint32_t {
  NONE = 0,
  HAS_ELEPHANTS = 1 << 1,
  HAS_CANNON_GALLEON = 1 << 2,
};

CivFlags operator&(CivFlags a, CivFlags b){
	return (CivFlags)((uint32_t)a & (uint32_t)b);
}

CivFlags operator|=(CivFlags &a, CivFlags b) {
	return a = (CivFlags)((uint32_t)a | (uint32_t)b), a;
}

CivFlags operator^=(CivFlags &a, CivFlags b) {
	return a = (CivFlags)((uint32_t)a ^ (uint32_t)b), a;	
}


struct Civilization {
  uint8_t id = 0;
  const char *name;
  CivFlags flags = CivFlags::NONE;
  Region region;
  Math::vec2 civ_uv;
};

Civilization civilizations[num_civs] = {0};

void InitCivilizations() {

  for (uint8_t i = 0; i < num_civs; i++) {
    civilizations[i] = {.id = i,
                        .name = civilization_names[ids[i]],
                        .flags = CivFlags::HAS_CANNON_GALLEON,
                        .region = Region::EUROPE};
  }

  civilizations[13].region = Region::AMERICA;
  civilizations[16].region = Region::AMERICA;
  civilizations[18].region = Region::AMERICA;
  
  civilizations[22].region = Region::AFRICA;
  civilizations[23].region = Region::AFRICA;
  civilizations[24].region = Region::AFRICA;
  
  civilizations[3].region = Region::ASIA;
  civilizations[6].region = Region::ASIA;
  civilizations[7].region = Region::ASIA;
  civilizations[8].region = Region::ASIA;
  civilizations[9].region = Region::ASIA;
  civilizations[11].region = Region::ASIA;
  civilizations[15].region = Region::ASIA;
  civilizations[27].region = Region::ASIA;
  civilizations[28].region = Region::ASIA;
  civilizations[29].region = Region::ASIA;
  civilizations[31].region = Region::ASIA;
  civilizations[33].region = Region::ASIA;
  civilizations[38].region = Region::ASIA;
  civilizations[39].region = Region::ASIA;
  civilizations[40].region = Region::ASIA;
  civilizations[41].region = Region::ASIA;
  civilizations[43].region = Region::ASIA;
  civilizations[44].region = Region::ASIA;
  
  civilizations[8].flags |= CivFlags::HAS_ELEPHANTS;
  civilizations[41].flags |= CivFlags::HAS_ELEPHANTS;
  civilizations[26].flags |= CivFlags::HAS_ELEPHANTS;
  civilizations[27].flags |= CivFlags::HAS_ELEPHANTS;
  civilizations[28].flags |= CivFlags::HAS_ELEPHANTS;
  civilizations[29].flags |= CivFlags::HAS_ELEPHANTS;
  civilizations[38].flags |= CivFlags::HAS_ELEPHANTS;
  civilizations[39].flags |= CivFlags::HAS_ELEPHANTS;
  civilizations[40].flags |= CivFlags::HAS_ELEPHANTS;


  civilizations[16].flags ^= CivFlags::HAS_CANNON_GALLEON;
  civilizations[18].flags ^= CivFlags::HAS_CANNON_GALLEON;
  civilizations[13].flags ^= CivFlags::HAS_CANNON_GALLEON;
  civilizations[31].flags ^= CivFlags::HAS_CANNON_GALLEON;
  civilizations[14].flags ^= CivFlags::HAS_CANNON_GALLEON;
}
  struct Card {
  float speed = 0.0;
  float angle;
  Object renderObject;
  const Civilization *civilization = nullptr;
  bool done = false;
  int spins = 100;
};

uint8_t num_cards = 3;
Card cards[5] = {
    {.angle = 1.0f + 0.5, .civilization = &civilizations[0], .spins = 50},
    {.angle = 1.0f + 0.1, .civilization = &civilizations[1], .spins = 75},
    {.angle = 1.0f + 0.9, .civilization = &civilizations[2], .spins = 100},
    {.angle = 1.0f + 0.8, .civilization = &civilizations[3], .spins = 250},
    {.angle = 1.0f + 0.4, .civilization = &civilizations[4], .spins = 300}};

const int grid_size = 9;
const float item_width = 1.0f / (float)grid_size;
const float item_height = 1.0f / (float)grid_size;

Math::vec2 getCivCoord(unsigned id) {
  return {((float)(id % grid_size)) * item_width,
          ((float)(id / grid_size)) * item_height};
}


bool should_run = false;

struct Filters_t {
	bool no_elephants = false;
	bool no_cannons = false;
	bool asians = true;
	bool africans = true;
	bool americans = true;
	bool europeans = true;
} Filters;


bool Filter(int civ) {

  using CivFlags::HAS_ELEPHANTS;
  using CivFlags::HAS_CANNON_GALLEON;
  if(Filters.no_elephants && 0 != (uint32_t)(civilizations[civ].flags & HAS_ELEPHANTS)) return false;
  if(Filters.no_cannons && 0 != (uint32_t)(civilizations[civ].flags & HAS_CANNON_GALLEON)) return false;


  return true;
}

extern "C" void onClick_reset() {
  cards[0].done = false;
  cards[1].done = false;
  cards[2].done = false;
  cards[0].spins = 50;
  cards[1].spins = 75;
  cards[2].spins = 100;

  cards[0].speed = 0;
  cards[1].speed = 0;
  cards[2].speed = 0;
  should_run = false;
}

int available_civs[45] = {};
int available_civs_count = 0;

void InitCivArray() {
  available_civs_count = 0;

  for(int i = 0; i < 45; i++) {
	  bool passes_filter = Filter(i);
    if(civilizations[i].region == Region::ASIA && Filters.asians && passes_filter) {
      available_civs[available_civs_count++] = i;
      continue;
    }
    if(civilizations[i].region == Region::AFRICA && Filters.africans && passes_filter) {
      available_civs[available_civs_count++] = i;
      continue;
    }
    if(civilizations[i].region == Region::AMERICA && Filters.americans && passes_filter) {
      available_civs[available_civs_count++] = i;
      continue;
    }
    if(civilizations[i].region == Region::EUROPE && Filters.europeans && passes_filter) {
      available_civs[available_civs_count++] = i;
      continue;
    }
  }
}

int randomCiv() { 
int id = (int)Math::floor(Math::rand() * available_civs_count);
	std::printf("%:%", id, available_civs[id]);
	return available_civs[id]; }
extern "C" void onClick_spin() { 
  onClick_reset();
  Filters.no_elephants = (bool)getCheckboxValue("elephants");
  Filters.no_cannons = (bool)getCheckboxValue("cannons");
  Filters.asians = (bool)getCheckboxValue("input-asia");
  Filters.americans = (bool)getCheckboxValue("input-america");
  Filters.africans = (bool)getCheckboxValue("input-africa");
  Filters.europeans = (bool)getCheckboxValue("input-europe");
  
  if(!Filters.asians && !Filters.americans && !Filters.africans && !Filters.europeans) return;

  InitCivArray();

  std::printf("Available civs: %", available_civs_count);
  std::printf("no elephants: %", (size_t)Filters.no_elephants);
  if(available_civs_count >= 3)
  	should_run = true; 
  else {

    for(int i = 0; i < 45; i++) available_civs[i] = i;
    available_civs_count = 45;
  }
}


void updateCard(int index, float dt) {
  dt = Math::clamp(dt, 0.016f, 0.032f);
  float &angle = cards[index].angle;
  float &speed = cards[index].speed;
  int &spins = cards[index].spins;

  if (cards[index].done)
    return;

  angle = angle + dt * speed;
  if (!should_run) {
    if (speed <= 0.01) {
      speed = (Math::rand() * 2.0) * Math::two_pi;
      // angle = 1.0f + Math::rand();
    }

    speed = Math::mix(speed, 0.0f, dt);
  }
  if (should_run) {
    if (spins > 3) {
      speed = 5.0;

    } else {
      speed = Math::mix(speed, 0.0f, dt * 1.25f);
    }
  }
  if (spins < 1 && angle >= 0.5) {
    cards[index].done = true;
    std::printf("% done with: %", index, cards[index].civilization->id);
   
  }

  if (angle >= 1.0) {
    if(should_run)
    spins--;

    int civ = 0;
    angle = 0.0;
    for(;;) {

      civ = randomCiv();

      bool valid = true;

      if(should_run) valid = Filter(civ);

      if(!valid) continue;
      
      if(index == 0 && cards[index].civilization->id != civ) break;

      if(index == 1) {
        if(cards[0].civilization->id == civ) continue;
	break;
      }

      if(index == 2) {
        if(cards[1].civilization->id != civ && cards[0].civilization->id != civ){
		if(available_civs_count < 4) break;
		std::printf("break: % % %", cards[0].civilization->id, cards[1].civilization->id, civ);
		if(cards[index].civilization->id != civ)
			break; 
	}
      }

      //if(civ != cards[index].civilization->id && valid && available_civs_count > 3) break;

    }


    cards[index].civilization = &civilizations[civ];

    char div_name[10];
    std::sprintf(div_name, "civ-%", index + 1);

    std::SetElementText(div_name, "%", cards[index].civilization->name);
  }
}

extern "C" void render_loop(float dt) {

  if (!assetsLoaded)
    return;
  time += dt;

  using Math::mat4;
  using Math::vec2;
  using Math::vec3;

  uint32_t width = getWindowWidth();
  uint32_t height = getWindowHeight();

  mat4 view = Math::lookAt(vec3(0.0f, 0.0f, 6.0f), vec3(0.0f, 0.0, 0.0f),
                           vec3(0.0f, 1.0f, 0.0f));

  mat4 proj = Math::perspective(Math::radians(30.0f), width / (height + 0.001),
                                0.1f, 100.0f);

  auto pass = RenderBegin();
  float x = -1.5;
  for (int i = 0; i < num_cards; i++) {
    Card *card = &cards[i];
    updateCard(i, dt);
    mat4 r = Math::rotate(Math::pi * card->angle, vec3(1.0, 0.0, 0.0));
    mat4 t = Math::translate(vec3(x, 0.2, 0.0));
    mat4 s = Math::scale(vec3(0.85f));
    mat4 transform = proj * view * t * r * s;
    card->renderObject.transform = transform;
    card->renderObject.bindGroup = bindGroup[i];
    x += 1.5;

    Draw(pass, {.transform = cards[i].renderObject.transform,
                .uv = getCivCoord(cards[i].civilization->id),
                .renderObject = cards[i].renderObject});
  }
  RenderEnd(pass);
};

void BufferCopy(Buffer buffer, void *buffer_data, size_t offset, size_t size) {
  void *data = BufferGetMappedRange(buffer, offset, size);
  std::memcpy(data, buffer_data, size);
  BufferUnmap(buffer);
}

int main(int argc, char **argv) {

  for(int i = 0; i < 45; i++) available_civs[i] = i;
  available_civs_count = 45;

  if (state.device = GetDevice(); state.device == 0) {
    puts("Error opening device.");
    return 1;
  }

  queue = DeviceGetQueue(state.device);

  ShaderModule fragmentShader = CreateShaderModule(state.device, fragmentCode);
  ShaderModule vertexShader = CreateShaderModule(state.device, vertexCode);

  engine::Quad cube;
  size_t indexCount = cube.data().num_indices;
  size_t cube_vertex_size = cube.data().num_vertices * sizeof(Math::Vertex);
  size_t cube_index_size = indexCount * sizeof(uint32_t);

  vtxBuffer =
      CreateBuffer(state.device, {.size = cube_vertex_size,
                                  .usage = (BufferUsage)(BufferUsage::VERTEX),
                                  .mappedAtCreation = true});
  BufferCopy(vtxBuffer, (void *)cube.data().Vertices, 0, cube_vertex_size);

  idxBuffer =
      CreateBuffer(state.device, {.size = cube_index_size,
                                  .usage = (BufferUsage)(BufferUsage::INDEX),
                                  .mappedAtCreation = true});
  BufferCopy(idxBuffer, (void *)cube.data().Indices, 0, cube_index_size);

  uniformBuffer = CreateBuffer(
      state.device,
      {.size = 256 + 256 + 80 * 3,
       .usage = (BufferUsage)(BufferUsage::UNIFORM | BufferUsage::COPY_DST),
       .mappedAtCreation = false});

  InitCivilizations();
  for (int i = 0; i < num_cards; i++) {
    cards[i].renderObject.vtxBuffer = vtxBuffer;
    cards[i].renderObject.idxBuffer = idxBuffer;
    cards[i].renderObject.uniformBuffer = uniformBuffer;
    cards[i].renderObject.uniformBuffer = uniformBuffer;
    cards[i].renderObject.vtx_count = cube.data().num_vertices;
    cards[i].renderObject.idx_count = indexCount;
    cards[i].renderObject.uniform_offset = i * 256;
  }

  cubeTexture =
      CreateTexture(state.device, {
                                      .size = {4096, 4096, 0},
                                      .dimension = TextureDimension::d2D,
                                      .format = TextureFormat::Rgba8Unorm,
                                      .usage = TextureUsage::RENDER_ATTACHMENT |
                                               TextureUsage::COPY_DST |
                                               TextureUsage::TEXTURE_BINDING,
                                  });

  fetch("../../media/aoe2/civs.png").then([](Response response) {
    response.blob().then([](Blob blob) {
      createImageBitmap(blob.handle).then([](Image image) {
        QueueCopyExternalImageToTexture(queue, (void *)image.handle,
                                        cubeTexture, {4096, 4096, 0});
        assetsLoaded = true;
      });
    });
  });

  for(int i = 0; i < 45; i++)
    std::printf("%: %", civilizations[i].name, (uint32_t)civilizations[i].flags & (uint32_t)CivFlags::HAS_ELEPHANTS);


  build_pipeline();

#ifdef __wasm__
  request_animation_frame((void *)render_loop);
#endif

#ifndef __wasm__
  DestroyBuffer(vtxBuffer);
#endif
}
