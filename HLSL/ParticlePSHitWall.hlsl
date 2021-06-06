#include "Particle.hlsli"


Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
SamplerState samLinear : register(s0);

//�{���͉~�̃t�F�[�h����
//�Ȃ����l�p�ɂȂ��Ă�
float4 RoundFade(float4 fade1, float inputTex, float aspect)
{
	float2 uv = inputTex;
	uv.x *= aspect;

	if (distance(uv, float2(0.5f, aspect*0.5f)) > sin(Time.z))
	{
		fade1.a = 0.0f;
	}

	return fade1;
}

//��̉摜�����킹��t�F�[�h����
float4 CrossFadeNormal(float4 fade1, float4 fade2)
{
	return lerp(fade1, fade2, sin(Time.x));
}
//�ʂ̉摜�̐F�������ɓ���������^�C�~���O�����炷
float4 CrossFadeShift(float4 fade1, float alpha)
{
	fade1.a = alpha;
	return fade1;
}
//�ʂ̉摜�̐F�������ɓ���������^�C�~���O�����炷�i���F�ɕς���j
float4 CrossFadeWhite(float4 fade1,float alpha)
{
	return lerp(float4(1, 1, 1, 1), fade1, alpha);
}

//�ʂ̉摜�̐F�������ɓ���������^�C�~���O�����炷�i���F�ɕς���j
float4 CrossFadeBlack(float4 fade1, float alpha)
{
	return lerp(float4(0, 0, 0, 1), fade1, alpha);
}

//�ʂ̉摜�̐F�������ɓ���������^�C�~���O�����炷�i�͂�����ɕς���j
float4 CrossFadeClearly(float4 fade1,float alpha)
{
	fade1.a = step(alpha, 0);
	return fade1;
}
//�ʂ̉摜�̐F�������ɓ���������^�C�~���O�����炷�i��������������ɔ��F�ɂ���j
float4 CrossFadeTipWhite(float4 fade1, float alpha)
{
	fade1 = lerp(float4(1, 1, 1, 1), fade1, step(alpha, 0));
	fade1 = lerp(float4(0, 0, 0, 0), fade1, step(alpha, 0.1f));
	return fade1;
}

//��̉摜�����킹��t�F�[�h����
float4 CrossFade(float4 fade1, float4 fade2)
{
	//��̉摜�����킹��t�F�[�h
	float alpha = fade2.r - sin(Time.x);

	return CrossFadeClearly(fade1,alpha);
}

//�m�C�Y�t�F�[�h����
//����
float random(float2 uv)
{
	return frac(sin(dot(uv, float2(12.9898f, 78.233f)))* 43758.5453f);
}
//���C��
float4 noise(float4 fade1, float2 uv)
{
	float2 p = floor(uv);
	float rand = random(p);
	float4 blockNoise = float4(rand, rand, rand, 1);

	return blockNoise;
}

//�O���[�X�P�[��
float4 gray(float4 fade1)
{
	float g = fade1.r * 0.3f + fade1.g * 0.6f + fade1.b * 0.1f;
	fade1 = float4(g, g, g, fade1.a);

	return fade1;
}

//�p�[���[�m�C�Y
float perlinNoise(float2 uv)
{
	float2 p = floor(uv);
	float2 f = frac(uv);
	float2 u = f * f * (3.0f - 2.0f * f);
	float v00 = -1 + 2 * random(p + float2(0, 0));
	float v10 = -1 + 2 * random(p + float2(1, 0));
	float v01 = -1 + 2 * random(p + float2(0, 1));
	float v11 = -1 + 2 * random(p + float2(1, 1));

	return lerp(
		lerp(dot(v00, f - float2(0, 0)), dot(v10, f - float2(1, 0)), u.x),
		lerp(dot(v01, f - float2(0, 1)), dot(v11, f - float2(1, 1)), u.x),
		u.y); //+ 0.5f;
}

//FBM����
float FBM(float2 uv)
{
	float f = 0;
	float2 q = uv;
	f += 0.5000f*perlinNoise(q); q = q * 2.01f;
	f += 0.2500f*perlinNoise(q); q = q * 2.02f;
	f += 0.2500f*perlinNoise(q);
	return f;
}

//�u���[
float4 Blur(float2 uv)
{
	float power = 0.1f;
	float center = 1.0f - power * 4.0f;

	float4 output = 0;
	for (int i = 0; i < 4; i++)
	{
		output += tex.Sample(samLinear, uv
			+ float2(((i % 3) - 1)*0.01f, ((i / 3) - 1)*0.01f)) * power;
	}
	output += tex.Sample(samLinear, uv) * center;

	return output;
}

//���[�V�����u���[
float4 MotionBlur(float2 uv)
{
	float power = 0.1f;

	float4 output = 0;
	for (int i = 0; i < 9; i++)
	{
		output += tex.Sample(samLinear, uv
			+ float2(sin(Time.x - i*0.1f)*0.05f, 0.0f)) * power*(10.0f - i) / 5.0f;
	}

	return output;
}

//���U�C�N
float4 Mosaic(float2 baseUV, float mag)
{
	return tex.Sample(samLinear, floor(baseUV * mag) / mag);
}




float4 main(PS_INPUT input) : SV_TARGET
{
	//�摜
	float4 textureData = tex.Sample(samLinear, input.Tex);

	//�~�̔��a
	float halfSize = 0.5f;

	//�~��\�����鋗��
	float length = 10.0f;

	//�~�̔��a(���S���炾�񂾂�傫���Ȃ�)
	float rad = lerp(halfSize,((length - Time.w) / length),step(((length - Time.w) / length), halfSize));

	//�摜���~�̌`�ɂ���(1,0f�͊��S�ȕs����)
	textureData.a = lerp(1.0f, 0.0f, step(rad, distance(input.Tex, halfSize)));

	//�ǂɋ߂Â��قǕs�����ɂȂ�(0.06f�͊��S�ɕs�����ɂ͂Ȃ�Ȃ����߂ɂ�����)
	textureData.a *= (length -Time.w) * 0.06f;

	return textureData;
}