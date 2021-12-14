
#pragma once
#include "Module/ConstraintModule.h"
#include "Peridynamics/NeighborData.h"
#include "types.h"
namespace dyno {

	/**
	  * @brief This is an implementation of elasticity based on projective peridynamics.
	  *		   For more details, please refer to[He et al. 2017] "Projective Peridynamics for Modeling Versatile Elastoplastic Materials"
	  */
	template<typename TDataType>
	class CapillaryWaveModule : public ConstraintModule
	{
		DECLARE_CLASS_1(CapillaryWaveModule, TDataType)

	public:
		typedef typename TDataType::Real Real;
		typedef typename TDataType::Coord Coord;
		typedef typename TDataType::Matrix Matrix;
		typedef TPair<TDataType> NPair;

		CapillaryWaveModule();
		~CapillaryWaveModule() override;
		
		void constrain() override;

		virtual void solveElasticity();

	protected:
		void preprocess() override;

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
		 * @brief Horizon
		 * A positive number represents the radius of neighborhood for each point
		 */
		DEF_VAR_IN(Real, Horizon, "");

		DEF_VAR_IN(Real, TimeStep, "");

		/**
		 * @brief Particle position
		 */
		DEF_ARRAY_IN(Coord, Position, DeviceType::GPU, "Particle position");

		/**
			* @brief Particle velocity
			*/
		DEF_ARRAY_IN(Coord, Velocity, DeviceType::GPU, "Particle velocity");

		/**
		 * @brief Neighboring particles
		 * 
		 */
		DEF_ARRAYLIST_IN(int, NeighborIds, DeviceType::GPU, "Neighboring particles' ids");

		DEF_ARRAYLIST_IN(NPair, RestShape, DeviceType::GPU, "Reference shape");

	public:
		/**
		 * @brief Lame parameters
		 * m_lambda controls the isotropic part while mu controls the deviatoric part.
		 */
		DEF_VAR(Real, Mu, 0.001, "Lame parameters: mu");

		DEF_VAR(Real, Lambda, 0.01, "Lame parameters: lambda");

		DEF_VAR(uint, IterationNumber, 10, "Iteration number");

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
	};
}