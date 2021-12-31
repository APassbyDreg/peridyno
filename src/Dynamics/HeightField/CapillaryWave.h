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
	public:
		typedef typename TDataType::Real Real;
		typedef typename TDataType::Coord Coord;
		typedef typename Vector<float, 4> Coord4;

		CapillaryWave(int size, float patchLength, std::string name = "default");
		virtual ~CapillaryWave();

		DArray2D<Coord4> GetHeight() { return mHeight; }
	protected:
		void resetStates() override;

		void updateStates() override;

		void updateTopology() override;

		void initialize();
		void initDynamicRegion();
		void initSource();
		void resetSource();
		void swapDeviceGrid();
		void compute();
		void initHeightPosition();

	public:
		int mResolution;

		float mChoppiness;  //�����˼�ļ����ԣ���Χ0~1

	protected:
		float patchLength;
		float realGridSize;

		float simulatedRegionWidth;
		float simulatedRegionHeight;

		size_t gridPitch;

		float horizon = 2.0f;			//ˮ���ʼ�߶�
		float* mWeight;

		DArray2D<Coord4> mHeight;				//�߶ȳ�
		DArray2D<Coord4> mDeviceGrid;		//��ǰ��̬����״̬
		DArray2D<Coord4> mDeviceGridNext;
		DArray2D<Coord4> mDisplacement;   // λ�Ƴ�
		DArray2D<Vec2f> mSource;				//������Ӵ���ˮ����

	};
}