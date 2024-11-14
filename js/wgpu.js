
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


var TextureFormatName = [
    "R8Unorm",
    "R8Snorm",
    "R8Uint",
    "R8Sint",
    "R16Uint",
    "R16Sint",
    "R16Unorm",
    "R16Snorm",
    "R16Float",
    "Rg8Unorm",
    "Rg8Snorm",
    "Rg8Uint",
    "Rg8Sint",
    "R32Uint",
    "R32Sint",
    "R32Float",
    "Rg16Uint",
    "Rg16Sint",
    "Rg16Unorm",
    "Rg16Snorm",
    "Rg16Float",
    "Rgba8Unorm",
    "Rgba8UnormSrgb",
    "Rgba8Snorm",
    "Rgba8Uint",
    "Rgba8Sint",
    "Bgra8Unorm",
    "Bgra8UnormSrgb",
    "Rgb9e5Ufloat",
    "Rgb10a2Uint",
    "Rgb10a2Unorm",
    "Rg11b10Ufloat",
    "Rg32Uint",
    "Rg32Sint",
    "Rg32Float",
    "Rgba16Uint",
    "Rgba16Sint",
    "Rgba16Unorm",
    "Rgba16Snorm",
    "Rgba16Float",
    "Rgba32Uint",
    "Rgba32Sint",
    "Rgba32Float",
    "Stencil8",
    "Depth16Unorm",
    "Depth24Plus",
    "Depth24PlusStencil8",
    "Depth32Float",
    "Depth32FloatStencil8",
    "NV12",
    "Bc1RgbaUnorm",
    "Bc1RgbaUnormSrgb",
    "Bc2RgbaUnorm",
    "Bc2RgbaUnormSrgb",
    "Bc3RgbaUnorm",
    "Bc3RgbaUnormSrgb",
    "Bc4RUnorm",
    "Bc4RSnorm",
    "Bc5RgUnorm",
    "Bc5RgSnorm",
    "Bc6hRgbUfloat",
    "Bc6hRgbFloat",
    "Bc7RgbaUnorm",
    "Bc7RgbaUnormSrgb",
    "Etc2Rgb8Unorm",
    "Etc2Rgb8UnormSrgb",
    "Etc2Rgb8A1Unorm",
    "Etc2Rgb8A1UnormSrgb",
    "Etc2Rgba8Unorm",
    "Etc2Rgba8UnormSrgb",
    "EacR11Unorm",
    "EacR11Snorm",
    "EacRg11Unorm",
    "EacRg11Snorm",
    "AstcBlock",
    "AstcChannel"
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

const PipelineInfoSize = 6;

var getPipelineCreationInfo = function (offset) {
    const mem = getPointer(offset, PipelineInfoSize);
    return {
        layout: mem[0]== 255 ? 'auto' : '',
        vertex: { module: mem[1] },
        fragment: {
            module: mem[2],
            targets: [{
                format: getPointer(mem[3], mem[4])[0],
            }]
        },
        primitive: {
            topology: PrimitiveTopologyName[mem[5]]
        }
    };
}


let nativeWebGPU = function () {

    const device = GlobalGPUContext["device"];
    const context = GlobalGPUContext["context"];

    const presentationFormat = navigator.gpu.getPreferredCanvasFormat();
    console.log(presentationFormat)

    const triangleVertWGSL = "@vertex \
                                fn main(\
                                @builtin(vertex_index) VertexIndex : u32\
                                ) -> @builtin(position) vec4f {\
                                var pos = array<vec2f, 3>(\
                                    vec2(0.0, 0.5),\
                                    vec2(-0.5, -0.5),\
                                    vec2(0.5, -0.5)\
                                );\
                                return vec4f(pos[VertexIndex], 0.0, 1.0);\
                                }";

    const redFragWGSL = "@fragment\
                         fn main() -> @location(0) vec4f {\
                         return vec4(1.0, 0.0, 0.0, 1.0);\
                         }";

    const pipeline = GlobalGPUContext.get(4).object;
/*
    const pipeline = device.createRenderPipeline({
        layout: 'auto',
        vertex: {
            module: device.createShaderModule({
                code: triangleVertWGSL,
            }),
        },
        fragment: {
            module: device.createShaderModule({
                code: redFragWGSL,
            }),
            targets: [
                {
                    format: presentationFormat,
                },
            ],
        },
        primitive: {
            topology: 'triangle-list',
        },
    });*/

    function frame() {
        const commandEncoder = device.createCommandEncoder();
        var textureView = context.getCurrentTexture().createView();

        const renderPassDescriptor = {
            colorAttachments: [
                {
                    view: textureView,
                    clearValue: [0, 0, 0, 0], // Clear to transparent
                    loadOp: 'clear',
                    storeOp: 'store',
                },
            ],
        };

        const passEncoder = commandEncoder.beginRenderPass(renderPassDescriptor);
        passEncoder.setPipeline(pipeline);
        passEncoder.draw(3);
        passEncoder.end();

        device.queue.submit([commandEncoder.finish()]);
        requestAnimationFrame(frame);
    }

    requestAnimationFrame(frame);
}