"use strict"

var Context = {
    Debug: true
}

function getString(offset) {
    const buffer = WasmContext["memory"];
    const str_buffer = new Uint32Array(buffer, offset, 2);
    return new TextDecoder("utf8").decode(new Uint8Array(buffer, str_buffer[0], str_buffer[1]));
}

function getStringFromPointer(offset, size) {
    const buffer = WasmContext["memory"];
    return new TextDecoder("utf8").decode(new Uint8Array(buffer, offset, size));
}

function wasm_print(heap_base) {
    const str = getString(heap_base);
    console.log(str);
}
let lastTime = 0.0;
let updateFps = 1.0
let numFrames = 0
let accumFrameTime = 0
function request_animation_frame(clbk) {
    lastTime = Date.now() / 1000.0
    function loop() {
        let t = Date.now() / 1000.0;
        var dt = t - lastTime;
        lastTime = t;
        updateFps += dt
        numFrames++
        accumFrameTime += dt
        if(updateFps >= 1.0) {
            let avgFrametime = accumFrameTime/numFrames
            document.getElementById("framerate").textContent = Math.trunc(1.0/avgFrametime) + " [" + (avgFrametime*1000).toFixed(3) + "ms]";
            updateFps=0;
            numFrames=0
            accumFrameTime=0
        }
        WasmContext["vtable"].get(clbk)(dt)
        setTimeout(loop, 1000.0/60.0);
    }

    setTimeout(loop, 0);
}

function sbrk(increment) {
    var heapOld = WASM_HEAP, heapNew = heapOld + increment, heapGrow = heapNew - WASM_MEMORY.buffer.byteLength;
    //console.log('[SBRK] Increment: ' + increment + ' - HEAP: ' + heapOld + ' -> ' + heapNew + (heapGrow > 0 ? ' - GROW BY ' + heapGrow + ' (' + (heapGrow>>16) + ' pages)' : ''));
    if (heapNew > WASM_HEAP_MAX) abort('MEM', 'Out of memory');
    if (heapGrow > 0) { WASM_MEMORY.grow((heapGrow + 65535) >> 16); MemorySetBufferViews(); }
    WASM_HEAP = heapNew;
    return heapOld | 0;
}

function ieee32ToFloat(intval) {
    var fval = 0.0;
    var x;//exponent
    var m;//mantissa
    var s;//sign
    s = (intval & 0x80000000) ? -1 : 1;
    x = ((intval >> 23) & 0xFF);
    m = (intval & 0x7FFFFF);
    switch (x) {
        case 0:
            //zero, do nothing, ignore negative zero and subnormals
            break;
        case 0xFF:
            if (m) fval = NaN;
            else if (s > 0) fval = Number.POSITIVE_INFINITY;
            else fval = Number.NEGATIVE_INFINITY;
            break;
        default:
            x -= 127;
            m += 0x800000;
            fval = s * (m / 8388608.0) * Math.pow(2, x);
            break;
    }
    return fval;
}

let time = function () { return Date.now() / 1000 }

let getWindowWidth = function() {
    return WasmWindow.width
}

let getWindowHeight = function() {
    return WasmWindow.height
}

