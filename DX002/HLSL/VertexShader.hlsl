struct VSout
{
	float4 out_color: VertexColor;
	float4 out_position : SV_Position;
};

cbuffer Cbuf
{
	matrix modelMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
	matrix mvpMatrix;
	matrix normalMatrix;
};

VSout main( float3 pos : Position, float4 color : Color )
{
	VSout vs_out;
	vs_out.out_color = float4(color.x, color.y, color.z,  1.0f);
	vs_out.out_position = mul(mvpMatrix, float4(pos , 1.0f));
	return vs_out;
}