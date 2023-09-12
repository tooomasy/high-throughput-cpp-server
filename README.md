## Performance Analysis
### Basic
```
Running 30s test @ http://localhost:8080
  16 threads and 2000 connections
  Thread calibration: mean lat.: 1.749ms, rate sampling interval: 10ms
  Thread calibration: mean lat.: 9223372036854776.000ms, rate sampling interval: 10ms
  Thread calibration: mean lat.: 9223372036854776.000ms, rate sampling interval: 10ms
  Thread calibration: mean lat.: 9223372036854776.000ms, rate sampling interval: 10ms
  Thread calibration: mean lat.: 9223372036854776.000ms, rate sampling interval: 10ms
  Thread calibration: mean lat.: 9223372036854776.000ms, rate sampling interval: 10ms
  Thread calibration: mean lat.: 9223372036854776.000ms, rate sampling interval: 10ms
  Thread calibration: mean lat.: 9223372036854776.000ms, rate sampling interval: 10ms
  Thread calibration: mean lat.: 9223372036854776.000ms, rate sampling interval: 10ms
  Thread calibration: mean lat.: 9223372036854776.000ms, rate sampling interval: 10ms
  Thread calibration: mean lat.: 9223372036854776.000ms, rate sampling interval: 10ms
  Thread calibration: mean lat.: 9223372036854776.000ms, rate sampling interval: 10ms
  Thread calibration: mean lat.: 9223372036854776.000ms, rate sampling interval: 10ms
  Thread calibration: mean lat.: 9223372036854776.000ms, rate sampling interval: 10ms
  Thread calibration: mean lat.: 9223372036854776.000ms, rate sampling interval: 10ms
  Thread calibration: mean lat.: 9223372036854776.000ms, rate sampling interval: 10ms
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency     1.12ms  343.58us   5.66ms   68.15%
    Req/Sec     6.45     26.06   222.00     94.04%
  3001 requests in 30.64s, 111.37KB read
  Socket errors: connect 0, read 0, write 0, timeout 28860
Requests/sec:     97.96
Transfer/sec:      3.64KB
```
---
Basic (Thread per Connection)
```
Running 30s test @ http://localhost:8080
  16 threads and 2000 connections
  Thread calibration: mean lat.: 2.772ms, rate sampling interval: 10ms
  Thread calibration: mean lat.: 2.603ms, rate sampling interval: 10ms
  Thread calibration: mean lat.: 9223372036854776.000ms, rate sampling interval: 10ms
  Thread calibration: mean lat.: 9223372036854776.000ms, rate sampling interval: 10ms
  Thread calibration: mean lat.: 2.870ms, rate sampling interval: 10ms
  Thread calibration: mean lat.: 9223372036854776.000ms, rate sampling interval: 10ms
  Thread calibration: mean lat.: 9223372036854776.000ms, rate sampling interval: 10ms
  Thread calibration: mean lat.: 2.509ms, rate sampling interval: 10ms
  Thread calibration: mean lat.: 2.635ms, rate sampling interval: 10ms
  Thread calibration: mean lat.: 99.060ms, rate sampling interval: 966ms
  Thread calibration: mean lat.: 9223372036854776.000ms, rate sampling interval: 10ms
  Thread calibration: mean lat.: 9223372036854776.000ms, rate sampling interval: 10ms
  Thread calibration: mean lat.: 2.838ms, rate sampling interval: 10ms
  Thread calibration: mean lat.: 2.984ms, rate sampling interval: 10ms
  Thread calibration: mean lat.: 2.707ms, rate sampling interval: 10ms
  Thread calibration: mean lat.: 2.520ms, rate sampling interval: 10ms
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency     1.76ms    1.48ms  33.41ms   91.18%
    Req/Sec     6.25k     5.46k   33.67k    53.68%
  2971789 requests in 30.63s, 107.70MB read
  Socket errors: connect 0, read 0, write 0, timeout 14000
Requests/sec:  97015.23
Transfer/sec:      3.52MB
```
---
I/O Multiplexing Server
```
Running 30s test @ http://localhost:8080
  16 threads and 2000 connections
  Thread calibration: mean lat.: 1382.324ms, rate sampling interval: 6705ms
  Thread calibration: mean lat.: 1552.633ms, rate sampling interval: 6815ms
  Thread calibration: mean lat.: 1385.837ms, rate sampling interval: 6717ms
  Thread calibration: mean lat.: 1606.748ms, rate sampling interval: 6914ms
  Thread calibration: mean lat.: 1389.953ms, rate sampling interval: 6721ms
  Thread calibration: mean lat.: 1385.969ms, rate sampling interval: 6713ms
  Thread calibration: mean lat.: 2680.249ms, rate sampling interval: 8732ms
  Thread calibration: mean lat.: 2271.148ms, rate sampling interval: 8048ms
  Thread calibration: mean lat.: 3363.840ms, rate sampling interval: 9814ms
  Thread calibration: mean lat.: 3790.224ms, rate sampling interval: 10452ms
  Thread calibration: mean lat.: 1389.854ms, rate sampling interval: 6721ms
  Thread calibration: mean lat.: 1396.600ms, rate sampling interval: 6733ms
  Thread calibration: mean lat.: 4800.952ms, rate sampling interval: 11960ms
  Thread calibration: mean lat.: 1393.609ms, rate sampling interval: 6725ms
  Thread calibration: mean lat.: 1390.975ms, rate sampling interval: 6721ms
  Thread calibration: mean lat.: 4445.876ms, rate sampling interval: 11436ms
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency     8.18s     2.22s   13.74s    62.72%
    Req/Sec     7.72k   163.74     8.12k    82.14%
  3524675 requests in 30.00s, 127.73MB read
  Socket errors: connect 0, read 0, write 0, timeout 11
Requests/sec: 117496.10
Transfer/sec:      4.26MB
```
---
Reactor Pattern
```
Running 30s test @ http://localhost:8080
  16 threads and 2000 connections
  Thread calibration: mean lat.: 2.276ms, rate sampling interval: 10ms
  Thread calibration: mean lat.: 2.427ms, rate sampling interval: 10ms
  Thread calibration: mean lat.: 1.987ms, rate sampling interval: 10ms
  Thread calibration: mean lat.: 2.329ms, rate sampling interval: 10ms
  Thread calibration: mean lat.: 2.227ms, rate sampling interval: 10ms
  Thread calibration: mean lat.: 1.861ms, rate sampling interval: 10ms
  Thread calibration: mean lat.: 2.513ms, rate sampling interval: 10ms
  Thread calibration: mean lat.: 2.265ms, rate sampling interval: 10ms
  Thread calibration: mean lat.: 2.146ms, rate sampling interval: 10ms
  Thread calibration: mean lat.: 2.506ms, rate sampling interval: 10ms
  Thread calibration: mean lat.: 2.239ms, rate sampling interval: 10ms
  Thread calibration: mean lat.: 2.253ms, rate sampling interval: 10ms
  Thread calibration: mean lat.: 2.496ms, rate sampling interval: 10ms
  Thread calibration: mean lat.: 2.102ms, rate sampling interval: 10ms
  Thread calibration: mean lat.: 2.421ms, rate sampling interval: 10ms
  Thread calibration: mean lat.: 2.255ms, rate sampling interval: 10ms
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency    16.64ms   92.05ms   1.01s    97.55%
    Req/Sec    13.56k     4.99k  108.67k    83.10%
  5889680 requests in 30.84s, 213.44MB read
Requests/sec: 190965.36
Transfer/sec:      6.92MB
```