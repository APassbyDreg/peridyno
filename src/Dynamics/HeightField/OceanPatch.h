#pragma once
#include <cuda_runtime.h>
#include <cufft.h>
#include <vector>
#include <math_constants.h>
#include "Node.h"

namespace dyno {

struct WindParam
{
    float windSpeed;
    float A;
    float choppiness;
    float global;
};

class OceanPatch : public Node
{
public:
    OceanPatch(int size, float patchSize, int windType = 8, std::string name = "default");
    OceanPatch(int size, float wind_dir, float windSpeed, float A_p, float max_choppiness, float global);
    ~OceanPatch();

    void animate(float t);

    float getMaxChoppiness();
    float getChoppiness();

    //����ʵ�ʸ����������mΪ��λ
    float getPatchSize()
    {
        return m_realPatchSize;
    }

    //��������ֱ���
    float getGridSize()
    {
        return m_size;
    }
    float getGlobalShift()
    {
        return m_globalShift;
    }
    float getGridLength()
    {
        return m_realPatchSize / m_size;
    }
    void setChoppiness(float value)
    {
        m_choppiness = value;
    }

	Vec2f* getHeightField()
    {
        return m_ht;
    }
	Vec4f* getDisplacement()
    {
        return m_displacement;
    }
    //GLuint getDisplacementTextureId() { return m_displacement_texture; }
    //GLuint getGradientTextureId() { return m_gradient_texture; }

public:
    float m_windSpeed = 0;                   //����
    float windDir     = CUDART_PI_F / 3.0f;  //�糡����
    int   m_windType;                        //�����ȼ���Ŀǰ����Ϊ0~12
    float m_fft_real_length = 10;
    float m_fft_flow_speed  = 1.0f;

    Vec4f* m_displacement = nullptr;  // λ�Ƴ�
	Vec4f* m_gradient     = nullptr;  // gradient field

protected:
	void resetStates() override;

	void updateStates() override;

private:
    void  generateH0(Vec2f* h0);
    float gauss();
    float phillips(float Kx, float Ky, float Vdir, float V, float A, float dir_depend);

    int m_size;

    int m_spectrumW;  //Ƶ�׿��
    int m_spectrumH;  //Ƶ�׳���

    float m_choppiness;  //�����˼�ļ����ԣ���Χ0~1

    std::vector<WindParam> m_params;  //��ͬ�����ȼ��µ�FFT�任����

    const float g = 9.81f;          //����
    float       A = 1e-7f;          //��������ϵ��
    float       m_realPatchSize;    //ʵ�ʸ����������mΪ��λ
    float       dirDepend = 0.07f;  //�糤���������

    float m_maxChoppiness;  //����choppiness����
    float m_globalShift;    //��߶�ƫ�Ʒ���

	Vec2f* m_h0;  //��ʼƵ��
	Vec2f* m_ht;  //��ǰʱ��Ƶ��

	Vec2f* m_Dxt;  //x����ƫ��
	Vec2f* m_Dzt;  //z����ƫ��

    cufftHandle fftPlan;
};

}  // namespace PhysIKA
