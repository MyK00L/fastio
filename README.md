# fastio
fast input/output for competitive programming compatible with c++11 or greater

### notes:
- BUFSIZE must be at least 32 (but default is recommended)
- Printer flushes automatically only when it is destroyed, so on interactive tasks you should automatically call .flush()
- If an unsupported type is passed, the compiler will probabilly complain of it having no .begin() method
- floating points are not 100% accurate

### float accuracy:
- tested with (no nan or inf, they're not supported):
	- 0,1,-1
	- 32768 binary random values st abs(val)<1e100 and abs(val)>1e-100
	- 32768 uniformly random values in (-1e3,1e3)
- scan:
	- float: max\_error: 0, avg\_error: 0
	- double: max\_error: 6.42353e-15, avg\_error: 8.28241e-16
	- long double: max\_error: 5.58098e-15, avg\_error: 7.08416e-16
- print:
	- float: max\_error: 0, avg\_error: 0
	- double: max\_error: 0, avg\_error: 0
	- long double: max\_error: 3.51204e-17, avg\_error: 6.9486e-18

Errors are calculated as abs(original value - new value)/abs(original value) (or simply new value if the original value was 0)

note that scanner cannot read floats with "e" notation
and printer can only print floats with "e" notation
