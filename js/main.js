"use strict"

function getString(offset) {
    const buffer = WasmContext["buffer"];
    const str_buffer = new Uint32Array(buffer, offset, 2);
    return new TextDecoder("utf8").decode(new Uint8Array(buffer, str_buffer[0], str_buffer[1]));
}

let wgpuGetDevice = function () {
    return GlobalGPUContext.register(GlobalGPUContext["device"])
}

let wgpuCreateRenderPipeline = function(device_id, pipelineInfo) {
    const device = GlobalGPUContext.get(device_id);
    const creation_info = getPipelineCreationInfo(pipelineInfo);
    console.log(device)
    console.log(creation_info)
    return 55
    return GlobalGPUContext.register(device.createRenderPipeline(
        {
            layout: 'auto',
            vertex: { module: GlobalGPUContext.get(creation_info).vertex },
            fragment: { module: GlobalGPUContext.get(creation_info).fragment,
                targets: [
                    {
                        format: creation_info.format,
                    },
                ],
             },
            primitive: { topology: creation_info.topology, },
        }
    ));
}

let wgpuGetPreferredCanvasFormat = function() {
    return TextureFormatMap[navigator.gpu.getPreferredCanvasFormat().toString()]
}

let wgpuCreateShaderModule = function(device_id, code) {
    const code_str = getString(code);
    const device = GlobalGPUContext.get(device_id);
    if(device === undefined || device === null) { console.error("device is null"); return -1;}
    return GlobalGPUContext.register(device.createShaderModule({
        code: code_str,
    }));
}

var WasmContext = {}

var GlobalGPUContext = {
    objects: [undefined],
    register: function(obj) {
        let wrapper = {};
        var id = this.objects.length;
        wrapper.refcount = 1;
        wrapper.object = obj;
        this.objects[id] = wrapper
        return id;
    },
    get: function(id) {
        if(id === 0) return undefined;
        return this.objects[id].object;
    }
}

async function initWebGpu() {

    const canvas = document.querySelector('canvas');
    if (canvas === null) {
        console.error("canvas does not exist...");
    }
    const width = window.outerWidth;
    const height = window.outerHeight;
    canvas.width = width;
    canvas.height = height;

    const ctx = canvas.getContext('webgpu');

    GlobalGPUContext["context"] = ctx;

    const adapter = await navigator.gpu?.requestAdapter();
    return await adapter.requestDevice();
}


function wasm_print(heap_base) {
    const str = getString(heap_base);
    console.log(str);
}

async function init(wasmPath) {
    initWebGpu().then(async function (device) {

        GlobalGPUContext["device"] = device;

        const { instance } = await WebAssembly.instantiateStreaming(fetch(wasmPath), {
            "env": {
                "print": wasm_print,
                "wgpuGetDevice": wgpuGetDevice,
                "wgpuQueueSubmit": GPUQueue.prototype.submit,
                "wgpuGetPreferredCanvasFormat": wgpuGetPreferredCanvasFormat,
                "wgpuCreateShaderModule": wgpuCreateShaderModule,
                "wgpuCreateRenderPipeline": wgpuCreateRenderPipeline,
                "request_animation_frame": request_animation_frame,
                "trunc": Math.trunc
            }
        });

        WasmContext["vtable"] = instance.exports.__indirect_function_table
        WasmContext["heap_base"] = instance.exports.__heap_base.value;
        WasmContext["buffer"] = instance.exports.memory.buffer;



        function request_animation_frame(clbk) {
            if (clbk !== undefined) {
                clbk_index = clbk
            }
        }

        const presentationFormat = navigator.gpu.getPreferredCanvasFormat();

        GlobalGPUContext["context"].configure({
            device: GlobalGPUContext["device"],
            format: presentationFormat,
        });

        instance.exports.wasm_main();
        nativeWebGPU();
/*
        const presentationFormat = navigator.gpu.getPreferredCanvasFormat();

        console.log(presentationFormat)

        let clbk_index = 0;
        function loop(time) {
            if (clbk_index > 0) {
                vtable.get(clbk_index)()
            }
            window.requestAnimationFrame(loop);
        }



        instance.exports.hello(heap_base);
        let str2 = getString(buffer, heap_base);
        console.log(str2)

        let str_buffer = new Float32Array(buffer, heap_base, 5);
        str_buffer[0] = 34;
        str_buffer[1] = Number.parseFloat(11.69)
        str_buffer[2] = 42.39;
        str_buffer[3] = 13;
        str_buffer[4] = 27;
        instance.exports.marshall(heap_base);

        let str = new TextEncoder("utf8").encodeInto("Hello\0", new Uint8Array(buffer, heap_base, 6))//new Uint32Array(buffer, heap_base+12, 6);
        instance.exports.marshall_string(heap_base);

        instance.exports.wasm_main();

        window.requestAnimationFrame(loop);
        */
    })
}
window.onload = () => {
    'use strict';
    if ('serviceWorker' in navigator) {
        navigator.serviceWorker
            .register('./js/sw.js');
    }
    init("/wasm/test.wasm");
};
