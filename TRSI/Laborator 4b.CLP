(defrule summa
  =>
  (bind ?sum 0)                           
  (bind ?t1 4)                  
  (bind ?t2 2)
  (bind ?sum (+ ?t1 ?t2))         
  (printout t "sum=" ?sum crlf)
)

(defrule diferenta
  =>
  (bind ?diff 0)
  (bind ?t1 4)                  
  (bind ?t2 2)
  (bind ?diff (/ ?t2 ?t1))
  (printout t "diff=" ?diff crlf)
)

(defrule gcd
  =>
  (bind ?gcd 0)
  (bind ?t1 18)                  
  (bind ?t2 27)
   (while (neq ?t1 ?t2)
   (if (> ?t1 ?t2)
      then (bind ?t1 (- ?t1 ?t2))
      else (bind ?t2 (- ?t2 ?t1))
      )
   )
   (bind ?gcd ?t2)
  (printout t "gcd=" ?gcd crlf)
)