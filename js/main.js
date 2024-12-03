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
    return str.length
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
        if (updateFps >= 1.0) {
            let avgFrametime = accumFrameTime / numFrames
            document.getElementById("framerate").textContent = Math.trunc(1.0 / avgFrametime) + " [" + (avgFrametime * 1000).toFixed(3) + "ms]";
            updateFps = 0;
            numFrames = 0
            accumFrameTime = 0
        }
        WasmContext["vtable"].get(clbk)(dt)
        setTimeout(loop, 1000.0 / 60.0);
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

let getWindowWidth = function () {
    return WasmWindow.width
}

let getWindowHeight = function () {
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
        "wgpuQueueCopyExternalImageToTexture": wgpuQueueCopyExternalImageToTexture,
        "wgpuCreateSampler": wgpuCreateSampler,
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
        "round": Math.round,

        "fopen": fopen,
        "fread": fread,
        "print_num": print_num,

        "env_fetch": c_fetch,
        "env_response_text": getResponseText,
        "env_response_text_promise": getResponseTextPromise,
        "env_promise_next": getPromiseNext,
        "env_response_blob_promise": getResponseBlobPromise,
        "env_createImageBitmap": c_createImageBitmap
    };
}

let files = {}
var numFiles = 0;


function readTextFile(file) {
    /*
        From https://xhr.spec.whatwg.org/#sync-warning

        "Synchronous XMLHttpRequest outside of workers is in the process of being removed 
        from the web platform as it has detrimental effects to the end user’s experience. 
        (This is a long process that takes many years.) Developers must not pass false for 
        the async argument when the current global object is a Window object. User agents 
        are strongly encouraged to warn about such usage in developer tools and may experiment 
        with throwing an "InvalidAccessError" DOMException when it occurs."

        But we are using JS here, that's detrimental enough for UX

        You can't achieve this behaviour with the modern fetch() API
        Why can't I just write my code as I want because you don't know how to use more than one thread? 

    */
    var allText;
    var rawFile = new XMLHttpRequest();
    rawFile.open("POST", file, true);
    rawFile.onreadystatechange = function () {
        if (rawFile.readyState === 4) {
            if (rawFile.status === 200 || rawFile.status == 0) {
                console.log("file %s succefully loaded!", file)
            }
        }
    }
    rawFile.send(null);
    return rawFile
}

let fopen = (path_ptr) => {
    var path = getString(path_ptr);
    console.log("Trying to open:", path)

    var file = fetch(path)
    //var file = readTextFile(path)

    let object = {}
    object.file = file
    object.text = undefined
    files[++numFiles] = object;

    return numFiles;
}

let fread = async (file, buffer, size, callback) => {
    console.log("received file:", file)
    let b = new Int8Array(WasmContext["memory"], buffer, size)
    let object = files[file]
    var res = await object.file;

    let str = await res.text()
    let arr = []
    str.split("").forEach((c) => {
        arr.push(c.charCodeAt(0))
    })
    b.set(arr, 0)
    WasmContext["vtable"].get(callback)(buffer)
    console.log("reading from buffer %d of size %d", buffer, size)
}

var promiseMgr = {
    promises: {},
    numPromises: 1,
    register: function(promise) {
        if(promise === null || promise === undefined) 
            return;
        var wrapper = {}
        wrapper.promise = promise
        wrapper.count = 1;
        let id = this.numPromises
        this.numPromises++
        this.promises[id] = wrapper;
        
        return id;
    },

    get: function(id) {
        if(id === 0) return;
        return this.promises[id]
    },

    reference: function(id) {
        if(id === 0) return;
        this.promises[id].count++;
    },

    dereference: function() {
        if(id === 0) return;
        this.promises[id].count--;

        if(this.promises[id].count <= 0) delete this.promises[id]
    },

    release: function(id) {
        if(id === 0) return;
        delete this.promises[id]
    }

};

let getPromiseNext = function(promise_id, callback) {

    let promise = promiseMgr.get(promise_id).promise
    promise.then( async (value) => { 
        if(Response.prototype.isPrototypeOf(value))
            WasmContext["vtable"].get(callback)(promiseMgr.register(value))
        else if(Promise.prototype.isPrototypeOf(value))
            WasmContext["vtable"].get(callback)(promiseMgr.register(value))
        else if(Blob.prototype.isPrototypeOf(value))
            WasmContext["vtable"].get(callback)(promiseMgr.register(value))
        else if(ImageBitmap.prototype.isPrototypeOf(value))
            WasmContext["vtable"].get(callback)(promiseMgr.register(value))
        if(typeof value === 'string') {

            let str = value
            let b = new Uint8Array(WasmContext["memory"], WasmContext.heap_base, str.length)
            let arr = []
            str.split("").forEach((c) => {
                arr.push(c.charCodeAt(0))
            })
            b.set(arr, 0)
            WasmContext["vtable"].get(callback)(WasmContext.heap_base)
        }
        
    })
}

let getResponseText = function(promise_id, callback) {

    let text = promiseMgr.get(promise_id).promise

    WasmContext["vtable"].get(callback)(text())
}

let getResponseTextPromise = (promise_id) => {
    let promise = promiseMgr.get(promise_id).promise

    return promiseMgr.register(promise.text())
}

let getResponseBlobPromise = (promise_id) => {
    let promise = promiseMgr.get(promise_id).promise

    return promiseMgr.register(promise.blob())
}



let getCreateImageBitmapPromise = function(promise_id) {}


let c_createImageBitmap = function(obj) {

    var imagePromise = promiseMgr.get(obj).promise;

    var promise = createImageBitmap(imagePromise)

    return promiseMgr.register(promise);
}

let c_fetch = (filename) => {
    var response = fetch(getString(filename))

    return promiseMgr.register(response)
}

/*
let c_createImageBitmap = async (filename, lambda) => {
    var file = await createImageBitmap(getString(filename))
    let str = await file.text()
    let b = new Uint8Array(WasmContext["memory"], WasmContext.heap_base, str.length)
    let arr = []
    str.split("").forEach((c) => {
        arr.push(c.charCodeAt(0))
    })
    b.set(arr, 0)
    WasmContext["vtable"].get(lambda)(WasmContext.heap_base)

}*/

let print_num = function(num) {
    console.log(num)
}

async function init(wasmPath) {
    initWebGpu().then(async function (device) {

        if (device === undefined || device == null) {
            document.getElementById("not_available").style.display = 'flex';
            document.querySelector('canvas').style.display = 'none';
            document.getElementById("fps").style.display = 'none';
            WasmContext.deviceAvailable = false;
            return
            //abort();
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
        
        try{
        instance.exports.__main_argc_argv();
        } catch(err) {
            console.log(err)
        }
    })
}
