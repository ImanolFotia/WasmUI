#pragma once

#include <stdint.h>
#include <stddef.h>
#include "string.hpp"

extern "C" size_t env_fetch(JsString);
extern "C" char* env_promise_next(size_t promise, void* lambda);
extern "C" size_t env_response_text(size_t response);
extern "C" size_t env_response_text_promise(size_t response);
extern "C" size_t env_response_blob_promise(size_t response);
extern "C" size_t env_createImageBitmap(size_t response);

extern "C" void print_num(int);

extern "C" void my_callback(char *buffer) {}

template <typename Signature> struct function {
  function() = default;
  function(Signature &&func) { callback = func; }

  template <typename Func> function(Func &&f) { callback = f; }

  template <typename... Args> auto operator()(Args &&...args) {
    return callback(args...);
  }

  template <typename Func> void operator=(Func &&f) { callback = f; }

  Signature *callback;
};

// Promises at home:


template <typename Signature> struct Promise {
  Promise() = default;
  Promise(size_t handle) : mPromiseHandle(handle) {}

  template <typename Func> void then(Func func) {
    then_func = func;
    env_promise_next(mPromiseHandle, (void*)then_func.callback);
  }

  function<Signature> then_func;
  size_t mPromiseHandle = 0;
  JsString path;
};

struct Blob {
  size_t handle;
};


struct Response {

  Promise<void(char *)> text() {
    return {env_response_text_promise(response_handle)};
  }

  Promise<void(Blob)> blob() {
    Promise <void(Blob)> promise(env_response_blob_promise(response_handle));
    return promise;
  }

  size_t response_handle = 0;

};

struct Image {
  Image() = default;
  Image(size_t h) : handle(h) {

  }
  size_t handle;
};


static Promise<void(Response)> fetch(const char *file_path) {
  return env_fetch(file_path);
}

static Promise<void(Image)> createImageBitmap(Image image) {
  return env_createImageBitmap(image.handle);
}

