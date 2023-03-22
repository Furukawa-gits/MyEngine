cbuffer cbuff0 : register(b0)
{
	matrix mat; // ３Ｄ変換行列
    matrix matBillboard;
	float4 color; // 色(RGBA)
};

// 頂点シェーダーからピクセルシェーダーへのやり取りに使用する構造体
struct VSOutput
{
	float4 pos : SV_POSITION; // システム用頂点座標
    float scale : TEXCOORD;
	float4 color : COLOR;
	//float3 normal :NORMAL; // 法線ベクトル
	//float2 uv  :TEXCOORD; // uv値
};

//ジオメトリシェーダーからピクセルシェーダーへの出力
struct GSOutput
{
    float4 svpos : SV_POSITION; // システム用頂点座標
    //float3 normal : NORMAL; // 法線ベクトル
    float2 uv : TEXCOORD; // uv値
	float4 color : COLOR;
};
