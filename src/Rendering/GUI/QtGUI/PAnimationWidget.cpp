#include "PAnimationWidget.h"

#include "PSimulationThread.h"

#include <QGridLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QLineEdit>
#include <QIntValidator>
#include <QDebug>
namespace dyno
{
	PAnimationWidget::PAnimationWidget(QWidget *parent) : 
		QWidget(parent),
		m_startSim(nullptr),
		m_resetSim(nullptr)
	{
		mTotalFrame = 800;

		QHBoxLayout* layout = new QHBoxLayout();
		setLayout(layout);

		QGridLayout* frameLayout	= new QGridLayout();

		mTotalFrameSpinbox = new QSpinBox();
		mTotalFrameSpinbox->setFixedSize(60, 25);
		mTotalFrameSpinbox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

		mTotalFrameSpinbox->setMaximum(1000000);
		mTotalFrameSpinbox->setValue(mTotalFrame);

		QGridLayout* GLayout = new QGridLayout;
		mFrameSlider = new PAnimationQSlider(0, mTotalFrame, this);

		mFrameSlider->setStyleSheet(
			"QSlider{background-color: #B0B0B0; border-radius: 2px;}"
			"QSlider::groove:horizontal{height: 20px; margin-top: 0px; margin-bottom: 5px; left: 0px; right: 0px; border:0px; border-radius:0px; border: 1px solid rgb(200, 200, 200); background:rgba(0,0,0,30);}"
 			"QSlider::handle:horizontal{width: 10; height: 20px; margin-top: 0px; margin-left: 0px; margin-bottom: 0px; margin-right: 0px; border-radius:3px; border: 1px solid rgb(30, 30, 30); background:rgba(109,109,109,255);}"
 			"QSlider::sub-page:horizontal{margin-top: 1px; margin-bottom: 6px; background:rgba(0,149,176,1);}"
		);

		frameLayout->addWidget(mFrameSlider, 0, 0, 0 , (labelSize - 1) * 2);

		QGridLayout* operationLayout = new QGridLayout();

		m_startSim = new QPushButton("Start");
		m_resetSim = new QPushButton("Reset");

		m_resetSim->setCheckable(false);

		operationLayout->addWidget(mTotalFrameSpinbox, 0, 0);
		operationLayout->addWidget(m_startSim, 0, 1);
		operationLayout->addWidget(m_resetSim, 0, 2);

		m_startSim->setCheckable(true);

		layout->addLayout(frameLayout, 10);
		layout->addLayout(operationLayout, 1);
		
		connect(m_startSim, SIGNAL(released()), this, SLOT(toggleSimulation()));
		connect(m_resetSim, SIGNAL(released()), this, SLOT(resetSimulation()));
		connect(PSimulationThread::instance(), SIGNAL(simulationFinished()), this, SLOT(simulationFinished()));

		connect(PSimulationThread::instance(), SIGNAL(oneFrameFinished()), this, SLOT(updateSlider()));

		// ��̬�ı� Slider
		connect(mTotalFrameSpinbox, SIGNAL(valueChanged(int)), mFrameSlider, SLOT(maximumChanged(int)));

		PSimulationThread::instance()->start();
	}

	PAnimationWidget::~PAnimationWidget()
	{
		PSimulationThread::instance()->stop();
		PSimulationThread::instance()->deleteLater();
		PSimulationThread::instance()->wait();  //����ȴ��߳̽���
	}

	void PAnimationWidget::toggleSimulation()
	{
		if (m_startSim->isChecked())
		{
			PSimulationThread::instance()->resume();
			m_startSim->setText("Pause");
			m_resetSim->setDisabled(true);
			mTotalFrameSpinbox->setEnabled(false);
			mFrameSlider->setEnabled(false);
		}
		else
		{
			PSimulationThread::instance()->pause();
			m_startSim->setText("Resume");
			m_resetSim->setDisabled(false);

			mTotalFrameSpinbox->setEnabled(true);
			mFrameSlider->setEnabled(true);
		}
	}

	void PAnimationWidget::resetSimulation()
	{
		PSimulationThread::instance()->reset(mTotalFrameSpinbox->value());

		m_startSim->setText("Start");
		m_startSim->setEnabled(true);
		m_startSim->setChecked(false);

		mTotalFrameSpinbox->setEnabled(true);
		mFrameSlider->setEnabled(true);
		mFrameSlider->setValue(0);
	}

	void PAnimationWidget::simulationFinished()
	{
		m_startSim->setText("Finished");
		m_startSim->setDisabled(true);
		m_startSim->setChecked(false);

		m_resetSim->setDisabled(false);

		mTotalFrameSpinbox->setEnabled(true);
	}
	
	void PAnimationWidget::updateSlider()
	{
		mFrameSlider->setValue(PSimulationThread::instance()->getCurrentFrameNum());
	}
}
