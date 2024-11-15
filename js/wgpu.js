
var getPointer = function (address, size) {
    return new Uint32Array(WasmContext["buffer"], address, size)
}

var PrimitiveTopologyName = [
    "point-list",
    "line-list",
    "line-strip",
    "triangle-list",
    "triangle-strip",
];

var PrimitiveTopologyMap = [
    { "point-list": 0 },
    { "line-list": 1 },
    { "line-strip": 2 },
    { "triangle-list": 3 },
    { "triangle-strip": 4 },
];

var LoadOpName = [
    "clear",
    "load",

];

var LoadOpMap = [
    { "clear": 0 },
    { "load": 1 },
];

var StoreOpName = [
    "store",
    "discard"
];

var StoreOpMap = [
    { "store": 0 },
    { "discard": 1 },
];


var TextureFormatName = [
    "r8unorm",
    "r8snorm",
    "r8uint",
    "r8sint",
    "r16uint",
    "r16sint",
    "r16unorm",
    "r16snorm",
    "r16float",
    "rg8unorm",
    "rg8snorm",
    "rg8uint",
    "rg8sint",
    "r32uint",
    "r32sint",
    "r32float",
    "rg16uint",
    "rg16sint",
    "rg16unorm",
    "rg16snorm",
    "rg16float",
    "rgba8unorm",
    "rgba8unormsrgb",
    "rgba8snorm",
    "rgba8uint",
    "rgba8sint",
    "bgra8unorm",
    "bgra8unormsrgb",
    "rgb9e5ufloat",
    "rgb10a2uint",
    "rgb10a2unorm",
    "rg11b10ufloat",
    "rg32uint",
    "rg32sint",
    "rg32float",
    "rgba16uint",
    "rgba16sint",
    "rgba16unorm",
    "rgba16snorm",
    "rgba16float",
    "rgba32uint",
    "rgba32sint",
    "rgba32float",
    "stencil8",
    "depth16unorm",
    "depth24plus",
    "depth24plusstencil8",
    "depth32float",
    "depth32floatstencil8",
    "nv12",
    "bc1rgbaunorm",
    "bc1rgbaunormsrgb",
    "bc2rgbaunorm",
    "bc2rgbaunormsrgb",
    "bc3rgbaunorm",
    "bc3rgbaunormsrgb",
    "bc4runorm",
    "bc4rsnorm",
    "bc5rgunorm",
    "bc5rgsnorm",
    "bc6hrgbufloat",
    "bc6hrgbfloat",
    "bc7rgbaunorm",
    "bc7rgbaunormsrgb",
    "etc2rgb8unorm",
    "etc2rgb8unormsrgb",
    "etc2rgb8a1unorm",
    "etc2rgb8a1unormsrgb",
    "etc2rgba8unorm",
    "etc2rgba8unormsrgb",
    "eacr11unorm",
    "eacr11snorm",
    "eacrg11unorm",
    "eacrg11snorm",
    "astcblock",
    "astcchannel"
];
var iota = 0;
var TextureFormatMap = {
    r8unorm: iota++,
    r8snorm: iota++,
    r8uint: iota++,
    r8sint: iota++,
    r16uint: iota++,
    r16sint: iota++,
    r16unorm: iota++,
    r16snorm: iota++,
    r16float: iota++,
    rg8unorm: iota++,
    rg8snorm: iota++,
    rg8uint: iota++,
    rg8sint: iota++,
    r32uint: iota++,
    r32sint: iota++,
    r32float: iota++,
    rg16uint: iota++,
    rg16sint: iota++,
    rg16unorm: iota++,
    rg16snorm: iota++,
    rg16float: iota++,
    rgba8unorm: iota++,
    rgba8unormsrgb: iota++,
    rgba8snorm: iota++,
    rgba8uint: iota++,
    rgba8sint: iota++,
    bgra8unorm: iota++,
    bgra8unormsrgb: iota++,
    rgb9e5ufloat: iota++,
    rgb10a2uint: iota++,
    rgb10a2unorm: iota++,
    rg11b10ufloat: iota++,
    rg32uint: iota++,
    rg32sint: iota++,
    rg32float: iota++,
    rgba16uint: iota++,
    rgba16sint: iota++,
    rgba16unorm: iota++,
    rgba16snorm: iota++,
    rgba16float: iota++,
    rgba32uint: iota++,
    rgba32sint: iota++,
    rgba32float: iota++,
    stencil8: iota++,
    depth16unorm: iota++,
    depth24plus: iota++,
    depth24plusstencil8: iota++,
    depth32float: iota++,
    depth32floatstencil8: iota++,
    nv12: iota++,
    bc1rgbaunorm: iota++,
    bc1rgbaunormsrgb: iota++,
    bc2rgbaunorm: iota++,
    bc2rgbaunormsrgb: iota++,
    bc3rgbaunorm: iota++,
    bc3rgbaunormsrgb: iota++,
    bc4runorm: iota++,
    bc4rsnorm: iota++,
    bc5rgunorm: iota++,
    bc5rgsnorm: iota++,
    bc6hrgbufloat: iota++,
    bc6hrgbfloat: iota++,
    bc7rgbaunorm: iota++,
    bc7rgbaunormsrgb: iota++,
    etc2rgb8unorm: iota++,
    etc2rgb8unormsrgb: iota++,
    etc2rgb8a1unorm: iota++,
    etc2rgb8a1unormsrgb: iota++,
    etc2rgba8unorm: iota++,
    etc2rgba8unormsrgb: iota++,
    eacr11unorm: iota++,
    eacr11snorm: iota++,
    eacrg11unorm: iota++,
    eacrg11snorm: iota++,
    astcblock: iota++,
    astcchannel: iota++
};
iota = 0;

