//
// Created by cstro29 on 19/5/21.
//

#ifndef ENDE_SINGLETON_H
#define ENDE_SINGLETON_H

namespace ende {

    template <typename T>
    class Singleton {
    protected:

        explicit Singleton<T>() = default;

        static T& instance() {
            static T inst;
            return inst;
        }

    };

}

#endif //ENDE_SINGLETON_H