function createEnvironment() {

    return {
        "puts": wasm_print,
        "wgpuGetDevice": wgpuGetDevice,
        "wgpuGetPreferredCanvasFormat": wgpuGetPreferredCanvasFormat,
        "wgpuCreateShaderModule": wgpuCreateShaderModule,
        "wgpuCreateRenderPipeline": wgpuCreateRenderPipeline,
        "wgpuCreateCommandEncoder": wgpuCreateCommandEncoder,
        "wgpuSwapChainGetCurrentTextureView": wgpuSwapChainGetCurrentTextureView,
        "wgpuCommandEncoderBeginRenderPass": wgpuCommandEncoderBeginRenderPass,
        "wgpuRenderPassEncoderSetPipeline": wgpuRenderPassEncoderSetPipeline,
        "wgpuRenderPassEncoderDraw": wgpuRenderPassEncoderDraw,
        "wgpuRenderPassEncoderEnd": wgpuRenderPassEncoderEnd,
        "wgpuDeviceGetQueue": wgpuDeviceGetQueue,
        "wgpuCommandEncoderFinish": wgpuCommandEncoderFinish,
        "wgpuTextureViewRelease": wgpuTextureViewRelease,
        "wgpuCommandEncoderRelease": wgpuCommandEncoderRelease,
        "wgpuRenderPassEncoderRelease": wgpuRenderPassEncoderRelease,
        "wgpuCommandBufferRelease": wgpuCommandBufferRelease,
        "wgpuRenderPassEncoderSetVertexBuffer": wgpuRenderPassEncoderSetVertexBuffer,
        "wgpuQueueSubmit": wgpuQueueSubmit,
        "wgpuCreateBuffer": wgpuCreateBuffer,
        "wgpuDestroyBuffer": wgpuDestroyBuffer,
        "wgpuBufferGetMappedRange": wgpuBufferGetMappedRange,
        "wgpuBufferUnmap": wgpuBufferUnmap,
        "wgpuDeviceCreateBindGroupLayout": wgpuDeviceCreateBindGroupLayout,
        "wgpuCreatePipelineLayout": wgpuCreatePipelineLayout,
        "wgpuReleaseBindGroupLayout": wgpuReleaseBindGroupLayout,
        "wgpuReleasePipelineLayout": wgpuReleasePipelineLayout,
        "wgpuReleaseBindGroup": wgpuReleaseBindGroup,
        "wgpuCreateBindGroup": wgpuCreateBindGroup,
        "wgpuRenderPassEncoderSetBindGroup": wgpuRenderPassEncoderSetBindGroup,
        "wgpuQueueWriteBuffer": wgpuQueueWriteBuffer,
        "wgpuCreateTexture": wgpuCreateTexture,
        "wgpuCreateTextureView": wgpuCreateTextureView,
        "request_animation_frame": request_animation_frame,
        "getWindowWidth": getWindowWidth,
        "getWindowHeight": getWindowHeight,

        "time": time,
        "gettimeofday": function (ptr) { var now = Date.now(); WasmContext["heapViewu32"][ptr >> 2] = (now / 1000) | 0; WasmContext["heapViewu32"][(ptr + 4) >> 2] = ((now % 1000) * 1000) | 0; },

        "ceil": Math.ceil,
        "exp": Math.exp,
        "floor": Math.floor,
        "log": Math.log,
        "pow": Math.pow,
        "cos": Math.cos,
        "trunc": Math.trunc,
        "sin": Math.sin,
        "tan": Math.tan,
        "acos": Math.acos,
        "asin": Math.asin,
        "sqrt": Math.sqrt,
        "atan": Math.atan2,
        "fabs": Math.abs,
        "round": Math.round
    };
}

async function init(wasmPath) {
    initWebGpu().then(async function (device) {

        if (device === undefined || device == null) {
            document.getElementById("not_available").style.display = 'flex';
            document.querySelector('canvas').style.display = 'none';
            WasmContext.deviceAvailable = false;
            return;
        }


        GlobalGPUContext["device"] = device;

        const importObject = {
            env: createEnvironment(),
        }

        const { instance } = await WebAssembly.instantiateStreaming(fetch(wasmPath),
            { "env": createEnvironment() }
        ).catch(function (error) {
            console.error(error);
        });

        WasmContext["heap_base"] = instance.exports.__heap_base.value;
        WasmContext["vtable"] = instance.exports.__indirect_function_table
        WasmContext["memory"] = instance.exports.memory.buffer;
        WasmContext["exports"] = instance.exports;
        WasmContext["heapViewu8"] = new Uint8Array(WasmContext["memory"]);
        WasmContext["heapViewu16"] = new Uint16Array(WasmContext["memory"]);
        WasmContext["heapViewu32"] = new Uint32Array(WasmContext["memory"]);
        WasmContext["heapViewf32"] = new Float32Array(WasmContext["memory"]);
        WasmContext["heapView32"] = new Int32Array(WasmContext["memory"]);

        const presentationFormat = navigator.gpu.getPreferredCanvasFormat();

        GlobalGPUContext["context"].configure({
            device: GlobalGPUContext["device"],
            format: presentationFormat,
        });


        if (instance.exports.__wasm_call_ctors) instance.exports.__wasm_call_ctors();

        WasmWindow.width = GlobalGPUContext.context.canvas.width;
        WasmWindow.height = GlobalGPUContext.context.canvas.height;

        instance.exports.wasm_main();

    })
}
