#pragma once

namespace CE {
    class Application {
    public:
        Application();
        virtual ~Application();

        virtual int Run() = 0;
    };
}