const PipelineInfoSize = 6;
const RenderPassDescriptorSize = 7;

let wgpuGetDevice = function () {
    return GlobalGPUContext.register(GlobalGPUContext["device"])
}

let wgpuCreateRenderPipeline = function (device_id, pipelineInfo) {
    const device = GlobalGPUContext.get(device_id);
    const creation_info = getPipelineCreationInfo(pipelineInfo);
    return GlobalGPUContext.register(device.createRenderPipeline(
        {
            layout: 'auto',
            vertex: { module: GlobalGPUContext.get(creation_info.vertex.module) },
            fragment: {
                module: GlobalGPUContext.get(creation_info.fragment.module),
                targets: creation_info.fragment.targets
            },
            primitive: { topology: creation_info.topology, },
        }
    ));
}

let wgpuGetPreferredCanvasFormat = function () {
    return TextureFormatMap[navigator.gpu.getPreferredCanvasFormat().toString()]
}

let wgpuCreateShaderModule = function (device_id, code) {
    const code_str = getString(code);
    const device = GlobalGPUContext.get(device_id);
    if (device === undefined || device === null) { console.error("device is null"); return -1; }
    return GlobalGPUContext.register(device.createShaderModule({
        code: code_str,
    }));
}

let wgpuCreateCommandEncoder = function (device) {
    return GlobalGPUContext.register(GlobalGPUContext.get(device).createCommandEncoder());
}

let wgpuSwapChainGetCurrentTextureView = function () {
    return GlobalGPUContext.register(GlobalGPUContext["context"].getCurrentTexture().createView());
}

let wgpuCommandEncoderBeginRenderPass = function (encoder_id, renderPassDescriptor) {
    var descriptor = getRenderPassDescriptor(renderPassDescriptor);
    var encoder = GlobalGPUContext.get(encoder_id);
    return GlobalGPUContext.register(encoder.beginRenderPass(descriptor));
}

let wgpuCommandEncoderSetPipeline = function (encoder_id, pipeline_id) {
    var encoder = GlobalGPUContext.get(encoder_id);
    var pipeline = GlobalGPUContext.get(pipeline_id);
    encoder.setPipeline(pipeline);
}

