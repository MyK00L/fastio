# fastio
fast input/output for competitive programming compatible with c++11

### notes:
- BUFSIZE must be at least 32 (but default is recommended)
- Printer flushes automatically only when it is destroyed, so on interactive tasks you should automatically call .flush()
- if you `#define FAST_FIFSS`, Scanner will be faster, but harder to use interactively.
- If an unsupported type is passed, the compiler will probabilly complain of it having no .begin() method
- only 64bit floating points are supported (calls with `float` and `long double`, internally will use `double`)
- integer printing and floating points are from [here](https://research.swtch.com/fp)
- integer parsing is (partially) from [here](https://kholdstare.github.io/technical/2020/05/26/faster-integer-parsing.html)
- I use `unistd.h`, so it won't compile on windows, but it should be easy to change read/write calls to fread/fwrite

