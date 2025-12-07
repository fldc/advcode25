      ******************************************************************
      * ADVENT OF CODE DAY 7 - PART 1 SOLUTION
      ******************************************************************
       IDENTIFICATION DIVISION.
       PROGRAM-ID. BEAMSPLIT.
       AUTHOR. FREDRIK STORM.

       ENVIRONMENT DIVISION.
       INPUT-OUTPUT SECTION.
       FILE-CONTROL.
           SELECT INPUT-FILE ASSIGN TO "input"
               ORGANIZATION IS LINE SEQUENTIAL.

       DATA DIVISION.
       FILE SECTION.
       FD  INPUT-FILE.
       01  INPUT-RECORD            PIC X(150).

       WORKING-STORAGE SECTION.
       01  WS-EOF                  PIC A VALUE 'N'.
       01  WS-LINE-NUM             PIC 9(5) VALUE 0.
       01  WS-COL-NUM              PIC 9(5) VALUE 0.
       01  WS-WIDTH                PIC 9(5) VALUE 0.
       01  WS-HEIGHT               PIC 9(5) VALUE 0.
       01  WS-START-COL            PIC 9(5) VALUE 0.
       01  WS-SPLIT-COUNT          PIC 9(10) VALUE 0.
       01  WS-QUEUE-START          PIC 9(10) VALUE 1.
       01  WS-QUEUE-END            PIC 9(10) VALUE 1.
       01  WS-CURRENT-ROW          PIC 9(5).
       01  WS-CURRENT-COL          PIC 9(5).
       01  WS-BEAM-ROW             PIC 9(5).
       01  WS-BEAM-COL             PIC 9(5).
       01  WS-FOUND                PIC A.
       01  WS-VIS-IDX              PIC 9(10).
       01  WS-VIS-COUNT            PIC 9(10) VALUE 0.
       01  WS-TEMP-COL             PIC 9(5).
       01  WS-SPLIT-HIT-IDX        PIC 9(10).
       01  WS-SPLIT-HIT-COUNT      PIC 9(10) VALUE 0.
       01  WS-SPLIT-FOUND          PIC A.

       01  GRID-DATA.
           05  GRID-LINE OCCURS 200 TIMES.
               10  GRID-CHAR OCCURS 200 TIMES PIC X.

       01  BEAM-QUEUE.
           05  BEAM-ENTRY OCCURS 100000 TIMES.
               10  BEAM-ROW        PIC 9(5).
               10  BEAM-COL        PIC 9(5).

       01  VISITED-TABLE.
           05  VISITED-ENTRY OCCURS 100000 TIMES.
               10  VIS-ROW         PIC 9(5).
               10  VIS-COL         PIC 9(5).

       01  SPLITTER-HIT-TABLE.
           05  SPLITTER-HIT-ENTRY OCCURS 10000 TIMES.
               10  SPLIT-ROW       PIC 9(5).
               10  SPLIT-COL       PIC 9(5).

       PROCEDURE DIVISION.
       MAIN-PROCEDURE.
           PERFORM READ-INPUT.
           PERFORM SIMULATE-BEAMS.
           DISPLAY "Beam split count: " WS-SPLIT-COUNT.
           STOP RUN.

       READ-INPUT.
           OPEN INPUT INPUT-FILE.
           PERFORM UNTIL WS-EOF = 'Y'
               READ INPUT-FILE INTO INPUT-RECORD
                   AT END MOVE 'Y' TO WS-EOF
                   NOT AT END
                       ADD 1 TO WS-LINE-NUM
                       IF WS-WIDTH = 0
                           MOVE 141 TO WS-WIDTH
                       END-IF
                       PERFORM VARYING WS-COL-NUM FROM 1 BY 1
                           UNTIL WS-COL-NUM > WS-WIDTH
                           MOVE INPUT-RECORD(WS-COL-NUM:1)
                               TO GRID-CHAR(WS-LINE-NUM, WS-COL-NUM)
                           IF INPUT-RECORD(WS-COL-NUM:1) = 'S'
                               MOVE WS-COL-NUM TO WS-START-COL
                           END-IF
                       END-PERFORM
               END-READ
           END-PERFORM.
           CLOSE INPUT-FILE.
           MOVE WS-LINE-NUM TO WS-HEIGHT.

       SIMULATE-BEAMS.
           MOVE 1 TO BEAM-ROW(1).
           MOVE WS-START-COL TO BEAM-COL(1).
           MOVE 2 TO WS-QUEUE-END.

           PERFORM UNTIL WS-QUEUE-START >= WS-QUEUE-END
               MOVE BEAM-ROW(WS-QUEUE-START) TO WS-BEAM-ROW
               MOVE BEAM-COL(WS-QUEUE-START) TO WS-BEAM-COL
               ADD 1 TO WS-QUEUE-START

               PERFORM CHECK-VISITED
               IF WS-FOUND = 'N'
                   PERFORM ADD-VISITED
                   PERFORM PROCESS-BEAM
               END-IF
           END-PERFORM.

       CHECK-VISITED.
           MOVE 'N' TO WS-FOUND.
           PERFORM VARYING WS-VIS-IDX FROM 1 BY 1
               UNTIL WS-VIS-IDX > WS-VIS-COUNT
                   OR WS-FOUND = 'Y'
               IF VIS-ROW(WS-VIS-IDX) = WS-BEAM-ROW
                   AND VIS-COL(WS-VIS-IDX) = WS-BEAM-COL
                   MOVE 'Y' TO WS-FOUND
               END-IF
           END-PERFORM.

       ADD-VISITED.
           ADD 1 TO WS-VIS-COUNT.
           MOVE WS-BEAM-ROW TO VIS-ROW(WS-VIS-COUNT).
           MOVE WS-BEAM-COL TO VIS-COL(WS-VIS-COUNT).

       CHECK-SPLITTER-HIT.
           MOVE 'N' TO WS-SPLIT-FOUND.
           PERFORM VARYING WS-SPLIT-HIT-IDX FROM 1 BY 1
               UNTIL WS-SPLIT-HIT-IDX > WS-SPLIT-HIT-COUNT
                   OR WS-SPLIT-FOUND = 'Y'
               IF SPLIT-ROW(WS-SPLIT-HIT-IDX) = WS-CURRENT-ROW
                   AND SPLIT-COL(WS-SPLIT-HIT-IDX) = WS-CURRENT-COL
                   MOVE 'Y' TO WS-SPLIT-FOUND
               END-IF
           END-PERFORM.

       ADD-SPLITTER-HIT.
           ADD 1 TO WS-SPLIT-HIT-COUNT.
           MOVE WS-CURRENT-ROW TO SPLIT-ROW(WS-SPLIT-HIT-COUNT).
           MOVE WS-CURRENT-COL TO SPLIT-COL(WS-SPLIT-HIT-COUNT).

       PROCESS-BEAM.
           MOVE WS-BEAM-ROW TO WS-CURRENT-ROW.
           MOVE WS-BEAM-COL TO WS-CURRENT-COL.

           PERFORM UNTIL WS-CURRENT-ROW > WS-HEIGHT
               IF GRID-CHAR(WS-CURRENT-ROW, WS-CURRENT-COL) = '^'
                   PERFORM CHECK-SPLITTER-HIT
                   IF WS-SPLIT-FOUND = 'N'
                       PERFORM ADD-SPLITTER-HIT
                       ADD 1 TO WS-SPLIT-COUNT
                   END-IF

                   IF WS-CURRENT-COL > 1
                       MOVE WS-CURRENT-ROW
                           TO BEAM-ROW(WS-QUEUE-END)
                       SUBTRACT 1 FROM WS-CURRENT-COL
                           GIVING WS-TEMP-COL
                       MOVE WS-TEMP-COL TO BEAM-COL(WS-QUEUE-END)
                       ADD 1 TO WS-QUEUE-END
                   END-IF

                   IF WS-CURRENT-COL < WS-WIDTH
                       MOVE WS-CURRENT-ROW
                           TO BEAM-ROW(WS-QUEUE-END)
                       ADD 1 TO WS-CURRENT-COL
                           GIVING WS-TEMP-COL
                       MOVE WS-TEMP-COL TO BEAM-COL(WS-QUEUE-END)
                       ADD 1 TO WS-QUEUE-END
                   END-IF

                   MOVE WS-HEIGHT TO WS-CURRENT-ROW
               END-IF
               ADD 1 TO WS-CURRENT-ROW
           END-PERFORM.
