export module ende.Singleton;

namespace ende {

export template <typename T>
class Singleton {
  protected:
    explicit Singleton() = default;

    static T &instance() {
        static T inst;
        return inst;
    }
};

}
