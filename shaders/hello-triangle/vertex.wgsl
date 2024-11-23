struct FragmentIn {
    @builtin(position) position: vec4f,
    @location(0) color: vec4f,
}; 
@fragment fn main(fsInput: FragmentIn) -> @location(0) vec4f {
    return fsInput.color;
}