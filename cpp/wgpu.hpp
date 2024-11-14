#pragma once

#include "defs.hpp"

namespace wgpu {

    extern "C" size_t wgpuGetDevice();
    extern "C" void wgpuQueueSubmit();

    struct Queue {
        void submit() { wgpuQueueSubmit(); }
    };

    struct Device {
    Queue queue;
    };

    struct Texture {};

    struct TextureView {
        //TextureSize size;
    };

    namespace SwapChain {
    }

    namespace queue {

    }
}