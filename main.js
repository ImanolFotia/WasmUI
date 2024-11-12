function init(wasmPath) {
    const canvas = document.getElementById("main-canvas")

    if(canvas === null) {
        console.error("canvas does not exist...");
    }

    const ctx = app.getContext("2d");

    const w = await WebAssembly.instantiateStreaming(fetch(wasmPath))

    const heap_base = w.instance.exports.__heap_base.value;
}
