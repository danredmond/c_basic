~
~ Count as high as specified or until error
~
10 PRINT "Input max value as integer:"
20 INPUT MAXVALUE
30 PRINT "Counting..."
40 MAXVALUE = MAXVALUE + 1
50 X = 0
60 PRINT X
70 X = X + 1
80 IF X < MAXVALUE THEN GOTO 60
90 END
