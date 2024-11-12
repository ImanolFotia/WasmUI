"use strict";

function getString(buffer, offset) {
    const str_buffer = new Uint32Array(buffer, offset, 2)
    return new TextDecoder("utf8").decode(new Uint8Array(buffer, str_buffer[0], str_buffer[1]))
}

async function init(wasmPath) {
    const canvas = document.getElementById("main_canvas")

    if (canvas === null) {
        console.error("canvas does not exist...");
    }

    canvas.width = 640;
    canvas.height = 480
    const ctx = canvas.getContext("2d");

    ctx.fillStyle = 'orange';
    ctx.fillRect(10, 10, 640, 480);

    const { instance } = await WebAssembly.instantiateStreaming(fetch(wasmPath), {
        "env": {
            "print": export_log
        }
    })

    function export_log(heap_base) {
        const buffer = instance.exports.memory.buffer;
        const str = getString(buffer, heap_base);
        console.log(str)
    }

    const heap_base = instance.exports.__heap_base.value

    instance.exports.test()

    const buffer = instance.exports.memory.buffer
    const res = instance.exports.hello(heap_base)
    let str = getString(buffer, heap_base)
    document.getElementById("hello").textContent = str
    console.log(str);
}

document.addEventListener('readystatechange', () => {
    if (document.readyState == 'complete') init("../wasm/test.wasm");
});

window.onload = () => {
    'use strict';

    if ('serviceWorker' in navigator) {
        navigator.serviceWorker
            .register('./sw.js');
    }
}
