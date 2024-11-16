"use strict"

function getString(offset) {
    const buffer = WasmContext["buffer"];
    const str_buffer = new Uint32Array(buffer, offset, 2);
    return new TextDecoder("utf8").decode(new Uint8Array(buffer, str_buffer[0], str_buffer[1]));
}

function wasm_print(heap_base) {
    const str = getString(heap_base);
    console.log(str);
}

function request_animation_frame(clbk) {
    function loop(time) {
        WasmContext["vtable"].get(clbk)(1569)
        window.requestAnimationFrame(loop);
    }

    window.requestAnimationFrame(loop);
}

function createEnvironment() {
    return {
        "print": wasm_print,
        "wgpuGetDevice": wgpuGetDevice,
        "wgpuGetPreferredCanvasFormat": wgpuGetPreferredCanvasFormat,
        "wgpuCreateShaderModule": wgpuCreateShaderModule,
        "wgpuCreateRenderPipeline": wgpuCreateRenderPipeline,
        "wgpuCreateCommandEncoder": wgpuCreateCommandEncoder,
        "wgpuSwapChainGetCurrentTextureView": wgpuSwapChainGetCurrentTextureView,
        "wgpuCommandEncoderBeginRenderPass": wgpuCommandEncoderBeginRenderPass,
        "wgpuCommandEncoderSetPipeline": wgpuCommandEncoderSetPipeline,
        "wgpuCommandEncoderDraw": wgpuCommandEncoderDraw,
        "wgpuCommandEncoderEnd": wgpuCommandEncoderEnd,
        "wgpuDeviceGetQueue": wgpuDeviceGetQueue,
        "wgpuCommandEncoderFinish": wgpuCommandEncoderFinish,
        "wgpuTextureViewRelease": wgpuTextureViewRelease,
        "wgpuCommandEncoderRelease": wgpuCommandEncoderRelease,
        "wgpuRenderPassEncoderRelease": wgpuRenderPassEncoderRelease,
        "wgpuRenderCommandBufferRelease": wgpuRenderCommandBufferRelease,
        "wgpuQueueSubmit": wgpuQueueSubmit,
        "request_animation_frame": request_animation_frame,
        "trunc": Math.trunc
    };
}

async function init(wasmPath) {
    initWebGpu().then(async function (device) {

        if(device === undefined || device == null) {
            document.getElementById("not_available").style.display = 'flex';
            document.querySelector('canvas').style.display = 'none';
            WasmContext.deviceAvailable = false;
            return;
        }

        GlobalGPUContext["device"] = device;

        const { instance } = await WebAssembly.instantiateStreaming(fetch(wasmPath), {
            "env": createEnvironment()
        }).catch(function(error) {
            console.error(error);
        });

        WasmContext["vtable"] = instance.exports.__indirect_function_table
        WasmContext["heap_base"] = instance.exports.__heap_base.value;
        WasmContext["buffer"] = instance.exports.memory.buffer;

        const presentationFormat = navigator.gpu.getPreferredCanvasFormat();

        GlobalGPUContext["context"].configure({
            device: GlobalGPUContext["device"],
            format: presentationFormat,
        });

        instance.exports.wasm_main();

    })
}
