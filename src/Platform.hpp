
namespace adm
{
    #ifdef DEBUG
    constexpr bool Debug = false;
    #else
    constexpr bool Debug = true;
    #endif

    // These aren't used in AdmUtils, since the code should be
    // portable to all platforms specified below, BUT, it may be
    // useful to whoever uses this library
    struct Platforms
    {
        static constexpr int Windows = 0;
        static constexpr int Linux = 1;

        static constexpr int Undefined = -1;
    };

    #ifdef WIN32
    constexpr int Platform = Platforms::Windows;
    #elif __linux__
    constexpr int Platform = Platforms::Linux;
    #else
    static_assert(false, "Unsupported platform");
    #endif
}
