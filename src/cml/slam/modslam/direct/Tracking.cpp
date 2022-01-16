#include "Hybrid.h"


bool Hybrid::directNeedNewKeyFrame(PFrame currentFrame) {

    if (!mTrackingOk) {
        return false;
    }

    /*if (mDirectMappingQueue.getCurrentSize() > 0) {
        if (mLastPhotometricTrackingResidual.saturatedRatio() > 0.1) {
            while (mDirectMappingQueue.getCurrentSize() > 0) {
                CML::usleep(1);
            }
        }
        return false;
    }*/

    if (mTrackedWithDirect) {

        scalar_t setting_maxShiftWeightT = 0.04 * (640 + 480);
        scalar_t setting_maxShiftWeightR = 0.0 * (640 + 480);
        scalar_t setting_maxShiftWeightRT = 0.02 * (640 + 480);
        scalar_t setting_maxAffineWeight = 2;

        Vector2 refToFh = mLastDirectKeyFrame->getExposure().to(currentFrame->getExposure()).getParameters();

        bool flowTooBig = setting_maxShiftWeightT * CML::sqrt(mLastPhotometricTrackingResidual.flowVector[0]) /
                          (currentFrame->getWidth(0) + currentFrame->getHeight(0)) +
                          setting_maxShiftWeightR * CML::sqrt(mLastPhotometricTrackingResidual.flowVector[1]) /
                          (currentFrame->getWidth(0) + currentFrame->getHeight(0)) +
                          setting_maxShiftWeightRT * CML::sqrt(mLastPhotometricTrackingResidual.flowVector[2]) /
                          (currentFrame->getWidth(0) + currentFrame->getHeight(0)) +
                          setting_maxAffineWeight * abs(log(refToFh[0])) > 1;

        if (mFirstDirectRMSE < 0) {
            mFirstDirectRMSE = mLastPhotometricTrackingResidual.rmse();
        }

        bool trackingResidualTooBig = mLastPhotometricTrackingResidual.rmse() > 2 * mFirstDirectRMSE;
        //bool trackingResidualTooBig = false;

        if (flowTooBig) {
            logger.info("Creating new keyframe because of flow");
        }

        if (trackingResidualTooBig) {
            logger.info("Creating new keyframe because of tracking residual too big : " +
                        std::to_string( mLastPhotometricTrackingResidual.rmse()) + " > 2 x " + std::to_string(mPhotometricTracker->getFirstRMSE(mLastDirectKeyFrame))
            );
        }

        bool photometricNeedKeyframe = flowTooBig || trackingResidualTooBig;


        if (photometricNeedKeyframe) {
            return true;
        }

    }

    return false;

}