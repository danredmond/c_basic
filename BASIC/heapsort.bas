~ Heapsort tester
~ Usage:  c_basic heapsort.bas
~ Parser output is in file parse.dat
~ Symbol table output is in file symbol.dat
~
1 RNUMFSEED = 10
2 RNUMINCR  = 7
3 RNUMMAX   = 13
4 RNUMOFF   = (0-.23)
~
~                                   Variable Declarations
~
10 dim a[100]
12 swaptemp  = 0
14 number    = 0
16 arraysize = 0
18 hsize     = 0
20 parent    = 0
22 child     = 0
24 rnseed    = 0
26 outi      = 0
28 sorti     = 0
30 buildi    = 0
32 buildj    = 0
34 pdc       = 0
35 m         = 0
36 pdc1      = 0
37 asizetemp = 0
~
~				    Similar to main body
~
100 print "Enter number of elements to be sorted: "
110 input arraysize
120 hsize = arraysize
125 asizetemp = arraysize - 1
130 rnseed = RNUMFSEED
~
~				    Start of buildheap
~
200 for buildi = 1 to hsize
210 buildj = (hsize+1) - buildi
~
~				    Create the pseudo-random number
~
220 rnseed = rnseed + RNUMINCR
230 if rnseed > RNUMMAX then rnseed = rnseed - RNUMMAX
235 if rnseed = RNUMMAX then rnseed = rnseed - RNUMMAX
240 if rnseed < 0.0 then rnseed = rnseed + RNUMMAX
250 number = rnseed + RNUMOFF
~
~				    Continue with buildheap
~
260 a[buildj] = number
270 parent = buildj
~
~				    Performs the percdown procedure
~
300 child = parent
310 pdc = 2*parent
320 pdc1 = pdc+1
325 m = 0
330 if pdc < hsize then m = 1
335 if pdc = hsize then m = 1
340 if m = 1 then if a[pdc] < a[child] then child = pdc
350 if m = 1 then if pdc < hsize then if a[pdc1] < a[child] then child = pdc1
360 m = 0
370 if child = parent then goto 410
~
~				    Now the swap is performed
~
380 swaptemp = a[parent] : a[parent] = a[child] : a[child] = swaptemp
390 parent = child
400 goto 300
~
~				    End of percdown
~
410 next buildi
~
~				    End of buildheap
~
500 for sorti = 1 to asizetemp
510 parent = 1
520 child = hsize
530 swaptemp = a[parent] : a[parent] = a[child] : a[child] = swaptemp
540 hsize = hsize - 1
~
~				    Performs the percdown procedure
~
600 child = parent
610 pdc = 2*parent
620 pdc1 = pdc+1
625 m = 0
630 if pdc < hsize then m = 1
635 if pdc = hsize then m = 1
640 if m = 1 then if a[pdc] < a[child] then child = pdc
650 if m = 1 then if pdc < hsize then if a[pdc1] < a[child] then child = pdc1
660 m = 0
670 if child = parent then goto 710
~
~				    Now the swap is performed
~
680 swaptemp = a[parent] : a[parent] = a[child] : a[child] = swaptemp
690 parent = child
700 goto 600
~
~				    End of percdown
~
710 next sorti
~
~				    End of sortheap
~
800 print "Sorted Data: "
810 for outi = 1 to arraysize
820 print a[outi]
830 next outi
840 end
