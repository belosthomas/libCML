//
// Created by thomas on 30/11/2020.
//

#ifndef CML_PARAMETER_H
#define CML_PARAMETER_H

#include <cml/config.h>

namespace CML {

    typedef enum {
        INTEGER, FLOATING, BOOLEAN
    } ParameterType;

    using ParameterFloatingType = scalar_t;

    class Parameter {

        friend class AbstractFunction;

    public:
        class Observer {

        public:
            virtual void onValueChange(const Parameter &parameter) {

            }

        };

        ~Parameter() {
            //delete mObserversMutex;
        }

        inline std::string name() {
            return mName;
        }

        EIGEN_STRONG_INLINE ParameterFloatingType f() {
            assertThrow(mType == FLOATING, "The type of the parameter is not floating");
            return get<ParameterFloatingType>();
        }

        EIGEN_STRONG_INLINE int i() {
            assertThrow(mType == INTEGER, "The type of the parameter is not integer");
            return get<int>();
        }

        EIGEN_STRONG_INLINE bool b() {
            assertThrow(mType == BOOLEAN, "The type of the parameter is not boolean");
            return get<bool>();
        }

        void set(ParameterFloatingType v) {
            assertThrow(mType == FLOATING, "The type of the parameter is not floating");
            logger.important("Change the value of '" + mName + "' to " + std::to_string(v));
            get<ParameterFloatingType>() = v;
            for (auto observer : mObservers) {
                observer->onValueChange(*this);
            }
        }

        void set(int v) {
            assertThrow(mType == INTEGER, "The type of the parameter is not integer");
            logger.important("Change the value of '" + mName + "' to " + std::to_string(v));
            get<int>() = v;
            for (auto observer : mObservers) {
                observer->onValueChange(*this);
            }
        }

        void set(bool v) {
            assertThrow(mType == BOOLEAN, "The type of the parameter is not boolean");
            logger.important("Change the value of '" + mName + "' to " + std::to_string(v));
            get<bool>() = v;
            for (auto observer : mObservers) {
                observer->onValueChange(*this);
            }
        }

        inline ParameterType type() {
            return mType;
        }

        inline void subscribeObserver(Observer *observer) {
            assertThrow(observer != nullptr, "null observer passed to subscribeObserver");
            LockGuard lg(*mObserversMutex);
            mObservers.insert(observer);
        }

        inline void removeObserver(Observer *observer) {
            LockGuard lg(*mObserversMutex);
            mObservers.erase(observer);
        }

        bool operator==(const Parameter &other) const {
            return this->mData == other.mData;
        }

        bool operator!=(const Parameter &other) const {
            return this->mData != other.mData;
        }

    protected:
 //   public:
        Parameter(std::string name, ParameterFloatingType value) {
            //mObservers.set_empty_key((Observer*)1);
            //mObservers.set_deleted_key((Observer*)2);
            mName = name;
            mType = FLOATING;
            mData = (void*)new ParameterFloatingType[1];
            ((ParameterFloatingType*)mData)[0] = value;
            mObserversMutex = new Mutex;
        }

        Parameter(std::string name, int value) {
            //mObservers.set_empty_key((Observer*)1);
            //mObservers.set_deleted_key((Observer*)2);
            mName = name;
            mType = INTEGER;
            mData = (void*)new int[1];
            ((int*)mData)[0] = value;
            mObserversMutex = new Mutex;
        }

        Parameter(std::string name, bool value) {
            //mObservers.set_empty_key((Observer*)1);
            //mObservers.set_deleted_key((Observer*)2);
            mName = name;
            mType = BOOLEAN;
            mData = new bool[1];
            ((bool*)mData)[0] = value;
            mObserversMutex = new Mutex;
        }

        template <typename T> T& get() {
            return ((T*)mData)[0];
        }

    private:
        std::string mName;
        ParameterType mType;
        void *mData;

        Set<Observer*> mObservers;
        Mutex *mObserversMutex;

    };

}

#endif //CML_PARAMETER_H
