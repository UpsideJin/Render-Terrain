struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float4 worldpos : POSITION;
	float4 norm : NORMAL;
    float4 tex : TEXCOORD;
};

cbuffer ConstantBuffer : register(b0)
{
    float4x4 viewproj;
	float4 eye;
    int height;
    int width;
}

Texture2D<float4> heightmap : register(t0);

// 4 point neighbourhood
//       B
//     E A C
//       D
//     /|\
//    /_|_\
//    \ | /
//     \|/
/*VS_OUTPUT main(float3 input : POSITION)
{
	VS_OUTPUT output;

	float scale = height / 4;
    float4 mysample = heightmap.Load(int3(input));
	output.pos = float4(input.x, input.y, mysample.r * scale, 1.0f);
	output.tex = float4(input.x / height, input.y / width, output.pos.z, scale);
    output.pos = mul(output.pos, viewproj);

    // calculate vertex normal from heightmap
    float zb = heightmap.Load(int3(input.xy + int2(0, -1), 0)).r * scale;
    float zc = heightmap.Load(int3(input.xy + int2(1, 0), 0)).r * scale;
    float zd = heightmap.Load(int3(input.xy + int2(0, 1), 0)).r * scale;
    float ze = heightmap.Load(int3(input.xy + int2(-1, 0), 0)).r * scale;

    output.norm = float4(normalize(float3(ze - zc, zb - zd, 2.0f)), 1.0f);
	output.worldpos = float4(input, 1.0f);

 	return output;
}*/


// 8 point neighbourhood
//    I B C
//    H A D
//    G F E
// _______
// |\ | /|
// |_\|/_|
// | /|\ |
// |/_|_\|
/*VS_OUTPUT main(float3 input : POSITION) {
	VS_OUTPUT output;

	float scale = height / 4;
	float4 mysample = heightmap.Load(int3(input));
	output.pos = float4(input.x, input.y, mysample.r * scale, 1.0f);
	output.tex = float4(input.x / height, input.y / width, output.pos.z, scale);
	output.pos = mul(output.pos, viewproj);
	
	float zb = heightmap.Load(int3(input.xy + int2(0, -1), 0)).r * scale;
	float zc = heightmap.Load(int3(input.xy + int2(1, -1), 0)).r * scale;
	float zd = heightmap.Load(int3(input.xy + int2(1, 0), 0)).r * scale;
	float ze = heightmap.Load(int3(input.xy + int2(1, 1), 0)).r * scale;
	float zf = heightmap.Load(int3(input.xy + int2(0, 1), 0)).r * scale;
	float zg = heightmap.Load(int3(input.xy + int2(-1, 1), 0)).r * scale;
	float zh = heightmap.Load(int3(input.xy + int2(-1, 0), 0)).r * scale;
	float zi = heightmap.Load(int3(input.xy + int2(-1, -1), 0)).r * scale;

	float x = zg + 2 * zh + zi - zc - 2 * zd - ze;
	float y = 2 * zb + zc + zi - ze - 2 * zf - zg;
	float z = 8.0f;
	
	output.norm = float4(normalize(float3(x, y, z)), 1.0f);
	
	output.worldpos = float4(input, 1.0f);

	return output;
}*/

// 6 point neighbourhood - matches actual triangles being drawn
//    G B 
//    F A C
//      E D
// _____
// |\  |\  
// | \ | \ 
// |__\|__\
//  \  |\  |
//   \ | \ |
//    \|__\|
VS_OUTPUT main(float3 input : POSITION) {
	VS_OUTPUT output;

	float scale = height / 4;
	float4 mysample = heightmap.Load(int3(input));
	output.pos = float4(input.x, input.y, mysample.r * scale, 1.0f);
	output.tex = float4(input.x / height, input.y / width, output.pos.z, scale);
	output.pos = mul(output.pos, viewproj);

	float zb = heightmap.Load(int3(input.xy + int2(0, -1), 0)).r * scale;
	float zc = heightmap.Load(int3(input.xy + int2(1, 0), 0)).r * scale;
	float zd = heightmap.Load(int3(input.xy + int2(1, 1), 0)).r * scale;
	float ze = heightmap.Load(int3(input.xy + int2(0, 1), 0)).r * scale;
	float zf = heightmap.Load(int3(input.xy + int2(-1, 0), 0)).r * scale;
	float zg = heightmap.Load(int3(input.xy + int2(-1, -1), 0)).r * scale;

	float x = 2 * zf + zc + zg - zb - 2 * zc - zd;
	float y = 2 * zb + zc + zg - zd - 2 * ze - zf;
	float z = 6.0f;

	output.norm = float4(normalize(float3(x, y, z)), 1.0f);
	
	output.worldpos = float4(input, 1.0f);

	return output;
}