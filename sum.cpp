
class String {
    public:
    int p;
    int len;
};

    namespace console {
        extern "C" void log(String);
    };

extern "C" {


    int sum(int a, int b) { 
        return a + b;
    } 

    void test() {
	    console::log({(int)"Calling console.log from C++!", 29});
    }

    String hello() {
	    return {(int)"Hello, world from C++!", 22};
    }
}


