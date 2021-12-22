#pragma once
#include "Module/ConstraintModule.h"
#include "Peridynamics/NeighborData.h"
#include "types.h"
#include "Module/ComputeModule.h"
namespace dyno {

	/**
	  * @brief This is an implementation of elasticity based on projective peridynamics.
	  *		   For more details, please refer to[He et al. 2017] "Projective Peridynamics for Modeling Versatile Elastoplastic Materials"
	  */
	template<typename TDataType>
	class CapillaryWaveModule : public ComputeModule
	{
		DECLARE_CLASS_1(CapillaryWaveModule, TDataType)

	public:
		typedef typename TDataType::Real Real;
		typedef typename TDataType::Coord Coord;
		typedef typename TDataType::Matrix Matrix;
		typedef TPair<TDataType> NPair;

		CapillaryWaveModule();
		~CapillaryWaveModule();

		void compute() override;
		
		void constrain();

		virtual void solveElasticity();

	protected:
		void preprocess();

		/**
		 * @brief Correct the particle position with one iteration
		 * Be sure computeInverseK() is called as long as the rest shape is changed
		 */
		virtual void enforceElasticity();
		virtual void computeMaterialStiffness();

		void updateVelocity();
		void computeInverseK();

	public:


		/**
		 * @brief Particle position
		 */
		DEF_ARRAY_IN(Coord, Position, DeviceType::GPU, "Particle position");

		/**
			* @brief Particle velocity
			*/
		DEF_ARRAY_IN(Coord, Velocity, DeviceType::GPU, "Particle velocity");

	public:


	protected:
		DArray<Real> mBulkStiffness;
		DArray<Real> mWeights;

		DArray<Coord> mDisplacement;
		DArray<Coord> mPosBuf;

		DArray<Matrix> mF;
		DArray<Matrix> mInvK;


	public:
		void swapDeviceGrid();

		float m_horizon = 2.0f;			//ˮ���ʼ�߶�

		float m_patch_length;
		float m_realGridSize;			//����ʵ�ʾ���

		int m_simulatedRegionWidth;		//��̬������
		int m_simulatedRegionHeight;	//��̬����߶�

		int m_Nx;
		int m_Ny;

		int m_simulatedOriginX = 0;			//��̬�����ʼx����
		int m_simulatedOriginY = 0;			//��̬�����ʼy����

		gridpoint* m_device_grid;		//��ǰ��̬����״̬
		gridpoint* m_device_grid_next;
		size_t m_grid_pitch;

		float4* m_height = nullptr;				//�߶ȳ�

		float2* m_source;				//������Ӵ���ˮ����
		float* m_weight;


		void initialize();

		void initDynamicRegion();
		void initSource();

		void resetSource();
	};
}