#pragma once
#include "OceanPatch.h"

namespace dyno
{
	template<typename TDataType>
	class Ocean : public Node
	{
	public:
		Ocean();
		~Ocean();

		void animate(float dt);

		//	void moveDynamicRegion(int nx, int ny);
		// 	GLuint getDisplacementTextureId() { return m_patch->getDisplacementTextureId(); }
		// 	GLuint getGradientTextureId() { return m_patch->getGradientTextureId(); }

		float2 getWindDirection() { return make_float2(cosf(m_patch->windDir), sinf(m_patch->windDir)); }
		float getFftRealSize() { return m_fft_real_size; }
		int getFftResolution() { return m_fft_size; }
		float getChoppiness() { return m_choppiness; }
		void setChoppiness(float chopiness) {
			m_choppiness = chopiness;
			m_patch->setChoppiness(m_choppiness);
		}
		//OceanPatch* getOceanPatch() { return m_patch; }
		int getGridSize() { return m_fft_size; }

		//����ÿ����ʵ�ʸ��ǵľ���
		float getPatchLength();
		float getGridLength();

		//void addTrail(CapillaryWave* trail);

		//CapillaryWave* getTrail(int i) { if (i < m_trails.size())return m_trails[i]; else return nullptr; }

	//	void addOceanTrails(vertex* oceanVertex);

		DEF_NODE_PORT(OceanPatch<TDataType>, OceanPatch, "");

	protected:
		void resetStates() override;
		void updateStates() override;

	public:
		//��ѹ�����γɼ���
		float m_choppiness = 1.0f;

		//��ʼ�缶
		int m_windType = 8;

		//fft����ֱ���
		int m_fft_size = 512;

		//fft������ͼ�������� ��λ��
		float m_fft_real_size = 2.0f;

		//�߶ȳ���ͼ�������� ��λ��
		float m_heightfield_real_size = 5000;

		int m_oceanWidth;
		int m_oceanHeight;

		float m_eclipsedTime;

		float m_patchSize = 512;
		float m_virtualGridSize;
		float m_realGridSize;

		int Nx = 4;
		int Ny = 4;

	private:

		Vec4f* oceanVertex;

		//fft wave simulation object
		OceanPatch<DataType3f>* m_patch = nullptr;

		//std::vector<CapillaryWave*> m_trails;
	};
}