struct VertexOut {
    @builtin(position) position: vec4f,
    @location(0) color: vec4f,
}; 
@vertex fn main(
    @builtin(vertex_index) VertexIndex : u32 ) -> VertexOut {
    var pos = array<vec2f, 3>(
        vec2(0.0, 0.5),
        vec2(-0.5, -0.5),
        vec2(0.5, -0.5)
    );
    var color = array<vec4f, 3>(
        vec4(1.0, 0.0, 0.0, 1.0),
        vec4(0.0, 1.0, 0.0, 1.0),
        vec4(0.0, 0.0, 1.0, 1.0)
    );
    var vsOut: VertexOut;
    vsOut.position = vec4f(pos[VertexIndex], 0.0, 1.0);
    vsOut.color = color[VertexIndex];
    return vsOut;
}