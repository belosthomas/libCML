#ifndef CML_LOOKUPTABLE_H
#define CML_LOOKUPTABLE_H

#include <cml/config.h>

namespace CML {

    class GrayLookupTable {

    public:
        GrayLookupTable() {
            for (int i = 0; i < 256; i++) {
                mValues[i] = i;
                mInv[i] = i;
            }
        }

        GrayLookupTable(const Vectorf<256> &values) {
            mValues = values;
            computeInverse();
        }

        EIGEN_STRONG_INLINE float operator()(uint8_t input) const {
            return mValues[input];
        }

        EIGEN_STRONG_INLINE float inverse(uint8_t input) const {
            return mInv[input];
        }

    protected:
        void computeInverse() {
            // from dso
            for(int i=1;i<255;i++)
            {
                // find val, such that Binv[val] = i.
                // I dont care about speed for this, so do it the stupid way.

                for(int s=1;s<255;s++)
                {
                    if(mValues[s] <= i && mValues[s+1] >= i)
                    {
                        mInv[i] = s+(i - mValues[s]) / (mValues[s+1]-mValues[s]);
                        break;
                    }
                }
            }
            mInv[0] = 0;
            mInv[255] = 255;
        }

    private:
        Vectorf<256> mValues, mInv;

    };

}

#endif