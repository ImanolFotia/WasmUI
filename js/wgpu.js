
var getPointer = function (address, size) {
    return new Uint32Array(WasmContext["memory"], address, size)
}
var getColor = function (buffer, offset, size) {
    return new Uint32Array(buffer, offset, size)
}

let WasmWindow = {
    width: 0,
    height: 0
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


var CompareFunctionName = [
    "never",
    "less",
    "equal",
    "less-equal",
    "greater",
    "not-equal",
    "greater-equal",
    "always",
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


var BufferUsageName = [
    "map_read",
    "map_write",
    "copy_src",
    "copy_dst",
    "index",
    "vertex",
    "uniform",
    "storage",
    "indirect",
    "query_resolve",
    "blas_input",
    "tlas_input"
];

var bufferBindingTypeName = [
    "uniform",
    "storage",
    "read-only-storage",

]

var TextureFormatName = [
    "none",
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


var VertexFormatName = [
    "uint8x2",
    "uint8x4",
    "sint8x2",
    "sint8x4",
    "unorm8x2",
    "unorm8x4",
    "snorm8x2",
    "snorm8x4",
    "uint16x2",
    "uint16x4",
    "sint16x2",
    "sint16x4",
    "unorm16x2",
    "unorm16x4",
    "snorm16x2",
    "snorm16x4",
    "float16x2",
    "float16x4",
    "float32",
    "float32x2",
    "float32x3",
    "float32x4",
    "uint32",
    "uint32x2",
    "uint32x3",
    "uint32x4",
    "sint32",
    "sint32x2",
    "sint32x3",
    "sint32x4",
    "float64",
    "float64x2",
    "float64x3",
    "float64x4",
    "unorm10_10_10_2",
];

var DimensionName = [
    "1d", "2d", "3d"
]

var iota = 0;
var TextureFormatMap = {
    none: iota++,
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

var C_STRUCT = {
    RenderTarget: 4,
    VertexAttribute: 12,
    VertexBufferLayout: 16,
    VertexState: 40,
    BlendComponent: 12,
    BlendState: 24,
    ColorTargetState: 32,
    FragmentState: 40,
    BufferBindingLayout: 4,
    PushConstantRange: 12,
    BindGroupLayoutDescriptor: 40,
    PipelineLayoutDescriptor: 16,
    PrimitiveState: 28,
    StencilFaceState: 16,
    DepthBiasState: 12,
    DepthStencilState: 64,
    MultiSampleState: 12,
    RenderPipelineDescriptor: 208,
    Operations: 8,
    RenderPassColorAttachment: 40,
    RenderPassDepthStencilAttachment: 8 * 4,
    RenderPassTimestampWrites: 16,
    RenderPassDescriptor: 56,
    BufferDescriptor: 24,
    BindGroupLayoutEntry: 24,
    BindGroupLayoutDescriptor: 8,
    BindGroupDescriptor: 12,
    BindGroupEntry: 12,
    BindingResource: 20,
    TextureDescriptor: 32
}

let wgpuGetDevice = function () {
    return GlobalGPUContext.register(GlobalGPUContext["device"])
}

let wgpuCreateRenderPipeline = function (device_id, pipelineInfo) {
    const device = GlobalGPUContext.get(device_id).object;
    const creation_info = getPipelineCreationInfo(pipelineInfo);
    let depthStencil = undefined

    if (creation_info.depthStencil.format != "none") {
        depthStencil = {
            depthWriteEnabled: creation_info.depthStencil.depthWriteEnabled,
            depthCompare: creation_info.depthStencil.depthCompare,
            format: creation_info.depthStencil.format,
        }
    }
    try {
        return GlobalGPUContext.register(device.createRenderPipeline(
            {
                layout: creation_info.layout,
                vertex: {
                    module:
                        GlobalGPUContext.get(creation_info.vertex.module).object,
                    entryPoint: creation_info.vertex.entry_point,
                    buffers: creation_info.vertex.buffers
                },
                fragment: {
                    module: GlobalGPUContext.get(creation_info.fragment.module).object,
                    entryPoint: creation_info.fragment.entry_point,
                    targets: creation_info.fragment.targets
                },
                primitive: {
                    topology: creation_info.primitive.topology,
                    cullMode: 'back',
                },
                depthStencil: depthStencil
            }
        ));
    } catch (error) {
        console.error("An error has ocurred:", error)
        abort();
    }
}

let wgpuDeviceCreateBindGroupLayout = function (device_id, ld_id) {
    let device = GlobalGPUContext.get(device_id).object;
    let layout_descriptor = getPointer(ld_id, C_STRUCT.BindGroupLayoutDescriptor);
    let numEntries = layout_descriptor[0];
    let entries_ptr = getPointer(layout_descriptor[1], numEntries * C_STRUCT.BindGroupLayoutEntry);
    let entries = []
    for (var i = 0; i < numEntries; i++) {
        let entry = {}
        entry.binding = entries_ptr[numEntries * i + 0];
        entry.visibility = entries_ptr[numEntries * i + 1];
        if (entries_ptr[numEntries * i + 2] != 0) entry.buffer = getBufferEntry(entries_ptr[numEntries * i + 2]);
        //TODO: IMPLEMENT THE REST
        //else if (entries_ptr[numEntries * i + 3] != 0) entry.sampler = getSamplerEntry(entries_ptr[numEntries * i + 3]);
        //else if (entries_ptr[numEntries * i + 4] != 0) entry.texture = getTextureEntry(entries_ptr[numEntries * i + 4]);
        //else if (entries_ptr[numEntries * i + 5] != 0) entry.storageTexture = getStorageTexEntry(entries_ptr[numEntries * i + 5]);
        entries.push(entry);
    }
    return GlobalGPUContext.register(device.createBindGroupLayout({ entries: entries }))
}

let wgpuReleaseBindGroupLayout = function (layout_id) {
    GlobalGPUContext.release(layout_id);
}

let wgpuReleasePipelineLayout = function (layout_id) {
    GlobalGPUContext.release(layout_id);
}



function getBufferEntry(ptr) {

    var entry_ptr = getPointer(ptr, 12);

    return {
        type: bufferBindingTypeName[entry_ptr[0]],
        hasDynamicOffset: entry_ptr[1],
        minBindingSize: entry_ptr[2]
    };
}

let wgpuGetPreferredCanvasFormat = function () {
    return TextureFormatMap[navigator.gpu.getPreferredCanvasFormat().toString()]
}

let wgpuCreateShaderModule = function (device_id, code) {
    const code_str = getString(code);
    const device = GlobalGPUContext.get(device_id).object;
    if (device === undefined || device === null) { console.error("device is null"); return -1; }
    return GlobalGPUContext.register(device.createShaderModule({
        code: code_str,
    }));
}

let wgpuCreateCommandEncoder = function (device) {
    return GlobalGPUContext.register(GlobalGPUContext.get(device).object.createCommandEncoder());
}

let wgpuSwapChainGetCurrentTextureView = function () {
    return GlobalGPUContext.register(GlobalGPUContext["context"].getCurrentTexture().createView());
}


let wgpuCreateTextureView = function (texture_id, descriptor) {
    //if(descriptor === undefined) {
    let texture = GlobalGPUContext.get(texture_id).object;
    return GlobalGPUContext.register(texture.createView());
    //}
}

let wgpuCommandEncoderBeginRenderPass = function (encoder_id, renderPassDescriptor) {
    var descriptor = getRenderPassDescriptor(renderPassDescriptor);
    var encoder = GlobalGPUContext.get(encoder_id).object;
    return GlobalGPUContext.register(encoder.beginRenderPass(descriptor));
}

let wgpuRenderPassEncoderSetPipeline = function (encoder_id, pipeline_id) {
    var encoder = GlobalGPUContext.get(encoder_id).object;
    var pipeline = GlobalGPUContext.get(pipeline_id).object;
    encoder.setPipeline(pipeline);
}

let wgpuRenderPassEncoderSetBindGroup = function (encoder_id, index, bindgroup_id, dynamicOffsetCount, dynamicOffset) {
    var encoder = GlobalGPUContext.get(encoder_id).object;
    var bindGroup = GlobalGPUContext.get(bindgroup_id).object;
    encoder.setBindGroup(index, bindGroup);
}

let wgpuRenderPassEncoderDraw = function (encoder_id, count) {
    var encoder = GlobalGPUContext.get(encoder_id).object;
    encoder.draw(count);
}

let wgpuRenderPassEncoderEnd = function (encoder_id) {
    var encoder = GlobalGPUContext.get(encoder_id).object;
    encoder.end();
}

let wgpuDeviceGetQueue = function (device_id) {
    return GlobalGPUContext.register(GlobalGPUContext.get(device_id).object.queue);
}

let wgpuCommandEncoderFinish = function (encoder) {
    let commandEncoder = GlobalGPUContext.get(encoder).object
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

let wgpuCommandBufferRelease = function (commandBuffer) {
    GlobalGPUContext.release(commandBuffer);

}

let wgpuQueueSubmit = function (queue_id, count, commands) {
    let queue = GlobalGPUContext.get(queue_id).object;
    let command = GlobalGPUContext.get(commands).object;
    queue.submit([command]);
}


let wgpuCreateBuffer = function (device_id, bufferInfo) {
    return GlobalGPUContext.register(GlobalGPUContext.get(device_id).object.createBuffer(getBufferDescriptor(bufferInfo)));
}

let wgpuBufferGetMappedRange = function (buffer_id, offset, size) {
    let buffer = GlobalGPUContext.get(buffer_id);

    var mapped = buffer.object.getMappedRange(offset, size);
    var ptr = WasmContext["exports"].alloc_arena(WasmContext["exports"].transfer_arena, size);

    buffer.onUnmap = function () {
        var heap = new Uint8Array(WasmContext["memory"]);
        new Uint8Array(mapped).set(heap.subarray(ptr, ptr + mapped.byteLength));
        WasmContext["exports"].reset_arena(ptr);
    }

    return ptr;
}

let wgpuBufferUnmap = function (buffer_id) {
    let bufferWrapper = GlobalGPUContext.get(buffer_id);
    bufferWrapper.onUnmap();
    bufferWrapper.object.unmap();
}

let wgpuBufferMappedRangeRelease = function (buffer_range_id) {
    return GlobalGPUContext.release(buffer_range_id);
}

let wgpuDestroyBuffer = function (buffer_id) {
    GlobalGPUContext.get(buffer_id).object.destroy()
    GlobalGPUContext.release(buffer_id);
}

let wgpuRenderPassEncoderSetVertexBuffer = function (encoder_id, slot, vertex_buffer_id, offset, size) {
    let vertex_buffer = GlobalGPUContext.get(vertex_buffer_id).object;
    let encoder = GlobalGPUContext.get(encoder_id).object;
    encoder.setVertexBuffer(slot, vertex_buffer, offset, size);
}

let wgpuCreatePipelineLayout = function (device_id, layout_desc) {
    let device = GlobalGPUContext.get(device_id).object;
    layout_descriptor = getPipelineLayoutDescriptor(getPointer(layout_desc, C_STRUCT.PipelineLayoutDescriptor));
    return GlobalGPUContext.register(device.createPipelineLayout(layout_descriptor));
}

let wgpuCreateBindGroup = function (device_id, desc) {
    let device = GlobalGPUContext.get(device_id).object;
    let descriptor = getPointer(desc, C_STRUCT.BindGroupDescriptor);

    let bindGroupLayout = GlobalGPUContext.get(descriptor[0]).object;
    let entryCount = descriptor[1];
    let entries_ptr = getPointer(descriptor[2], C_STRUCT.BindGroupEntry * entryCount);
    let entries = []
    for (var i = 0; i < entryCount; i++) {
        //let entryResource = getPointer(entries_ptr[entryCount * i + 1], entryCount*C_STRUCT.BindingResource);
        let resource = {}
        if (entries_ptr[entryCount * i + 1] != 0) resource.textureView = GlobalGPUContext.get(entries_ptr[entryCount * i + 1]).object;
        else if (entries_ptr[entryCount * i + 2] != 0) resource.sampler = GlobalGPUContext.get(entries_ptr[entryCount * i + 2]).object;
        else if (entries_ptr[entryCount * i + 3] != 0) {
            resource.buffer = GlobalGPUContext.get(entries_ptr[entryCount * i + 3]).object;
            resource.offset = entries_ptr[entryCount * i + 4];
            resource.size = entries_ptr[entryCount * i + 5];
        }
        entries.push({
            binding: entries_ptr[entryCount * i + 0],
            resource: resource
        })
    }
    return GlobalGPUContext.register(device.createBindGroup({
        layout: bindGroupLayout,
        entries: entries
    }))
}

let wgpuReleaseBindGroup = function (bindGroup) {
    GlobalGPUContext.release(bindGroup);
}

let wgpuQueueWriteBuffer = function (queue_id, buffer_id, offset, ptr, size) {
    var queue = GlobalGPUContext.get(queue_id).object;
    var buffer = GlobalGPUContext.get(buffer_id).object;
    var heap = new Uint8Array(WasmContext["memory"]);
    var subarray = heap.subarray(ptr, ptr + size);
    queue.writeBuffer(buffer, offset, subarray, 0, size);
}

let wgpuCreateTexture = function (device_id, descriptor) {
    let device = GlobalGPUContext.get(device_id).object;
    let mem = getPointer(descriptor, C_STRUCT.TextureDescriptor);

    size = []
    if (mem[5] === 0) size = [mem[0]]
    else if (mem[5] === 1) size = [mem[0], mem[1]]
    else if (mem[5] === 2) size = [mem[0], mem[1], mem[2]]

    return GlobalGPUContext.register(device.createTexture({
        size: size,
        mipLevelCount: mem[3],
        sampleCount: mem[4],
        dimension: DimensionName[mem[5]],
        format: TextureFormatName[mem[6]],
        usage: mem[7]
    }));
}


var WasmContext = {
    adapterAvailable: true,
    deviceAvailable: true,
    heapViewu8: 0,
    heapViewu16: 0,
    heapViewu32: 0,
    heapViewf32: 0,
    heapView32: 0,
    heap_max: 256 * 1024 * 1024, //max 256MB
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
        return this.objects[id];
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

    try {
        const adapter = await navigator.gpu.requestAdapter().catch((err) => {
            WasmContext.adapterAvailable = false;
        })
        if (adapter === undefined || adapter === null) { WasmContext.adapterAvailable = false; return undefined }
        return await adapter.requestDevice();
    } catch (error) {
        console.log(error)
        document.getElementById("not_available").style.display = 'flex';
        document.querySelector('canvas').style.display = 'none';
        document.getElementById("fps").style.display = 'none';
        WasmContext.deviceAvailable = false;
        abort();
    }
}

var getPipelineLayoutDescriptor = function (mem) {
    let bg_layouts = []
    let pc_ranges = []
    for (var i = 0; i < mem[0]; i++) bg_layouts.push(GlobalGPUContext.get(getPointer(mem[1], 1)[0]).object)
    for (var i = 0; i < mem[2]; i++) pc_ranges.push(GlobalGPUContext.get(getPointer(mem[3], 1)[0]).object)
    return {
        bindGroupLayouts: bg_layouts,
        pushConstantRanges: pc_ranges
    };
}

var getVertexBufferLayouts = function (mem) {
    var layouts = []
    var layouts_count = mem[4];
    let layouts_ptr = getPointer(mem[5])
    for (var i = 0; i < layouts_count; i++) {
        var attributeCount = layouts_ptr[i + 1];
        var attribute = getPointer(layouts_ptr[i + 2])
        var attributes = []
        for (var j = 0; j < attributeCount; j++) {
            attributes.push({
                format: VertexFormatName[attribute[(C_STRUCT.VertexAttribute / 4) * j + 0]],
                offset: attribute[(C_STRUCT.VertexAttribute / 4) * j + 1],
                shaderLocation: attribute[(C_STRUCT.VertexAttribute / 4) * j + 2]
            })
        }
        layouts.push({
            arrayStride: layouts_ptr[i + 0],
            attributes: attributes
        }

        )
    }
    return layouts
}


var getPipelineCreationInfo = function (offset) {
    const mem = getPointer(offset, C_STRUCT.RenderPipelineDescriptor);
    return {
        layout: mem[0] == 0 ? 'auto' : GlobalGPUContext.get(mem[0]).object,
        vertex: {
            module: mem[1],
            entry_point: getStringFromPointer(mem[2], mem[3]),
            buffers: getVertexBufferLayouts(mem)
        },
        primitive: {
            topology: PrimitiveTopologyName[mem[6]]
        },
        fragment: {
            module: mem[32],
            entry_point: getStringFromPointer(mem[33], mem[34]),
            targets: [{
                format: TextureFormatName[getPointer(mem[36], C_STRUCT.ColorTargetState)[0]],
            }]

        },
        depthStencil: {
            format: TextureFormatName[mem[13]],
            depthWriteEnabled: mem[14],
            depthCompare: CompareFunctionName[mem[15]]
        }
    };
}

var getColorAttachment = function (mem) {
    const attachmentCount = mem[0];
    var attachments = []
    var ptr = getPointer(mem[1], C_STRUCT.RenderPassColorAttachment * attachmentCount);
    for (var i = 0; i < attachmentCount; i++) {
        var cv = getColor(ptr, 4, 4);
        attachments.push({
            view: GlobalGPUContext.get(ptr[0 + i]).object,
            resolve_target: ptr[1 + i],
            loadOp: LoadOpName[ptr[2 + i]],
            storeOp: StoreOpName[ptr[3 + i]],
            clearValue: [ieee32ToFloat(ptr[4 + i]),
            ieee32ToFloat(ptr[5 + i]),
            ieee32ToFloat(ptr[6 + i]),
            ieee32ToFloat(ptr[7 + i])]
        });
    }
    return attachments;
}

var getDepthAttachment = function (mem) {
    const attachmentCount = mem[2];
    var attachments = {}
    var ptr = getPointer(mem[3], C_STRUCT.RenderPassDepthStencilAttachment * attachmentCount);
    for (var i = 0; i < attachmentCount; i++) {
        attachments = {
            view: GlobalGPUContext.get(ptr[0 + i]).object,
            depthLoadOp: LoadOpName[ptr[1 + i]],
            depthStoreOp: StoreOpName[ptr[2 + i]],
            stencilLoadOp: LoadOpName[ptr[3 + i]],
            stencilStoreOp: StoreOpName[ptr[4 + i]],
            depthClearValue: ieee32ToFloat(ptr[5 + i]),
            stencilClearValue: ieee32ToFloat(ptr[6 + i]),
            readOnly: ptr[7 + i]
        };
    }
    return attachments;
}

var getRenderPassDescriptor = function (offset) {
    const mem = getPointer(offset, C_STRUCT.RenderPassDescriptor);
    const color_attachments = getColorAttachment(mem);
    const depth_attachments = mem[2] === 0 ? undefined : getDepthAttachment(mem);
    const canvas = document.querySelector('canvas');

    return {
        colorAttachments: color_attachments,
        depthStencilAttachment: depth_attachments
    };
}

var getBufferDescriptor = function (offset) {
    const mem = getPointer(offset, C_STRUCT.BufferDescriptor);
    return {
        size: mem[0],
        usage: mem[1],
        mappedAtCreation: mem[2] == 1 ? true : false,
    };

}

var getCommands = function (count, offset) {
    const mem = getPointer(offset, RenderPassDescriptorSize);
    return {
        colorAttachments: [{
            view: GlobalGPUContext.get(mem[0]).object,
            clearValue: [mem[1], mem[2], mem[3], mem[4]],
            loadOp: LoadOpName[mem[5]],
            storeOp: StoreOpName[mem[6]]
        }]
    };

}