let wgpuCommandEncoderDraw = function (encoder_id, count) {
    var encoder = GlobalGPUContext.get(encoder_id);
    encoder.draw(count);
}

let wgpuCommandEncoderEnd = function (encoder_id) {
    var encoder = GlobalGPUContext.get(encoder_id);
    encoder.end();
}

let wgpuDeviceGetQueue = function (device_id) {
    return GlobalGPUContext.register(GlobalGPUContext.get(device_id).queue);
}

let wgpuCommandEncoderFinish = function (encoder) {
    let commandEncoder = GlobalGPUContext.get(encoder)
    return GlobalGPUContext.register(commandEncoder.finish());
}

let wgpuTextureViewRelease = function (texture_id) {
    GlobalGPUContext.release(texture_id)
}

let wgpuCommandEncoderRelease = function (encoder) {
    GlobalGPUContext.release(encoder)
}

let wgpuRenderPassEncoderRelease = function (renderPass) {
    GlobalGPUContext.release(renderPass)
}

let wgpuRenderCommandBufferRelease = function(commandBuffer) {
    GlobalGPUContext.release(commandBuffer);

}

let wgpuQueueSubmit = function (queue_id, count, commands) {
    let queue = GlobalGPUContext.get(queue_id);
    let command = GlobalGPUContext.get(commands);
    queue.submit([command]);
}

var WasmContext = {
    adapterAvailable: true,
    deviceAvailable: true
}

var GlobalGPUContext = {
    objects: {},
    currentId: 1,
    register: function (obj) {
        let wrapper = {};
        var id = this.currentId++;
        wrapper.refcount = 1;
        wrapper.object = obj;
        this.objects[id] = wrapper
        return id;
    },
    get: function (id) {
        if (id === 0) return undefined;
        return this.objects[id].object;
    },
    reference: function (id) {
        if (id === 0) return;
        this.objects[id].refcount++;
    },
    release: function (id) {
        if (id === 0) return;
        if (this.objects[id].refcount <= 0) return;
        this.objects[id].refcount--;
        if (this.objects[id].refcount <= 0) delete this.objects[id];
    },
}

async function initWebGpu() {

    const canvas = document.querySelector('canvas');
    if (canvas === null) {
        console.error("canvas does not exist...");
    }
    const width = window.innerWidth;
    const height = window.innerHeight;
    canvas.width = width;
    canvas.height = height;

    const ctx = canvas.getContext('webgpu');

    GlobalGPUContext["context"] = ctx;

    const adapter = await navigator.gpu.requestAdapter().catch((err) => {
            WasmContext.adapterAvailable = false;
    })
    if(adapter === undefined) { WasmContext.adapterAvailable = false; return undefined}
    return await adapter.requestDevice();
}


var getPipelineCreationInfo = function (offset) {
    const mem = getPointer(offset, PipelineInfoSize);
    return {
        layout: mem[0] == 255 ? 'auto' : '',
        vertex: { module: mem[1] },
        fragment: {
            module: mem[2],
            targets: [{
                format: TextureFormatName[getPointer(mem[3], mem[4])[0]],
            }]
        },
        primitive: {
            topology: PrimitiveTopologyName[mem[5]]
        }
    };
}

var getRenderPassDescriptor = function (offset) {
    const mem = getPointer(offset, RenderPassDescriptorSize);
    return {
        colorAttachments: [{
            view: GlobalGPUContext.get(mem[0]),
            clearValue: [mem[1], mem[2], mem[3], mem[4]],
            loadOp: LoadOpName[mem[5]],
            storeOp: StoreOpName[mem[6]]
        }]
    };
}

var getCommands = function (count, offset) {
    const mem = getPointer(offset, RenderPassDescriptorSize);
    return {
        colorAttachments: [{
            view: GlobalGPUContext.get(mem[0]),
            clearValue: [mem[1], mem[2], mem[3], mem[4]],
            loadOp: LoadOpName[mem[5]],
            storeOp: StoreOpName[mem[6]]
        }]
    };

}
