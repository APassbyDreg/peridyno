#pragma once
#include "Node.h"
#include <cuda_runtime.h>
#include <cufft.h>
#include <vector>
#include <math_constants.h>
#include "types.h"
namespace dyno
{
	/*!
	*	\class	CapillaryWave
	*	\brief	Peridynamics-based CapillaryWave.
	*/
	template<typename TDataType>
	class CapillaryWave : public Node
	{
		//DECLARE_CLASS_1(CapillaryWave, TDataType)
	public:
		typedef typename TDataType::Real Real;
		typedef typename Vector<Real, 2> Coord2D;
		typedef typename Vector<Real, 4> Coord4D;

		CapillaryWave(int size, float patchLength, std::string name = "default");
		virtual ~CapillaryWave();

	public:

		DEF_ARRAY2D_STATE(Coord2D, Position, DeviceType::GPU, "Height field velocity");

	protected:
		void resetStates() override;

		void updateStates() override;

		void updateTopology() override;

	public:
		int mResolution;

		float mChoppiness;  //�����˼�ļ����ԣ���Χ0~1

		Vec4f* m_displacement = nullptr;  // λ�Ƴ�

	public:
		float m_patch_length;
		float m_realGridSize;

		float m_simulatedRegionWidth;
		float m_simulatedRegionHeight;

		Vec4f* m_device_grid;		//��ǰ��̬����״̬

		Vec4f* m_device_grid_next;

		Vec4f* m_height = nullptr;				//�߶ȳ�

		size_t m_grid_pitch;

		float m_horizon = 2.0f;			//ˮ���ʼ�߶�

		Vec2f* m_source;				//������Ӵ���ˮ����
		float* m_weight;
	protected:
		void initialize();
		void initDynamicRegion();
		void initSource();
		void resetSource();
		void swapDeviceGrid();
		void compute();
		void initHeightPosition();
		Vec4f* GetHeight() { return m_height; }
	};
}