#ifndef CML_SLAMTHREAD
#define CML_SLAMTHREAD

#include <cml/config.h>
#include <cml/base/AbstractSlam.h>

#include <thread>

namespace CML {

    class SlamThread {

    public:
        SlamThread(AbstractSlam &slam, std::string name, std::function<void()> function) : mSlam(slam), mFunction(function) {
            mThread = std::thread(&SlamThread::run, this);
            pthread_setname_np(mThread.native_handle(), name.c_str());
        }

        ~SlamThread() {
            mThread.join();
        }

    protected:
        void run() {
            GarbageCollectorInstance gci = mSlam.getMap().getGarbageCollector().newInstance();
            while (!mSlam.isStopped()) {
                mSlam.getMap().getGarbageCollector().collect(gci);
                mFunction();
                usleep(10);
            }
        }

    private:
        AbstractSlam &mSlam;
        std::function<void()> mFunction;
        std::thread mThread;

    };

}

#endif