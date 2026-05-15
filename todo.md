initialize all coders
coder id, coder number of compiles, left and right dongles and arguments
create a function to assign both dongles to coder
    coders = [c1, c2, c3, c4, c5]
    dongles = [d1, d2, d3, d4, d5]

    d5 <- C1 -> d1
    d1 <- C2 -> d2
    d2 <- C3 -> d3      // This means that the first coder has last dongle on left side
    d3 <- C4 -> d4
    d4 <- C5 -> d5

    technique: every coder n has left_dongle = n - 1 and right dongle = n the only exculsion
    is C1 which has left_dongle = number_of_coders  and right_dongle = n