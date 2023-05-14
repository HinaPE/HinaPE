#version 430
layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 4) out;
uniform mat4 proj;
uniform float particleRadius;
in vec4 vPosition[];
flat in int vertex_number[];
out vec2 gPosition;
flat out int vert_num;
void main() {
   vert_num = vertex_number[0];
   vec4 p = vPosition[0];
   gPosition = vec2(-1.0, -1.0);
   gl_Position = proj * vec4(p.xy + gPosition * particleRadius, p.zw);
   EmitVertex();
   gPosition = vec2(-1.0, 1.0);
   gl_Position = proj * vec4(p.xy + gPosition * particleRadius, p.zw);
   EmitVertex();
   gPosition = vec2(1.0, -1.0);
   gl_Position = proj * vec4(p.xy + gPosition * particleRadius, p.zw);
   EmitVertex();
   gPosition = vec2(1.0, 1.0);
   gl_Position = proj * vec4(p.xy + gPosition * particleRadius, p.zw);
   EmitVertex();
   EndPrimitive();
}
